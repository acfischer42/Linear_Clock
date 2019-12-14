Instructions Linear Clock Setup

© Jonathan, Aliki, Nicolas, Astrid & Achim Fischer 2019

Install Windows Driver
======================

-   Open this link

<https://github.com/acfischer42/ESP8266/raw/Rheinturmuhr/LinearClock-release/CH341SER_WINDOWS.zip>

-   Save to Downloads folder and run (double click)

Write Program to device:
========================

1.  Connect clock to USB port of the PC

2.  Download Firmware: open Link and save to Downloads folder.  
    <https://github.com/acfischer42/ESP8266/raw/Rheinturmuhr/LinearClock-release/LinearClock.nodemcu.v2.bin>

3.  Download Flash Tool: open Link and save to Downloads folder.  
    <https://github.com/acfischer42/ESP8266/raw/Rheinturmuhr/LinearClock-release/NodeMCU-PyFlasher-4.0-x86.exe>

-   Double Click to Run the program

1.  Start Flash Tool:

![](media/8589a13d4ef9b141e200692610349f65.png)

1.  Click on

![](media/47c01ac3b6d663f5b7a1afe059c0c60d.png)

, select Auto-select for the serial port

![](media/1dc23e8bbfce2d9c6faa2de4a93bf774.png)

1.  Select Firmware that was downloaded in step 1:

![](media/6d54edc72be0fa901978b93120ddc6fa.png)

1.  Keep Baud Rate and Flash Mode at default (115200, DIO)

2.  Select erase all

![](media/34945772308f49e8cd1b09596c9f5c09.png)

1.  Click on

![](media/3fb5cc3bcb12df4b68f9229faa3693aa.png)

![](media/07b319327cace9f64fa47f456fff283d.png)

![](media/171ef442bb1dd7d149eca6c8d21541f2.png)

![](media/a72b019b992c20b1db3496072973c5bf.png)

**Unplug from PC and replug or plug into power supply**

Configure Device:
=================

1.  Use a PC/Laptop with Wireless adapter or disable Cellular Data on the phone.

2.  Power up the Clock

3.  Connect Wifi on pc/phone to IOT_AP

    1.  Password: 12345678

4.  Open browser. Enter address <http://192.168.4.1>

5.  Click on 2. Configure Wifi

![](media/e94fcddb3c07c671fc26af5672f44147.png)

![](media/80c454581b06b3a2686e4934a68d7282.png)

1.  Enter your Network SSID and password.

2.  Click on Connect/Disconnect – Device will reboot and connect to your
    wireless network.

3.  You are done

4.  If you need to configure a timeserver (normally not needed)

5.  Click on 1. configure Time.

![](media/8cd33d0da7e3479958dc32be4382e690.png)

1.  Enter Timezone (<https://www.timeanddate.com/worldclock/>)

2.  Enter Timeserver = us.pool.ntp.org or other eu. Ap.

3.  Click on Save Timesettings – you get automatically forwarded to the WIFI
    config page
