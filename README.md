# Cinesense: an Audio Sensing Add-on for Bose Cinemate Speakers
![Assembled Board](.github/installed.jpeg)

## Overview
Do you have Bose Cinemate speakers? Do you wish they would just turn on automatically when you need them
and turn off when you're done. This project provides everything you need to do just that. Using it, you can create an
adapter that plugs into the interface module cable and adds audio sensing capability to your Bose Cinemate speakers.*

\* This project only works with Bose Cinemate __Series II__ speakers. Since the Cinemate Series I speakers don't have an optical audio input, the Cinesense will not work with them.

[Buy One!](#buy-one)

[Build It!](#the-parts-list)

## What People are Saying

> Amazing! Works perfectly. Didn’t expect it to auto shutoff after a couple of seconds when the source is powered off, so that’s a plus! Thanks again! *— Chris T.*

> Just received it and installed it. What an awesome little gadget! Thanks for doing this for us and sending some joy our way. It may be a small thing. But pretty awesome! *— Jennifer S.*

> Works like a charm! Thanks very much again. *— Miroslav V.*

> Hey- just wanted to tell you the adapter works great!  Thanks again! *— Graham L.*

> Hey the adapter is installed and works great, thanks for making one for me, it's worth paying you for your ingenuity. I'm so much happier not having any remotes since I use google voice to do everything else other than volume changes. *— Enrique I.*

> They work perfectly, thank you so much! *— David A.*

> Received it today and it works as described. Thanks again! *— Jeremiah O.*

> You’ve changed my life Jay!!!! Thank you!! *— Nathan R.*

> The adapter works great! Thanks again Jay. *— Dmitrij P.*

> Just got it in the mail, it's amazing!! Thank you so much! Timing was impeccable.. I literally was just looking at new speakers now I don't have to! You have no idea how happy this makes me haha *— Julie C.*

> Dude, you’re a wizard! Love this thing. Who knew something so simple looking could be so helpful? Thanks again! *— Brian W.*

### How It Works
When placed inline between the sub woofer module and the interface module, the Cinesense adapter repeatedly polls the 
optical audio input for a signal. When a signal is sensed, the speakers are automatically turned on. The adapter will 
continue to monitor the signal while the speakers are in use. Once the signal has stopped for ten seconds, the adapter 
will turn the speakers off.

NOTE: The adapter only looks at the optical input and will not respond to audio signals on the analog inputs. This is a 
potential future enhancement that can be made to the design. Also, the term "Audio Signal" here is referring to optical 
audio "data" which is not the same as "sound". It's possible for the optical audio signal to be present _and_ silent 
(e.g. music player is powered on but is paused).

Aside from the new found ability to automatically turn on and off in response to the audio signal, all aspects of the 
speakers are the same. You can still use your remote to control the volume and mute the speakers. You can even use your 
remote to turn the speakers off, though if there is an audio signal present, they'll just turn right back on again.

## Theory of Operation (for nerds)
Below I've included not just how the Cinesense works but everything I learned about these speakers in the course 
of building this project. Hopefully it will be helpful to someone else interested in hacking these speakers.

![Circuit overview](.github/connections.png)

Some notes:

  - Pin 4 - Interface module CONTROL LINE
    - Has three possible states
      - LOW - power off
      - HIGH - power on, LED on
      - FLOAT - power on, LED off
        - This is how the LED is able to flash when the speakers are muted and in response to remote button presses.
        - This allows us to turn on the optical input without also turning on the LED in the interface module. 👍
  - Pin 8 - Optical audio
    - Digital audio data (Didn't seem to be at the same 3.3v logic level--it was lower.)
  - Pin 9 - IR Line
    - Decoded IR data (NEC protocol) with no carrier frequency

As you can see in the diagram above, the micro-controller interrupts the control line in order to allow independent control of the interface 
module. The control line input can be polled to determine the power state of the speakers, the resistors on the control 
line input act as a voltage divider allowing the micro-controller to accurately detect the floating state. By floating 
the control line output, the interface module can be turned on without lighting its LED or turning on the speakers. This
supplies power to the optical input and allows us to check for a signal. 

In my experimentation, it didn't appear that I could turn on the speakers by simply bringing the control line high. So, 
instead, when an audio signal is sensed, I inject the 'ON' infrared command directly into the IR Line. This simulates 
pressing the on button on the remote and the speaker system powers on in turn. Since we are tapping into the IR data line 
directly, we need only inject the command itself with no carrier frequency. These speakers use the NEC IR protocol. 
Here are the commands that they support:

  - 0x5D0532CD On/Off
  - 0x5D05C03F Vol. Up
  - 0x5D0540BF Vol. Down
  - 0x5D05807F Mute

## Buy One
Unfortunately, due to COVID-19 related microchip shortages, I have stopped taking orders for adapters. If you would like to be notified when they become available again, [click here to join the waiting list](https://forms.gle/aZ4epW23LM2YaGUw5).
### International shipping
If you're not in the U.S., [contact me](https://github.com/jaytavares) for a shipping quote. Typically, shipping fees to most other countries will add $15 to the price.

## The Parts List
You'll need a few parts:

### The PCB

- 1x [Printed circuit board](https://oshpark.com/shared_projects/PZXtdoM9)

### The Components

Fast order: Click for a shopping cart with all the needed components

[Digi-Key](https://www.digikey.com/short/znq4vb) | [Mouser](https://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=052330197b)

- 1x Dual sided hood
- 1x DB9 Male connector
- 1x DB9 Female connector
- 1x ATTINY85 microchip
- 2x 1㏁ resistors
- 1x 1µF capacitor
- 1x 2x3 header (_Optional_--only needed if you plan to program the chip on the board.)

## The Tools
You'll need soldering supplies to assemble the board and a way to program the microchip. I use the [Pocket AVR Programmer](https://www.sparkfun.com/products/9825) from SparkFun.
 
## The Circuit Board

![Assembled circuit board](.github/board.jpeg)

### Quick & Easy
For convenience, You can purchase the board on [OSHPark](https://oshpark.com/shared_projects/PZXtdoM9).

### Schematic and Board layout file
The board was designed using Eagle. If you need to make any modifications, you will find the original CAD files in the [board](./board) folder.

### Assembly
![Tabs on circuit boards](.github/tabs.jpeg)

Before you start assembly, be sure to file off any tabs left on your boards as a result of panelization. If you skip this
step, you'll have trouble fitting the completed board into the housing. This is especially important for tabs along the 
short edges where the DB9 connectors attach. After removing the tabs, I dry fit the connectors on the board and placed 
everything in one half of the housing. I was then able to tack solder the connectors in place to ensure the final 
assembly would fit in the housing. 

## The Software

### Use Precompiled software (Easiest option)
 1. Download the most recent `firmware.hex` file from the [Releases](https://github.com/jaytavares/cinesense/releases) page.
 2. Connect your AVR Programmer to the chip. (I've included an ISP header on the board if you don't have a standalone programmer for ATTINY chips.)
 3. Use [avrdude](http://savannah.nongnu.org/projects/avrdude/) to program the ATTINY chip. Assuming you're using the "Pocket AVR Programer", run:
```
$ avrdude -c usbtiny -p t85 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m -U flash:w:firmware.hex:i
```

### Build from the source (Most flexible option)
If you would like to customize your Cinesense, you'll have to build the software from the source. The software for this project is built using the fantastic [PlatformIO](https://platformio.org). 

#### Programming the chip
 1. Connect your AVR Programmer to the chip. (I've included an ISP header on the board if you don't have a standalone programmer for ATTINY chips.)
 2. In the root directory of this project, run:
```
platformio run
```
This should compile the software, burn the correct fuses, and upload the software to the chip.

## Legal
This project is licensed under the MIT License. See [LICENSE.txt](LICENSE.txt) for more information. "Bose" and "Cinemate" are registered trademarks of Bose Corporation, Framingham, MA. This project is not endorsed by or affiliated with the Bose Corporation.
