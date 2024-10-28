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

    mAccessToken = "";

    VerifyOrDie(Login() == Status::Success);

    std::string tanksUrl;
    VerifyOrDie(HandleGetHref(sBaseUrl, "tanks", tanksUrl) == Status::Success);

    // Find our tank specific URL
    VerifyOrDie(HandleGetTanksURL(tanksUrl, mTankUrl) == Status::Success);
    VerifyOrDie(HandleGetHref(mTankUrl, "latest_measurement", mMeasurementUrl) == Status::Success);

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

    Logout();

    DeviceLayer::SystemLayer().CancelTimer(GetPowerMeasurementsTimer, this);
    DeviceLayer::SystemLayer().CancelTimer(GetDataTimer, this);
}

Status ExampleAPI::Login()
{
    Status status = Status::Success;

    if (mAccessToken != "")
    {
        ChipLogError(AppServer, "ExampleAPI::Login - mAccessToken already set and not logged out");
        return Status::Failure;
    }

    CURL * pCurlHandle = curl_easy_init();
    if (pCurlHandle == nullptr)
    {
        ChipLogError(AppServer, "ExampleAPI::Login failed to initialised CURL");
        chipDie();
    }

    std::string data     = "{\"username\": \"" + sUsername + "\", \"password\": \"" + sPassword + "\"}";
    std::string loginUrl = sBaseUrl + "/account/login";
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, loginUrl.c_str());

    std::string postResponse;
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    // Set HTTP POST method
    curl_easy_setopt(pCurlHandle, CURLOPT_POST, 1L);

    // Set headers
    struct curl_slist * headers = NULL;
    headers                     = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPHEADER, headers);

    // Set JSON data
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDS, data.c_str());

    // Perform the POST request
    CURLcode res = curl_easy_perform(pCurlHandle);
    // Check for errors
    if (res != CURLE_OK)
    {
        ChipLogError(AppServer, "ExampleAPI::Login: request failed %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return Status::Failure;
    }

    //  Parse the JSON response
    Json::Value jsonResponse;
    Json::CharReaderBuilder jsonReader;
    std::istringstream responseStream(postResponse);
    if (Json::parseFromStream(jsonReader, responseStream, &jsonResponse, nullptr))
    {
        // Extract values from the parsed JSON
        mAccessToken = jsonResponse["token"].asString();

        ChipLogProgress(AppServer, "ExampleAPI::Login Access token %s", mAccessToken.c_str());
    }

    if (mAccessToken == "")
    {
        ChipLogProgress(AppServer, "ExampleAPI::Login: No token received - Login Failure!");
        status = Status::Failure;
    }
    else
    {
        ChipLogProgress(AppServer, "ExampleAPI::Login: Login successful!");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(pCurlHandle);

    return status;
}

Status ExampleAPI::Logout()
{
    Status status = Status::Success;

    if (mAccessToken == "")
    {
        ChipLogProgress(AppServer, "Access Token empty - so cannot logout!");
        return Status::Failure;
    }

    CURL * pCurlHandle = curl_easy_init();
    if (pCurlHandle == nullptr)
    {
        ChipLogError(AppServer, "ExampleAPI::Logout failed to initialised CURL");
        return Status::Failure;
    }

    std::string loginUrl = sBaseUrl + "/account/login";
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, loginUrl.c_str());

    std::string postResponse;
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    // Set HTTP DELETE method
    curl_easy_setopt(pCurlHandle, CURLOPT_CUSTOMREQUEST, "DELETE");

    // Set headers
    struct curl_slist * headers = NULL;
    std::string authStr         = "Authorization: Bearer " + mAccessToken;
    headers                     = curl_slist_append(headers, authStr.c_str());

    // Set JSON data
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDS, "");

    // Perform the POST request
    CURLcode res = curl_easy_perform(pCurlHandle);
    // Check for errors
    if (res != CURLE_OK)
    {
        ChipLogError(AppServer, "ExampleAPI::Logout: request failed %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return Status::Failure;
    }

    ChipLogProgress(AppServer, "ExampleAPI::Login: Logout successful!");
    mAccessToken = "";

    curl_slist_free_all(headers);
    curl_easy_cleanup(pCurlHandle);

    return status;
}

