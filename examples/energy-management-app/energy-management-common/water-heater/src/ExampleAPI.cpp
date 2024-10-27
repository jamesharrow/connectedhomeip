/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
 *    All rights reserved.
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

#include <curl/curl.h>
#include <iostream>
#include <json/json.h>

#include <ExampleAPI.h>

#include <ElectricalPowerMeasurementDelegate.h>
#include <WhmManufacturer.h>
#include <lib/support/CodeUtils.h>

#include <app-common/zap-generated/attribute-type.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Clusters.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::WaterHeaterManagement;
using namespace chip::app::Clusters::ElectricalPowerMeasurement;

using chip::Protocols::InteractionModel::Status;

using namespace std;

static constexpr EndpointId kTempSensorEndpoint               = 1;
static constexpr uint16_t sGetPowerMeasurementsTimerDurationS = 2;
static constexpr uint16_t sGetDataTimerDurationS              = 60;

ExampleAPI::ExampleAPI()
{
    mpCurlHandle = static_cast<CURL *>(curl_easy_init());
    if (mpCurlHandle == nullptr)
    {
        ChipLogError(AppServer, "ExampleAPI::ExampleAPI failed to initialised CURL");
        chipDie();
    }

    CHIP_ERROR err = DeviceLayer::SystemLayer().StartTimer(System::Clock::Seconds32(1), GetPowerMeasurementsTimer, this);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(AppServer, "Unable to start a GetPowerMeasurementsTimer timer: %" CHIP_ERROR_FORMAT, err.Format());
    }

    err = DeviceLayer::SystemLayer().StartTimer(System::Clock::Seconds32(1), GetDataTimer, this);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(AppServer, "Unable to start a GetDataTimer timer: %" CHIP_ERROR_FORMAT, err.Format());
    }
}

ExampleAPI::~ExampleAPI()
{
    if (mpCurlHandle)
    {
        curl_easy_cleanup(mpCurlHandle);
    }
    DeviceLayer::SystemLayer().CancelTimer(GetPowerMeasurementsTimer, this);
    DeviceLayer::SystemLayer().CancelTimer(GetDataTimer, this);
}

void ExampleAPI::GetPowerMeasurements()
{
    static Power_mW oldValue_mW = 0;

    std::string powerMeasUrl = sBaseUrl + "/get_mqtt_data";
    curl_easy_setopt(mpCurlHandle, CURLOPT_URL, powerMeasUrl.c_str());

    std::string postResponse;
    curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    // Perform the POST request
    CURLcode res = curl_easy_perform(mpCurlHandle);
    if (res != CURLE_OK)
    {
        ChipLogError(AppServer, "ExampleAPI::GetPowerMeasurements: request failed %s", curl_easy_strerror(res));
        return;
    }

    //  Parse the JSON response
    Json::Value jsonResponse;
    Json::CharReaderBuilder jsonReader;
    std::istringstream responseStream(postResponse);
    if (Json::parseFromStream(jsonReader, responseStream, &jsonResponse, nullptr))
    {
        if (!jsonResponse.isArray())
        {
            ChipLogError(AppServer, "ExampleAPI::GetPowerMeasurements expected an array jsonResponse but did not get one");
            return;
        }

        if (!jsonResponse[jsonResponse.size() - 1].isArray())
        {
            ChipLogError(AppServer, "ExampleAPI::GetPowerMeasurements expected an array2 jsonResponse but did not get one");
            return;
        }

        std::string str = jsonResponse[jsonResponse.size() - 1][1].asString();

        float activePowerW      = std::stof(str);
        Power_mW activePower_mW = static_cast<Power_mW>(activePowerW * 1000);
        if (oldValue_mW != activePower_mW)
        {
            ChipLogProgress(AppServer, "ExampleAPI::GetPowerMeasurements activePower %f (W)", activePowerW);
            oldValue_mW = activePower_mW;
        }

        WhmManufacturer * mn = GetWhmManufacturer();

        CHIP_ERROR err = mn->SendPowerReading(1, activePower_mW, static_cast<Voltage_mV>(240 * 1000),
                                              static_cast<Amperage_mA>(activePowerW * 1000 / 240));
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(AppServer, "Unable to SendPowerReading: %" CHIP_ERROR_FORMAT, err.Format());
        }
    }
}

