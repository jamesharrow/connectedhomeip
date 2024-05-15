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

#include <utils.h>

#include <app/clusters/device-energy-management-server/DeviceEnergyManagementTestEventTriggerHandler.h>

#include <DeviceEnergyManagementManufacturerImpl.h>
#include <DeviceEnergyManagementDelegateImpl.h>


using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::DeviceEnergyManagement;
using namespace chip::app::Clusters::DeviceEnergyManagement::Attributes;

DeviceEnergyManagementManufacturerImpl & DeviceEnergyManagementManufacturerImpl::GetInstance()
{
    static DeviceEnergyManagementManufacturerImpl sInstance;

    return sInstance;
}

DeviceEnergyManagementManufacturerImpl::DeviceEnergyManagementManufacturerImpl()
{
}

DeviceEnergyManagementManufacturerImpl::~DeviceEnergyManagementManufacturerImpl()
{
}

Status DeviceEnergyManagementManufacturerImpl::Configure(DeviceEnergyManagementDelegate &demDelegate)
{
    mpDemDelgate = &demDelegate;

    Status status = ConfigureForecast();

    return status;
}

Status DeviceEnergyManagementManufacturerImpl::ConfigureForecast()
{
    uint32_t chipEpoch = 0;

    CHIP_ERROR err = UtilsGetEpochTS(chipEpoch);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Support, "DeviceEnergyManagementManufacturerImpl::ConfigureForecast could not get time");
        return Status::Failure;
    }

    mForecastStruct.startTime = static_cast<uint32_t>(chipEpoch); // planned start time, in UTC, for the entire Forecast.

    // earliest start time, in UTC, that the entire Forecast can be shifted to. null value indicates that it can be started
    // immediately.
    mForecastStruct.earliestStartTime = Optional<DataModel::Nullable<uint32_t>>{ DataModel::Nullable<uint32_t>{ chipEpoch } };
    mForecastStruct.endTime           = static_cast<uint32_t>(chipEpoch * 3); // planned end time, in UTC, for the entire Forecast.
    mForecastStruct.latestEndTime =
        Optional<uint32_t>(static_cast<uint32_t>(chipEpoch * 3)); // latest end time, in UTC, for the entire Forecast

    mForecastStruct.isPauseable = true;

    mSlots[0].minDuration = 10;
    mSlots[0].maxDuration = 20;

    // time to when the tariff is good,
    mSlots[0].defaultDuration =15;
    mSlots[0].elapsedSlotTime = 0;
    mSlots[0].remainingSlotTime = 0;
    mSlots[0].slotIsPauseable.SetValue(true);
    mSlots[0].minPauseDuration.SetValue(2);
    mSlots[0].maxPauseDuration.SetValue(10);

    // minPauseDuration=maxPauseDuration=null,
    mSlots[0].nominalPower.SetValue(0);
    mSlots[0].minPower = mSlots[1].minPower;
    mSlots[0].maxPower = mSlots[1].maxPower;

    // Slot 2 has e.g.
    // set by battery charge capacity
    mSlots[1].nominalEnergy.SetValue(20000 * 1000);

    // nominalPower set by a reasonable charge rate for efficiency
    mSlots[1].nominalPower.SetValue(20000 * 1000);

    // maxPower set by EV and EVSE etc capability
    mSlots[1].maxPower.SetValue(70000 * 1000);

    // minPower set by EVSE and EV capability
    mSlots[1].minPower.SetValue(2300 * 1000);

    mSlots[1].minDuration     = 20;
    mSlots[1].maxDuration     = 40;
    mSlots[1].defaultDuration = 30;

    //    P = E * T
    //72000 * 3600
    // elapsedSlotTime and remainingSlotTime start as null and show live values as the slot is reached

    mSlots[1].slotIsPauseable.SetValue(false);
    mSlots[1].minPauseDuration.SetValue(2);
    mSlots[1].maxPauseDuration.SetValue(10);

    // no clue on costs(omit),

    mSlots[1].minPowerAdjustment.SetValue(mSlots[1].minPower.Value());
    mSlots[1].maxPowerAdjustment.SetValue(mSlots[1].maxPower.Value());
    mSlots[1].minDurationAdjustment.SetValue(mSlots[1].minDuration);
    mSlots[1].maxDurationAdjustment.SetValue(mSlots[1].maxDuration);

    mForecastStruct.activeSlotNumber.SetNonNull<uint16_t>(0);

    mForecastStruct.slots = DataModel::List<const Structs::SlotStruct::Type>(mSlots, 2);

    DataModel::Nullable<Structs::ForecastStruct::Type> forecast(mForecastStruct);

    mpDemDelgate->SetForecast(forecast);

    return Status::Success;
}

