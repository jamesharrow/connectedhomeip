/*
 *
 *    Copyright (c) 2023 Project CHIP Authors
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

#include <app-common/zap-generated/cluster-objects.h>
#include "DeviceEnergyManagementManufacturer.h"

namespace chip {
namespace app {
namespace Clusters {
namespace DeviceEnergyManagement {

/**
 * Product specific class interface. Manaages all product specific behaviour.
 */
class DeviceEnergyManagementManufacturerImpl : public DeviceEnergyManagementManufacturer
{
public:

    static DeviceEnergyManagementManufacturerImpl & GetInstance();

    Status Configure(DeviceEnergyManagementDelegate &demDelegate) override;

    bool HandleDeviceEnergyManagementTestEventTrigger(uint64_t eventTrigger);

private:
    DeviceEnergyManagementManufacturerImpl();
    virtual ~DeviceEnergyManagementManufacturerImpl();

    Status ConfigureForecast();

    void SetTestEventTrigger_PowerAdjustment();
    void SetTestEventTrigger_PowerAdjustClear();

    void SetTestEventTrigger_StartTimeAdjustment();
    void SetTestEventTrigger_StartTimeAdjustmentClear();

    void SetTestEventTrigger_UserOptOutOptimization(OptOutStateEnum optOutState);

    void SetTestEventTrigger_PausableNextSlot();

private:
    DeviceEnergyManagementDelegate *mpDemDelgate;

    Structs::SlotStruct::Type mSlots[2];
    Structs::ForecastStruct::Type mForecastStruct;

    Structs::PowerAdjustStruct::Type mPowerAdjustments[1];

};

} // namespace DeviceEnergyManagement
} // namespace Clusters
} // namespace app
} // namespace chip
