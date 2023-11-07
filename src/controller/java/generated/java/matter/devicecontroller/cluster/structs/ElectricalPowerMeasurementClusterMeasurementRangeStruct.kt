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
package matter.devicecontroller.cluster.structs

import matter.devicecontroller.cluster.*
import matter.tlv.ContextSpecificTag
import matter.tlv.Tag
import matter.tlv.TlvReader
import matter.tlv.TlvWriter

class ElectricalPowerMeasurementClusterMeasurementRangeStruct(
  val measurementType: UInt,
  val min: Long?,
  val minTimestamp: UInt?,
  val max: Long?,
  val maxTimestamp: UInt?
) {
  override fun toString(): String = buildString {
    append("ElectricalPowerMeasurementClusterMeasurementRangeStruct {\n")
    append("\tmeasurementType : $measurementType\n")
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
      if (min != null) {
        put(ContextSpecificTag(TAG_MIN), min)
      } else {
        putNull(ContextSpecificTag(TAG_MIN))
      }
      if (minTimestamp != null) {
        put(ContextSpecificTag(TAG_MIN_TIMESTAMP), minTimestamp)
      } else {
        putNull(ContextSpecificTag(TAG_MIN_TIMESTAMP))
      }
      if (max != null) {
        put(ContextSpecificTag(TAG_MAX), max)
      } else {
        putNull(ContextSpecificTag(TAG_MAX))
      }
      if (maxTimestamp != null) {
        put(ContextSpecificTag(TAG_MAX_TIMESTAMP), maxTimestamp)
      } else {
        putNull(ContextSpecificTag(TAG_MAX_TIMESTAMP))
      }
      endStructure()
    }
  }

  companion object {
    private const val TAG_MEASUREMENT_TYPE = 0
    private const val TAG_MIN = 1
    private const val TAG_MIN_TIMESTAMP = 2
    private const val TAG_MAX = 3
    private const val TAG_MAX_TIMESTAMP = 4

    fun fromTlv(
      tlvTag: Tag,
      tlvReader: TlvReader
    ): ElectricalPowerMeasurementClusterMeasurementRangeStruct {
      tlvReader.enterStructure(tlvTag)
      val measurementType = tlvReader.getUInt(ContextSpecificTag(TAG_MEASUREMENT_TYPE))
      val min =
        if (!tlvReader.isNull()) {
          tlvReader.getLong(ContextSpecificTag(TAG_MIN))
        } else {
          tlvReader.getNull(ContextSpecificTag(TAG_MIN))
          null
        }
      val minTimestamp =
        if (!tlvReader.isNull()) {
          tlvReader.getUInt(ContextSpecificTag(TAG_MIN_TIMESTAMP))
        } else {
          tlvReader.getNull(ContextSpecificTag(TAG_MIN_TIMESTAMP))
          null
        }
      val max =
        if (!tlvReader.isNull()) {
          tlvReader.getLong(ContextSpecificTag(TAG_MAX))
        } else {
          tlvReader.getNull(ContextSpecificTag(TAG_MAX))
          null
        }
      val maxTimestamp =
        if (!tlvReader.isNull()) {
          tlvReader.getUInt(ContextSpecificTag(TAG_MAX_TIMESTAMP))
        } else {
          tlvReader.getNull(ContextSpecificTag(TAG_MAX_TIMESTAMP))
          null
        }

      tlvReader.exitContainer()

      return ElectricalPowerMeasurementClusterMeasurementRangeStruct(
        measurementType,
        min,
        minTimestamp,
        max,
        maxTimestamp
      )
    }
  }
}
