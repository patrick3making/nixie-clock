# nixie-clock
This alternative clock imitates a nixie tube by edge lighting acrylic tiles. The number on the tile indicates the current hour, and the color aproximately incates the minute.  
![circuit drawing](https://github.com/patrick3coffee/nixie-clock/blob/master/3-20-PM.jpg?raw=true)  
  
## Background  
I have access to a laser cutter at work and I wanted to use it. I searched thingiverse and found [this single led nixie digit ](https://www.thingiverse.com/thing:880429). I modified and laser cut the tiles from that design. The base and circuit assembly is my design.  
  
# Construction
![circuit drawing](https://github.com/patrick3coffee/nixie-clock/blob/master/inside.jpg?raw=true)  
## Electronics
![circuit drawing](https://github.com/patrick3coffee/nixie-clock/blob/master/drawing.png?raw=true)  
The electronics are controlled by an [Arduino Pro Mini](https://sfe.i0/p11113). Time is kept by a [SparkFun RTC Module](https://sfe.io/p12708). Light is provided by some [loose WS2812](https://sfe.io/p13667)s. That LED requires some decoupling, so I soldered the power connections together in four rows and added 10uf capacitors at the beginning of each row. It's not ideal, but the seem to work well.  
Power is provided through a USB connector I stole from a broken dev board. Since this runs on 5 volts, USB provides what I need and helps me use up a pile of old cables that might otherwise end up in the trash.  
![circuit diagram](https://github.com/patrick3coffee/nixie-clock/blob/master/schematic.png?raw=true)  
  
## Base  
The base has holes for the teeth of each tile that fit just right. The array of holes is staggered very close to right, but there is some error from one tile to the next. The space undneath has square holes for the LEDs. The bottom cavity is 1 inch deep because I wanted plenty of room for the electronics.  
  
## Assembly
The best way to do this project is make a custom pcb for the LEDs and controls. In lieu of doing it correctly, I spent immensely greater time and effort putting tiny wires between tiny smd components.  
**Recommendation**: If you make this, redesign the base for [through hole WS2812](https://sfe.io/p12999)s. That will make the soldering easier, and those long leads will save SO MUCH wire stripping.  
After 3D printing the base, I super-glued the LEDs into their sockets. Next I soldered about 100 wires, most of which were less than an inch long. They came from an old 25-pair telephone cable. It makes great hookup wire because it's solid core copper and it's not twisted like cat 5. Power is distributed across a piece of prototyping board.
