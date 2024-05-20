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

#include <EVSEManufacturerImpl.h>
#include <DeviceEnergyManagementDelegateImpl.h>
#include <app/clusters/device-energy-management-server/DeviceEnergyManagementTestEventTriggerHandler.h>

#include <utils.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::DeviceEnergyManagement;

static DeviceEnergyManagement::Structs::SlotStruct::Type sSlots[2];
static DeviceEnergyManagement::Structs::ForecastStruct::Type sForecastStruct;
static DeviceEnergyManagement::Structs::PowerAdjustStruct::Type sPowerAdjustments[1];

DeviceEnergyManagementDelegate * GetDEMDelegate()
{
    EVSEManufacturer * mn = GetEvseManufacturer();
    VerifyOrDieWithMsg(mn != nullptr, AppServer, "EVSEManufacturer is null");
    DeviceEnergyManagementDelegate * dg = mn->GetDEMDelegate();
    VerifyOrDieWithMsg(dg != nullptr, AppServer, "DEM Delegate is null");

    return dg;
}

CHIP_ERROR ConfigureForecast()
{
    uint32_t chipEpoch = 0;

    CHIP_ERROR err = UtilsGetEpochTS(chipEpoch);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Support, "ConfigureForecast could not get time");
        return err;
    }

    sForecastStruct.startTime = static_cast<uint32_t>(chipEpoch); // planned start time, in UTC, for the entire Forecast.

    // earliest start time, in UTC, that the entire Forecast can be shifted to. null value indicates that it can be started
    // immediately.
    sForecastStruct.earliestStartTime = Optional<DataModel::Nullable<uint32_t>>{ DataModel::Nullable<uint32_t>{ chipEpoch } };
    sForecastStruct.endTime           = static_cast<uint32_t>(chipEpoch * 3); // planned end time, in UTC, for the entire Forecast.
    sForecastStruct.latestEndTime =
        Optional<uint32_t>(static_cast<uint32_t>(chipEpoch * 3)); // latest end time, in UTC, for the entire Forecast

    sForecastStruct.isPauseable = true;

    sSlots[0].minDuration = 10;
    sSlots[0].maxDuration = 20;

    // time to when the tariff is good,
    sSlots[0].defaultDuration =15;
    sSlots[0].elapsedSlotTime = 0;
    sSlots[0].remainingSlotTime = 0;
    sSlots[0].slotIsPauseable.SetValue(true);
    sSlots[0].minPauseDuration.SetValue(2);
    sSlots[0].maxPauseDuration.SetValue(10);

    // minPauseDuration=maxPauseDuration=null,
    sSlots[0].nominalPower.SetValue(0);
    sSlots[0].minPower = sSlots[1].minPower;
    sSlots[0].maxPower = sSlots[1].maxPower;

    // Slot 2 has e.g.
    // set by battery charge capacity
    sSlots[1].nominalEnergy.SetValue(20000 * 1000);

    // nominalPower set by a reasonable charge rate for efficiency
    sSlots[1].nominalPower.SetValue(20000 * 1000);

    // maxPower set by EV and EVSE etc capability
    sSlots[1].maxPower.SetValue(70000 * 1000);

    // minPower set by EVSE and EV capability
    sSlots[1].minPower.SetValue(2300 * 1000);

    sSlots[1].minDuration     = 20;
    sSlots[1].maxDuration     = 40;
    sSlots[1].defaultDuration = 30;

    //    P = E * T
    //72000 * 3600
    // elapsedSlotTime and remainingSlotTime start as null and show live values as the slot is reached

    sSlots[1].slotIsPauseable.SetValue(false);
    sSlots[1].minPauseDuration.SetValue(2);
    sSlots[1].maxPauseDuration.SetValue(10);

    // no clue on costs(omit),

    sSlots[1].minPowerAdjustment.SetValue(sSlots[1].minPower.Value());
    sSlots[1].maxPowerAdjustment.SetValue(sSlots[1].maxPower.Value());
    sSlots[1].minDurationAdjustment.SetValue(sSlots[1].minDuration);
    sSlots[1].maxDurationAdjustment.SetValue(sSlots[1].maxDuration);

    sForecastStruct.activeSlotNumber.SetNonNull<uint16_t>(0);

    sForecastStruct.slots = DataModel::List<const DeviceEnergyManagement::Structs::SlotStruct::Type>(sSlots, 2);

    DataModel::Nullable<DeviceEnergyManagement::Structs::ForecastStruct::Type> forecast(sForecastStruct);

    EVSEManufacturer * mn = GetEvseManufacturer();
    mn->GetDEMDelegate()->SetForecast(forecast);

    return CHIP_NO_ERROR;
}

