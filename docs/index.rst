.. _index:

Welcome to nRF Connect SDK – LWM2M water meter
##############################################

The water meter sample demonstrates how an nRF9160-based device
communicates with the AVsystem server over LWM2M. This sample showcases
the use of the Zephyr’s `Lightweight Machine to Machine
(LwM2M) <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/connectivity/networking/api/lwm2m.html>`__\ library
and nRF Connect SDK `LwM2M client
utils <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/networking/lwm2m_client_utils.html#lib-lwm2m-client-utils>`__
library .

The overal application in this example implements a Low Power Pulse
counting water meter. The nRF9160 connect to the LwM2M server and enter
PSM; meanwhile it counts the pulses from a GPIO; depending on the
notification settings on the cloud, the application will send updated
data to the cloud periodically or whenever there is an alarm.
Communication is encrypted via DTLS.

In this example, PSM requested is 24h; lwm2m lifetime is 24h, and will
send registration update 10 minutes before lifetime expiration. Which
means that if there is no notification and alarm message to send, device
will wakeup once a day to save power consumption.

The sample also supports FOTA through `LwM2M Advanced Firmware
Update <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/samples/nrf9160/lwm2m_client/fota.html>`__.
You can update the firmware of the application and modem firmware (both
full and delta) flexible, for more information, please refer to the
**Firmware Over-the-Air (FOTA)** chapter below.

This documentation provides instructions on how to correctly setup LWM2M water meter application for the `nRF Connect SDK`_.

.. toctree::
   :maxdepth: 1
   :glob:
   :caption: Subpages:

   about_lwm2m water meter.rst
   environment_setup.rst
   samples.rst
   testing.rst
   additional_resources.rst

.. _nRF Connect SDK: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.3.0/nrf/index.html

