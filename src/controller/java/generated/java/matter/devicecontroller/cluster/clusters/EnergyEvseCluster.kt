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

class EnergyEvseCluster(private val controller: MatterController, private val endpointId: UShort) {
  class StateAttribute(val value: UInt?)

  class EnableChargeTimeAttribute(val value: UInt?)

  class EnableDischargeTimeAttribute(val value: UInt?)

  class NextChargeStartTimeAttribute(val value: UInt?)

  class NextChargeTargetTimeAttribute(val value: UInt?)

  class NextChargeRequiredEnergyAttribute(val value: Int?)

  class NextChargeTargetSocAttribute(val value: UByte?)

  class ApproxEvEfficiencyAttribute(val value: UShort?)

  class StateOfChargeAttribute(val value: UByte?)

  class BatteryCapacityAttribute(val value: Int?)

  class VehicleIdAttribute(val value: String?)

  class GeneratedCommandListAttribute(val value: List<UInt>)

  class AcceptedCommandListAttribute(val value: List<UInt>)

  class EventListAttribute(val value: List<UInt>)

  class AttributeListAttribute(val value: List<UInt>)

  suspend fun disable(timedInvokeTimeoutMs: Int? = null) {
    val commandId = 1L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun enableCharging(
    enableChargeTime: UInt?,
    minimumChargeCurrent: UInt,
    maximumChargeCurrent: UInt,
    timedInvokeTimeoutMs: Int? = null
  ) {
    val commandId = 2L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun enableDischarging(
    enableDischargeTime: UInt?,
    maximumDischargeCurrent: UInt,
    timedInvokeTimeoutMs: Int? = null
  ) {
    val commandId = 3L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun startDiagnostics(timedInvokeTimeoutMs: Int? = null) {
    val commandId = 4L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun setTargets(
    dayOfWeekForSequence: UInt,
    chargingTargets: List<EnergyEvseClusterChargingTargetStruct>,
    timedInvokeTimeoutMs: Int? = null
  ) {
    val commandId = 5L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun getTargets(daysToReturn: UInt, timedInvokeTimeoutMs: Int? = null) {
    val commandId = 6L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun clearTargets(timedInvokeTimeoutMs: Int? = null) {
    val commandId = 7L

    if (timedInvokeTimeoutMs != null) {
      // Do the action with timedInvokeTimeoutMs
    } else {
      // Do the action without timedInvokeTimeoutMs
    }
  }

  suspend fun readStateAttribute(): StateAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeStateAttribute(minInterval: Int, maxInterval: Int): StateAttribute {
    // Implementation needs to be added here
  }

  suspend fun readSupplyStateAttribute(): UByte {
    // Implementation needs to be added here
  }

  suspend fun subscribeSupplyStateAttribute(minInterval: Int, maxInterval: Int): UByte {
    // Implementation needs to be added here
  }

  suspend fun readFaultStateAttribute(): UByte {
    // Implementation needs to be added here
  }

  suspend fun subscribeFaultStateAttribute(minInterval: Int, maxInterval: Int): UByte {
    // Implementation needs to be added here
  }

  suspend fun readEnableChargeTimeAttribute(): EnableChargeTimeAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeEnableChargeTimeAttribute(
    minInterval: Int,
    maxInterval: Int
  ): EnableChargeTimeAttribute {
    // Implementation needs to be added here
  }

  suspend fun readEnableDischargeTimeAttribute(): EnableDischargeTimeAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeEnableDischargeTimeAttribute(
    minInterval: Int,
    maxInterval: Int
  ): EnableDischargeTimeAttribute {
    // Implementation needs to be added here
  }

  suspend fun readCircuitCapacityAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun subscribeCircuitCapacityAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readMinimumChargeCurrentAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun subscribeMinimumChargeCurrentAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readMaximumChargeCurrentAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun subscribeMaximumChargeCurrentAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readMaximumdDischargeCurrentAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun subscribeMaximumdDischargeCurrentAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readUserMaximumChargeCurrentAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun writeUserMaximumChargeCurrentAttribute(
    value: UInt,
    timedWriteTimeoutMs: Int? = null
  ) {
    if (timedWriteTimeoutMs != null) {
      // Do the action with timedWriteTimeoutMs
    } else {
      // Do the action without timedWriteTimeoutMs
    }
  }

  suspend fun subscribeUserMaximumChargeCurrentAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readRandomisationDelayWindowAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun writeRandomisationDelayWindowAttribute(
    value: UInt,
    timedWriteTimeoutMs: Int? = null
  ) {
    if (timedWriteTimeoutMs != null) {
      // Do the action with timedWriteTimeoutMs
    } else {
      // Do the action without timedWriteTimeoutMs
    }
  }

  suspend fun subscribeRandomisationDelayWindowAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readNumberOfWeeklyTargetsAttribute(): UByte {
    // Implementation needs to be added here
  }

  suspend fun subscribeNumberOfWeeklyTargetsAttribute(minInterval: Int, maxInterval: Int): UByte {
    // Implementation needs to be added here
  }

  suspend fun readNumberOfDailyTargetsAttribute(): UByte {
    // Implementation needs to be added here
  }

  suspend fun subscribeNumberOfDailyTargetsAttribute(minInterval: Int, maxInterval: Int): UByte {
    // Implementation needs to be added here
  }

  suspend fun readNextChargeStartTimeAttribute(): NextChargeStartTimeAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeNextChargeStartTimeAttribute(
    minInterval: Int,
    maxInterval: Int
  ): NextChargeStartTimeAttribute {
    // Implementation needs to be added here
  }

  suspend fun readNextChargeTargetTimeAttribute(): NextChargeTargetTimeAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeNextChargeTargetTimeAttribute(
    minInterval: Int,
    maxInterval: Int
  ): NextChargeTargetTimeAttribute {
    // Implementation needs to be added here
  }

  suspend fun readNextChargeRequiredEnergyAttribute(): NextChargeRequiredEnergyAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeNextChargeRequiredEnergyAttribute(
    minInterval: Int,
    maxInterval: Int
  ): NextChargeRequiredEnergyAttribute {
    // Implementation needs to be added here
  }

  suspend fun readNextChargeTargetSocAttribute(): NextChargeTargetSocAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeNextChargeTargetSocAttribute(
    minInterval: Int,
    maxInterval: Int
  ): NextChargeTargetSocAttribute {
    // Implementation needs to be added here
  }

  suspend fun readApproxEvEfficiencyAttribute(): ApproxEvEfficiencyAttribute {
    // Implementation needs to be added here
  }

  suspend fun writeApproxEvEfficiencyAttribute(value: UShort, timedWriteTimeoutMs: Int? = null) {
    if (timedWriteTimeoutMs != null) {
      // Do the action with timedWriteTimeoutMs
    } else {
      // Do the action without timedWriteTimeoutMs
    }
  }

  suspend fun subscribeApproxEvEfficiencyAttribute(
    minInterval: Int,
    maxInterval: Int
  ): ApproxEvEfficiencyAttribute {
    // Implementation needs to be added here
  }

  suspend fun readStateOfChargeAttribute(): StateOfChargeAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeStateOfChargeAttribute(
    minInterval: Int,
    maxInterval: Int
  ): StateOfChargeAttribute {
    // Implementation needs to be added here
  }

  suspend fun readBatteryCapacityAttribute(): BatteryCapacityAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeBatteryCapacityAttribute(
    minInterval: Int,
    maxInterval: Int
  ): BatteryCapacityAttribute {
    // Implementation needs to be added here
  }

  suspend fun readVehicleIdAttribute(): VehicleIdAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeVehicleIdAttribute(minInterval: Int, maxInterval: Int): VehicleIdAttribute {
    // Implementation needs to be added here
  }

  suspend fun readSessionIdAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun subscribeSessionIdAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readEventSequenceNumberAttribute(): UShort {
    // Implementation needs to be added here
  }

  suspend fun subscribeEventSequenceNumberAttribute(minInterval: Int, maxInterval: Int): UShort {
    // Implementation needs to be added here
  }

  suspend fun readSessionDurationAttribute(): UInt {
    // Implementation needs to be added here
  }

  suspend fun subscribeSessionDurationAttribute(minInterval: Int, maxInterval: Int): UInt {
    // Implementation needs to be added here
  }

  suspend fun readSessionEnergyChargedAttribute(): Int {
    // Implementation needs to be added here
  }

  suspend fun subscribeSessionEnergyChargedAttribute(minInterval: Int, maxInterval: Int): Int {
    // Implementation needs to be added here
  }

  suspend fun readSessionEnergyDischargedAttribute(): Int {
    // Implementation needs to be added here
  }

  suspend fun subscribeSessionEnergyDischargedAttribute(minInterval: Int, maxInterval: Int): Int {
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
    const val CLUSTER_ID: UInt = 153u
  }
}
