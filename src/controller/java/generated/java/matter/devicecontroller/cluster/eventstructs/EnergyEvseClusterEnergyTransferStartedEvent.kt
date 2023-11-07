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

class EnergyEvseClusterEnergyTransferStartedEvent(
  val sessionId: UInt,
  val state: UInt,
  val maximumCurrent: UInt
) {
  override fun toString(): String = buildString {
    append("EnergyEvseClusterEnergyTransferStartedEvent {\n")
    append("\tsessionId : $sessionId\n")
    append("\tstate : $state\n")
    append("\tmaximumCurrent : $maximumCurrent\n")
    append("}\n")
  }

  fun toTlv(tlvTag: Tag, tlvWriter: TlvWriter) {
    tlvWriter.apply {
      startStructure(tlvTag)
      put(ContextSpecificTag(TAG_SESSION_ID), sessionId)
      put(ContextSpecificTag(TAG_STATE), state)
      put(ContextSpecificTag(TAG_MAXIMUM_CURRENT), maximumCurrent)
      endStructure()
    }
  }

  companion object {
    private const val TAG_SESSION_ID = 0
    private const val TAG_STATE = 1
    private const val TAG_MAXIMUM_CURRENT = 2

    fun fromTlv(tlvTag: Tag, tlvReader: TlvReader): EnergyEvseClusterEnergyTransferStartedEvent {
      tlvReader.enterStructure(tlvTag)
      val sessionId = tlvReader.getUInt(ContextSpecificTag(TAG_SESSION_ID))
      val state = tlvReader.getUInt(ContextSpecificTag(TAG_STATE))
      val maximumCurrent = tlvReader.getUInt(ContextSpecificTag(TAG_MAXIMUM_CURRENT))

      tlvReader.exitContainer()

      return EnergyEvseClusterEnergyTransferStartedEvent(sessionId, state, maximumCurrent)
    }
  }
}