bool DeviceEnergyManagementManufacturerImpl::HandleDeviceEnergyManagementTestEventTrigger(uint64_t eventTrigger)
{
    DeviceEnergyManagementTrigger trigger = static_cast<DeviceEnergyManagementTrigger>(eventTrigger);

    switch (trigger)
    {
    case DeviceEnergyManagementTrigger::kPowerAdjustment:
        ChipLogProgress(Support, "[PowerAdjustment-Test-Event] => Create PowerAdjustment struct");
        SetTestEventTrigger_PowerAdjustment();
        break;
    case DeviceEnergyManagementTrigger::kPowerAdjustmentClear:
        ChipLogProgress(Support, "[PowerAdjustmentClear-Test-Event] => Clear PowerAdjustment struct");
        SetTestEventTrigger_PowerAdjustClear();
        break;
    case DeviceEnergyManagementTrigger::kUserOptOutLocalOptimization:
        ChipLogProgress(Support, "[UserOptOutLocalOptimization-Test-Event] => Set User opt-out Local Optimization");
        SetTestEventTrigger_UserOptOutOptimization(OptOutStateEnum::kLocalOptOut);
        break;
    case DeviceEnergyManagementTrigger::kUserOptOutGridOptimization:
        ChipLogProgress(Support, "[UserOptOutGrisOptimization-Test-Event] => Set User opt-out Grid Optimization");
        SetTestEventTrigger_UserOptOutOptimization(OptOutStateEnum::kGridOptOut);
        break;
    case DeviceEnergyManagementTrigger::kUserOptOutClearAll:
        ChipLogProgress(Support, "[UserOptOutClearAll-Test-Event] => Clear all User opt-outs");
        SetTestEventTrigger_UserOptOutOptimization(OptOutStateEnum::kNoOptOut);
        break;
    case DeviceEnergyManagementTrigger::kStartTimeAdjustment:
        ChipLogProgress(Support, "[StartTimeAdjustment-Test-Event] => Create simulated forecast to allow StartTimeAdjustment");
        SetTestEventTrigger_StartTimeAdjustment();
        break;
    case DeviceEnergyManagementTrigger::kStartTimeAdjustmentClear:
        ChipLogProgress(Support, "[StartTimeAdjustmentClear-Test-Event] => Clear StartTimeAdjustment forecast");
        SetTestEventTrigger_StartTimeAdjustmentClear();
        break;
    case DeviceEnergyManagementTrigger::kPausable:
        ChipLogProgress(Support, "[Pausable-Test-Event] => Create Pausable forecast");
        // Already configured in ConfigureForecast()
        break;
    case DeviceEnergyManagementTrigger::kPausableNextSlot:
        ChipLogProgress(Support, "[PausableNextSlot-Test-Event] => Move to next Pausable slot in forecast");
        SetTestEventTrigger_PausableNextSlot();
        break;
    case DeviceEnergyManagementTrigger::kPausableClear:
        ChipLogProgress(Support, "[PausableClear-Test-Event] => Clear Pausable forecast");
        // TODO call implementation - NOTHING TO DO?
        break;

    default:
        return false;
    }

    return true;
}

void DeviceEnergyManagementManufacturerImpl::SetTestEventTrigger_PowerAdjustment()
{
    mPowerAdjustments[0].minPower =  5000 * 1000; // 5kW
    mPowerAdjustments[0].maxPower = 30000 * 1000; // 30kW
    mPowerAdjustments[0].minDuration = 30;        // 30s
    mPowerAdjustments[0].maxDuration = 60;        // 60s

    DataModel::List<const Structs::PowerAdjustStruct::Type> powerAdjustmentList(mPowerAdjustments, 1);

    CHIP_ERROR err = mpDemDelgate->SetPowerAdjustmentCapability(MakeNullable(powerAdjustmentList));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Support, "SetTestEventTrigger_PowerAdjustment failed %s", chip::ErrorStr(err));
    }
}

