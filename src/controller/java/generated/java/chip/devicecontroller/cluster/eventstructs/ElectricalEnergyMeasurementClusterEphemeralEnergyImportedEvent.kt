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
package chip.devicecontroller.cluster.eventstructs

import chip.devicecontroller.cluster.*
import matter.tlv.ContextSpecificTag
import matter.tlv.Tag
import matter.tlv.TlvReader
import matter.tlv.TlvWriter

class ElectricalEnergyMeasurementClusterEphemeralEnergyImportedEvent(
  val importedTimestamp: ULong,
  val energyImported: ULong
) {
  override fun toString(): String = buildString {
    append("ElectricalEnergyMeasurementClusterEphemeralEnergyImportedEvent {\n")
    append("\timportedTimestamp : $importedTimestamp\n")
    append("\tenergyImported : $energyImported\n")
    append("}\n")
  }

  fun toTlv(tlvTag: Tag, tlvWriter: TlvWriter) {
    tlvWriter.apply {
      startStructure(tlvTag)
      put(ContextSpecificTag(TAG_IMPORTED_TIMESTAMP), importedTimestamp)
      put(ContextSpecificTag(TAG_ENERGY_IMPORTED), energyImported)
      endStructure()
    }
  }

  companion object {
    private const val TAG_IMPORTED_TIMESTAMP = 0
    private const val TAG_ENERGY_IMPORTED = 1

    fun fromTlv(
      tlvTag: Tag,
      tlvReader: TlvReader
    ): ElectricalEnergyMeasurementClusterEphemeralEnergyImportedEvent {
      tlvReader.enterStructure(tlvTag)
      val importedTimestamp = tlvReader.getULong(ContextSpecificTag(TAG_IMPORTED_TIMESTAMP))
      val energyImported = tlvReader.getULong(ContextSpecificTag(TAG_ENERGY_IMPORTED))

      tlvReader.exitContainer()

      return ElectricalEnergyMeasurementClusterEphemeralEnergyImportedEvent(
        importedTimestamp,
        energyImported
      )
    }
  }
}
