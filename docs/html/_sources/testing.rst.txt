.. _testing:

Testing
#######


server setup
************

1. | Open `Coiote Device Management
     server <https://eu.iot.avsystem.cloud/>`__.

2. | Click on **Device inventory** in the left menu in the UI.

3. | Click on **Add new device**.

4. | Click on **Connect your LwM2M device directly via the management
     server**.

5. | Enter the following data and click **Add device**:

     -  Endpoint - urn:imei:*your Device IMEI*

     -  Friendly Name - *recognizable name*

     -  Security mode - psk (Pre-Shared Key)

     -  Key - your key (for example, ``000102030405060708090a0b0c0d0e0f``)
  
      Also, make sure to select the **Key in hexadecimal** checkbox


Enabling notifications
************************

1. | Open `Coiote Device Management
     server <https://eu.iot.avsystem.cloud/>`__.

2. | Click **Device inventory** tab in the top.

3. | Identify your device in the list and click on the anchor text
     corresponding to the device ID in the **Identity** column.

4. | Click the **Objects** tab in the new menu to the left, just below
     **Dashboard**.

5. | Identify one or more objects that you want to receive notifications
     from, and expand it by clicking on them.

6. | Identify one or more resources of the object that you want to track.

     *  You can track either a single resource or all the resources of an
        object. It is recommended to track only the resources that are
        expected to change.

7. | Click the **Value Tracking** button of the selected resource.

8. | Select **Observe** or **Monitoring** from the dropdown menu.

     *  Selecting **Observe** will only update the Value field of the
        resource when it receives a notification.
     *  Selecting **Monitoring** will additionally create a graph of the
        logged datapoints.
9. | Click **Limit data usage** to configure how often notifications are
     sent. For a real deployment, typical notification interval is 6h (4
     updates per day); for testing it can be used 10minutes.

 .. note::
    If this interval is shorter than the PSM interval, any time
    the devices wakes up to send data, it will also perform TAU and
    reset the PSM timer. If the interval is too short (less than 2
    minutes), device will not enter sleep and the water meter data will
    not be updated, so the notification interval must be greater than 2
    minutes.


Testing
*******

After programming the sample to your development kit, complete the
following steps to test it:

1. | Connect the kit to the computer using a USB cable. The kit is
     assigned a COM port (Windows) or ttyACM device (Linux), which is
     visible in the Device Manager.

2. | Connect to the kit with a terminal emulator (for example, PuTTY). See
     `How to connect with
     PuTTY <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/test_and_optimize/testing.html#putty>`__
     for the required settings.

3. | Observe that the sample starts in the terminal window.

4. | Check that the device is connected to the chosen LwM2M server.

5. | Test water flow: If you have a nRF52840DK, you can connect the P0.15
     on nRF52840 DK to P0.13 on nRF9160 DK to output pulse to simulate
     water flow sensor. After you flash the **pwm-pulse-count** program to
     nRF52840DK, pulse waveforms with different frequency can be generated
     by pressing button1, and a measured water volume will be printed in
     the log (you can not include ``overlay-lowpower.conf`` file).

6. | Test leak detection: Press Button1 on nRF9160DK for a long time to
     simulate leak alert.

 .. note::
    * if the button press time exceeds 9 seconds (configurable)will trigger leak alert and report leak event to server.
    * If there is a leak alarm and the button press time is between 1.5 seconds and 5 seconds, the water leakage alarm will be cleared.
  
7. | Test downlink command: customer can send command to reset the
     cumulated water value and pulse value on the server side. As we
     enable queue mode , so the command will be executed when the device
     next connect server.


Firmware Over-the-Air (FOTA)
****************************

You can update the firmware of the device if you are using Coiote Device
Management server. Application firmware updates and modem firmware (both
full and delta) updates are supported.
To update the firmware, complete the following steps:

1. | Identify the firmware image file to be uploaded to the device. See
     `LTE
     modem <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/device_guides/working_with_nrf/nrf91/nrf91_features.html#lte-modem>`__
     and `FOTA
     upgrades <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/device_guides/working_with_nrf/nrf91/nrf91_features.html#nrf9160-fota>`__
     for more information.

2. | Change
     `CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/kconfig/index.html#!CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION>`__
     to a different version and rebuild the sample to generate the upgrade
     image.

3. | Enable TLV data format:
     `CONFIG_LWM2M_RW_OMA_TLV_SUPPORT=y <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/kconfig/index.html#!CONFIG_LWM2M_RW_OMA_TLV_SUPPORT>`__

4. | Open `Coiote Device Management
     server <https://eu.iot.avsystem.cloud/>`__ and click **Update
     Firmware** label.

5. | Click **Multi-component Firmware Update**.

6. | Click **Next** to upload the image file.

7. | **Selected components**, You can choose whether to upgrade the modem
     or the application in the dropdown box.

8. | Click **Upload Firmware** in the bottom left corner and upload the
     firmware image file you generated, and fill in relevant information.

9. | Click **Next** to config the firmware update settings in the menu ,
     Image transport type support **Cops(UDP)** and **HTTP**. we recommend
     **HTTP**. URI format support both **DNS based** and **Raw IP address
     based**. **Timeout** depends on your network condition, the recommend
     value is 60 minutes.

10. | Click **Next** to Schedule Updates.
 
11. | Click **Schedule Update** to wait the upgrade start.

12. | Observe in the terminal window that the image file is being
      downloaded. The download will take some time. If you do not increase
      the server lifetime, the Coiote server might drop the connection to
      the device. The device reconnects later.

13. | When the download is complete, the device restarts on its own after
      installing the firmware. Restart the device manually if it has not
      started automatically. The device runs the updated firmware and
      reconnects to Coiote Device Management server automatically.


Measuring power consumption
***************************

if you want to measure power consumption, you need a `Power Profiler Kit
II
(PPK2) <https://infocenter.nordicsemi.com/topic/ug_ppk2/UG/ppk/PPK_user_guide_Intro.html>`__
and install `nRF Connect for
Desktop <https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop>`__.

`Power Profiler Kit II
(PPK2) <https://infocenter.nordicsemi.com/topic/ug_ppk2/UG/ppk/PPK_user_guide_Intro.html>`__
is an affordable, flexible tool that measures the real-time power
consumption of your designs.

`nRF Connect for
Desktop <https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop>`__
is a cross-platform tool framework for assisting development on nRF
devices. It contains many apps to test, monitor, measure, optimize and
program your applications. **Power Profiler** app is installed as an app
for `nRF Connect for
Desktop <https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop>`__.

The following sections describe how to use **Power Profiler** app to
measure power consumption.

1. | Prepare the nRF9160 DK for current measurements by doing some
     modifications to the DK.
     See the `nRF9160 DK User
     Guide <https://infocenter.nordicsemi.com/index.jsp?topic=%2Fug_nrf91_dk%2FUG%2Fnrf91_DK%2Fprepare_board.html>`__
     for instructions on how to do this.

2. | Connect the PPK2 to the nRF9160 DK with a 4-pin measurement cable using the following pins:
   | • PPK2 VIN to P22 VDD_nRF'
   | • PPK2 VOUT to P22 VDD_nRF
   | • PPK2 GND to P21
3. | Connect the DK to a computer using a micro-USB cable.

4. | Open the Power Profiler app using `nRF Connect for
     Desktop <https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop>`__.

5. | Click **Select Device** (in the top left corner) and select the PPK2
     from the list.

6. | Select **Ampere meter** as the mode, then click **Start** label to
     monitor the power consumption.