void DeviceEnergyManagementManufacturerImpl::SetTestEventTrigger_PowerAdjustClear()
{
    mPowerAdjustments[0].minPower = 0;
    mPowerAdjustments[0].maxPower = 0;
    mPowerAdjustments[0].minDuration = 0;
    mPowerAdjustments[0].maxDuration = 0;

    DataModel::List<const Structs::PowerAdjustStruct::Type> powerAdjustmentList(mPowerAdjustments, 1);

    CHIP_ERROR err = mpDemDelgate->SetPowerAdjustmentCapability(MakeNullable(powerAdjustmentList));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Support, "SetTestEventTrigger_PowerAdjustment failed %s", chip::ErrorStr(err));
    }
}

void DeviceEnergyManagementManufacturerImpl::SetTestEventTrigger_StartTimeAdjustment()
{
    // Get the current forecast ad update the earliestStartTime and latestEndTime
    mForecastStruct = mpDemDelgate->GetForecast().Value();

    uint32_t chipEpoch = 0;

    CHIP_ERROR err = UtilsGetEpochTS(chipEpoch);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Support, "DeviceEnergyManagementManufacturerImpl::ConfigureForecast_EarliestStartLatestEndTimes could not get time");
    }

    // planned start time, in UTC, for the entire Forecast.
    mForecastStruct.startTime = static_cast<uint32_t>(chipEpoch);

    // Set the earliest start time, in UTC, to that before the startTime
    mForecastStruct.earliestStartTime = Optional<DataModel::Nullable<uint32_t>>{ DataModel::Nullable<uint32_t>{ chipEpoch - 60 } };

    // Planned end time, in UTC, for the entire Forecast.
    mForecastStruct.endTime = static_cast<uint32_t>(chipEpoch * 3);

    // Latest end time, in UTC, for the entire Forecast which is > mForecastStruct.endTime
    mForecastStruct.latestEndTime = Optional<uint32_t>(static_cast<uint32_t>(chipEpoch * 3 + 60));

    DataModel::Nullable<Structs::ForecastStruct::Type> forecast(mForecastStruct);

    mpDemDelgate->SetForecast(forecast);
}

void DeviceEnergyManagementManufacturerImpl::SetTestEventTrigger_StartTimeAdjustmentClear()
{
    // Get the current forecast ad update the earliestStartTime and latestEndTime
    mForecastStruct = mpDemDelgate->GetForecast().Value();

    mForecastStruct.startTime = static_cast<uint32_t>(0);
    mForecastStruct.endTime = static_cast<uint32_t>(0);

    mForecastStruct.earliestStartTime = Optional<DataModel::Nullable<uint32_t>>();
    mForecastStruct.latestEndTime = Optional<uint32_t>();

    DataModel::Nullable<Structs::ForecastStruct::Type> forecast(mForecastStruct);

    mpDemDelgate->SetForecast(forecast);
}

void DeviceEnergyManagementManufacturerImpl::SetTestEventTrigger_UserOptOutOptimization(OptOutStateEnum optOutState)
{
    mpDemDelgate->SetOptOutState(optOutState);
}

void DeviceEnergyManagementManufacturerImpl::SetTestEventTrigger_PausableNextSlot()
{
    // Get the current forecast ad update the active slot number
    mForecastStruct = mpDemDelgate->GetForecast().Value();
    mForecastStruct.activeSlotNumber.SetNonNull(1);

    DataModel::Nullable<Structs::ForecastStruct::Type> forecast(mForecastStruct);

    mpDemDelgate->SetForecast(forecast);
}

bool HandleDeviceEnergyManagementTestEventTrigger(uint64_t eventTrigger)
{
    return DeviceEnergyManagementManufacturerImpl::GetInstance().HandleDeviceEnergyManagementTestEventTrigger(eventTrigger);
}
