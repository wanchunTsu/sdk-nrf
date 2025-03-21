.. _modem_shell_application:

nRF9160: Modem Shell
####################

.. contents::
   :local:
   :depth: 2

The Modem Shell (MoSh) sample application enables you to test various device connectivity features, including data throughput.

Requirements
************

The sample supports the following development kits:

.. table-from-sample-yaml::

Overview
********

MoSh enables testing of different connectivity features such as LTE link handling, TCP/IP connections, data throughput (iperf3 and curl), SMS, GNSS, FOTA updates and PPP.
Hence, this sample is not only code sample, which it also is in many aspects, but also a test application for aforementioned features.
MoSh uses the LTE link control driver to establish an LTE connection and initializes the Zephyr shell to provide a shell command-line interface for users.

The subsections list the MoSh features and show shell command examples for their usage.

LTE link control
================

MoSh command: ``link``

LTE link control changes and queries the state of the LTE connection.
Many of the changes are applied when going to online mode for the next time.
You can store some link subcommand parameters into settings, which are persistent between sessions.

Examples
--------

* Change the system mode to NB-IoT and query the connection status:

  .. code-block:: console

     link funmode -4
     link sysmode -n
     link funmode -1
     link status

* Open another PDP context and close it:

  .. code-block:: console

     link connect -a internet.operator.com
     link status
     link disconnect -I 1

* Enable and configure eDRX:

  .. code-block:: console

     link edrx -e -m --edrx_value 0010 --ptw 0010

* Subscribe for modem TAU and sleep notifications, enable and configure PSM:

  .. code-block:: console

     link tau --subscribe
     link msleep --subscribe
     link psm -e --rptau 01100100 --rat 00100010

* Disable autoconnect when the DK starts (modem will remain in pwroff functional mode and the link is not connected):

  .. code-block:: console

     link nmodeauto --disable

