

## Introduction

The LwM2M water meter demonstrates usage of the [Lightweight Machine to Machine (LwM2M)](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/documentation/glossary.html#term-Lightweight-Machine-to-Machine-LwM2M) protocol to connect a water meter to AVsystem through LTE/NBIOT. This sample uses the [LwM2M client utils](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/networking/lwm2m_client_utils.html#lib-lwm2m-client-utils) library.

The sample also supports a proprietary mechanism to fetch location assistance data from [nRF Cloud](https://nrfcloud.com/?__hstc=8439722.cbeac76abe662bf67fe62ab3247a2a9b.1663037262283.1688002378986.1688026636411.129&__hssc=8439722.4.1688026636411&__hsfp=4099787016) by proxying it through the LwM2M server. For this, the sample makes use of the [LwM2M location assistance](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/networking/lwm2m_location_assistance.html#lib-lwm2m-location-assistance) library.



### requirements

The sample runs on a nRF9160DK, based on NCS2.4.99 SDK. 

When built for an `_ns` build target, the sample is configured to compile and run as a non-secure application with [Cortex-M Security Extensions enabled](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/config_and_build/board_support.html#app-boards-spe-nspe-cpuapp-ns). Therefore, it automatically includes [Trusted Firmware-M](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/security/tfm.html#ug-tfm) that prepares the required peripherals and secure services to be available for the application.

Additionally, the sample requires an activated SIM card, and an LwM2M server, we select AVSystem’s [Coiote Device Management](https://www.avsystem.com/products/coiote-iot-device-management-platform/) server. To know more about the AVSystem integration with nRF Connect SDK, see [AVSystem integration](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/external_comp/avsystem.html#ug-avsystem).



### overview

The sample implements the following LwM2M objects:

| LwM2M objects                       | Object ID | nRF9160 DK |
| ----------------------------------- | --------- | ---------- |
| LwM2M Server                        | 1         | Yes        |
| Device                              | 3         | Yes        |
| Connectivity Monitoring             | 4         | Yes        |
| Water meter                         | 3424      | Yes        |
| ECID-Signal Measurement Information | 10256     | Yes        |
| GNSS Assistance                     | 33625     | Optional   |
| Ground Fix                          | 33626     | Yes        |
| Advanced Firmware Update            | 33629     | Yes        |



water meter objects has below resources:

![](C:\Work\git\LET_sample\lwm2m_water_meter\docs\water_meter_object.png)



### Notifications

LwM2M specifies the Notify operation, which can be used to notify the server of changes to the value of a resource field, for example, the measured value of a water meter sensor. This allows active monitoring while using minimal bandwidth, as notifications can be sent with the updated values without the need for the server querying the client regularly.

To enable notifications, the server must initiate an observation request on one or more resources of interest. For more information, see **Enabling notifications** chapter below.



### LWM2M_send

LwM2M also supports active data reporting, which can be used to send the alarm message to cloud timely, for example, if the water leakage alarm is detected, the device will immediately be awakened and connected to the LWM2M server, reporting the alarm message to the cloud timely. 



### [Release Assistance Indication (RAI)](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/test_and_optimize/optimizing/power_nrf9160.html#release-assistance-indication-rai)

If you have low-level control over the protocol your IOT device uses, you might know when you should not expect more data. In that case, you can request to skip the RRC idle mode using [Release Assistance Indication (RAI)](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/documentation/glossary.html#term-Release-Assistance-Indication-RAI). The recommended way to do this is enable below macro:

**CONFIG_LWM2M_CLIENT_UTILS_RAI=y**



## sample description

The water meter sample demonstrates how an nRF9160-based device communicates with the AVsystem server over LWM2M. This sample showcases the use of the Zephyr’s [Lightweight Machine to Machine (LwM2M)](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/connectivity/networking/api/lwm2m.html)library and  nRF Connect SDK [LwM2M client utils](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/networking/lwm2m_client_utils.html#lib-lwm2m-client-utils) library . 

The overal application in this example implements a Low Power Pulse counting water meter. The nRF9160 connect to the LwM2M server and enter PSM; meanwhile it counts the pulses from a GPIO; depending on the notification settings on the cloud, the application will send updated data to the cloud periodically or whenever there is an alarm. Communication is encrypted via DTLS. 

In this example, PSM requested is 24h; lwm2m lifetime is 24h, and will send registration update 10 minutes before lifetime expiration. Which means that if there is no notification and alarm message to send, device will wakeup once a day to save power consumption.

The sample also supports FOTA through [LwM2M Advanced Firmware Update](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/samples/nrf9160/lwm2m_client/fota.html). You can update the firmware of the application and modem firmware (both full and delta) flexible, for more information, please refer to the **Firmware Over-the-Air (FOTA)** chapter below.



### pull the source code

> https://github.com/Noy0908/lwm2m-water-meter/tree/water-meter_v2.4.99
>
> The branch **water-meter** is the application code which runs on nRF9160DK.
>
> The branch **pwm-pulse-count** is used to generate pulse waveforms which runs on nRF52840DK to simulate water flow sensor.



### Configuration and test

##### Set the LWM2M PSK

- Open `src/prj.conf`.
- Set [CONFIG_APP_LWM2M_PSK](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/samples/nrf9160/lwm2m_client/sample_description.html#config-app-lwm2m-psk) to the hexadecimal representation of the PSK you will use when registering the device with the server.



##### Build the sample with the following overlays:

- `overlay-avsystems.conf`

- `overlay-lwm2m-1.1.conf`

- `overlay-assist-cell.conf`

- `overlay-adv-firmware.conf`

- `overlay-lowpower.conf`

- `overlay-dtls-cid.conf`

- `boards/nrf9160dk_nrf9160_ns.conf`

  **if you want to use NBIOT network, you need to add below config file.**

- `overlay-nbiot.conf`



##### server setup

- Open [Coiote Device Management server](https://eu.iot.avsystem.cloud/).

- Click on **Device inventory** in the left menu in the UI.

- Click on **Add new device**.

- Click on **Connect your LwM2M device directly via the management server**.

- Enter the following data and click **Add device**:

  - Endpoint - urn:imei:*your Device IMEI*
  - Friendly Name - *recognizable name*
  - Security mode - psk (Pre-Shared Key)
  - Key - your key (for example, `000102030405060708090a0b0c0d0e0f`)

  Also, make sure to select the **Key in hexadecimal** checkbox



##### Enabling notifications

- Open [Coiote Device Management server](https://eu.iot.avsystem.cloud/).
- Click **Device inventory** tab in the top.
- Identify your device in the list and click on the anchor text corresponding to the device ID in the **Identity** column.
- Click the **Objects** tab in the new menu to the left, just below **Dashboard**.
- Identify one or more objects that you want to receive notifications from, and expand it by clicking on them.
- Identify one or more resources of the object that you want to track.
  - You can track either a single resource or all the resources of an object. It is recommended to track only the resources that are expected to change.
- Click the **Value Tracking** button of the selected resource.
- Select **Observe** or **Monitoring** from the dropdown menu.
  - Selecting **Observe** will only update the Value field of the resource when it receives a notification.
  - Selecting **Monitoring** will additionally create a graph of the logged datapoints.
- Click **Limit data usage** to configure how often notifications are sent. For a real deployment, typical notification interval is 6h (4 updates per day); for testing it can be used 10minutes.
  <font color=red>NOTE: if this interval is shorter than the PSM interval, any time the devices wakes up to send data, it will also perform TAU and reset the PSM timer. If the interval is too short（less than 2 minutes), device will not enter sleep and the water meter data will not be updated, so the notification interval must be greater than 2 minutes. </font>



##### Testing

After programming the sample to your development kit, complete the following steps to test it:

- Connect the kit to the computer using a USB cable. The kit is assigned a COM port (Windows) or ttyACM device (Linux), which is visible in the Device Manager.

- Connect to the kit with a terminal emulator (for example, PuTTY). See [How to connect with PuTTY](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/test_and_optimize/testing.html#putty) for the required settings.

- Observe that the sample starts in the terminal window.

- Check that the device is connected to the chosen LwM2M server.

- Test water flow: If you have a nRF52840DK, you can connect the P0.15 on nRF52840 DK to P0.13 on nRF9160 DK to output pulse to simulate water flow sensor. After you flash the **pwm-pulse-count** program to nRF52840DK, pulse waveforms with different frequency can be generated by pressing button1, and a measured water volume will be printed in the log (you can not include `overlay-lowpower.conf` file).

- Test leak detection: Press Button1 on nRF9160DK for a long time to simulate leak alert. 

  > ​	if the button press time exceeds 9 seconds (configurable)will trigger leak alert and report leak event to server.
  >
  > ​	If there is a leak alarm and the button press time is between 1.5 seconds and 5 seconds, the water leakage alarm will be cleared.

- Test downlink command: customer can send command to reset the cumulated water value and pulse value on the server side. As we enable queue mode , so the command will be executed when the device next connect server.



##### Measuring power consumption

if you want to measure power consumption, you need a [Power Profiler Kit II (**PPK2**)](https://infocenter.nordicsemi.com/topic/ug_ppk2/UG/ppk/PPK_user_guide_Intro.html) and install [nRF Connect for Desktop](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop).

[Power Profiler Kit II (**PPK2**)](https://infocenter.nordicsemi.com/topic/ug_ppk2/UG/ppk/PPK_user_guide_Intro.html) is an affordable, flexible tool that measures the real-time power consumption of your designs.

[nRF Connect for Desktop](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop) is a cross-platform tool framework for assisting development on nRF devices. It contains many apps to test, monitor, measure, optimize and program your applications. **Power Profiler** app is installed as an app for [nRF Connect for Desktop](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop).

The following sections describe how to use **Power Profiler** app to measure power consumption.

1. Prepare the nRF9160 DK for current measurements by doing some modifications to the DK. 

   See the [nRF9160 DK User Guide](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fug_nrf91_dk%2FUG%2Fnrf91_DK%2Fprepare_board.html) for instructions on how to do this.

2. Connect the PPK2 to the nRF9160 DK with a 4-pin measurement cable using the following pins:
   • PPK2 VIN to P22 VDD_nRF'
   • PPK2 VOUT to P22 VDD_nRF
   • PPK2 GND to P21

3. Connect the DK to a computer using a micro-USB cable.

4. Open the Power Profiler app using  [nRF Connect for Desktop](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop).

5. Click **Select Device** (in the top left corner) and select the PPK2 from the list.

6. Select **Ampere meter** as the mode, then click **Start** label to monitor the power consumption.



### Firmware Over-the-Air (FOTA)

You can update the firmware of the device if you are using Coiote Device Management server. Application firmware updates and modem firmware (both full and delta) updates are supported.

To update the firmware, complete the following steps:

- Identify the firmware image file to be uploaded to the device. See [LTE modem](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/device_guides/working_with_nrf/nrf91/nrf91_features.html#lte-modem) and [FOTA upgrades](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/device_guides/working_with_nrf/nrf91/nrf91_features.html#nrf9160-fota) for more information.
- Change [`CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION`](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/kconfig/index.html#CONFIG_MCUBOOT_IMAGE_VERSION) to a different version and rebuild the sample to generate the upgrade image.
- Enable TLV data format: [`CONFIG_LWM2M_RW_OMA_TLV_SUPPORT=y`](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/kconfig/index.html#!CONFIG_LWM2M_RW_OMA_TLV_SUPPORT)
- Open [Coiote Device Management server](https://eu.iot.avsystem.cloud/) and click **Update Firmware** label.
- Click **Multi-component Firmware Update**.
- Click **Next** to upload the image file.
- **Selected components**, You can choose whether to upgrade the modem or the application in the dropdown box.
- Click **Upload Firmware** in the bottom left corner and upload the firmware image file you generated, and fill in relevant information.
- Click **Next** to config the firmware update settings in the menu , Image transport type support **Cops(UDP)** and **HTTP**. we recommend **HTTP**. URI format support both **DNS based** and **Raw IP address based**. **Timeout** depends on your network condition, the recommend value is 60 minutes.
- Click **Next** to Schedule Updates.
- Click **Schedule Update** to wait the upgrade start.
- Observe in the terminal window that the image file is being downloaded. The download will take some time. If you do not increase the server lifetime, the Coiote server might drop the connection to the device. The device reconnects later.
- When the download is complete, the device restarts on its own after installing the firmware. Restart the device manually if it has not started automatically. The device runs the updated firmware and reconnects to Coiote Device Management server automatically.

