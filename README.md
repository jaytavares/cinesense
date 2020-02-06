#Cinemate Audio Sense
![Assembled Board](.github/installed.jpeg)

Do you have Bose Cinemate speakers? Do you wish they would just turn on automatically when you need them
and turn off when you're done. This project provides everything you need to do just that. Using it, you can create an
adapter that plugs into the interface module cable and adds audio sensing capability to your Bose Cinemate speakers.*

\* This project has only been tested with Bose Cinemate GS Series II speakers. I _think_ it should work with other
related models, but YMMV.

## The Parts List
This project only requires a few parts:

- 1x [Dual sided hood](https://www.showmecables.com/db9-hd15-vga-dual-sided-hood-plastic)
- 1x [DB9 Male connector](https://www.showmecables.com/db9-male-solder-connector)
- 1x [DB9 Female connector](https://www.showmecables.com/db9-female-solder-connector)
- 1x [ATTINY85 microchip](https://www.digikey.com/product-detail/en/microchip-technology/ATTINY85-20PU/ATTINY85-20PU-ND/735469)
- 2x [1㏁ resistors](https://www.digikey.com/product-detail/en/stackpole-electronics-inc/CF14JT1M00/CF14JT1M00CT-ND/1830423)*
- 1x [.1µF capacitor](https://www.digikey.com/product-detail/en/kemet/C320C104M5U5TA7301/399-9872-1-ND/3726097)

\* Linked resistor should work, though I just used some spares I had on hand of unknown brand.

## The Circuit Board

![Assembled circuit board](.github/board.jpeg)

### Quick & Easy
For convenience, You can purchase the board on [OSHPark](https://oshpark.com/shared_projects/EzLrNW1y).

### Schematic and Board layout file
The board was designed using Eagle. If you need to make any modifications, you will find the original CAD files in the [board](./board) folder.

## The Software
You're best bet is to head over to the [Releases](releases) page where you'll find the latest hex file compiled for the 
ATTINY85.