// This API fetches a resource to get a child key looking for the _href element
Status ExampleAPI::HandleGetHref(std::string baseUrl, std::string resource, std::string & link)
{
    Status status = Status::Success;
    CURLcode res;
    struct curl_slist * headers = NULL;

    CURL * pCurlHandle = curl_easy_init();
    if (pCurlHandle == nullptr)
    {
        ChipLogError(AppServer, "ExampleAPI::HandleGetHref failed to initialised CURL");
        return Status::Failure;
    }

    curl_easy_setopt(pCurlHandle, CURLOPT_URL, baseUrl.c_str());

    std::string authStr = "Authorization: Bearer " + mAccessToken;
    headers             = curl_slist_append(headers, authStr.c_str());

    std::string postResponse;
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPGET, 1L);

    // Perform the GET request
    res = curl_easy_perform(pCurlHandle);

    // Check for errors
    if (res != CURLE_OK)
    {
        ChipLogError(AppServer, "HttpGetRequest::HandleGetHref: request failed %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return Status::Failure;
    }

    // ChipLogProgress(AppServer, "HttpGetRequest::HandleGetHref: response %s", postResponse.c_str());

    //  Parse the JSON response
    Json::Value jsonResponse;
    Json::CharReaderBuilder jsonReader;
    std::istringstream responseStream(postResponse);
    std::string errs;

    if (!Json::parseFromStream(jsonReader, responseStream, &jsonResponse, nullptr))
    {
        ChipLogError(AppServer, "HttpGetRequest::HandleGetHref request failed to parse responseStream %s", errs.c_str());
        curl_slist_free_all(headers);
        return Status::Failure;
    }

    if (!jsonResponse.isMember("_links"))
    {
        ChipLogError(AppServer, "HandleGetHref: Could not find _links");
        return Status::Failure;
    }

    if (!jsonResponse["_links"].isMember(resource))
    {
        ChipLogError(AppServer, "HandleGetHref Could not find '%s'", resource.c_str());
        return Status::Failure;
    }

    if (!jsonResponse["_links"][resource].isMember("href"))
    {
        ChipLogError(AppServer, "HandleGetHref Could not find '%s'.href", resource.c_str());
        return Status::Failure;
    }

    link = jsonResponse["_links"][resource]["href"].asString();

    ChipLogProgress(AppServer, "HandleGetHref link is %s", link.c_str());

    curl_slist_free_all(headers);
    curl_easy_cleanup(pCurlHandle);

    return status;
}

Status ExampleAPI::HandleGetTanksURL(std::string baseUrl, std::string & link)
{
    Status status = Status::Success;
    CURLcode res;
    struct curl_slist * headers = NULL;

    CURL * pCurlHandle = curl_easy_init();
    if (pCurlHandle == nullptr)
    {
        ChipLogError(AppServer, "ExampleAPI::HandleGetTanksURL failed to initialised CURL");
        return Status::Failure;
    }

    curl_easy_setopt(pCurlHandle, CURLOPT_URL, baseUrl.c_str());

    std::string authStr = "Authorization: Bearer " + mAccessToken;
    headers             = curl_slist_append(headers, authStr.c_str());

    std::string postResponse;
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPGET, 1L);

    // Perform the GET request
    res = curl_easy_perform(pCurlHandle);

    // Check for errors
    if (res != CURLE_OK)
    {
        ChipLogError(AppServer, "HttpGetRequest::HandleGetTanksURL: request failed %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return Status::Failure;
    }

    ChipLogProgress(AppServer, "HttpGetRequest::HandleGetTanksURL: response %s", postResponse.c_str());

    //  Parse the JSON response
    Json::Value jsonResponse;
    Json::CharReaderBuilder jsonReader;
    std::istringstream responseStream(postResponse);
    std::string errs;

    if (!Json::parseFromStream(jsonReader, responseStream, &jsonResponse, nullptr))
    {
        ChipLogError(AppServer, "HttpGetRequest::HandleGetTanksURL request failed to parse responseStream %s", errs.c_str());
        curl_slist_free_all(headers);
        return Status::Failure;
    }

    if (!jsonResponse.isMember("_embedded"))
    {
        ChipLogError(AppServer, "HandleGetTanksURL: Could not find _embedded");
        return Status::Failure;
    }

    if (!jsonResponse["_embedded"].isMember("tankList"))
    {
        ChipLogError(AppServer, "HandleGetTanksURL Could not find 'tankList'");
        return Status::Failure;
    }

    // Assume 1 tank
    if (!jsonResponse["_embedded"]["tankList"][0].isMember("_links"))
    {
        ChipLogError(AppServer, "HandleGetTanksURL Could not find ._links");
        return Status::Failure;
    }

    if (!jsonResponse["_embedded"]["tankList"][0]["_links"].isMember("self"))
    {
        ChipLogError(AppServer, "HandleGetTanksURL Could not find .self");
        return Status::Failure;
    }

    if (!jsonResponse["_embedded"]["tankList"][0]["_links"]["self"].isMember("href"))
    {
        ChipLogError(AppServer, "HandleGetTanksURL Could not find .href");
        return Status::Failure;
    }

    link = jsonResponse["_embedded"]["tankList"][0]["_links"]["self"]["href"].asString();

    ChipLogProgress(AppServer, "HandleGetTanksURL link is %s", link.c_str());

    curl_slist_free_all(headers);
    curl_easy_cleanup(pCurlHandle);

    return status;
}

void ExampleAPI::GetPowerMeasurements()
{
    static Power_mW oldValue_mW = 0;

    CURL * pCurlHandle = curl_easy_init();
    if (pCurlHandle == nullptr)
    {
        ChipLogError(AppServer, "ExampleAPI::GetPowerMeasurements failed to initialised CURL");
        return;
    }

    std::string powerMeasUrl = sBaseUrl + "/get_mqtt_data";
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, powerMeasUrl.c_str());

    std::string postResponse;
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    // Perform the POST request
    CURLcode res = curl_easy_perform(pCurlHandle);
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
    curl_easy_cleanup(pCurlHandle);
}

