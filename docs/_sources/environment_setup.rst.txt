.. _environment_setup:

Requirements
############

This page outlines the requirements that you need to meet before you start working with the LWM2M water meter application.
Once completed, you will be able to run and test the ultra low power application with Nordic nRF91 series.

Hardware requirements
*********************

To meet the hardware requirements, ensure you have two Nordic's boards from the list of below supported DKs:

   * ``nRF9160 DK``
   * ``nRF9151 DK``

To start working with the devices, refer to the following guidelines:

  * `Getting started with nRF91 Series`_

Software requirements
*********************

| To meet the software requirements, install ``nRF Connect SDK`` and ``nRF Command Line Tools``.
| Currently we support ``nRF Connect SDK v2.4.99`` and ``nRF Connect SDK v2.7.0``.
| For nRF9151DK, you can only choose ``nRF Connect SDK v2.7.0`` version.

nRF Connect SDK
===============

Perform the following steps to install `nRF Connect SDK`_ and setup boards:

#. Installing the nRF Connect SDK:

   * Follow `nRF Connect SDK`_ guildelines to install the 'nRF Connect SDK'.

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
.. _nRF Connect SDK: https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/installation/install_ncs.html#install-ncs
.. _nRF Command Line Tools: https://www.nordicsemi.com/Software-and-Tools/Development-Tools/nRF-Command-Line-Tools/Download#infotabs
.. _Getting started with nRF91 Series: https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/gsg_guides/nrf9160_gs.html
.. _nRF Connect SDK Getting started guide: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.3.0/nrf/getting_started.html
.. _Coiote Device Management: https://avsystem.com/coiote-iot-device-management-platform/
.. _AVSystem integration: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/external_comp/avsystem.html#ug-avsystem