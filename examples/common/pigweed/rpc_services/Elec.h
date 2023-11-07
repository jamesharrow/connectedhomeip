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

#include "app/util/attribute-storage.h"
#include "elec_service/elec_service.rpc.pb.h"
#include "pigweed/rpc_services/internal/StatusUtils.h"
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <platform/PlatformManager.h>

namespace chip {
namespace rpc {

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;

class Elec : public pw_rpc::nanopb::Elec::Service<Elec>
{
public:
    virtual ~Elec() = default;
    virtual pw::Status SendPowerReading(const chip_rpc_ElecPowerMeasurementMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        int64_t current = request.current;
        int64_t voltage = request.voltage;
        int64_t power   = request.power;
        // TODO add call into EvseMgr
        RETURN_STATUS_IF_NOT_OK(app::Clusters::ElectricalPowerMeasurement::Attributes::Current::Set(kEndpoint, current));
        RETURN_STATUS_IF_NOT_OK(app::Clusters::ElectricalPowerMeasurement::Attributes::Voltage::Set(kEndpoint, voltage));
        RETURN_STATUS_IF_NOT_OK(app::Clusters::ElectricalPowerMeasurement::Attributes::ActivePower::Set(kEndpoint, power));

        return pw::OkStatus();
    }

    // Note we only support Cummaulative or Ephemeral readings (not periodic)
    virtual pw::Status SendEnergyReading(const chip_rpc_ElecEnergyMeasurementMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        uint32_t type = request.type;

        if (request.has_energy_imported)
        {
            uint64_t energy_in = request.energy_imported;

            if (type == 0x0) // Cummulative
            {
                uint32_t energy_in_ts = request.energy_imported_ts;

                RETURN_STATUS_IF_NOT_OK(app::Clusters::ElectricalEnergyMeasurement::Attributes::CumulativeEnergyImportedTime::Set(
                    kEndpoint, energy_in_ts));
                RETURN_STATUS_IF_NOT_OK(
                    app::Clusters::ElectricalEnergyMeasurement::Attributes::CumulativeEnergyImported::Set(kEndpoint, energy_in));
            }
            else
            { // Ephemeral
              // RETURN_STATUS_IF_NOT_OK(app::Clusters::ElectricalEnergyMeasurement::Attributes::EphemeralEnergyImportedTime::Set(
              //    kEndpoint, energy_in_ts));
                RETURN_STATUS_IF_NOT_OK(
                    app::Clusters::ElectricalEnergyMeasurement::Attributes::EphemeralEnergyImported::Set(kEndpoint, energy_in));
            }
        }

        if (request.has_energy_exported)
        {
            uint64_t energy_ex = request.energy_exported;
            if (type == 0x0) // Cummulative
            {
                uint32_t energy_ex_ts = request.energy_exported_ts;

                RETURN_STATUS_IF_NOT_OK(app::Clusters::ElectricalEnergyMeasurement::Attributes::CumulativeEnergyExportedTime::Set(
                    kEndpoint, energy_ex_ts));
                RETURN_STATUS_IF_NOT_OK(
                    app::Clusters::ElectricalEnergyMeasurement::Attributes::CumulativeEnergyExported::Set(kEndpoint, energy_ex));
            }
            else
            { // Ephemeral
                RETURN_STATUS_IF_NOT_OK(
                    app::Clusters::ElectricalEnergyMeasurement::Attributes::EphemeralEnergyExported::Set(kEndpoint, energy_ex));
                // RETURN_STATUS_IF_NOT_OK(
                //                    app::Clusters::ElectricalEnergyMeasurement::Attributes::EphemeralEnergyExportedTime::Set(kEndpoint,
                //                    energy_ex_ts));
            }
        }

        return pw::OkStatus();
    }

protected:
    static constexpr EndpointId kEndpoint = 1;
};

} // namespace rpc
} // namespace chip
