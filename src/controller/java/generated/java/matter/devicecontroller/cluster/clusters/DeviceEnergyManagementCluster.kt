/*
 *
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

package matter.devicecontroller.cluster.clusters

import matter.controller.MatterController
import matter.devicecontroller.cluster.structs.*

class DeviceEnergyManagementCluster(
  private val controller: MatterController,
  private val endpointId: UShort
) {
  class PowerAdjustmentCapabilityAttribute(
    val value: List<DeviceEnergyManagementClusterPowerAdjustStruct>?
  )

  class ForecastAttribute(val value: DeviceEnergyManagementClusterForecastStruct?)

  class GeneratedCommandListAttribute(val value: List<UInt>)

  class AcceptedCommandListAttribute(val value: List<UInt>)

  class EventListAttribute(val value: List<UInt>)

  class AttributeListAttribute(val value: List<UInt>)

  suspend fun powerAdjustRequest(power: Int, duration: UInt, timedInvokeTimeoutMs: Int? = null) {
    val commandId = 0L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun cancelPowerAdjustRequest(timedInvokeTimeoutMs: Int? = null) {
    val commandId = 1L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun startTimeAdjustRequest(requestedStartTime: UInt, timedInvokeTimeoutMs: Int? = null) {
    val commandId = 2L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun pauseRequest(timedInvokeTimeoutMs: Int? = null) {
    val commandId = 3L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun resumeRequest(timedInvokeTimeoutMs: Int? = null) {
    val commandId = 4L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun modifyForecastRequest(
    forecastId: UInt,
    slotAdjustments: List<DeviceEnergyManagementClusterSlotAdjustmentStruct>,
    timedInvokeTimeoutMs: Int? = null
  ) {
    val commandId = 5L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun requestConstraintBasedForecast(
    constraints: List<DeviceEnergyManagementClusterConstraintsStruct>,
    timedInvokeTimeoutMs: Int? = null
  ) {
    val commandId = 6L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun readEsaTypeAttribute(): UByte {
    // Implementation needs to be added here
  }

  suspend fun subscribeEsaTypeAttribute(minInterval: Int, maxInterval: Int): UByte {
    // Implementation needs to be added here
  }

  suspend fun readEsaIsGeneratorAttribute(): Boolean {
    // Implementation needs to be added here
  }

  suspend fun subscribeEsaIsGeneratorAttribute(minInterval: Int, maxInterval: Int): Boolean {
    // Implementation needs to be added here
  }

  suspend fun readEsaStateAttribute(): UByte {
    // Implementation needs to be added here
  }

  suspend fun subscribeEsaStateAttribute(minInterval: Int, maxInterval: Int): UByte {
    // Implementation needs to be added here
  }

  suspend fun readAbsMinPowerAttribute(): Int {
    // Implementation needs to be added here
  }

  suspend fun subscribeAbsMinPowerAttribute(minInterval: Int, maxInterval: Int): Int {
    // Implementation needs to be added here
  }

  suspend fun readAbsMaxPowerAttribute(): Int {
    // Implementation needs to be added here
  }

  suspend fun subscribeAbsMaxPowerAttribute(minInterval: Int, maxInterval: Int): Int {
    // Implementation needs to be added here
  }

  suspend fun readPowerAdjustmentCapabilityAttribute(): PowerAdjustmentCapabilityAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribePowerAdjustmentCapabilityAttribute(
    minInterval: Int,
    maxInterval: Int
  ): PowerAdjustmentCapabilityAttribute {
    // Implementation needs to be added here
  }

  suspend fun readForecastAttribute(): ForecastAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeForecastAttribute(minInterval: Int, maxInterval: Int): ForecastAttribute {
    // Implementation needs to be added here
  }

  suspend fun readGeneratedCommandListAttribute(): GeneratedCommandListAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeGeneratedCommandListAttribute(
    minInterval: Int,
    maxInterval: Int
  ): GeneratedCommandListAttribute {
    // Implementation needs to be added here
  }

  suspend fun readAcceptedCommandListAttribute(): AcceptedCommandListAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeAcceptedCommandListAttribute(
    minInterval: Int,
    maxInterval: Int
  ): AcceptedCommandListAttribute {
    // Implementation needs to be added here
  }

  suspend fun readEventListAttribute(): EventListAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeEventListAttribute(minInterval: Int, maxInterval: Int): EventListAttribute {
    // Implementation needs to be added here
  }

  suspend fun readAttributeListAttribute(): AttributeListAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeAttributeListAttribute(
    minInterval: Int,
    maxInterval: Int
  ): AttributeListAttribute {
    // Implementation needs to be added here
  }

  suspend fun readFeatureMapAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun subscribeFeatureMapAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readClusterRevisionAttribute(): UShort {
    // Implementation needs to be added here
  }

  suspend fun subscribeClusterRevisionAttribute(minInterval: Int, maxInterval: Int): UShort {
    // Implementation needs to be added here
  }

  companion object {
    const val CLUSTER_ID: UInt = 152u
  }
}