void ExampleAPI::GetHeaterData()
{
    CURLcode res;
    struct curl_slist * headers = NULL;

    CURL * pCurlHandle = curl_easy_init();
    if (pCurlHandle == nullptr)
    {
        ChipLogError(AppServer, "ExampleAPI::GetHeaterData failed to initialised CURL");
        return;
    }

    curl_easy_setopt(pCurlHandle, CURLOPT_URL, mMeasurementUrl.c_str());

    std::string authStr = "Authorization: Bearer " + mAccessToken;
    headers             = curl_slist_append(headers, authStr.c_str());

    std::string postResponse;
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPGET, 1L);

    // Perform the GET request
    res = curl_easy_perform(pCurlHandle);

    // Check for errors
    if (res != CURLE_OK)
    {
        ChipLogError(AppServer, "ExampleAPI::GetHeaterData: request failed %s", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        return;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(pCurlHandle);

    //  Parse the JSON response
    Json::Value jsonResponse;
    Json::CharReaderBuilder jsonReader;
    std::istringstream responseStream(postResponse);
    std::string errs;

    if (!Json::parseFromStream(jsonReader, responseStream, &jsonResponse, nullptr))
    {
        ChipLogError(AppServer, "GetHeaterData request failed to parse responseStream %s", errs.c_str());
        return;
    }

    if (!jsonResponse.isMember("topTemperature"))
    {
        ChipLogError(AppServer, "GetHeaterData: Could not find topTemperature");
        return;
    }

    if (!jsonResponse.isMember("charge"))
    {
        ChipLogError(AppServer, "GetHeaterData: Could not find charge");
        return;
    }

    if (!jsonResponse.isMember("state"))
    {
        ChipLogError(AppServer, "GetHeaterData: Could not find state");
        return;
    }

    std::string stateJson = jsonResponse["state"].asString();
    ChipLogProgress(AppServer, "State %s", stateJson.c_str());

    const float temperatureMeasurement = jsonResponse["topTemperature"].asFloat();
    const int estimatedHeatRequiredmWh = 2400;
    const int occupiedSetpoint         = 55;
    const int heaterTypes              = 0x2;
    const int tankPercentage           = static_cast<int>(jsonResponse["charge"].asFloat());
    const int tankVolume               = 120; // TODO work this out from model number

    //  Parse the 'state' JSON response
    Json::Value jsonResponse2;
    Json::CharReaderBuilder jsonReader2;
    std::istringstream responseStream2(stateJson);

    if (!Json::parseFromStream(jsonReader2, responseStream2, &jsonResponse2, nullptr))
    {
        ChipLogError(AppServer, "GetHeaterData request failed to parse responseStream2 %s", errs.c_str());
        return;
    }

    if (!jsonResponse2.isMember("current"))
    {
        ChipLogError(AppServer, "GetHeaterData could not find 'current' in state");
        return;
    }
    if (!jsonResponse2["current"].isMember("immersion"))
    {
        ChipLogError(AppServer, "GetHeaterData could not find 'immersion' in state");
        return;
    }

    std::string onOffStr = jsonResponse2["current"]["immersion"].asString();
    const int heatDemand = (onOffStr == "On") ? 0x2 : 0x0; // Set or Clear bit 1 if On/Off

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
            TemperatureMeasurement::Attributes::MeasuredValue::Set(kTempSensorEndpoint,
                                                                   static_cast<int16_t>(temperatureMeasurement * 100));

            dg->SetTargetWaterTemperature(static_cast<int16_t>(occupiedSetpoint * 100));
            dg->SetEstimatedHeatRequired(static_cast<chip::Energy_mWh>(estimatedHeatRequiredmWh));
            dg->SetTankPercentage(static_cast<chip::Percent>(tankPercentage));
            dg->SetTankVolume(static_cast<uint16_t>(tankVolume));
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

    CURL * pCurlHandle = curl_easy_init();
    if (pCurlHandle == nullptr)
    {
        ChipLogError(AppServer, "ExampleAPI::SetBoost failed to initialised CURL");
        return Status::Failure;
    }

    std::string boostActivateUrl = sBaseUrl + "/toggle_boost_" + pOp;

    curl_easy_setopt(pCurlHandle, CURLOPT_URL, boostActivateUrl.c_str());

    std::string postResponse;
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, ExampleAPI::WriteCallback);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, &postResponse);

    curl_easy_setopt(pCurlHandle, CURLOPT_HTTPHEADER, headers);

    // Set the request body for the GET request
    curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDS, "");

    // Perform the POST request
    res = curl_easy_perform(pCurlHandle);

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
    curl_easy_cleanup(pCurlHandle);
    return Status::Success;
}

// Callback function to handle curl's response
/* static */
size_t ExampleAPI::WriteCallback(void * contents, size_t size, size_t nmemb, void * userp)
{
    ((string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
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
