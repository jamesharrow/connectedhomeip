/*
 *    Copyright (c) 2023 Project CHIP Authors
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

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/AttributeAccessInterface.h>
#include <app/CommandHandler.h>
#include <app/ConcreteCommandPath.h>
#include <app/util/af.h>
#include <app/util/attribute-storage.h>
#include <lib/core/CHIPEncoding.h>
#include <lib/core/Optional.h>
#include <lib/support/CHIPPlatformMemory.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/ConnectivityManager.h>

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::DeviceEnergyManagement;
using namespace chip::app::Clusters::DeviceEnergyManagement::Attributes;

void MatterDeviceEnergyManagementPluginServerInitCallback() {}

void emberAfDeviceEnergyManagementClusterServerInitCallback(chip::EndpointId endpoint) {}

bool emberAfDeviceEnergyManagementClusterPowerAdjustRequestCallback(app::CommandHandler * commandObj,
                                                                    const app::ConcreteCommandPath & commandPath,
                                                                    const Commands::PowerAdjustRequest::DecodableType & commandData)
{
    // TODO
    return false;
}

bool emberAfDeviceEnergyManagementClusterCancelPowerAdjustRequestCallback(
    app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
    const Commands::CancelPowerAdjustRequest::DecodableType & commandData)
{
    // TODO
    return false;
}

bool emberAfDeviceEnergyManagementClusterStartTimeAdjustRequestCallback(
    app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
    const Commands::StartTimeAdjustRequest::DecodableType & commandData)
{
    // TODO
    return false;
}
bool emberAfDeviceEnergyManagementClusterPauseRequestCallback(app::CommandHandler * commandObj,
                                                              const app::ConcreteCommandPath & commandPath,
                                                              const Commands::PauseRequest::DecodableType & commandData)
{
    // TODO
    return false;
}
bool emberAfDeviceEnergyManagementClusterResumeRequestCallback(app::CommandHandler * commandObj,
                                                               const app::ConcreteCommandPath & commandPath,
                                                               const Commands::ResumeRequest::DecodableType & commandData)
{
    // TODO
    return false;
}
bool emberAfDeviceEnergyManagementClusterModifyForecastRequestCallback(
    app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
    const Commands::ModifyForecastRequest::DecodableType & commandData)
{
    // TODO
    return false;
}
bool emberAfDeviceEnergyManagementClusterRequestConstraintBasedForecastCallback(
    app::CommandHandler * commandObj, const app::ConcreteCommandPath & commandPath,
    const Commands::RequestConstraintBasedForecast::DecodableType & commandData)
{
    // TODO
    return false;
}
