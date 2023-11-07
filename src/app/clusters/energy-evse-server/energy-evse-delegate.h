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

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-objects.h>

namespace chip {
namespace app {
namespace Clusters {
namespace EnergyEvse {

// typedef void (*OnTimeSyncCompletion)(void * context, TimeSourceEnum timeSource, GranularityEnum granularity);
// typedef void (*OnFallbackNTPCompletion)(void * context, bool timeSyncSuccessful);

/** @brief
 *    Defines methods for implementing application-specific logic for the EVSE Management Cluster.
 */
class Delegate
{

public:
    inline bool HasFeature(Feature feature)
    {
        uint32_t map;
        bool success = (Attributes::FeatureMap::Get(mEndpoint, &map) == EMBER_ZCL_STATUS_SUCCESS);
        return success ? (map & to_underlying(feature)) : false;
    }

    inline EndpointId GetEndpoint() { return mEndpoint; }
    inline void SetEndpoint(EndpointId ep) { mEndpoint = ep; }

    /**
     * @brief Delegate should implement a handler to disable the Evse. It should report CHIP_NO_ERROR if successful and may
     * return other CHIP_ERRORs if it fails
     */
    virtual CHIP_ERROR Disable() = 0;

    /**
     * @brief Delegate should implement a handler to enable Evse Charging.
     * It should report CHIP_NO_ERROR if successful and may return other CHIP_ERRORs if it fails
     */
    virtual CHIP_ERROR EnableCharging(const chip::app::DataModel::Nullable<uint32_t> & enableChargeTime,
                                      const uint32_t & minimumChargeCurrent, const uint32_t & maximumChargeCurrent) = 0;

    /**
     * @brief Delegate should implement a handler to enable Evse Discharging.
     * It should report CHIP_NO_ERROR if successful and may return other CHIP_ERRORs if it fails
     */
    virtual CHIP_ERROR EnableDischarging(const chip::app::DataModel::Nullable<uint32_t> & enableDischargeTime,
                                         const uint32_t & maximumDischargeCurrent) = 0;

    /**
     * @brief Delegate should implement a handler to enable Evse Diagnostics.
     * It should report CHIP_NO_ERROR if successful and may return other CHIP_ERRORs if it fails
     */
    virtual CHIP_ERROR StartDiagnostics() = 0;

    virtual ~Delegate() = default;

private:
    EndpointId mEndpoint = kRootEndpointId;
};

} // namespace EnergyEvse
} // namespace Clusters
} // namespace app
} // namespace chip