void ExampleAPI::GetHeaterData()
{
    std::string powerMeasUrl = sBaseUrl + "/water_heater";
    curl_easy_setopt(mpCurlHandle, CURLOPT_URL, powerMeasUrl.c_str());

    std::string postResponse;
    curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    // Perform the POST request
    CURLcode res = curl_easy_perform(mpCurlHandle);
    if (res != CURLE_OK)
    {
        ChipLogError(AppServer, "ExampleAPI::GetHeaterData: request failed %s", curl_easy_strerror(res));
        return;
    }

    // We expect a complex JSON structure in return which contains
    // {
    //   "temperatureMeasurement": {
    //     "value": 39.5
    //   },
    //   "thermostat": {
    //     "occupiedSetpoint": 62,
    //     "schedule": {
    //       "schedule": [
    //         {
    //           "daysBitmap": 2,
    //           "transitions": [
    //             {
    //               "setpoint": 62,
    //               "transitionTime": 610
    //             },
    //             {
    //               "setpoint": 40,
    //               "transitionTime": 1020
    //             }
    //           ]
    //         },
    //         {
    //           "daysBitmap": 4,
    //           "transitions": [
    //             {
    //               "setpoint": 62,
    //               "transitionTime": 650
    //             },
    //             {
    //               "setpoint": 40,
    //               "transitionTime": 1010
    //             }
    //           ]
    //         },
    //         {
    //           "daysBitmap": 8,
    //           "transitions": [
    //             {
    //               "setpoint": 62,
    //               "transitionTime": 430
    //             },
    //             {
    //               "setpoint": 40,
    //               "transitionTime": 610
    //             },
    //             {
    //               "setpoint": 62,
    //               "transitionTime": 660
    //             },
    //             {
    //               "setpoint": 40,
    //               "transitionTime": 1020
    //             }
    //           ]
    //         },
    //         {
    //           "daysBitmap": 16,
    //           "transitions": [
    //             {
    //               "setpoint": 62,
    //               "transitionTime": 660
    //             },
    //             {
    //               "setpoint": 40,
    //               "transitionTime": 1020
    //             }
    //           ]
    //         },
    //         {
    //           "daysBitmap": 97,
    //           "transitions": [
    //             {
    //               "setpoint": 62,
    //               "transitionTime": 360
    //             },
    //             {
    //               "setpoint": 40,
    //               "transitionTime": 1320
    //             }
    //           ]
    //         }
    //       ]
    //     },
    //     "systemMode": 4
    //   },
    //   "waterHeater": {
    //     "estimatedHeatRequired_mWh": 2613.75,
    //     "heatDemand": 2,
    //     "heaterTypes": 2,
    //     "tankPercentage": 52,
    //     "tankVolume": 100
    //   }
    // }

    //  Parse the JSON response
    Json::Value jsonResponse;
    Json::CharReaderBuilder jsonReader;
    std::istringstream responseStream(postResponse);
    if (Json::parseFromStream(jsonReader, responseStream, &jsonResponse, nullptr))
    {
        const float temperatureMeasurement = jsonResponse["temperatureMeasurement"]["value"].asFloat();
        const int estimatedHeatRequiredmWh = jsonResponse["waterHeater"]["estimatedHeatRequired_mWh"].asInt();
        const int occupiedSetpoint         = jsonResponse["thermostat"]["occupiedSetpoint"].asInt();
        const int heatDemand               = jsonResponse["waterHeater"]["heatDemand"].asInt();
        const int heaterTypes              = jsonResponse["waterHeater"]["heaterTypes"].asInt();
        const int tankPercentage           = jsonResponse["waterHeater"]["tankPercentage"].asInt();
        const int tankVolume               = jsonResponse["waterHeater"]["tankVolume"].asInt();

        ChipLogProgress(AppServer, "ExampleAPI::GetHeaterData temperatureMeasurement %f", temperatureMeasurement);
        ChipLogProgress(AppServer, "ExampleAPI::GetHeaterData estimatedHeatRequired_mWh %d", estimatedHeatRequiredmWh);
        ChipLogProgress(AppServer, "ExampleAPI::GetHeaterData occupiedSetpoint %d", occupiedSetpoint);
        ChipLogProgress(AppServer, "ExampleAPI::GetHeaterData heatDemand %d", heatDemand);
        ChipLogProgress(AppServer, "ExampleAPI::GetHeaterData heaterTypes %d", heaterTypes);
        ChipLogProgress(AppServer, "ExampleAPI::GetHeaterData tankPercentage %d", tankPercentage);
        ChipLogProgress(AppServer, "ExampleAPI::GetHeaterData tankVolume %d", tankVolume);

        WhmManufacturer * mn = GetWhmManufacturer();
        if (mn)
        {
            WaterHeaterManagementDelegate * dg = mn->GetWhmDelegate();
            if (dg)
            {
                // Note we don't bother with the HeaterTypes and HeatDemand for now - this is handled elsewhere
                // TODO set current temperature using Ember
                TemperatureMeasurement::Attributes::MeasuredValue::Set(kTempSensorEndpoint,
                                                                       static_cast<int16_t>(temperatureMeasurement * 100));

                dg->SetTargetWaterTemperature(static_cast<int16_t>(occupiedSetpoint * 100));
                dg->SetEstimatedHeatRequired(static_cast<chip::Energy_mWh>(estimatedHeatRequiredmWh));
                dg->SetTankPercentage(static_cast<chip::Percent>(tankPercentage));
                dg->SetTankVolume(static_cast<uint16_t>(tankVolume));
            }
        }
    }
}

void ExampleAPI::GetPowerMeasurementsTimer(System::Layer * systemLayer, void * delegate)
{
    ExampleAPI * dg = reinterpret_cast<ExampleAPI *>(delegate);

    dg->GetPowerMeasurements();

    CHIP_ERROR err = DeviceLayer::SystemLayer().StartTimer(System::Clock::Seconds32(sGetPowerMeasurementsTimerDurationS),
                                                           GetPowerMeasurementsTimer, dg);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(AppServer, "Unable to start a GetPowerMeasurementsTimer timer: %" CHIP_ERROR_FORMAT, err.Format());
    }
}

void ExampleAPI::GetDataTimer(System::Layer * systemLayer, void * delegate)
{
    ExampleAPI * dg = reinterpret_cast<ExampleAPI *>(delegate);

    dg->GetHeaterData();

    CHIP_ERROR err = DeviceLayer::SystemLayer().StartTimer(System::Clock::Seconds32(sGetDataTimerDurationS), GetDataTimer, dg);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(AppServer, "Unable to start a GetDataTimer timer: %" CHIP_ERROR_FORMAT, err.Format());
    }
}

void ExampleAPI::SetSerialNumber(const char * pSerialNumber)
{
    mSerialNumber = pSerialNumber;
}

Status ExampleAPI::BoostActivate()
{
    ChipLogProgress(AppServer, "ExampleAPI::BoostActivate");

    return SetBoost(true);
}

Status ExampleAPI::BoostDeactivate()
{
    ChipLogProgress(AppServer, "ExampleAPI::BoostDeactivate");

    return SetBoost(false);
}

