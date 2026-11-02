/*
 *
 *    Copyright (c) 2026 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "DeviceCallbacks.h"

#include <common/CHIPDeviceManager.h>
#include <common/Esp32AppServer.h>
#include <common/Esp32ThreadInit.h>
#if CONFIG_ENABLE_SNTP_TIME_SYNC
#include <time/TimeSync.h>
#endif

#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "shell_extension/launch.h"
#include "shell_extension/openthread_cli_register.h"
#include <app/server/Dnssd.h>
#include <setup_payload/OnboardingCodesUtil.h>
#include <app/util/attribute-storage.h>
#include <lib/support/CodeUtils.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <platform/ESP32/ESP32Utils.h>

#if CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER
#include <platform/ESP32/ESP32FactoryDataProvider.h>
#endif // CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER

#if CONFIG_ENABLE_PW_RPC
#include "Rpc.h"
#endif

#if CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER
#include <platform/ESP32/ESP32DeviceInfoProvider.h>
#else
#include <DeviceInfoProviderImpl.h>
#endif // CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER

#if CONFIG_SEC_CERT_DAC_PROVIDER
#include <platform/ESP32/ESP32SecureCertDACProvider.h>
#endif

#include <electrical-alarm-stub.h>
#include <electrical-distribution-stub.h>
#include <electrical-protection-alarm-stub.h>
#include <power-topology-stub.h>

using namespace ::chip;
using namespace ::chip::DeviceManager;
using namespace ::chip::Credentials;

static const char * TAG = "electrical-protection-app";

static AppDeviceCallbacks EchoCallbacks;

namespace {
constexpr EndpointId kEnclosureEndpointId = 1; // Electrical Distribution Enclosure (0x0517)
constexpr EndpointId kBreakerEndpointId   = 2; // Electrical Circuit Breaker (0x0516)

// The enclosure device type requires each breaker child endpoint to carry a semantic tag from the
// Common Number namespace, so this one is breaker number one.
constexpr uint8_t kCommonNumberNamespaceId = 0x07;
constexpr uint8_t kNumberOneTag            = 0x01;

const app::Clusters::Descriptor::Structs::SemanticTagStruct::Type kBreakerTagList[] = {
    { .namespaceID = kCommonNumberNamespaceId, .tag = kNumberOneTag },
};

#if CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER
DeviceLayer::ESP32FactoryDataProvider sFactoryDataProvider;
#endif // CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER

#if CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER
DeviceLayer::ESP32DeviceInfoProvider gExampleDeviceInfoProvider;
#else
DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;
#endif // CONFIG_ENABLE_ESP32_DEVICE_INFO_PROVIDER

#if CONFIG_SEC_CERT_DAC_PROVIDER
DeviceLayer::ESP32SecureCertDACProvider gSecureCertDACProvider;
#endif // CONFIG_SEC_CERT_DAC_PROVIDER

chip::Credentials::DeviceAttestationCredentialsProvider * get_dac_provider(void)
{
#if CONFIG_SEC_CERT_DAC_PROVIDER
    return &gSecureCertDACProvider;
#elif CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER
    return &sFactoryDataProvider;
#else // EXAMPLE_DAC_PROVIDER
    return chip::Credentials::Examples::GetExampleDACProvider();
#endif
}
} // namespace

void ApplicationInit()
{
    ESP_LOGD(TAG, "Electrical Protection App: ApplicationInit()");

    using namespace chip::app::Clusters;

    // The generated Init callbacks of these clusters are no-ops, so the app owns the instances and
    // registers them imperatively. Endpoint layout and features match the Linux app.
    VerifyOrDie(ElectricalDistribution::ElectricalDistributionInit(kEnclosureEndpointId) == CHIP_NO_ERROR);
    VerifyOrDie(ElectricalProtectionAlarm::ElectricalProtectionAlarmInit(kEnclosureEndpointId) == CHIP_NO_ERROR);

    VerifyOrDie(PowerTopology::PowerTopologyInit(
                    kEnclosureEndpointId, BitMask<PowerTopology::Feature>(PowerTopology::Feature::kTreeTopology)) == CHIP_NO_ERROR);
    VerifyOrDie(PowerTopology::PowerTopologyInit(kBreakerEndpointId,
                                                 BitMask<PowerTopology::Feature>(PowerTopology::Feature::kTreeTopology,
                                                                                 PowerTopology::Feature::kElectricalCircuit)) ==
                CHIP_NO_ERROR);

    VerifyOrDie(SetTagList(kBreakerEndpointId, Span<const Descriptor::Structs::SemanticTagStruct::Type>(kBreakerTagList)) ==
                CHIP_NO_ERROR);

    VerifyOrDie(ElectricalAlarm::ElectricalAlarmInit(kBreakerEndpointId, ElectricalAlarm::kAllFeatures) == CHIP_NO_ERROR);
}

void ApplicationShutdown()
{
    ESP_LOGD(TAG, "Electrical Protection App: ApplicationShutdown()");

    using namespace chip::app::Clusters;

    ElectricalAlarm::ElectricalAlarmShutdown();
    ElectricalProtectionAlarm::ElectricalProtectionAlarmShutdown();
    PowerTopology::PowerTopologyShutdown();
    ElectricalDistribution::ElectricalDistributionShutdown();
}

static void InitServer(intptr_t context)
{
    // Print QR Code URL
    PrintOnboardingCodes(chip::RendezvousInformationFlags(CONFIG_RENDEZVOUS_MODE));

    Esp32AppServer::Init(); // Init ZCL Data Model and CHIP App Server AND
                            // Initialize device attestation config
    // Application code should always be initialised after the initialisation of
    // server.
    ApplicationInit();

#if CONFIG_ENABLE_SNTP_TIME_SYNC
    const char kNtpServerUrl[]             = "pool.ntp.org";
    const uint16_t kSyncNtpTimeIntervalDay = 1;
    chip::Esp32TimeSync::Init(kNtpServerUrl, kSyncNtpTimeIntervalDay);
#endif
}

extern "C" void app_main()
{
    // Initialize the ESP NVS layer.
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "nvs_flash_init() failed: %s", esp_err_to_name(err));
        return;
    }
    err = esp_event_loop_create_default();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_event_loop_create_default() failed: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "==================================================");
    ESP_LOGI(TAG, "chip-esp32-electrical-protection-app starting");
    ESP_LOGI(TAG, "==================================================");

#if CONFIG_ENABLE_PW_RPC
    chip::rpc::Init();
#endif

#if CONFIG_ENABLE_CHIP_SHELL
#if CONFIG_OPENTHREAD_CLI
    chip::RegisterOpenThreadCliCommands();
#endif
    chip::LaunchShell();
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
    if (DeviceLayer::Internal::ESP32Utils::InitWiFiStack() != CHIP_NO_ERROR)
    {
        ESP_LOGE(TAG, "Failed to initialize WiFi stack");
        return;
    }
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI

    DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    CHIPDeviceManager & deviceMgr = CHIPDeviceManager::GetInstance();

    CHIP_ERROR error = deviceMgr.Init(&EchoCallbacks);
    if (error != CHIP_NO_ERROR)
    {
        ESP_LOGE(TAG, "device.Init() failed: %" CHIP_ERROR_FORMAT, error.Format());
        return;
    }

#if CONFIG_ENABLE_ESP32_FACTORY_DATA_PROVIDER
    SetCommissionableDataProvider(&sFactoryDataProvider);
#if CONFIG_ENABLE_ESP32_DEVICE_INSTANCE_INFO_PROVIDER
    SetDeviceInstanceInfoProvider(&sFactoryDataProvider);
#endif
#endif

    SetDeviceAttestationCredentialsProvider(get_dac_provider());

    RETURN_SAFELY_IGNORED chip::DeviceLayer::PlatformMgr().ScheduleWork(InitServer, reinterpret_cast<intptr_t>(nullptr));
}
