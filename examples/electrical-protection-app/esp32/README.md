# Matter ESP32 Electrical Protection Example

This example demonstrates the Matter Electrical Protection application on ESP
platforms. The application includes clusters for electrical protection,
electrical energy measurement, electrical power measurement, and power topology.

Please
[setup ESP-IDF and CHIP Environment](../../../docs/platforms/esp32/setup_idf_chip.md)
and refer
[building and commissioning](../../../docs/platforms/esp32/build_app_and_commission.md)
guides to get started.

---

## Building the Example

### Using the Matter Build System

The Matter build system can be used to build this example:

```bash
cd examples/electrical-protection-app/esp32
idf.py set-target <target> # e.g., esp32, esp32c3, esp32c6, esp32h2
idf.py build
```

Where `<target>` is one of the supported ESP32 variants (esp32, esp32c3, esp32c6,
esp32h2, etc.).

### Flashing the Example

After building, flash the firmware:

```bash
idf.py -p <PORT> flash monitor
```

Replace `<PORT>` with the serial port your ESP32 is connected to (e.g.,
`/dev/ttyUSB0` on Linux or `COM3` on Windows).

---

## Commissioning

Commission the device using chip-tool or any Matter controller:

```bash
./out/debug/chip-tool pairing ble-wifi <NODE_ID> <SSID> <PASSWORD> 20202021 3840
```

---

## Cluster Control

After successful commissioning, you can interact with the electrical protection
clusters:

### Electrical Energy Measurement

Read cumulative energy imported:

```bash
./out/debug/chip-tool electricalэнергymeasurement read cumulative-energy-imported <NODE_ID> <ENDPOINT_ID>
```

### Electrical Power Measurement

Read active power:

```bash
./out/debug/chip-tool electricalpowermeasurement read active-power <NODE_ID> <ENDPOINT_ID>
```

### Power Topology

Read available endpoints:

```bash
./out/debug/chip-tool powertopology read available-endpoints <NODE_ID> <ENDPOINT_ID>
```

---

## Configuration Options

### Device Type

The application supports various ESP32 device types. Configure using:

```bash
idf.py menuconfig
```

Navigate to `Demo` → `Device Type` to select your board.

### Partition Table

The default partition table (`partitions.csv`) is configured for 4MB flash. For
different flash sizes, modify the partition table or use
`partitions_encrypted.csv` for encrypted OTA support.

### Optimization

For smaller binary sizes, use the optimization configuration:

```bash
idf.py -DSDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.optimize.defaults" build
```

---

## Supported Targets

-   ESP32
-   ESP32-C2
-   ESP32-C3
-   ESP32-C6
-   ESP32-H2
-   ESP32-H4
-   ESP32-H21
-   ESP32-S3
-   ESP32-S31

---

## Additional Resources

-   [Matter Specification](https://csa-iot.org/developer-resource/specifications-download-request/)
-   [ESP32 Platform Guide](../../../docs/platforms/esp32/)
-   [Electrical Protection App Common Code](../electrical-protection-common/)
