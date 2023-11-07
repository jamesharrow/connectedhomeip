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

#include "DefaultEnergyEvseDelegate.h"

using namespace chip::app::Clusters::EnergyEvse;

CHIP_ERROR DefaultEnergyEvseDelegate::Disable()
{
    ChipLogProgress(NotSpecified, "Evse Disable not implemented");
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR DefaultEnergyEvseDelegate::EnableCharging(const chip::app::DataModel::Nullable<uint32_t> & enableChargeTime,
                                                     const uint32_t & minimumChargeCurrent, const uint32_t & maximumChargeCurrent)
{
    ChipLogProgress(NotSpecified, "Evse EnableCharging not implemented");
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR DefaultEnergyEvseDelegate::EnableDischarging(const chip::app::DataModel::Nullable<uint32_t> & enableDischargeTime,
                                                        const uint32_t & maximumDischargeCurrent)
{
    ChipLogProgress(NotSpecified, "Evse EnableDischarging not implemented");
    return CHIP_ERROR_NOT_IMPLEMENTED;
}

CHIP_ERROR DefaultEnergyEvseDelegate::StartDiagnostics()
{
    ChipLogProgress(NotSpecified, "Evse Diagnostics not implemented");
    return CHIP_ERROR_NOT_IMPLEMENTED;
}