#
#    Copyright (c) 2024 Project CHIP Authors
#    All rights reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
# pylint: disable=invalid-name
"""Define Matter test case TC_DEM_2_9."""


import logging

import chip.clusters as Clusters
from matter_testing_support import EventChangeCallback, MatterBaseTest, TestStep, async_test_body, default_matter_test_main
from mobly import asserts
from TC_DEM_Utils import DEMBaseTestHelper

logger = logging.getLogger(__name__)


class TC_DEM_2_9(MatterBaseTest, DEMBaseTestHelper):
    """Implementation of test case TC_DEM_2_9."""

    def desc_TC_DEM_2_9(self) -> str:
        """Return a description of this test."""
        return "4.1.3. [TC-DEM-2.2] Power Adjustment feature functionality with DUT as Server"

    def pics_TC_DEM_2_9(self):
        """Return the PICS definitions associated with this test."""
        pics = [
            "DEM.S.F00",  # Depends on Feature 00 (PowerAdjustment)
        ]
        return pics

    def steps_TC_DEM_2_9(self) -> list[TestStep]:
        """Execute the test steps."""
        steps = [
            TestStep("1", "Commissioning, already done", is_commissioning=True),
            TestStep("2", "TH reads TestEventTriggersEnabled attribute from General Diagnostics Cluster. Verify that TestEventTriggersEnabled attribute has a value of 1 (True)"),
            TestStep("3", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for Forecast Test Event"),
            TestStep("3a", "TH reads Forecast attribute. Value has to include a valid slots[0].ManufacturerESAState"),
            TestStep("3b", "TH reads Forecast attribute. Value has to include valid slots[0].NominalPower, slots[0].MinPower, slots[0].MaxPower, slots[0].NominalEnergy"),
            TestStep("4", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for Forecast Test Event Clear"),
        ]

        return steps

    @async_test_body
    async def test_TC_DEM_2_9(self):
        # pylint: disable=too-many-locals, too-many-statements
        """Run the test steps."""
        self.step("1")
        # Commission DUT - already done

        # Subscribe to Events and when they are sent push them to a queue for checking later
        events_callback = EventChangeCallback(Clusters.DeviceEnergyManagement)
        await events_callback.start(self.default_controller,
                                    self.dut_node_id,
                                    self.matter_test_config.endpoint)



        self.step("2")
        await self.check_test_event_triggers_enabled()

        self.step("3")
        await self.send_test_event_trigger_forecast()

        self.step("3a")
        forecast = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_greater_equal(len(forecast.slots), 1)
        asserts.assert_is_not_none(forecast.slots[0].manufacturerESAState)

        self.step("3b")
        forecast = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_greater_equal(len(forecast.slots), 1)

        asserts.assert_is_not_none(forecast.slots[0].nominalPower)
        asserts.assert_is_not_none(forecast.slots[0].minPower)
        asserts.assert_is_not_none(forecast.slots[0].maxPower)
        asserts.assert_is_not_none(forecast.slots[0].nominalEnergy)

        self.step("4")
        await self.send_test_event_trigger_forecast_clear()

if __name__ == "__main__":
    default_matter_test_main()