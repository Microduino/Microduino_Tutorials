# ZMAC: Makes an Arduino Library from the Atmel 802.15.4 MAC distribution

## The Problem

Atmel produces an IEEE 802.15.4 MAC layer for its radios, which works great on Zigduino.  Unfortunately, it will not work in the Arduino IDE.  We cannot modify it and redistribute it, due to the license.

## The Solution

Here's how we solve this.  You download the MAC distribution yourself, sign up with Atmel, agree to the license, etc.  Then you get ZMAC, and run the fileprep script.  This will copy and modify the files as needed to organize them so the Arduino library can pick them up.  It will even translate some of the examples into a form that will load and compile and run using the Arduino IDE.

## Pre-requisites 

Make sure you have these all in place, or this libary won't do you much good.

1. [Zigduino hardware](http://www.logos-electro.com/zigduino/).
1. [Arduino environment](http://www.arduino.cc/download/).  Obviously, you'll need Arduino.  This has been tested with 0022.
1. [Zigduino modifications to Arduino environment](https://github.com/logos-electromechanical/Zigduino).  Make sure this is all set up properly, and you can upload the 'Blink' sketch successfully.
1. [Atmel's IEEE 802.15.4 MAC distribution](http://www.atmel.com/dyn/products/tools_card.asp?tool_id=4675).  This has been tested with 2.6.1.

## Installing

1. Unpack this distribution where you put your Arduino libaries, e.g. C:\users\me\Documents\Arduino\libraries\ZMAC
1. Download and install [gawk](http://gnuwin32.sourceforge.net/packages/gawk.htm)
1. Edit fileprep.cmd on Windows or fileprep.sh on Mac or Linux...
1. ...Update the MAC\_INSTALL variable to where you installed the MAC, typically "C:\Atmel\MAC\_v\_2\_6\_1"
1. ...Update the GAWK variable to the location of the gawk binary, typically "C:\Program Files\GnuWin32\bin\gawk.exe"
1. Open a command window, cd to the ZMAC directory
1. Type "fileprep.cmd" on Windows or "./fileprep.sh" on Mac or Linux
1. Exit Arduino
1. Start Arduino
1. Select Tools > Board > Zigduino
1. Select File > Examples > ZMAC > Star\_NoBeacon
1. Verify
1. Upload
1. Smile!

## How you know it's working

To test the Star\_NoBeacon example...

1. Upload it to two Zigduinos.
1. Only power up one of them.  This will become the coordinator.  Leave the other one off.
1. The first unit will search for a network (fast blinking red light), then give up and create its own (solid red light).
1. Then power on the second unit.  This will become the end device.
1. The second unit will search for the first one (fast blinking red light), and then find it.
1. They will then transmit data every few seconds, indicated by a slow blinking red light, in sync between the two units.

## Now the fun is just beginning

This MAC library is not particularly easy to use for beginners.  It expects a crisp understanding of the principles of the [IEEE 802.15.4 MAC](http://en.wikipedia.org/wiki/IEEE_802.15.4-2006) design.  There are two things to study to help understanding this:

1. 802.15.4 guides and documentation.  Try [This Whitepaper](http://www.daintree.net/downloads/whitepapers/understanding_sensor_networking_whitepaper.pdf?04689c51410dea3ab5cc32b2b01c7ae7=244dd3e55128d9329468ba2e55b08fec), and anything else you can find searching the web.
1. The user guide in the Atmel MAC.  Read through the Doc/User\_Guide/AVR2025\_User\_Guide.pdf.  This is a dense guide chock full of stuff you need to know.  Start with Chapter 9, which explains the sample applications in great detail.
1. Dig through the Star\_Nobeacon example code.  I recommend putting in "printf" debugging statements everywhere so you can keep track of what's going on.

## A better solution

The next logical step to this is to create an library that's easier to use for mainstream Arduino users (e.g. beginners) that presents a higher-level layer to the user.  Currently the app is expected to handle much of the details of scanning and association.  This could all be pulled into the higher-level library.

This hypothetical library should enable all the functionality covered in Robert Faludi's excellent [Wireless Sensor Networks](http://www.faludi.com/bwsn/) book.
