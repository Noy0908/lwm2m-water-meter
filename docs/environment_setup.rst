.. _environment_setup:

Requirements
############

This page outlines the requirements that you need to meet before you start working with the LWM2M water meter application.
Once completed, you will be able to run and test the ultra low power application with Nordic nRF91 series.

Hardware requirements
*********************

To meet the hardware requirements, ensure you have two Nordic's boards from the list of three supported models:

  * `nRF9160 DK`_
  
To start working with the devices, refer to the following guidelines:

  * `Getting started with nRF91 Series`_

Software requirements
*********************

To meet the software requirements, install `nRF Connect SDK v2.4.99`_ and `nRF Command Line Tools`_.

nRF Connect SDK
===============

Perform the following steps to install `nRF Connect SDK v2.4.99`_ and setup boards:

#. Set up your development environment by choosing one of the installation methods below:

   * Follow `Installing automatically`_ guildelines to perform an automatic installation through the Toolchain Manager.
   * Follow `Installing manually`_ guidelines to perform a manual installation.

   .. note::
      For additional information on setting up the device as well as Nordic’s development environment and tools, see the `nRF Connect SDK Getting started guide`_.

#. go to the nrf directory of your SDK, and fetch the origin update.

   .. code-block:: console

      $ git fetch origin

#. checkout to the main branch.

   .. code-block:: console

      $ git checkout main

#. pull the latest update to main branch.

   .. code-block:: console

      $ west update

nRF Command Line Tools
======================

Download the nRF Command Line from the `nRF Command Line Tools`_ page.

Server setup
*********************

Additionally, the application requires an activated SIM card, and an LwM2M
server, we select AVSystem’s `Coiote Device Management`_
server. To know more about the AVSystem integration with nRF Connect
SDK, see `AVSystem integration`_.


.. _nRF9160 DK: https://www.nordicsemi.com/Products/Development-hardware/nrf9160-dk
.. _nRF Connect SDK v2.4.99: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.99-dev2/nrf/index.html
.. _nRF Command Line Tools: https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF-Command-Line-Tools/Download#infotabs
.. _Getting started with nRF91 Series: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/device_guides/working_with_nrf/nrf91/nrf9160_gs.html#
.. _nRF Connect SDK Getting started guide: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.3.0/nrf/getting_started.html
.. _Installing automatically: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.3.0/nrf/gs_assistant.html#installing-automatically
.. _Installing manually: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.3.0/nrf/gs_installing.html#install-the-required-tools
.. _Coiote Device Management: https://www.avsystem.com/products/coiote-iot-device-management-platform
.. _AVSystem integration: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/external_comp/avsystem.html#ug-avsystem