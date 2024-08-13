# Bluetooth RAIL - SoC Energy Harvesting Sensor #

## Overview ##

This example uses a Dual Harvester Shield with a lithium capacitor as storage element and a photovoltaic cell as power source. The device's operation principle is taking sensor readings from the PMIC (AEM13920 - Power Management IC), then using **RAIL** to craft the BLE packet, and advertising the packet as non-connectable to optimize energy consumption.

![overview](image/overview.png)

## Table of Contents ##

- [Purpose/Scope](#purposescope)
  - [How The Application Works](#how-the-application-works)
    - [Reading data from AEM13920](#reading-data-from-aem13920)
    - [Advertisement Packet](#advertisement-packet)
- [Prerequisites](#prerequisites)
  - [Hardware Requirements](#hardware-requirements)
    - [Hardware Connection](#hardware-connection)
  - [Software Requirements](#software-requirements)
- [Steps to Run Demo Application](#steps-to-run-demo-application)
  - [Create a project based on an example project](#create-a-project-based-on-an-example-project)
  - [Interacting with the AEM13920](#interacting-with-the-aem13920)
  - [Testing](#testing)
    - [Test with mobile phone](#test-with-mobile-phone)
    - [Test with Bluetooth - Soc Energy Harvesting Observer application](#test-with-bluetooth---soc-energy-harvesting-observer-application)
- [Resources](#resources)
- [Report Bugs & Get Support](#report-bugs--get-support)

## Purpose/Scope ##

This example show users how to use Silicon Labs **RAIL** (Radio Abstraction Interface Layer) Library to craft the BLE advertising packet to reduce energy consumption.

To optimize the energy saving, the device will spend the majority of its lifetime in deep sleep EM4. It will wakeup only when need to advertise data and then then re-enter EM4 mode.

### How The Application Works ###

![Application overview](image/sensor_overview.png)

After a power-on reset the application reads the storage voltage from the e-peas PMIC on the Dual Harvestor Shield and fill the value into the advertising packet before starting to transmit it.

Once the first advertisement is done the application logic sends the microcontroller into EM2 sleep mode for 1 second and repeats this transmit cycle twice. The device goes into EM4 deep sleep mode right after transmitting the third message and stays in it until the BURTC (after 20 seconds) triggers the microcontroller to wake up.

The payload size, TX power and the advertisement time significantly influence the required energy to transmit the switch status successfully. In your implementation, you can vary the level and duration of sleep, and modify payload size and number of transmissions based on available energy or known energy budget.

### Reading data from AEM13920 ####

The device reads battery voltage from AEM13920 IC via I2C protocol. The PMIC can be configured in Hardware by resistors. However, the xG22 SoC can overwrite the default settings by accessing the configuration registers by I2C. The code in this example overwrites the registers using the same settings as the default, so if users need to change settings, they can easily modify it in this code.

#### Advertisement Packet ####

The AdvData field in the advertisement packet is as table below:

| DeviceName | Storage Element Voltage |
|-----|-----|
| EH Sensor | 2 byte (uint16_t value - Big endian)|

- Storage Element Voltage is in (mV) unit. Storage Element Voltage value is in big-endian format. It means that if the HEX value is 0x0BA5 and the DEC value will be equal to:

  ![hex_to_dec](image/hex_to_dec.png)

- Device is non-connectable. It sends [manufacturer specific advertisement](https://github.com/SiliconLabs/bluetooth_stack_features/tree/master/advertising/advertising_manufacturer_specific_data) packets.

## Prerequisites ##

### Hardware Requirements ###

The hardware needed for this example is all included in the xG22-EK8200A, EFR32xG22E Energy Harvesting Explorer Kit. In particular, the following items are used:

- [BRD2710A, EFR32xG22E Explorer Kit Board](https://www.silabs.com/development-tools/wireless/efr32xg22e-explorer-kit?tab=overview)
- BRD8201A, Dual Harvester Shield
- TPLC-3R8/10MR8X1, Lithium Capacitor
- AM-1417CA-DGK-E, Indoor Photovoltaic cell

#### Hardware Connection ####

The Silicon Labs - Dual Harvestor Shield (BRD8201A) is connected on top of the MikroBus header of the EFR32xG22E Explorer Kit (BRD2710A). Make sure that the 45-degree corner of the Silicon Labs - Dual Harvester Shield matches the 45-degree white line of the EFR32xG22E Explorer Kit. In this example, we use a Photovoltaic cell (AM-1417CA-DGK-E) as the power source element and a Lithium Capacitor (TPLC-3R8/10MR8X1) as the storage element. The hardware connection is shown in the image below. For correct assembly of the HW, please refer to UG591 section 4.1 for a description of the sequence required when assembling the setup.

![connection](image/hardware_connection.png)

### Software Requirements ###

- Simplicity Studio
  - Download the [Simplicity Studio v5 IDE](https://www.silabs.com/developers/simplicity-studio)
  - Follow the [Simplicity Studio User Guide](https://docs.silabs.com/simplicity-studio-5-users-guide/1.1.0/ss-5-users-guide-getting-started/install-ss-5-and-software#install-ssv5) to install Simplicity Studio IDE
- [Simplicity SDK Version 2024.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2024.6.2)
- [Energy Harvesting Applications Extension](https://github.com/SiliconLabs/energy_harvesting_applications), follow the [How to add to Simplicity Studio IDE](../../README.md#how-to-add-to-simplicity-studio-ide) to install the extension.
- Simplicity Connect Mobile Application [(iOS)](https://apps.apple.com/us/app/simplicity-connect/id1030932759) or [(Android)](https://play.google.com/store/apps/details?id=com.siliconlabs.bledemo&hl=en_NZ)

## Steps to Run Demo Application ##

To test this application, you can create a project in Simplicity Studio selecting the example project **Bluetooth RAIL- SoC Energy Harvesting Sensor**

### Create a project based on an example project ###

1. In Simplicity Studio from the Launcer Home, add your hardware to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project filtering by **"energy harvesting"**.

2. Click Create button on the project **Bluetooth RAIL- SoC Energy Harvesting Sensor** example for sensor device. Example project creation dialog pops up -> click Create and Finish and Project should be generated.

    ![create_example](image/create_example.png)

3. Build and flash the examples to the board.

### Interacting with the AEM13920 ###

The Energy Harvesting SDK provides a set of APIs for interacting with the PMIC. These APIs are built using proprietary libraries from e-peas. During operation, the device reads battery voltage from AEM13920 IC via I2C protocol and updates the payload data with the extracted data. The PMIC is configured in Hardware by a series of resistors connected to input pins to work with the components provided for this example. However, the xG22 SoC can overwrite the default settings by accessing the configuration registers by I2C. To do that, users need to:

1. Change the macro "USER_OVERRIDE_AEM13920_CONFIG" to "1" in line 45 file "app.c". Its default value is "0". See the example in the below image:

    ![aem13920_user_config_enable](image/aem13920_config_user_enable.png)

2. Modify the macros for the AEM13920 configuration defined in the app.c file:

    ![aem13920_change_config](image/modify_macro_config_aem13920.png)

3. Users exploring other energy sources and/or storage elements may need to add/remove some APIs in the "initAEM13920" function. Users are strongly encouraged to consult [the online software documentation on the e-peas’ website](https://doc.e-peas.com/aem13920/software/index.html) and download [the AEM13920 datasheet](https://e-peas.com/aem13920_datasheet/).

    > **_NOTE_:** The "AEM13920_LoadConfiguration" function loads all bit fields in the I2C registers. Fields that have not been overwritten by I2C maintain their factory default values, and the PMIC will ignore the hardware configuration of its input pins. Please make sure all parameters relative to your specific application (non-default) are set by calling the proper functions, as described in the software documentation.

    ![aem13920_change_init_fct](image/aem13920_init_fct.png)

### Testing ###

#### Test with mobile phone ####

You can use a smartphone application such as the Simplicity Connect Mobile App, to see the advertising packet from the Sensor.

- Open the Simplicity Connect Mobile App.

- Open the Bluetooth Browser.

- Find the device advertising as **EH Sensor**.

  | Open scan               | Filter                      | Visualize data          |
  | :---------------------: | :-------------------------: | :---------------------: |
  | ![find](image/find.png) | ![filter](image/filter.png) | ![view](image/view.png) |

**Note:** The data value displayed on the **Simplicity Connect Mobile App** is just the raw value that is parsed into BLE advertising packet if users want to monitor values in (mV) unit, use the **Bluetooth - Soc Energy Harvesting Observer application** example instead.

**Note:** The reception of BLE frames by the mobile app depends on the power settings of the phone. For best possible reception, avoid the low-power setting of the mobile phone".

#### Test with Bluetooth - Soc Energy Harvesting Observer application ####

Another appoach to test this example is using [**Bluetooth - Soc Energy Harvesting Observer**](https://github.com/SiliconLabs/energy_harvesting_applications/tree/main/example/bt_soc_energy_harvesting_application_observer) that is running on another device. This device will scan the BLE advertising packet, extract the data from the packet and then display it on the console window.

  ![observer_test](image/observer_test.png)

## Resources ##

- UG591: User's Guide to EFR32xG22E Energy Harvesting Explorer Kit
- [Energy Harvesting Documentation](https://www.silabs.com/development-tools/wireless/efr32xg22e-explorer-kit?tab=overview)

> **_NOTE_:** Methods for measuring current consumption specific to this hardware are discussed in the kit's User's Guide (UG591). Some example measurements are also provided in the same document.

## Report Bugs & Get Support ##

To report bugs in the [Energy Harvesting Extension](https://github.com/SiliconLabs/energy_harvesting_applications) projects, you can either

- create a new "Issue" in the "Issues" section of this repo
- or report any issues you found to us via [Silicon Labs Community](https://www.silabs.com/community)

Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.
