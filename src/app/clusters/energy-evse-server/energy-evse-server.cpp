/*
 *    Copyright (c) 2023 Project CHIP Authors
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

#include "DefaultEnergyEvseDelegate.h"
#include "energy-evse-delegate.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/AttributeAccessInterface.h>
#include <app/CommandHandler.h>
#include <app/ConcreteCommandPath.h>
#include <app/util/af.h>
#include <app/util/attribute-storage.h>
#include <lib/core/CHIPEncoding.h>
#include <lib/core/Optional.h>
#include <lib/support/CHIPPlatformMemory.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/ConnectivityManager.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::EnergyEvse;
using namespace chip::app::Clusters::EnergyEvse::Attributes;
using chip::Protocols::InteractionModel::Status;

// Delegate
// Implementation
namespace {

Delegate * gDelegate = nullptr;

Delegate * GetDelegate()
{
    if (gDelegate == nullptr)
    {
        static DefaultEnergyEvseDelegate dg;
        gDelegate = &dg;
    }
    return gDelegate;
}

} // namespace

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

void SetDefaultDelegate(Delegate * delegate)
{
    gDelegate = delegate;
}

Delegate * GetDefaultDelegate()
{
    return GetDelegate();
}

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip

void MatterEnergyEvsePluginServerInitCallback() {}

void emberAfEnergyEvseClusterServerInitCallback(chip::EndpointId endpoint) {}

bool emberAfEnergyEvseClusterDisableCallback(app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
                                             const Commands::Disable::DecodableType & commandData)
{
    Status status = Status::Success;

    status = (CHIP_NO_ERROR == GetDelegate()->Disable()) ? Status::Success : Status::Failure;
    commandObj->AddStatus(commandPath, status);

    return true;
}

bool emberAfEnergyEvseClusterEnableChargingCallback(app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
                                                    const Commands::EnableCharging::DecodableType & commandData)
{
    Status status = Status::Success;

    const auto & enableChargeTime     = commandData.enableChargeTime;
    const auto & minimumChargeCurrent = commandData.minimumChargeCurrent;
    const auto & maximumChargeCurrent = commandData.maximumChargeCurrent;

    status = (CHIP_NO_ERROR == GetDelegate()->EnableCharging(enableChargeTime, minimumChargeCurrent, maximumChargeCurrent))
        ? Status::Success
        : Status::Failure;

    commandObj->AddStatus(commandPath, status);

    return true;
}

bool emberAfEnergyEvseClusterEnableDischargingCallback(app::CommandHandler * commandObj,
                                                       const app::ConcreteCommandPath & commandPath,
                                                       const Commands::EnableDischarging::DecodableType & commandData)
{
    Status status = Status::Success;

    const auto & enableDischargeTime     = commandData.enableDischargeTime;
    const auto & maximumDischargeCurrent = commandData.maximumDischargeCurrent;

    status = (CHIP_NO_ERROR == GetDelegate()->EnableDischarging(enableDischargeTime, maximumDischargeCurrent)) ? Status::Success
                                                                                                               : Status::Failure;

    commandObj->AddStatus(commandPath, status);

    return true;
}

bool emberAfEnergyEvseClusterStartDiagnosticsCallback(app::CommandHandler * commandObj,
                                                      const app::ConcreteCommandPath & commandPath,
                                                      const Commands::StartDiagnostics::DecodableType & commandData)
{
    Status status = Status::Success;

    status = (CHIP_NO_ERROR == GetDelegate()->StartDiagnostics()) ? Status::Success : Status::Failure;
    commandObj->AddStatus(commandPath, status);

    return true;
}
bool emberAfEnergyEvseClusterSetTargetsCallback(app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
                                                const Commands::SetTargets::DecodableType & commandData)
{
    // TODO
    return false;
}
bool emberAfEnergyEvseClusterGetTargetsCallback(app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
                                                const Commands::GetTargets::DecodableType & commandData)
{
    // TODO
    return false;
}
bool emberAfEnergyEvseClusterClearTargetsCallback(app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
                                                  const Commands::ClearTargets::DecodableType & commandData)
{
    // TODO
    return false;
}
