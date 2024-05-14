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


import sys
import logging
import datetime

import chip.clusters as Clusters
from chip.clusters.Types import NullValue
from chip.interaction_model import Status
from matter_testing_support import EventChangeCallback, MatterBaseTest, TestStep, async_test_body, default_matter_test_main
from mobly import asserts
from TC_DEM_Utils import DEMBaseTestHelper

logger = logging.getLogger(__name__)


class TC_DEM_2_2(MatterBaseTest, DEMBaseTestHelper):

    def desc_TC_DEM_2_2(self) -> str:
        """Returns a description of this test"""
        return "4.1.3. [TC-DEM-2.2] Power Adjustment feature functionality with DUT as Server"

    def pics_TC_DEM_2_2(self):
        """This test case verifies the primary functionality of the Device Energy Management cluster server with Power Adjustment feature."""
        pics = [
            "DEM.S.F00",  # Depends on Feature 00 (PowerAdjustment)
        ]
        return pics

    def steps_TC_DEM_2_2(self) -> list[TestStep]:
        steps = [
            TestStep("1", "Commissioning, already done", is_commissioning=True),
            TestStep("2", "TH reads TestEventTriggersEnabled attribute from General Diagnostics Cluster. Verify that TestEventTriggersEnabled attribute has a value of 1 (True)"),
            TestStep("3", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for Power Adjustment Test Event"),
            TestStep("3a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("3b", "TH reads PowerAdjustmentCapability attribute. Note value for later. Determine the OverallMaxPower and OverallMaxDuration as the largest MaxPower and MaxDuration of the PowerAdjustStructs returned, and similarly the OverallMinPower and OverallMinDuration as the smallest of the MinPower and MinDuration values."),
            TestStep("3c", "TH reads OptOutState attribute. Verify value is 0x00 (NoOptOut)"),
            TestStep("4", "TH sends PowerAdjustRequest with power=MinPower and duration=MaxDuration from first PowerAdjustStruct, Cause=LocalOptimization. Verify Event DEM.S.E00(PowerAdjustStart) is sent."),
            TestStep("4a", "TH reads ESAState attribute. Verify value is 0x04 (PowerAdjustActive)"),
            TestStep("5", "TH sends CancelPowerAdjustRequest. Verify Event DEM.S.E01(PowerAdjustEnd) is sent with Cause=Cancelled."),
            TestStep("6", "TH sends CancelPowerAdjustRequest. Verify Command response is FAILURE."),
            TestStep("7", "TH sends PowerAdjustRequest with power=OverallMaxPower+1 duration=OverallMinDuration Cause=LocalOptimization. Verify Command response is FAILURE."),
            TestStep("8", "TH sends PowerAdjustRequest with power=OverallMinPower duration=OverallMaxDuration+1 Cause=LocalOptimization. Verify Command response is FAILURE."),
            TestStep("9", "TH sends PowerAdjustRequest with power=OverallMinPower-1 duration=OverallMaxDuration Cause=LocalOptimization. Verify Command response is FAILURE."),
            TestStep("10", "TH sends PowerAdjustRequest with power=OverallMaxPower duration=OverallMinDuration-1 Cause=LocalOptimization. Verify Command response is FAILURE."),
            TestStep("11", "TH sends PowerAdjustRequest with power=MaxPower,duration=MinDuration from first PowerAdjustStruct Cause=LocalOptimization. Verify Event DEM.S.E00(PowerAdjustStart) is sent."),
            TestStep("12", "TH sends PowerAdjustRequest with power=MinPower,duration=MaxDuration from first PowerAdjustStruct Cause=LocalOptimization. Verify no Event is sent."),
            TestStep("13", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for User Opt-out Grid Optimization Test Event"),
            TestStep("13a", "TH reads ESAState attribute. Verify value is 0x04 (PowerAdjustActive)"),
            TestStep("13b", "TH reads OptOutState attribute. Verify value is 0x02 (GridOptOut)"),
            TestStep("14", "TH sends PowerAdjustRequest with power=MaxPower,duration=MinDuration from first PowerAdjustStruct Cause=GridOptimization. Verify Command response is FAILURE."),
            TestStep("15", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for User Opt-out Local Optimization Test Event.  Verify Event DEM.S.E01(PowerAdjustEnd) is sent with Cause=UserOptOut, Duration= approx time from step 11 to step 15, EnergyUse= a valid value."),
            TestStep("15a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("15b", "TH reads OptOutState attribute. Verify value is 0x03 (OptOut)"),
            TestStep("16", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for User Opt-out Test Event Clear.)"),
            TestStep("16a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("16b", "TH reads OptOutState attribute. Verify value is 0x00 (NoOptOut)"),
            TestStep("17", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for Power Adjustment Test Event Clear.)"),
        ]

        return steps

    @async_test_body
    async def test_TC_DEM_2_2(self):

        min_power = 5 * 1000 * 1000
        max_power = 30 * 1000 * 1000

        min_duration = 30
        max_duration = 60

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
        await self.send_test_event_trigger_power_adjustment()

        self.step("3a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("3b")
        power_adjustment = await self.read_dem_attribute_expect_success(attribute="PowerAdjustmentCapability")
        asserts.assert_not_equal(power_adjustment, NullValue)

        # we should expect power_adjustment to have multiple entries with different max powers, min powers, max and min durations
        found_min_power = sys.maxsize
        found_max_power = 0
        found_min_duration = sys.maxsize
        found_max_duration = 0

        for entry in power_adjustment:
            found_min_power = min(found_min_power, entry.minPower)
            found_max_power = max(found_max_power, entry.maxPower)
            found_min_duration = min(found_min_duration, entry.minDuration)
            found_max_duration = max(found_max_duration, entry.maxDuration)
            print(f"entry : {entry}")

        logging.info(f"found_min_power {found_min_power} found_max_power {found_max_power} found_min_duration {found_min_duration} found_max_duration {found_max_duration}")

        asserts.assert_equal(found_min_power, min_power)
        asserts.assert_equal(found_max_power, max_power)
        asserts.assert_equal(found_min_duration, min_duration)
        asserts.assert_equal(found_max_duration, max_duration)

        self.step("3c")
        await self.check_dem_attribute("OptOutState", Clusters.DeviceEnergyManagement.Enums.OptOutStateEnum.kNoOptOut)

        self.step("4")
        await self.send_power_adjustment_command(power = max_power,
                                                 duration = max_duration,
                                                 cause = Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization)

        event_data = events_callback.wait_for_event_report(Clusters.DeviceEnergyManagement.Events.PowerAdjustStart)

        self.step("4a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kPowerAdjustActive)

        self.step("5")
        await self.send_cancel_power_adjustment_command()
        event_data = events_callback.wait_for_event_report(Clusters.DeviceEnergyManagement.Events.PowerAdjustEnd)
        asserts.assert_equal(event_data.cause, Clusters.DeviceEnergyManagement.Enums.CauseEnum.kCancelled)

        self.step("6")
        await self.send_cancel_power_adjustment_command(expected_status=Status.Failure)

        # TODO Check return value
        self.step("7")
        await self.send_power_adjustment_command(power = max_power + 1,
                                                 duration = min_duration,
                                                 cause = Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization,
                                                 expected_status=Status.ConstraintError)

        self.step("8")
        await self.send_power_adjustment_command(power = min_power,
                                                 duration = max_duration + 1,
                                                 cause = Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization,
                                                 expected_status=Status.ConstraintError)

        self.step("9")
        await self.send_power_adjustment_command(power = min_power - 1,
                                                 duration = max_duration,
                                                 cause = Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization,
                                                 expected_status=Status.ConstraintError)

        self.step("10")
        await self.send_power_adjustment_command(power = max_power,
                                                 duration = min_duration - 1,
                                                 cause = Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization,
                                                 expected_status=Status.ConstraintError)

        start = datetime.datetime.now()

        self.step("11")
        await self.send_power_adjustment_command(power = max_power,
                                                 duration = min_duration,
                                                 cause = Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization)

        event_data = events_callback.wait_for_event_report(Clusters.DeviceEnergyManagement.Events.PowerAdjustStart)

        self.step("12")
        await self.send_power_adjustment_command(power = min_power,
                                                 duration = max_duration,
                                                 cause = Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization)

        events_callback.wait_for_no_event_report(2)

        self.step("13")
        await self.send_test_event_trigger_user_opt_out_grid()

        events_callback.wait_for_no_event_report(2)


        self.step("13a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kPowerAdjustActive)

        self.step("13b")
        await self.check_dem_attribute("OptOutState", Clusters.DeviceEnergyManagement.Enums.OptOutStateEnum.kGridOptOut)

        self.step("14")
        await self.send_power_adjustment_command(power = max_power,
                                                 duration = max_duration,
                                                 cause = Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kGridOptimization,
                                                 expected_status=Status.Failure)

        self.step("15")
        await self.send_test_event_trigger_user_opt_out_local()
        event_data = events_callback.wait_for_event_report(Clusters.DeviceEnergyManagement.Events.PowerAdjustEnd)
        asserts.assert_equal(event_data.cause, Clusters.DeviceEnergyManagement.Enums.CauseEnum.kUserOptOut)

        elapsed = datetime.datetime.now() - start
        logging.info(f"elapsed {elapsed} event_data.duration {event_data.duration}")
        asserts.assert_less_equal(abs(elapsed.seconds - event_data.duration), 1)

        asserts.assert_greater_equal(event_data.energyUse, 1)

        self.step("15a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("15b")
        await self.check_dem_attribute("OptOutState", Clusters.DeviceEnergyManagement.Enums.OptOutStateEnum.kOptOut)

        self.step("16")
        await self.send_test_event_trigger_user_opt_out_clear_all()

        self.step("16a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("16b")
        await self.check_dem_attribute("OptOutState", Clusters.DeviceEnergyManagement.Enums.OptOutStateEnum.kNoOptOut)

        self.step("17")
        await self.send_test_event_trigger_power_adjustment_clear()

if __name__ == "__main__":
    default_matter_test_main()
