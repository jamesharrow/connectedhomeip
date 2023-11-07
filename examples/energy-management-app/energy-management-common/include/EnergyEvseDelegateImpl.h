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

#include "app/clusters/energy-evse-server/energy-evse-delegate.h"

#include <app/util/af.h>
#include <app/util/config.h>
#include <cstring>

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

/**
 * The application delegate.
 */

class EnergyEvseDelegate : public Delegate
{
public:
    EnergyEvseDelegate() : Delegate(){};

    virtual CHIP_ERROR Disable() override;
    virtual CHIP_ERROR EnableCharging(const chip::app::DataModel::Nullable<uint32_t> & enableChargeTime,
                                      const uint32_t & minimumChargeCurrent, const uint32_t & maximumChargeCurrent) override;
    virtual CHIP_ERROR EnableDischarging(const chip::app::DataModel::Nullable<uint32_t> & enableDischargeTime,
                                         const uint32_t & maximumDischargeCurrent) override;
    virtual CHIP_ERROR StartDiagnostics() override;

private:
    static EnergyEvseDelegate sInstance;
};

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip
