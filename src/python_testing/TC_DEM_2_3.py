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


import logging

import chip.clusters as Clusters
from chip.clusters.Types import NullValue
from chip.interaction_model import Status
from matter_testing_support import EventChangeCallback, MatterBaseTest, TestStep, async_test_body, default_matter_test_main
from mobly import asserts
from TC_DEM_Utils import DEMBaseTestHelper

logger = logging.getLogger(__name__)


class TC_DEM_2_3(MatterBaseTest, DEMBaseTestHelper):

    def desc_TC_DEM_2_3(self) -> str:
        """Returns a description of this test"""
        return "4.1.3. [TC-DEM-2.3] Start Time Adjustment feature functionality with DUT as Server"

    def pics_TC_DEM_2_3(self):
        """This test case verifies the primary functionality of the Device Energy Management cluster server with Start Time Adjustment feature."""
        pics = [
            "DEM.S.F03",  # Depends on F03(StartTimeAdjustment)
        ]
        return pics

    def steps_TC_DEM_2_3(self) -> list[TestStep]:
        steps = [
            TestStep("1", "Commissioning, already done", is_commissioning=True),
            TestStep("2", "TH reads TestEventTriggersEnabled attribute from General Diagnostics Cluster. Verify that TestEventTriggersEnabled attribute has a value of 1 (True)"),
            TestStep("3", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for Start Time Adjustment Test Event"),
            TestStep("3a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("3b", "TH reads Forecast attribute. Value has to include EarliestStartTime<=StartTime, LatestEndTime>=EndTime, and ForecastUpdateReason=Internal Optimization"),
            TestStep("3c", "TH reads OptOutState attribute. Verify value is 0x00 (NoOptOut)"),
            TestStep("4", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for User Opt-out Local Optimization Test Event"),
            TestStep("4a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("4b", "TH reads OptOutState attribute. Verify value is 0x01 (LocalOptOut)"),
            TestStep("5", "TH sends StartTimeAdjustRequest with RequestedStartTime=EarliestStartTime from Forecast, Cause=LocalOptimization. Verify Command response is ConstraintError"),
            TestStep("5a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("5b", "TH reads Forecast attribute. Value has to be unchanged from step 3b"),
            TestStep("6", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for User Opt-out Test Event Clear"),
            TestStep("6a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("6b", "TH reads OptOutState attribute. Verify value is 0x00 (NoOptOut)"),
            TestStep("7", "TH sends StartTimeAdjustRequest with RequestedStartTime=EarliestStartTime from Forecast, Cause=LocalOptimization. Verify Command response is Success"),
            TestStep("7a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("7b", "TH reads Forecast attribute. Value has to include EarliestStartTime=StartTime, LatestEndTime>=EndTime, and ForecastUpdateReason=Local Optimization"),
            TestStep("8", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for User Opt-out Local Optimization Test Event"),
            TestStep("8a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("8b", "TH reads OptOutState attribute. Verify value is 0x01 (LocalOptOut)"),
            TestStep("8c", "TH reads Forecast attribute. Value has to include EarliestStartTime<=StartTime, LatestEndTime>=EndTime, and ForecastUpdateReason=Internal Optimization"),
            TestStep("9", "TH sends StartTimeAdjustRequest with RequestedStartTime=StartTime+(LatestEndTime-EndTime) from Forecast, Cause=GridOptimization. Verify Command response is Success"),
            TestStep("9a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("9b", "TH reads Forecast attribute. Value has to include EarliestStartTime<=StartTime, LatestEndTime=EndTime, and ForecastUpdateReason=Grid Optimization"),
            TestStep("10", "TH sends CancelRequest. Verify Command response is Success"),
            TestStep("10a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("10b", "TH reads Forecast attribute. Value has to include EarliestStartTime<=StartTime, LatestEndTime>=EndTime, and ForecastUpdateReason=Internal Optimization"),
            TestStep("11", "TH sends StartTimeAdjustRequest with RequestedStartTime=EarliestStartTime-1 from Forecast, Cause=LocalOptimization. Verify Command response is ConstraintError"),
            TestStep("11a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("11b", "TH reads Forecast attribute. Value has to include StartTime and EndTime unchanged from step 10b"),
            TestStep("12", "TH sends StartTimeAdjustRequest with RequestedStartTime=StartTime+(LatestEndTime-EndTime)+1 from Forecast, Cause=LocalOptimization. Verify Command response is ConstraintError"),
            TestStep("12a", "TH reads ESAState attribute. Verify value is 0x01 (Online)"),
            TestStep("12b", "TH reads Forecast attribute. Value has to include StartTime and EndTime unchanged from step 10b"),
            TestStep("13", "TH sends CancelRequest. Verify Command response is InvalidInStateError"),
            TestStep("14", "TH sends TestEventTrigger command to General Diagnostics Cluster on Endpoint 0 with EnableKey field set to PIXIT.DEM.TEST_EVENT_TRIGGER_KEY and EventTrigger field set to PIXIT.DEM.TEST_EVENT_TRIGGER for Start Time Adjustment Test Event Clear"),
        ]

        return steps

    @async_test_body
    async def test_TC_DEM_2_3(self):

        logging.info(Clusters.Objects.DeviceEnergyManagement.Attributes.FeatureMap)

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
        await self.send_test_event_trigger_start_time_adjustment()

        self.step("3a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("3b")
        forecast = await self.read_dem_attribute_expect_success(attribute="Forecast")

        asserts.assert_not_equal(forecast, NullValue)
        if forecast is not NullValue:
            asserts.assert_less_equal(forecast.earliestStartTime, forecast.startTime,
                                         f"Expected forecast earliestStartTime {forecast.earliestStartTime} to be <= startTime {forecast.startTime}")
            asserts.assert_greater_equal(forecast.latestEndTime, forecast.endTime,
                                         f"Expected forecast latestEndTime {forecast.latestEndTime} to be >= endTime {forecast.endTime}")
            asserts.assert_equal(forecast.forecastUpdateReason, Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kInternalOptimization,
                                 f"Expected forecast forecastUpdateReason {forecast.forecastUpdateReason} to be == Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kInternalOptimization")
            for index, slot in enumerate(forecast.slots):
                logging.info(
                    f"   [{index}] MinDuration: {slot.minDuration} MaxDuration: {slot.maxDuration} DefaultDuration: {slot.defaultDuration}")
                logging.info(f"       ElapseSlotTime: {slot.elapsedSlotTime} RemainingSlotTime: {slot.remainingSlotTime}")
                logging.info(
                    f"       SlotIsPausable: {slot.slotIsPausable} MinPauseDuration: {slot.minPauseDuration} MaxPauseDuration: {slot.maxPauseDuration}")
                logging.info(f"       ManufacturerESAState: {slot.manufacturerESAState}")
                logging.info(f"       NominalPower: {slot.nominalPower} MinPower: {slot.minPower} MaxPower: {slot.maxPower}")
                logging.info(f"       MinPowerAdjustment: {slot.minPowerAdjustment} MaxPowerAdjustment: {slot.maxPowerAdjustment}")
                logging.info(
                    f"       MinDurationAdjustment: {slot.minDurationAdjustment} MaxDurationAdjustment: {slot.maxDurationAdjustment}")
                if slot.costs is not None:
                    for cost_index, cost in enumerate(slot):
                        logging.info(
                            f"   Cost: [{cost_index}]  CostType:{cost.costType} Value: {cost.value} DecimalPoints: {cost.decimalPoints} Currency: {cost.currency}")

        self.step("3c")
        await self.check_dem_attribute("OptOutState", Clusters.DeviceEnergyManagement.Enums.OptOutStateEnum.kNoOptOut)

        self.step("4")
        await self.send_test_event_trigger_user_opt_out_local()

        self.step("4a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("4b")
        await self.check_dem_attribute("OptOutState", Clusters.DeviceEnergyManagement.Enums.OptOutStateEnum.kLocalOptOut)

        self.step("5")
        await self.send_start_time_adjust_request_command(requestedStartTime=forecast.earliestStartTime,
                                                          cause=Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization,
                                                          expected_status=Status.ConstraintError)

        self.step("5a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("5b")
        forecast2 = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_equal(forecast, forecast2,
                             f"Expected same forcast {forecast} to be == {forecast2}")

        self.step("6")
        await self.send_test_event_trigger_user_opt_out_clear_all()

        self.step("6a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("6b")
        await self.check_dem_attribute("OptOutState", Clusters.DeviceEnergyManagement.Enums.OptOutStateEnum.kNoOptOut)

        self.step("7")
        await self.send_start_time_adjust_request_command(requestedStartTime=forecast.earliestStartTime,
                                                          cause=Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization)

        self.step("7a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("7b")
        forecast3 = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_equal(forecast3.earliestStartTime, forecast3.startTime,
                             f"Expected earliestStartTime {forecast3.earliestStartTime} to be == startTime {forecast3.startTime}")
        asserts.assert_greater_equal(forecast3.latestEndTime, forecast3.endTime,
                                     f"Expected latestEndTime {forecast3.latestEndTime} to be >= endTime {forecast3.endTime}")
        asserts.assert_equal(forecast3.forecastUpdateReason, Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kLocalOptimization,
                             f"Expected forecastUpdateReason {forecast3.forecastUpdateReason} to be == LocalOptimization {Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kLocalOptimization}")

        self.step("8")
        await self.send_test_event_trigger_user_opt_out_local()

        self.step("8a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("8b")
        await self.check_dem_attribute("OptOutState", Clusters.DeviceEnergyManagement.Enums.OptOutStateEnum.kLocalOptOut)

        self.step("8c")
        forecast4 = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_less_equal(forecast4.earliestStartTime, forecast4.startTime,
                                  f"Expected earliestStartTime {forecast4.earliestStartTime} to be <= startTime {forecast4.startTime}")
        asserts.assert_greater_equal(forecast4.latestEndTime, forecast4.endTime,
                                     f"Expected forecast latestEndTime {forecast4.latestEndTime} to be >= endTime {forecast4.endTime}")
        asserts.assert_equal(forecast4.forecastUpdateReason, Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kInternalOptimization,
                                   f"Expected forecastUpdateReason {forecast4.forecastUpdateReason} to be == InternalOptimization {Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kInternalOptimization}")

        self.step("9")
        await self.send_start_time_adjust_request_command(requestedStartTime=forecast4.startTime+forecast4.latestEndTime - forecast4.endTime,
                                                          cause=Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kGridOptimization)

        self.step("9a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("9b")
        forecast5 = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_less_equal(forecast5.earliestStartTime, forecast5.startTime,
                                  f"Expected earliestStartTime {forecast5.earliestStartTime} to be <= startTime {forecast5.startTime}")
        asserts.assert_equal(forecast5.latestEndTime, forecast5.endTime,
                             f"Expected latestEndTime {forecast5.latestEndTime} to be == endTime {forecast5.endTime}")
        asserts.assert_equal(forecast5.forecastUpdateReason, Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kGridOptimization,
                             f"Expected forecastUpdateReason {forecast5.forecastUpdateReason} to be == GridOptimization {Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kGridOptimization}")

        self.step("10")
        await self.send_cancel_request_command()

        self.step("10a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("10b")
        forecast6 = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_less_equal(forecast6.earliestStartTime, forecast6.startTime,
                                  f"Expected earliestStartTime {forecast6.earliestStartTime} to be <= startTime {forecast6.startTime}")
        asserts.assert_greater_equal(forecast6.latestEndTime, forecast6.endTime,
                                     f"Expected latestEndTime {forecast6.latestEndTime} to be >= endTime {forecast6.endTime}")
        asserts.assert_equal(forecast6.forecastUpdateReason, Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kInternalOptimization,
                             f"Expected forecastUpdateReason {forecast6.forecastUpdateReason} to be == InternalOptimization {Clusters.DeviceEnergyManagement.Enums.ForecastUpdateReasonEnum.kInternalOptimization}")

        self.step("11")
        await self.send_start_time_adjust_request_command(requestedStartTime=forecast6.earliestStartTime - 1,
                                                          cause=Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization,
                                                          expected_status=Status.ConstraintError)
        self.step("11a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("11b")
        forecast7 = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_equal(forecast6.startTime, forecast7.startTime,
                             f"Expected old startTime {forecast6.startTime} to be == startTime {forecast7.startTime}")
        asserts.assert_equal(forecast6.endTime, forecast7.endTime,
                             f"Expected old endTime {forecast6.endTime} to be == endTime {forecast7.endTime}")

        self.step("12")
        await self.send_start_time_adjust_request_command(requestedStartTime=forecast7.startTime+(forecast7.latestEndTime-forecast7.endTime)+1,
                                                          cause=Clusters.DeviceEnergyManagement.Enums.AdjustmentCauseEnum.kLocalOptimization,
                                                          expected_status=Status.ConstraintError)
        self.step("12a")
        await self.check_dem_attribute("ESAState", Clusters.DeviceEnergyManagement.Enums.ESAStateEnum.kOnline)

        self.step("12b")
        forecast8 = await self.read_dem_attribute_expect_success(attribute="Forecast")
        asserts.assert_equal(forecast7.startTime, forecast8.startTime,
                             f"Expected old startTime {forecast7.startTime} to be == startTime {forecast8.startTime}")
        asserts.assert_equal(forecast7.endTime, forecast8.endTime,
                             f"Expected old endTime {forecast7.endTime} to be == endTime {forecast8.endTime}")

        self.step("13")
        await self.send_cancel_request_command(expected_status=Status.InvalidInState)

        self.step("14")
        await self.send_test_event_trigger_start_time_adjustment_clear()


if __name__ == "__main__":
    default_matter_test_main()