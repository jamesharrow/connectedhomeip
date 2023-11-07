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

#include <EnergyEvseDelegateImpl.h>

#include <app/util/config.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::EnergyEvse;

EnergyEvseDelegate EnergyEvseDelegate::sInstance;

CHIP_ERROR EnergyEvseDelegate::Disable()
{
    ChipLogProgress(AppServer, "EnergyEvseDelegate::Disable()");
    return CHIP_NO_ERROR;
}

CHIP_ERROR EnergyEvseDelegate::EnableCharging(const chip::app::DataModel::Nullable<uint32_t> & evseEnableTime,
                                              const uint32_t & minimumChargeCurrent, const uint32_t & maximumChargeCurrent)
{
    ChipLogProgress(AppServer, "EnergyEvseDelegate::EnableCharging()");
    return CHIP_NO_ERROR;
}

CHIP_ERROR EnergyEvseDelegate::EnableDischarging(const chip::app::DataModel::Nullable<uint32_t> & evseEnableTime,
                                                 const uint32_t & maximumDischargeCurrent)
{
    ChipLogProgress(AppServer, "EnergyEvseDelegate::EnableDischarging()");
    return CHIP_NO_ERROR;
}

CHIP_ERROR EnergyEvseDelegate::StartDiagnostics()
{
    ChipLogProgress(AppServer, "EnergyEvseDelegate::StartDiagnostics()");
    return CHIP_NO_ERROR;
}
