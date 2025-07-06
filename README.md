# laserprojector
Code and schematics for a DIY laser projector device.
This is a laser projector which uses a single laser diode to project an image on a wall or screen.
It is capable of displaying 20kpps (points per second). Which is fast enough to display simple text and vector based images.

This project was mainly based on this instructables tutorial: https://www.instructables.com/Arduino-Laser-Show-With-Real-Galvos/

*I did not write the majority of the laser control code! This was mainly taken from the above tutorial. There were a number of bugs in that code that I needed to fix for it to work. Some of it was incompatible with ESP32 altogether and would cause it to crash and hard-reboot. A number of places in the code divide by zero for example, which may work on arduino but causes a hard-crash in ESP32.*

This is the finished projector.

![image0 (3)](https://github.com/user-attachments/assets/75a1c2a8-162f-43c3-8079-810a14de79bd)
![image1 (1)](https://github.com/user-attachments/assets/90277900-7f0b-49ee-a029-df1172de9a89)
![image2](https://github.com/user-attachments/assets/6c1e1e22-c128-4746-b07f-c87f8e8039f6)
![image3](https://github.com/user-attachments/assets/62696837-5e58-449c-bbf1-c83c73d2a6d6)
![image4](https://github.com/user-attachments/assets/a88c629c-8890-41c5-acd7-4fe47d6e0a00)
![image1 (2)](https://github.com/user-attachments/assets/9880bbd1-43ed-4bda-99fe-a5beb2c02468)
![image0 (4)](https://github.com/user-attachments/assets/d5d5be0b-eb59-43e4-be1c-2f0af1f2ce84)

![layout](https://github.com/user-attachments/assets/9cdcb44a-a3f0-472f-a6d1-0f7f6371f9b8)



Bill Of Materials:
- heltec esp32 oled “wifi kit 32” v3.  https://www.amazon.com/HiLetgo-Display-Bluetooth-Internet-Development/dp/B07DKD79Y9
- 488nm cyan laser 1mw.  https://www.ebay.com/itm/285647106367
- LCD Front-Panel Screen PWB51505C-COB (Optrex C-51505NFJ-SLW-AIN)
- TXS0108E Logic Level Shifter 3.3v 5v High Speed Full Duplex 8 Channel (allows interfacing 3v ESP32 with the 5v LCD)
- IRF540N transistor MOSFET (laser control switch).
- 20kpps Laser galvo X/Y + 15v power supply.  https://www.ebay.com/itm/167010013113
- Mains power outlet module with fuse.
- CH375B USB disk drive module.
- 12bit DAC - MCP4822.  https://www.ebay.com/itm/395490637084
- 2x TL082 opamp chips.
- 4 10kohm potentiometer knobs.
- assortment of carbon film resistors ranging from 200ohm to 47kohm.
- electrolytic capacitors for bypass power (can also be ceramic).
- makerbeam extrusions for structure (for the laser and fan).
- automobile cable clamps (to hold laser in place without making electrical contact).
- Small 12v fan.
- 12v blue power indicator lamp.
- Electrocookie protoboards.
- 10 inch by 8 inch by 3 inch metal enclosure.
- 3D printed 35mm x 42mm laser aperture.
- USB flash drive (for loading graphics).
