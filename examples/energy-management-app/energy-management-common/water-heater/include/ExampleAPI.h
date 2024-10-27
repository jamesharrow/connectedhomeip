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

#pragma once

#include <platform/CHIPDeviceLayer.h>
#include <string.h>
#include <system/SystemLayer.h>

#include <protocols/interaction_model/StatusCode.h>

extern std::string sBaseUrl;
namespace chip {
namespace app {
namespace Clusters {
namespace WaterHeaterManagement {

class CURL;

class ExampleAPI
{
public:
    ExampleAPI();
    ~ExampleAPI();

    void SetSerialNumber(const char * pSerialNumber);
    std::string GetSerialNumber() { return mSerialNumber; }

    Protocols::InteractionModel::Status GetCurrentTemperature();
    Protocols::InteractionModel::Status GetTargetTemperature();

    Protocols::InteractionModel::Status BoostActivate();
    Protocols::InteractionModel::Status BoostDeactivate();

    Protocols::InteractionModel::Status GetSchedule();
    Protocols::InteractionModel::Status GetStates();

    Protocols::InteractionModel::Status Login();

    static size_t WriteCallback(void * contents, size_t size, size_t nmemb, void * userp);

private:
    Protocols::InteractionModel::Status SetBoost(bool on);

    static void GetPowerMeasurementsTimer(System::Layer * systemLayer, void * delegate);
    static void GetDataTimer(System::Layer * systemLayer, void * delegate);

    void GetPowerMeasurements();
    void GetHeaterData();

private:
    CURL * mpCurlHandle;
    std::string mSerialNumber;
    std::string mBaseUrl;
};

} // namespace WaterHeaterManagement
} // namespace Clusters
} // namespace app
} // namespace chip
