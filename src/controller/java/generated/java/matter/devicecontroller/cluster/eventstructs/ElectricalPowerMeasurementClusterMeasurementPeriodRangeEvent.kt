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
package matter.devicecontroller.cluster.eventstructs

import matter.devicecontroller.cluster.*
import matter.tlv.ContextSpecificTag
import matter.tlv.Tag
import matter.tlv.TlvReader
import matter.tlv.TlvWriter

class ElectricalPowerMeasurementClusterMeasurementPeriodRangeEvent(
  val measurementType: UInt,
  val periodStart: UInt,
  val periodEnd: UInt,
  val min: Long,
  val minTimestamp: UInt,
  val max: Long,
  val maxTimestamp: UInt
) {
  override fun toString(): String = buildString {
    append("ElectricalPowerMeasurementClusterMeasurementPeriodRangeEvent {\n")
    append("\tmeasurementType : $measurementType\n")
    append("\tperiodStart : $periodStart\n")
    append("\tperiodEnd : $periodEnd\n")
    append("\tmin : $min\n")
    append("\tminTimestamp : $minTimestamp\n")
    append("\tmax : $max\n")
    append("\tmaxTimestamp : $maxTimestamp\n")
    append("}\n")
  }

  fun toTlv(tlvTag: Tag, tlvWriter: TlvWriter) {
    tlvWriter.apply {
      startStructure(tlvTag)
      put(ContextSpecificTag(TAG_MEASUREMENT_TYPE), measurementType)
      put(ContextSpecificTag(TAG_PERIOD_START), periodStart)
      put(ContextSpecificTag(TAG_PERIOD_END), periodEnd)
      put(ContextSpecificTag(TAG_MIN), min)
      put(ContextSpecificTag(TAG_MIN_TIMESTAMP), minTimestamp)
      put(ContextSpecificTag(TAG_MAX), max)
      put(ContextSpecificTag(TAG_MAX_TIMESTAMP), maxTimestamp)
      endStructure()
    }
  }

  companion object {
    private const val TAG_MEASUREMENT_TYPE = 0
    private const val TAG_PERIOD_START = 1
    private const val TAG_PERIOD_END = 2
    private const val TAG_MIN = 3
    private const val TAG_MIN_TIMESTAMP = 4
    private const val TAG_MAX = 5
    private const val TAG_MAX_TIMESTAMP = 6

    fun fromTlv(
      tlvTag: Tag,
      tlvReader: TlvReader
    ): ElectricalPowerMeasurementClusterMeasurementPeriodRangeEvent {
      tlvReader.enterStructure(tlvTag)
      val measurementType = tlvReader.getUInt(ContextSpecificTag(TAG_MEASUREMENT_TYPE))
      val periodStart = tlvReader.getUInt(ContextSpecificTag(TAG_PERIOD_START))
      val periodEnd = tlvReader.getUInt(ContextSpecificTag(TAG_PERIOD_END))
      val min = tlvReader.getLong(ContextSpecificTag(TAG_MIN))
      val minTimestamp = tlvReader.getUInt(ContextSpecificTag(TAG_MIN_TIMESTAMP))
      val max = tlvReader.getLong(ContextSpecificTag(TAG_MAX))
      val maxTimestamp = tlvReader.getUInt(ContextSpecificTag(TAG_MAX_TIMESTAMP))

      tlvReader.exitContainer()

      return ElectricalPowerMeasurementClusterMeasurementPeriodRangeEvent(
        measurementType,
        periodStart,
        periodEnd,
        min,
        minTimestamp,
        max,
        maxTimestamp
      )
    }
  }
}
