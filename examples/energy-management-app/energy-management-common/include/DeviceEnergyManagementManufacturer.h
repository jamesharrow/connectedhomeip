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

using chip::Protocols::InteractionModel::Status;
namespace chip {
namespace app {
namespace Clusters {
namespace DeviceEnergyManagement {

class DeviceEnergyManagementDelegate;

/**
 * The application delegate.
 */
class DeviceEnergyManagementManufacturer
{
public:
    DeviceEnergyManagementManufacturer()
    {
    }

    virtual ~DeviceEnergyManagementManufacturer()
    {
    }
    virtual Status Configure(DeviceEnergyManagementDelegate &demDelegate) = 0;
};

} // namespace DeviceEnergyManagement
} // namespace Clusters
} // namespace app
} // namespace chip