* Set the custom AT commands that are run when switching to normal mode (locking device to band #3):

  .. code-block:: console

     link funmode --poweroff
     link nmodeat --mem1 "at%xbandlock=2,\"100\""
     link funmode --normal


----

AT commands
===========

MoSh command: ``at``

You can use the AT command module to send AT commands to the modem, individually or
in a separate plain AT command mode where also pipelining of AT commands is supported.

Examples
--------

* Send AT command to query network status:

  .. code-block:: console

     at at+cereg?

* Send AT command to query neighbor cells:

  .. code-block:: console

     at at%NBRGRSRP

* Enable AT command events:

  .. code-block:: console

     at events_enable

* Disable AT command events:

  .. code-block:: console

     at events_disable

* Enable autostarting of AT command mode in next bootup:

  .. code-block:: console

     at at_cmd_mode enable_autostart

* Start AT command mode:

  .. code-block:: console

     at at_cmd_mode start
     MoSh AT command mode started, press ctrl-x ctrl-q to escape
     MoSh specific AT commands:
       ICMP Ping: AT+NPING=<addr>[,<payload_length>,<timeout_msecs>,<count>[,<interval_msecs>[,<cid>]]]
     Other custom functionalities:
       AT command pipelining, for example:
         at+cgmr|at+cfun?|at+nping="example.com"
     ===========================================================
     at
     OK

----

Ping
====

MoSh command: ``ping``

Ping is a tool for testing the reachability of a host on an IP network.

Examples
--------

* Ping a URL:

  .. code-block:: console

     ping -d ping.server.url

* Ping an IPv6 address with length of 500 bytes and 1 s intervals (the used PDN needs to support IPv6):

  .. code-block:: console

     ping -d 1a2b:1a2b:1a2b:1a2b::1 -6 -l 500 -i 1000

----

Iperf3
======

MoSh command: ``iperf``

Iperf3 is a tool for measuring data transfer performance both in uplink and downlink direction.

.. note::
   Some features, for example file operations and TCP option tuning, are not supported.

Examples
--------

* Download data over TCP for 30 seconds with a buffer size of 3540 bytes and use detailed output:

  .. code-block:: console

     iperf3 --client 111.222.111.222 --port 10000 -l 3540 --time 30 -V -R

* Upload data over TCP for 30 seconds with the payload size of 708 bytes using a PDN with ID #2 (use the ``link status`` command to see the active PDNs and their IDs):

  .. code-block:: console

     iperf3 -c 111.222.111.222 -p 10000 -l 708 -t 30 --pdn_id 2

* Upload data over UDP for 60 seconds with the payload size of 1240 bytes and use the detailed output as well as debug output:

  .. code-block:: console

     iperf3 --client 111.222.111.222 --port 10000 -l 1240 --time 60 -u -V -d

* Download data over TCP/IPv6 for 10 seconds (the used PDN needs to support IPv6, use the ``link status`` command to see PDP type support for active contexts):

  .. code-block:: console

     iperf3 --client 1a2b:1a2b:1a2b:1a2b::1 --port 20000 --time 10 -R -6

----

Curl
====

MoSh command: ``curl``

Curl is a command-line tool for transferring data specified with URL syntax.
It is a part of MoSh and enables you to test the data download with a "standard" tool.

.. note::
   File operations are not supported.

Examples
--------

* HTTP download:

  .. code-block:: console

     curl http://curl.server.url/small.txt
     curl http://curl.server.url/bigger_file.zip --output /dev/null

* HTTP upload for given data:

  .. code-block:: console

     curl http://curl.server.url/data -d "foo=bar"

* HTTP upload for given number of bytes sent in a POST body:

  .. code-block:: console

     curl http://curl.server.url/data -d #500000

* HTTP upload for given number of bytes sent in a POST body using a PDN with ID #1 (use the ``link status`` command to see active PDNs and their IDs):

  .. code-block:: console

     curl http://curl.server.url/data -d #500000 --pdn_id 1

----

Socket tool
===========

MoSh command: ``sock``

You can use the socket tool to:

* Create and manage socket connections.
* Send and receive data.

Examples
--------

* Open and connect to an IP address and port (IPv4 TCP socket):

  .. code-block:: console

     sock connect -a 111.222.111.222 -p 20000

* Open and connect to hostname and port (IPv4 TCP socket):

  .. code-block:: console

     sock connect -a google.com -p 20000

* Open and connect an IPv6 TCP socket and bind to a port:

  .. code-block:: console

     sock connect -a 1a2b:1a2b:1a2b:1a2b::1 -p 20000 -f inet6 -t stream -b 40000

* Open an IPv6 UDP socket:

  .. code-block:: console

     sock connect -a 1a2b:1a2b:1a2b:1a2b::1 -p 20000 -f inet6 -t dgram

* Open a raw socket:

  .. code-block:: console

     sock connect -f packet -t raw

* Open a socket to a non-default PDP context:

  .. code-block:: console

     link connect -a nondefault.context.com
     sock connect -f packet -t raw -I 1

* Send a string through the socket:

  .. code-block:: console

     sock send -i 0 -d testing

* Send 100 kB of data and show throughput statistics:

  .. code-block:: console

     sock send -i 0 -l 100000

* Send data periodically with 10 s interval:

  .. code-block:: console

     sock send -i 0 -e 10 -d test_periodic

* Calculate the receive throughput:

  .. code-block:: console

     <do whatever is needed to make device receive data after some time>
     sock recv -i 0 -r -l 1000000
     sock recv -i 0
     sock recv -i 0

* Close a socket:

  .. code-block:: console

     sock close -i 0

* Use RAI settings:

  .. code-block:: console

     link funmode -4
     link rai -e
     link funmode -1
     sock connect -a 111.222.111.222 -p 20000
     sock rai -i 0 --rai_last
     sock send -i 0 -d testing

* List open sockets:

  .. code-block:: console

     sock list

----

SMS tool
========

MoSh command: ``sms``

You can use the SMS tool for sending and receiving SMS messages.

Examples
--------

* Register the SMS service so that messages can be received if SIM subscription supports it:

  .. code-block:: console

     sms reg

* Send an SMS message (registration is done automatically if not already done):

  .. code-block:: console

     sms send -n +987654321 -m testing

----

Location tool
=============

MoSh command: ``location``

You can use the Location tool for retrieving device's location with different methods.
See :ref:`lib_location` library for information on the configuration of different location methods and services.
Some default configurations are available to facilitate trials.

Examples
--------

* Retrieve location with default configuration:

  .. code-block:: console

     location get

* Retrieve location with Wi-Fi positioning.
  You need to have a Wi-Fi-enabled device and build the sample with Wi-Fi support.
  If the location is not found, use cellular positioning:

  .. code-block:: console

     location get --method wifi --wifi_timeout 60 --method cellular --cellular_service nrf

* Retrieve location periodically every hour with GNSS and if not found, use cellular positioning:

  .. code-block:: console

     location get --interval 3600 --method gnss --gnss_timeout 300 --method cellular

* Cancel ongoing location request or periodic location request:

  .. code-block:: console

     location cancel

----

GNSS
====

MoSh command: ``gnss``

GNSS provides commands for searching the location of the device.

Examples
--------

* Start GNSS tracking and stop it:

  .. code-block:: console

     gnss start
     gnss stop

* Disable LTE, enable all GNSS output and start continuous tracking with power saving enabled:

  .. code-block:: console

     link funmode --lteoff
     gnss output 2 1 1
     gnss mode cont
     gnss config powersave perf
     gnss start

* Enable LTE PSM, only NMEA output, automatic A-GPS data fetching and start periodic fixes with 5 minute interval and 120 second timeout:

  .. code-block:: console

     link psm -e
     gnss output 0 1 0
     gnss agps automatic enable
     gnss mode periodic 300 120
     gnss start

----

FOTA
====

MoSh command: ``fota``

You can use FOTA to perform software updates over-the-air for both modem and application side.
However, to use this feature, you need to know which updates are available in the servers.
This feature is intended to be used only by selected users and customers to whom available image names are communicated separately.

Examples
--------

* Perform a FOTA update:

  .. code-block:: console

     fota download eu fw_update_package_filename.hex

----

PPP
===

MoSh command: ``ppp``

You can use the PPP (Point-to-Point Protocol) to enable dial-up access to the Internet.
The MoSh command is simple but you need to have a normal dial-up setup in your PC to be able to use the development kit's PPP interface.

.. note::

   On Windows, dial-up connection is not functional when using Segger virtual UART ports.
   PPP has been used successfully with FTDI UART port though.
   Refer to `nRF9160 Hardware Verification Guidelines - UART interface`_.

   PPP has been successfully used running Ubuntu Linux in a virtualization environment hosted by Windows.
   In the hosted virtual Linux environment, using PPP is possible also with plain Segger UART ports.

Examples
--------

* PPP network interface is brought up/down automatically when LTE connection is up/down. Set the PPP network interface up manually:

  .. code-block:: console

     ppp up

* Set the PPP network interface down manually:

  .. code-block:: console

     ppp down

* Set the custom baudrate configuration for PPP UART (default: 115200):

  .. code-block:: console

     ppp uartconf --baudrate 921600

* Set the ``rts_cts`` hardware flow control configuration for PPP UART (default: none):

  .. code-block:: console

     ppp uartconf --flowctrl rts_cts

----

REST client
===========

MoSh command: ``rest``

You can use the REST client for sending simple REST requests and receiving responses to them.

Examples
--------

* Sending a HEAD request with custom dummy headers:

  .. code-block:: console

     rest -d example.com -l 1024 -m head -H "X-foo1: bar1\x0D\x0A" -H "X-foo2: bar2\x0D\x0A"

----

Cloud
=====

MoSh command: ``cloud``

nRF Cloud is a platform for providing, among other things, various location services.
Modem Shell enables you to establish an MQTT connection to nRF Cloud using the :ref:`lib_nrf_cloud` library.
Currently, the ``cloud`` command is useful mostly when using the location services and MQTT is the desired transport protocol.
However, you can use any nRF Cloud services once the MQTT connection is established.

Examples
--------

* Establish the connection to nRF Cloud, request the cell-based location of the device, and disconnect when ready:

  .. code-block:: console

     cloud connect
     location get --method cellular
     cloud disconnect

----

Remote control using nRF Cloud
==============================

Once you have established an MQTT connection to nRF Cloud using the ``cloud`` command, you can use the :guilabel:`Terminal` window in the nRF Cloud portal to execute MoSh commands to the device.
This feature enables full remote control of the MoSh application running on a device that is connected to cloud.
MoSh output, such as responses to commands and other notifications can be echoed to the ``messages`` endpoint and the :guilabel:`Terminal` window of the nRF Cloud portal.
Use the ``print cloud`` command to enable this behavior.
The data format of the input data in the :guilabel:`Terminal` window must be JSON.

Examples
--------

* Establish the connection to nRF Cloud:

  .. code-block:: console

     cloud connect

* To request the device location, enter the following command in the :guilabel:`Terminal` window of the nRF Cloud portal:

   .. code-block:: console

     {"appId":"MODEM_SHELL", "data":"location get --method cellular"}

  The device location appears in the :guilabel:`Location` window.

* An AT command is sent to the modem:

   .. code-block:: console

     {"appId":"MODEM_SHELL", "data":"at AT+COPS=1,2,\\\"24412\\\""}

  Note the syntax for escaping the quotation marks.

Configuration
*************

|config|

Configuration options
=====================

Check and configure the following configuration options for the sample:

.. _CONFIG_MOSH_LINK:

CONFIG_MOSH_LINK
   Enable LTE link control feature in modem shell.

.. _CONFIG_MOSH_PING:

CONFIG_MOSH_PING
   Enable ping feature in modem shell.

.. _CONFIG_MOSH_IPERF3:

CONFIG_MOSH_IPERF3
   Enable iperf3 feature in modem shell.

.. _CONFIG_MOSH_CURL:

CONFIG_MOSH_CURL
   Enable curl feature in modem shell.

.. _CONFIG_MOSH_SOCK:

CONFIG_MOSH_SOCK
    Enable socket tool feature in modem shell.

.. _CONFIG_MOSH_SMS:

CONFIG_MOSH_SMS
    Enable SMS feature in modem shell

.. _CONFIG_MOSH_LOCATION:

CONFIG_MOSH_LOCATION
   Enable Location tool in modem shell.

.. _CONFIG_MOSH_GNSS:

CONFIG_MOSH_GNSS
   Enable GNSS feature in modem shell

.. _CONFIG_MOSH_FOTA:

CONFIG_MOSH_FOTA
   Enable FOTA feature in modem shell

.. _CONFIG_MOSH_PPP:

CONFIG_MOSH_PPP
   Enable PPP feature in modem shell

.. _CONFIG_MOSH_REST:

CONFIG_MOSH_REST
   Enable REST feature in modem shell


.. note::
   You may not be able to use all features at the same time due to memory restrictions.
   To see which features are enabled simultaneously, check the configuration files and overlays.

Additional configuration
========================

Check and configure the following library option that is used by the sample:

* :kconfig:option:`CONFIG_MODEM_ANTENNA_GNSS_EXTERNAL` - Selects an external GNSS antenna.

Building and running
********************

.. |sample path| replace:: :file:`samples/nrf9160/modem_shell`

.. include:: /includes/build_and_run_nrf9160.txt

See :ref:`cmake_options` for instructions on how to provide CMake options, for example to use a configuration overlay.

DK buttons
==========

The buttons have the following functions:

Button 1:
   Raises a kill or abort signal. A long press of the button will kill or abort all supported running commands. You can abort commands ``iperf3`` (also with ``th``), ``curl``, ``ping`` and ``sock send``.

Button 2:
   Enables or disables the UARTs for power consumption measurements. Toggles between UARTs enabled and disabled.

LED indications
===============

The LEDs have the following functions:

LED 1:
   Indicates the LTE registration status.

Testing
=======

After programming the application and all prerequisites to your development kit, test it by performing the following steps:

1. Connect the development kit to the computer using a USB cable.
   The development kit is assigned a COM port (Windows) or ttyACM device (Linux), which is visible in the Device Manager.

#. Create a serial connection to the development kit (J-Link COM port) with a terminal |ANSI| using the following settings:

   * Hardware flow control: disabled
   * Baud rate: 115200
   * Parity bit: no

#. Reset the development kit.

#. Observe in the terminal window that the application starts.
   This is indicated by output similar to the following (there is also a lot of additional information about the LTE connection):

   .. code-block:: console

      *** Booting Zephyr OS build v2.4.99-ncs1-3525-g4d068de3f50f  ***

      MOSH version:       v1.5.0-649-g7e657c2fab02
      MOSH build id:      152
      MOSH build variant: normal

      Initializing modemlib...

      Initialized modemlib
      Network registration status: searching
      Network registration status: Connected - home network
      mosh:~$

#. Type any of the commands listed in the Overview section to the terminal. When you type only the command, the terminal shows the usage, for example ``sock``.

ESP8266 Wi-Fi support
=====================

To build the MoSh sample with ESP8266 Wi-Fi chip support, use the ``-DDTC_OVERLAY_FILE=esp_8266_nrf9160ns.overlay`` and  ``-DOVERLAY_CONFIG=overlay-esp-wifi.conf`` options.
For example:

``west build -p -b nrf9160dk_nrf9160_ns -d build -- -DDTC_OVERLAY_FILE=esp_8266_nrf9160ns.overlay -DOVERLAY_CONFIG=overlay-esp-wifi.conf``

See :ref:`cmake_options` for more instructions on how to add these options.

PPP support
===========

To build the MoSh sample with PPP/dial up support, use the ``-DDTC_OVERLAY_FILE=ppp.overlay`` and ``-DOVERLAY_CONFIG=overlay-ppp.conf`` options.
For example:

.. code-block:: console

   west build -p -b nrf9160dk_nrf9160_ns -- -DDTC_OVERLAY_FILE=ppp.overlay -DOVERLAY_CONFIG=overlay-ppp.conf

After programming the development kit, test it in the Linux environment by performing the following steps:

1. Connect the development kit to the computer using a USB cable.
   The development kit is assigned a ttyACM device (Linux).

#. Open a serial connection to the development kit (/dev/ttyACM2) with a terminal |ANSI| (for example PuTTY).

#. Reset the development kit.

#. Observe in the terminal window that the MoSh starts with the PPP support.
   This is indicated by output similar to the following (there is also a lot of additional information about the LTE connection):

   .. code-block:: console

      Network registration status: searching
      Network registration status: Connected - home network
      Default PDN is active: starting PPP automatically
      PPP: started
      mosh:~$

#. Enter command``ppp uartconf`` that results in the following UART configuration:

   .. code-block:: console

      mosh:~$ ppp uartconf -r
      PPP uart configuration:
        baudrate:     921600
        flow control: RTS_CTS
        parity:       none
        data bits:    bits8
        stop bits:    bits1
      mosh:~$

#. In a Linux terminal, enter the following command to start the PPP connection:

   .. code-block:: console

      $ sudo pppd -detach /dev/ttyACM0 921600 noauth crtscts noccp novj nodeflate nobsdcomp local debug +ipv6 ipv6cp-use-ipaddr usepeerdns noipdefault defaultroute ipv6cp-restart 5 ipcp-restart 5 lcp-echo-interval 0

#. In a MoSh terminal, observe that the PPP connection is created:

   .. code-block:: console

      Dial up (IPv6) connection up
      Dial up (IPv4) connection up
      mosh:~$

#. Now, you are ready to browse Internet in Linux by using the MoSh PPP dial-up over LTE connection.

Application FOTA support
========================

To build the MoSh sample with application FOTA support, use the ``-DOVERLAY_CONFIG=overlay-app_fota.conf`` option.
For example:

.. code-block:: console

   west build -p -b nrf9160dk_nrf9160_ns -d build -- -DOVERLAY_CONFIG=overlay-app_fota.conf

LwM2M carrier library support
=============================

To build the MoSh sample with LwM2M carrier library support, use the ``-DOVERLAY_CONFIG=overlay-lwm2m_carrier.conf`` option.
For example:

.. code-block:: console

   west build -p -b nrf9160dk_nrf9160_ns -d build -- -DOVERLAY_CONFIG=overlay-lwm2m_carrier.conf

P-GPS support
=============

To build the MoSh sample with P-GPS support, use the ``-DOVERLAY_CONFIG=overlay-pgps.conf`` option.
For example:

.. code-block:: console

   west build -p -b nrf9160dk_nrf9160_ns -d build -- -DOVERLAY_CONFIG=overlay-pgps.conf

.. _cloud_build:

Cloud
=====

To build the MoSh sample with cloud connectivity, use the ``-DOVERLAY_CONFIG=overlay-cloud_mqtt.conf`` option.
For example:

.. code-block:: console

   west build -p -b nrf9160dk_nrf9160_ns -d build -- -DOVERLAY_CONFIG=overlay-cloud_mqtt.conf

Remote control using nRF Cloud
==============================

To enable the remote control feature, you need to build the sample with cloud connectivity, see :ref:`cloud_build`.

Zephyr native TCP/IP stack usage over nRF9160 LTE connection
============================================================

To build the MoSh sample with the nRF91 device driver that is not offloading the TCP/IP stack to modem, use the ``-DOVERLAY_CONFIG=overlay-non-offloading.conf`` option.
When running this configuration, the configured MoSh commands, for example iperf3, are using Zephyr native TCP/IP stack over nRF9160 LTE connection in default PDN context.

For example:

.. code-block:: console

   west build -p -b nrf9160dk_nrf9160_ns -- -DOVERLAY_CONFIG=overlay-non-offloading.conf

References
**********

* `nRF9160 Hardware Verification Guidelines - UART interface`_

Dependencies
************

This sample uses the following |NCS| libraries:

* :ref:`lte_lc_readme`
* :ref:`modem_info_readme`
* :ref:`at_cmd_parser_readme`
* :ref:`sms_readme`

This sample uses the following `sdk-nrfxlib`_ libraries:

* :ref:`nrfxlib:nrf_modem`