void SetTestEventTrigger_PowerAdjustment()
{
    sPowerAdjustments[0].minPower =  5000 * 1000; // 5kW
    sPowerAdjustments[0].maxPower = 30000 * 1000; // 30kW
    sPowerAdjustments[0].minDuration = 30;        // 30s
    sPowerAdjustments[0].maxDuration = 60;        // 60s

    DataModel::List<const DeviceEnergyManagement::Structs::PowerAdjustStruct::Type> powerAdjustmentList(sPowerAdjustments, 1);

    CHIP_ERROR err = GetDEMDelegate()->SetPowerAdjustmentCapability(MakeNullable(powerAdjustmentList));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Support, "SetTestEventTrigger_PowerAdjustment failed %s", chip::ErrorStr(err));
    }
}

void SetTestEventTrigger_PowerAdjustClear()
{
    sPowerAdjustments[0].minPower = 0;
    sPowerAdjustments[0].maxPower = 0;
    sPowerAdjustments[0].minDuration = 0;
    sPowerAdjustments[0].maxDuration = 0;

    DataModel::List<const DeviceEnergyManagement::Structs::PowerAdjustStruct::Type> powerAdjustmentList(sPowerAdjustments, 1);

    CHIP_ERROR err = GetDEMDelegate()->SetPowerAdjustmentCapability(MakeNullable(powerAdjustmentList));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Support, "SetTestEventTrigger_PowerAdjustment failed %s", chip::ErrorStr(err));
    }
}

void SetTestEventTrigger_StartTimeAdjustment()
{
    // Get the current forecast ad update the earliestStartTime and latestEndTime
    sForecastStruct = GetDEMDelegate()->GetForecast().Value();

    uint32_t chipEpoch = 0;

    CHIP_ERROR err = UtilsGetEpochTS(chipEpoch);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(Support, "ConfigureForecast_EarliestStartLatestEndTimes could not get time");
    }

    // planned start time, in UTC, for the entire Forecast.
    sForecastStruct.startTime = static_cast<uint32_t>(chipEpoch);

    // Set the earliest start time, in UTC, to that before the startTime
    sForecastStruct.earliestStartTime = Optional<DataModel::Nullable<uint32_t>>{ DataModel::Nullable<uint32_t>{ chipEpoch - 60 } };

    // Planned end time, in UTC, for the entire Forecast.
    sForecastStruct.endTime = static_cast<uint32_t>(chipEpoch * 3);

    // Latest end time, in UTC, for the entire Forecast which is > sForecastStruct.endTime
    sForecastStruct.latestEndTime = Optional<uint32_t>(static_cast<uint32_t>(chipEpoch * 3 + 60));

    DataModel::Nullable<DeviceEnergyManagement::Structs::ForecastStruct::Type> forecast(sForecastStruct);

    GetDEMDelegate()->SetForecast(forecast);
}

void SetTestEventTrigger_StartTimeAdjustmentClear()
{
    // Get the current forecast ad update the earliestStartTime and latestEndTime
    sForecastStruct = GetDEMDelegate()->GetForecast().Value();

    sForecastStruct.startTime = static_cast<uint32_t>(0);
    sForecastStruct.endTime = static_cast<uint32_t>(0);

    sForecastStruct.earliestStartTime = Optional<DataModel::Nullable<uint32_t>>();
    sForecastStruct.latestEndTime = Optional<uint32_t>();

    DataModel::Nullable<DeviceEnergyManagement::Structs::ForecastStruct::Type> forecast(sForecastStruct);

    GetDEMDelegate()->SetForecast(forecast);
}

