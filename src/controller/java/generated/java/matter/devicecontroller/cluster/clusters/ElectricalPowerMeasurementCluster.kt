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

class ElectricalPowerMeasurementCluster(
  private val controller: MatterController,
  private val endpointId: UShort
) {
  class AccuracyAttribute(
    val value: List<ElectricalPowerMeasurementClusterMeasurementAccuracyStruct>
  )

  class RangesAttribute(val value: List<ElectricalPowerMeasurementClusterMeasurementRangeStruct>)

  class VoltageAttribute(val value: Long?)

  class CurrentAttribute(val value: Long?)

  class ActivePowerAttribute(val value: Long?)

  class RmsCurrentAttribute(val value: Long?)

  class RmsPowerAttribute(val value: Long?)

  class ApparentPowerAttribute(val value: Long?)

  class ReactivePowerAttribute(val value: Long?)

  class FrequencyAttribute(val value: Long?)

  class HarmonicCurrentsAttribute(
    val value: List<ElectricalPowerMeasurementClusterHarmonicMeasurementStruct>?
  )

  class HarmonicPhasesAttribute(
    val value: List<ElectricalPowerMeasurementClusterHarmonicMeasurementStruct>?
  )

  class PowerFactorAttribute(val value: Long?)

  class LineCurrentAttribute(val value: Long?)

  class NeutralCurrentAttribute(val value: Long?)

  class GeneratedCommandListAttribute(val value: List<UInt>)

  class AcceptedCommandListAttribute(val value: List<UInt>)

  class EventListAttribute(val value: List<UInt>)

  class AttributeListAttribute(val value: List<UInt>)

  suspend fun readPowerModeAttribute(): UByte {
    // Implementation needs to be added here
  }

  suspend fun subscribePowerModeAttribute(minInterval: Int, maxInterval: Int): UByte {
    // Implementation needs to be added here
  }

  suspend fun readAccuracyAttribute(): AccuracyAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeAccuracyAttribute(minInterval: Int, maxInterval: Int): AccuracyAttribute {
    // Implementation needs to be added here
  }

  suspend fun readRangesAttribute(): RangesAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeRangesAttribute(minInterval: Int, maxInterval: Int): RangesAttribute {
    // Implementation needs to be added here
  }

  suspend fun readVoltageAttribute(): VoltageAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeVoltageAttribute(minInterval: Int, maxInterval: Int): VoltageAttribute {
    // Implementation needs to be added here
  }

  suspend fun readCurrentAttribute(): CurrentAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeCurrentAttribute(minInterval: Int, maxInterval: Int): CurrentAttribute {
    // Implementation needs to be added here
  }

  suspend fun readActivePowerAttribute(): ActivePowerAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeActivePowerAttribute(
    minInterval: Int,
    maxInterval: Int
  ): ActivePowerAttribute {
    // Implementation needs to be added here
  }

  suspend fun readRmsCurrentAttribute(): RmsCurrentAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeRmsCurrentAttribute(
    minInterval: Int,
    maxInterval: Int
  ): RmsCurrentAttribute {
    // Implementation needs to be added here
  }

  suspend fun readRmsPowerAttribute(): RmsPowerAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeRmsPowerAttribute(minInterval: Int, maxInterval: Int): RmsPowerAttribute {
    // Implementation needs to be added here
  }

  suspend fun readApparentPowerAttribute(): ApparentPowerAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeApparentPowerAttribute(
    minInterval: Int,
    maxInterval: Int
  ): ApparentPowerAttribute {
    // Implementation needs to be added here
  }

  suspend fun readReactivePowerAttribute(): ReactivePowerAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeReactivePowerAttribute(
    minInterval: Int,
    maxInterval: Int
  ): ReactivePowerAttribute {
    // Implementation needs to be added here
  }

  suspend fun readFrequencyAttribute(): FrequencyAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeFrequencyAttribute(minInterval: Int, maxInterval: Int): FrequencyAttribute {
    // Implementation needs to be added here
  }

  suspend fun readHarmonicCurrentsAttribute(): HarmonicCurrentsAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeHarmonicCurrentsAttribute(
    minInterval: Int,
    maxInterval: Int
  ): HarmonicCurrentsAttribute {
    // Implementation needs to be added here
  }

  suspend fun readHarmonicPhasesAttribute(): HarmonicPhasesAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeHarmonicPhasesAttribute(
    minInterval: Int,
    maxInterval: Int
  ): HarmonicPhasesAttribute {
    // Implementation needs to be added here
  }

  suspend fun readPowerFactorAttribute(): PowerFactorAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribePowerFactorAttribute(
    minInterval: Int,
    maxInterval: Int
  ): PowerFactorAttribute {
    // Implementation needs to be added here
  }

  suspend fun readLineCurrentAttribute(): LineCurrentAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeLineCurrentAttribute(
    minInterval: Int,
    maxInterval: Int
  ): LineCurrentAttribute {
    // Implementation needs to be added here
  }

  suspend fun readNeutralCurrentAttribute(): NeutralCurrentAttribute {
    // Implementation needs to be added here
  }

  suspend fun subscribeNeutralCurrentAttribute(
    minInterval: Int,
    maxInterval: Int
  ): NeutralCurrentAttribute {
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
    const val CLUSTER_ID: UInt = 26209u
  }
}
