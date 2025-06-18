# Silicon Labs Energy Harvesting Applications - Simplicity SDK Extension #

[![Version Badge](https://img.shields.io/badge/-v1.1.2-green)](https://github.com/SiliconLabs/energy_harvesting_applications/releases)
[![SiSDK Badge](https://img.shields.io/badge/SiSDK-v2024.6.2-green)](https://github.com/SiliconLabs/simplicity_sdk/releases)
![License badge](https://img.shields.io/badge/License-Zlib-green)

This repo contains documentation, demos, examples and all the code needed for the Energy Harvesting extension. The content of the repository is meant to be accessed through Silicon Labs Simplicity Studio and should not be used directly.

## Requirements ##

### Hardware ###

- [xG22-EK8200A](https://www.silabs.com/development-tools/wireless/efr32xg22e-energy-harvesting-explorer-kit?tab=overview) - EFR32xG22E Energy Harvesting Explorer Kit
- [xG24-EK2703A](https://www.silabs.com/development-tools/wireless/efr32xg24-explorer-kit?tab=overview) - xG24 Explorer Kit

### Software ###

- [Simplicity Studio v5 IDE](https://www.silabs.com/developers/simplicity-studio)
- [Simplicity SDK Version 2024.6.2](https://github.com/SiliconLabs/simplicity_sdk/releases/tag/v2024.6.2)
  - To install specific SDK version in Simplicity Studio 5, open **Installation Manager** via Help → Update Software. It is recommended to install and update all the items in the tab "Product Updates". In tab **SDKs**, find "Simplicity SDK - 32-bit and Wireless MCUs", click **Add** button on the right, and install **Simplicity SDK 2024.6.2**.
  - The compatible GCC(The GNU Compiler Collection) version of SiSDK 2024.6.2 is 12.2.1 which is provided with Simplicity Studio. Please make sure to use the compatible toolchain while creating the examples of energy harvesting projects. If it's not present, open **Installation Manager** and install it in **Toolchains** tab.
- [Simplicity Connect Mobile App](https://www.silabs.com/developer-tools/simplicity-connect-mobile-app)

### External tools ###

- Power profiling tool: [Qoitech Otii Ace Pro](https://www.qoitech.com/otii-ace/)

## How to add to Simplicity Studio IDE ##

- Clone the repository somewhere on your PC(A git-client tool is required to issue the following command.)

  `git clone https://github.com/SiliconLabs/energy_harvesting_applications.git`

- Add the SDK extension to the SiSDK

  - In Simplicity Studio go to **Preferences** → **Simplicity Studio** → **SDKs** and select the Simplicity SDK Suite to which the SDK extension will be added. Click **Add Extension…**
  - Click **Browse** and navigate to the root folder of the package above and click **Select Folder**.
  - The SDK extension should be displayed in the Detected SDK Extension window with the **Energy Harvesting** name, version, and path. Click **OK** and then **Trust** and **Apply and Close**.
  - Restart Simplicity Studio

> Make sure that **Evaluation** quality software components are enabled in the Software Component view.

## Examples ##

|  No. | Example | Document | Protocol | Type |
|:-----|:-------|:--------:|:--------:|:----:|
|1|Bluetooth - SoC Energy Harvesting Sensor|[Readme](example/bt_soc_energy_harvesting_sensor/README.md)|BLE |Sensor|
|2|Bluetooth RAIL - SoC Energy Harvesting Sensor|[Readme](example/bt_rail_soc_energy_harvesting_sensor/README.md)|Bluetooth RAIL |Sensor|
|3|Bluetooth RAIL - SoC Energy Harvesting Kinetic Switch|[Readme](example/bt_rail_soc_energy_harvesting_kinetic_switch/README.md)|Bluetooth RAIL |Switch|
|4|Bluetooth - SoC Energy Harvesting Application Observer|[Readme](example/bt_soc_energy_harvesting_application_observer/README.md)|BLE |Observer|
|5|Zigbee GPD - SoC Energy Harvesting Sensor|[Readme](example/zigbee_gpd_soc_energy_harvesting_sensor/README.md)|Zigbee |Sensor|
|6|Zigbee GPD - SoC Energy Harvesting Switch|[Readme](example/zigbee_gpd_soc_energy_harvesting_switch/README.md)|Zigbee |Switch|
|7|Zigbee GPC - SoC Energy Harvesting Observer|[Readme](example/zigbee_gpc_soc_energy_harvesting_observer/README.md)|Zigbee |Observer|

## Drivers ##

<table>
    <tbody>
        <tr>
            <td colspan="3" align="left"><b>Power Management</b></td>
        </tr>
      <tr></tr>
        <tr>
            <td>&nbsp;&nbsp;&nbsp;&nbsp;AEM13920 - Energy Harvesting PMIC</td>
            <td>
                <a href="./driver/public/aem13920/">Driver</a>
            </td>
            <td>
                <a href="./driver/docs/AEM13920.md">Document</a>
            </td>
        </tr>
    </tbody>
</table>

## Report Bugs & Get Support ##

To report bugs in the [Energy Harvesting Extension](https://github.com/SiliconLabs/energy_harvesting_applications) projects, you can either

- create a new "Issue" in the "Issues" section of this repo
- or report any issues you found to us via [Silicon Labs Community](https://www.silabs.com/community)

Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.
