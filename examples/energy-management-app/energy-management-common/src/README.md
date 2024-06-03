To run the DEM tests:

1. Start the chip-energy-management-app:
```
rm -f evse.bin; out/debug/chip-energy-management-app --enable-key 000102030405060708090a0b0c0d0e0f --KVS evse.bin --featureSet $featureSet
```
where the $featureSet should be set depending on the test being run:
```
TC_DEM_2_2.py: 0x01  // PA
TC_DEM_2_3.py: 0x3b  // STA, PAU, FA, CON + (PFR | SFR)
TC_DEM_2_4.py: 0x3b  // STA, PAU, FA, CON + (PFR | SFR)
TC_DEM_2_5.py: 0x3b  // STA, PAU, FA, CON + PFR
TC_DEM_2_6.py: 0x3d  // STA, PAU, FA, CON + SFR
TC_DEM_2_7.py: 0x3b  // STA, PAU, FA, CON + PFR
TC_DEM_2_8.py: 0x3d  // STA, PAU, FA, CON + SFR
TC_DEM_2_9.py: 0x3f  // STA, PAU, FA, CON + PFR + SFR
```
where
```
PA  - DEM.S.F00(PowerAdjustment)
PFR - DEM.S.F01(PowerForecastReporting)
SFR - DEM.S.F02(StateForecastReporting)
STA - DEM.S.F03(StartTimeAdjustment)
PAU - DEM.S.F04(Pausable)
FA  - DEM.S.F05(ForecastAdjustment)
CON  -DEM.S.F06(ConstraintBasedAdjustment)
```
Run the python
```
python $test_script -m on-network -d 3840 -p 20202021 --endpoint 1 --hex-arg enableKey:000102030405060708090a0b0c0d0e0f
```
The chip-energy-management-app will need to be stopped before running each test script as each test will commission chip-energy-management-app in the first step. That is also why the evse.bin is deleted before running chip-energy-management-app.