Status ExampleAPI::SetBoost(bool on)
{
    CURLcode res;
    struct curl_slist * headers = NULL;
    const char * pOp            = on ? "on" : "off";

    std::string boostActivateUrl = sBaseUrl + "/toggle_boost_" + pOp;

    curl_easy_setopt(mpCurlHandle, CURLOPT_URL, boostActivateUrl.c_str());

    std::string postResponse;
    curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    curl_easy_setopt(mpCurlHandle, CURLOPT_HTTPHEADER, headers);

    // Set the request body for the GET request
    curl_easy_setopt(mpCurlHandle, CURLOPT_POSTFIELDS, "");

    // Perform the POST request
    res = curl_easy_perform(mpCurlHandle);

    // Check for errors
    if (res != CURLE_OK)
    {
        ChipLogError(AppServer, "ExampleAPI::SetBoost %s request failed %s", pOp, curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return Status::Failure;
    }

    ChipLogProgress(AppServer, "ExampleAPI::SetBoost %s successful. Response: %s", pOp, postResponse.c_str());

    //  Parse the JSON response
    // Json::Value jsonResponse;
    // Json::CharReaderBuilder jsonReader;
    // std::istringstream responseStream(postResponse);
    // std::string errs;

    // if (!Json::parseFromStream(jsonReader, responseStream, &jsonResponse, nullptr))
    // {
    //     ChipLogError(AppServer, "ExampleAPI::SetBoost %s request failed to parse responseStream: %s", pOp,
    //     errs.c_str()); curl_slist_free_all(headers); return Status::Failure;
    // }

    // ChipLogProgress(AppServer, "ExampleAPI::SetBoost %s response: %s", pOp, jsonResponse.toStyledString().c_str());

    // curl_slist_free_all(headers);

    return Status::Success;
}

// Callback function to handle curl's response
/* static */
size_t ExampleAPI::WriteCallback(void * contents, size_t size, size_t nmemb, void * userp)
{
    ((string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

Status ExampleAPI::Login()
{
    // CURLcode res;
    // struct curl_slist * headers = NULL;

    // std::string authUrl = sBaseUrl + "/uaa/oauth/token";
    // curl_easy_setopt(mpCurlHandle, CURLOPT_URL, authUrl.c_str());

    // // Set the callback function to handle the response
    // std::string getTokenResponse;
    // curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    // curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEDATA, &getTokenResponse);

    // // Set the HTTP POST data for the login request in JSON format
    // Json::Value jsonPostData;
    // jsonPostData["grant_type"] = "client_credentials";
    // std::string postData       = jsonPostData.toStyledString();
    // curl_easy_setopt(mpCurlHandle, CURLOPT_POSTFIELDS, postData.c_str());

    // // Set the content type to JSON
    // headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    // headers = curl_slist_append(headers, "Authorization: Basic ZEVvdGDiEKJpLXJfd2hpIuIlbjpPNnlLOL1Zcmh1WGZyNwTF"),
    // headers = curl_slist_append(headers, "Ocp-Apim-Subscription-Key: f292pp12345640519b7d3345cc999999");
    // curl_easy_setopt(mpCurlHandle, CURLOPT_HTTPHEADER, headers);

    // // Set the callback function to handle the response
    // std::string postResponse;
    // curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    // curl_easy_setopt(mpCurlHandle, CURLOPT_WRITEDATA, &postResponse);
    // //    curl_easy_setopt(mpCurlHandle, CURLOPT_VERBOSE, 1);

    // // Perform the POST request
    // res = curl_easy_perform(mpCurlHandle);
    // if (res != CURLE_OK)
    // {
    //     ChipLogError(AppServer, "ExampleAPI::Login: Login request failed %s", curl_easy_strerror(res));
    //     return Status::Failure;
    // }

    // ChipLogProgress(AppServer, "ExampleAPI::Login: Login successful!");

    // //  Parse the JSON response
    // Json::Value jsonResponse;
    // Json::CharReaderBuilder jsonReader;
    // std::istringstream responseStream(postResponse);
    // if (Json::parseFromStream(jsonReader, responseStream, &jsonResponse, nullptr))
    // {
    //     // Extract values from the parsed JSON
    //     mAccessToken          = jsonResponse["access_token"].asString();
    //     mAccessTokenExpiresIn = jsonResponse["expires_in"].asInt();

    //     ChipLogProgress(AppServer, "ExampleAPI::Login Access token %s", mAccessToken.c_str());
    // }

    return Status::Success;
}

Status ExampleAPI::GetSchedule()
{
    // VwhGetScheduleRequest getScheduleRequest;
    // HttpGetRequest request(mpCurlHandle, getScheduleRequest);

    // Status status = request.HandleGet(mAccessToken, sBaseUrl, mSerialNumber);

    // return status;
    return Status::Success;
}

Status ExampleAPI::GetStates()
{
    // VwhSystemStateRequest getSystemStateRequest;
    // HttpGetRequest request(mpCurlHandle, getSystemStateRequest);

    // Status status = request.HandleGet(mAccessToken, sBaseUrl, mSerialNumber);

    // ChipLogProgress(AppServer, "ExampleAPI::GetStates: temperature %u targetTemperature %u trigger %u",
    //                 getSystemStateRequest.GetTemperature(), getSystemStateRequest.GetTargetTemperature(),
    //                 (int) getSystemStateRequest.GetTrigger());

    // return status;
    return Status::Success;
}