void SetTestEventTrigger_UserOptOutOptimization(OptOutStateEnum optOutState)
{
    GetDEMDelegate()->SetOptOutState(optOutState);
}

void SetTestEventTrigger_PausableNextSlot()
{
    // Get the current forecast ad update the active slot number
    sForecastStruct = GetDEMDelegate()->GetForecast().Value();
    sForecastStruct.activeSlotNumber.SetNonNull(1);

    DataModel::Nullable<DeviceEnergyManagement::Structs::ForecastStruct::Type> forecast(sForecastStruct);

    GetDEMDelegate()->SetForecast(forecast);
}

void SetTestEventTrigger_ForecastAdjustment()
{
    // The following values need to match the equivalent values in src/python_testing/TC_DEM_2_5.py
    sForecastStruct = GetDEMDelegate()->GetForecast().Value();
    sSlots[0].minPowerAdjustment.SetValue(20);
    sSlots[0].maxPowerAdjustment.SetValue(2000);
    sSlots[0].minDurationAdjustment.SetValue(120);
    sSlots[0].maxDurationAdjustment.SetValue(240);

    sForecastStruct.slots = DataModel::List<const DeviceEnergyManagement::Structs::SlotStruct::Type>(sSlots, 2);

    DataModel::Nullable<DeviceEnergyManagement::Structs::ForecastStruct::Type> forecast(sForecastStruct);

    GetDEMDelegate()->SetForecast(forecast);
}

void SetTestEventTrigger_ForecastAdjustmentNextSlot()
{
    sForecastStruct = GetDEMDelegate()->GetForecast().Value();
    sForecastStruct.activeSlotNumber.SetNonNull(sForecastStruct.activeSlotNumber.Value() + 1);
    DataModel::Nullable<DeviceEnergyManagement::Structs::ForecastStruct::Type> forecast(sForecastStruct);

    GetDEMDelegate()->SetForecast(forecast);
}

void SetTestEventTrigger_ForecastAdjustmentClear()
{
    // ModifyForecastRequest with:
    //     ForecastId=Forecast.ForecastId+1,
    //     SlotAdjustments[0].{SlotIndex=0, NominalPower=Forecast.Slots[0].MinPowerAdjustment, Duration=Forecast.Slots[0].MaxDurationAdjustment},
    //     Cause=GridOptimization.

    return;
    sForecastStruct = GetDEMDelegate()->GetForecast().Value();
    sForecastStruct.activeSlotNumber.SetNonNull(sForecastStruct.activeSlotNumber.Value() + 1);
    DataModel::Nullable<DeviceEnergyManagement::Structs::ForecastStruct::Type> forecast(sForecastStruct);

    GetDEMDelegate()->SetForecast(forecast);
}

bool HandleDeviceEnergyManagementTestEventTrigger(uint64_t eventTrigger)
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
    case DeviceEnergyManagementTrigger::kForecastAdjustment:
        ChipLogProgress(Support, "[ForecastAdjustment-Test-Event] => Forecast Adjustment");
        SetTestEventTrigger_ForecastAdjustment();
        break;
    case DeviceEnergyManagementTrigger::kForecastAdjustmentNextSlot:
        ChipLogProgress(Support, "[ForecastAdjustmentNextSlot-Test-Event] => Forecast Adjustment");
        SetTestEventTrigger_ForecastAdjustmentNextSlot();
        break;
    case DeviceEnergyManagementTrigger::kForecastAdjustmentClear:
        ChipLogProgress(Support, "[ForecastAdjustmentClear-Test-Event] => Forecast Adjustment");
        SetTestEventTrigger_ForecastAdjustmentClear();
        break;

    default:
        return false;
    }

    return true;
}


