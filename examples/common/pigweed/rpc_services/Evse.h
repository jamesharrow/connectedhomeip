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
#include "evse_service/evse_service.rpc.pb.h"
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
class Evse : public pw_rpc::nanopb::Evse::Service<Evse>
{
public:
    virtual ~Evse() = default;
    virtual pw::Status SetMaxHardwareCurrentLimit(const chip_rpc_CurrentLimitMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        uint16_t currentLimit = request.current;
        // TODO add call into EvseMgr
        RETURN_STATUS_IF_NOT_OK(app::Clusters::EnergyEvse::Attributes::MaximumChargeCurrent::Set(kEndpoint, currentLimit));

        return pw::OkStatus();
    }

    virtual pw::Status SetCircuitCapacity(const chip_rpc_CurrentLimitMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        uint16_t currentLimit = request.current;
        // TODO add call into EvseMgr
        RETURN_STATUS_IF_NOT_OK(app::Clusters::EnergyEvse::Attributes::CircuitCapacity::Set(kEndpoint, currentLimit));

        return pw::OkStatus();
    }

    virtual pw::Status SetCableAssemblyCurrentLimit(const chip_rpc_CurrentLimitMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        uint16_t currentLimit = request.current;
        // TODO add call into EvseMgr
        RETURN_STATUS_IF_NOT_OK(app::Clusters::EnergyEvse::Attributes::MaximumChargeCurrent::Set(kEndpoint, currentLimit));

        return pw::OkStatus();
    }

    virtual pw::Status SetUserMaximumChargeCurrent(const chip_rpc_CurrentLimitMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        uint16_t currentLimit = request.current;
        // TODO add call into EvseMgr
        RETURN_STATUS_IF_NOT_OK(app::Clusters::EnergyEvse::Attributes::MaximumChargeCurrent::Set(kEndpoint, currentLimit));

        return pw::OkStatus();
    }

    virtual pw::Status SetState(const chip_rpc_EvseStateMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        EnergyEvse::EvseStateEnum state;
        uint32_t value = request.state;
        state          = (EnergyEvse::EvseStateEnum) value;

        // TODO add call into EvseMgr
        RETURN_STATUS_IF_NOT_OK(app::Clusters::EnergyEvse::Attributes::State::Set(kEndpoint, state));

        return pw::OkStatus();
    }

    virtual pw::Status SetSupplyState(const chip_rpc_SupplyStateMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        EnergyEvse::SupplyStateEnum state;
        uint32_t value = request.state;
        state          = (EnergyEvse::SupplyStateEnum) value;

        // TODO add call into EvseMgr
        RETURN_STATUS_IF_NOT_OK(app::Clusters::EnergyEvse::Attributes::SupplyState::Set(kEndpoint, state));

        return pw::OkStatus();
    }

    virtual pw::Status SetFault(const chip_rpc_FaultMsg & request, pw_protobuf_Empty & response)
    {
        DeviceLayer::StackLock lock;

        EnergyEvse::FaultStateEnum fault;
        uint32_t value = request.state;
        fault          = (EnergyEvse::FaultStateEnum) value;
        // TODO add call into EvseMgr
        RETURN_STATUS_IF_NOT_OK(app::Clusters::EnergyEvse::Attributes::FaultState::Set(kEndpoint, fault));

        return pw::OkStatus();
    }

protected:
    static constexpr EndpointId kEndpoint = 1;
};

} // namespace rpc
} // namespace chip
