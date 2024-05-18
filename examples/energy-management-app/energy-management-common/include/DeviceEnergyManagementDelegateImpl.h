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

#include "app/clusters/device-energy-management-server/device-energy-management-server.h"

#include <app/util/config.h>
#include <cstring>

#define DELEGATE_TEST_DATA // TODO: comment out if not needed

using chip::Protocols::InteractionModel::Status;
namespace chip {
namespace app {
namespace Clusters {
namespace DeviceEnergyManagement {

class DEMManufacturerDelegate;

/**
 * The application delegate.
 */
class DeviceEnergyManagementDelegate : public DeviceEnergyManagement::Delegate
{
public:
    DeviceEnergyManagementDelegate();

    void SetDeviceEnergyManagementInstance(DeviceEnergyManagement::Instance & instance);
    uint32_t HasFeature(Feature feature) const;

    void SetDemManufacturerDelegate(DEMManufacturerDelegate & deviceEnergyManagementManufacturerDelegate);

    virtual Status PowerAdjustRequest(const int64_t power, const uint32_t duration, AdjustmentCauseEnum cause) override;
    virtual Status CancelPowerAdjustRequest() override;
    virtual Status StartTimeAdjustRequest(const uint32_t requestedStartTime, AdjustmentCauseEnum cause) override;
    virtual Status PauseRequest(const uint32_t duration, AdjustmentCauseEnum cause) override;
    virtual Status ResumeRequest() override;
    virtual Status
    ModifyForecastRequest(const uint32_t forecastId,
                          const DataModel::DecodableList<Structs::SlotAdjustmentStruct::DecodableType> & slotAdjustments,
                          AdjustmentCauseEnum cause) override;
    virtual Status
    RequestConstraintBasedForecast(const DataModel::DecodableList<Structs::ConstraintsStruct::DecodableType> & constraints,
                                   AdjustmentCauseEnum cause) override;
    virtual Status CancelRequest() override;

    // ------------------------------------------------------------------
    // Get attribute methods
    virtual ESATypeEnum GetESAType() override;
    virtual bool GetESACanGenerate() override;
    virtual ESAStateEnum GetESAState() override;
    virtual int64_t GetAbsMinPower() override;
    virtual int64_t GetAbsMaxPower() override;
    virtual Attributes::PowerAdjustmentCapability::TypeInfo::Type GetPowerAdjustmentCapability() override;
    virtual DataModel::Nullable<Structs::ForecastStruct::Type> GetForecast() override;
    virtual OptOutStateEnum GetOptOutState() override;

    // ------------------------------------------------------------------
    // Set attribute methods
    virtual CHIP_ERROR SetESAType(ESATypeEnum) override;
    virtual CHIP_ERROR SetESACanGenerate(bool) override;
    virtual CHIP_ERROR SetESAState(ESAStateEnum) override;
    virtual CHIP_ERROR SetAbsMinPower(int64_t) override;
    virtual CHIP_ERROR SetAbsMaxPower(int64_t) override;
    virtual CHIP_ERROR SetPowerAdjustmentCapability(Attributes::PowerAdjustmentCapability::TypeInfo::Type) override;
    virtual CHIP_ERROR SetForecast(DataModel::Nullable<Structs::ForecastStruct::Type>) override;
    virtual CHIP_ERROR SetOptOutState(OptOutStateEnum) override;

private:
    // Methods to handle when a PowerAdjustment completes
    static void PowerAdjustTimerExpiry(System::Layer * systemLayer, void * delegate);
    void HandlePowerAdjustTimerExpiry();

    // Method to cancel a PowerAdjustment
    CHIP_ERROR CancelPowerAdjustRequestAndSendEvent(CauseEnum cause);

    // Method to send a PowerAdjustEnd event
    CHIP_ERROR SendPowerAdjustEndEvent(CauseEnum cause);

    // Methods to handle when a PauseRequest completes
    static void PauseRequestTimerExpiry(System::Layer * systemLayer, void * delegate);
    void HandlePauseRequestTimerExpiry();

    // Method to cancel a PauseRequest
    CHIP_ERROR CancelPauseRequestAndSendEvent(CauseEnum cause);

    // Method to send a Paused event
    CHIP_ERROR SendResumedEvent(CauseEnum cause);

private:
    DeviceEnergyManagement::Instance *mpDEMInstance;
    
    DEMManufacturerDelegate *mpDEMManufacturerDelegate;

    ESATypeEnum mEsaType;
    bool mEsaCanGenerate;
    ESAStateEnum mEsaState;
    int64_t mAbsMinPower;
    int64_t mAbsMaxPower;
    OptOutStateEnum mOptOutState;

    Attributes::PowerAdjustmentCapability::TypeInfo::Type mPowerAdjustmentCapability;
    DataModel::Nullable<Structs::ForecastStruct::Type> mForecast;

    // Keep track whether a PowerAdjustment is in progress
    bool mPowerAdjustmentInProgress;

    // Keep track of when that PowerAdjustment started
    uint32_t mPowerAdjustmentStartTime;

    // Keep track whether a PauseRequest is in progress
    bool mPauseRequestInProgress;

    // Keep track of when that PauseRequest started
    uint32_t mPauseRequestStartTime;
};

} // namespace DeviceEnergyManagement
} // namespace Clusters
} // namespace app
} // namespace chip
