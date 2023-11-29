# Hardware Notes
Pinout is version 2.0: left side starts with pins 2, 3, 4.

Information is extracted from:
 - https://bob.cs.sonoma.edu/IntroCompOrg-RPi/sec-gpio-pins.html
 - https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf

# Pinout

BIT | GPIO | Register | Pin    | GPIO-Bits
------------------------------------------
  0 |   11 |  GPFSEL1 | FSEL11 | 5-3
  1 |   17 |  GPFSEL1 | FSEL17 | 23-21
  2 |   18 |  GPFSEL1 | FSEL18 | 26-24
  3 |   22 |  GPFSEL2 | FSEL22 | 8-6
  4 |   23 |  GPFSEL2 | FSEL23 | 11-9
  5 |   24 |  GPFSEL2 | FSEL24 | 14-12
  6 |   25 |  GPFSEL2 | FSEL25 | 17-15
  7 |   27 |  GPFSEL2 | FSEL27 | 23-21

# Function Select Register Addresses

Register | Memory Address
-------------------------
GPFSEL0  | 0x7e200000
GPFSEL1  | 0x7e200004
GPFSEL2  | 0x7e200008

Function Select Values:
 - 000 = GPIO Pin is an input
 - 001 = GPIO Pin is an output
 - 100 = GPIO Pin takes alternate function 0
 - 101 = GPIO Pin takes alternate function 1
 - 110 = GPIO Pin takes alternate function 2
 - 111 = GPIO Pin takes alternate function 3
 - 011 = GPIO Pin takes alternate function 4
 - 010 = GPIO Pin takes alternate function 5 

# Default Values (after boot):

Values after booting the OS:

`raspi-gpio get 11,17,18,22,23,24,25,27`
```
GPIO 11: level=0 fsel=0 func=INPUT
GPIO 17: level=0 fsel=0 func=INPUT
GPIO 18: level=0 fsel=0 func=INPUT
GPIO 22: level=0 fsel=0 func=INPUT
GPIO 23: level=0 fsel=0 func=INPUT
GPIO 24: level=0 fsel=0 func=INPUT
GPIO 25: level=0 fsel=0 func=INPUT
GPIO 27: level=0 fsel=0 func=INPUT
```

```
GPFLSEL0: 00 000 000 000 001 001 000 000 000 100 100
              9   8   7           4   3   2
GPFLSEL1: 00 000 000 000 001 100 100 000 000 000 000
                 18  17                      11  10
GPFLSEL2: 00 000 000 000 000 100 100 000 000 000 100
                     27      25  24  23  22
                                               
```
  4 |   23 |  GPFSEL2 | FSEL23 | 11-9
  5 |   24 |  GPFSEL2 | FSEL24 | 14-12
  6 |   25 |  GPFSEL2 | FSEL25 | 17-15
  7 |   27 |  GPFSEL2 | FSEL27 | 23-21



# Pin Set/Clear Register Addresses

Register | Memory Address
-------------------------
GPSET0   | 0x7e20001c
GPCLR0   | 0x7e200028

