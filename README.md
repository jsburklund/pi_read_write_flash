# Hardware Notes
Pinout is version 2.0: left side starts with pins 2, 3, 4.

Information is extracted from:
 - https://bob.cs.sonoma.edu/IntroCompOrg-RPi/sec-gpio-pins.html
 - https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf

# Pinout

BIT | GPIO | Register | Pin    | GPIO-Bits
------------------------------------------
  0 |    2 |  GPFSEL0 | FSEL2  | 8-6
  1 |    3 |  GPFSEL0 | FSEL3  | 11-9
  2 |    4 |  GPFSEL0 | FSEL4  | 14-12
  3 |    7 |  GPFSEL0 | FSEL7  | 23-21
  4 |    8 |  GPFSEL0 | FSEL8  | 26-24
  5 |    9 |  GPFSEL0 | FSEL9  | 29-27
  6 |   10 |  GPFSEL1 | FSEL10 | 0-2
  7 |   11 |  GPFSEL1 | FSEL11 | 5-3

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

`raspi-gpio get 2,3,4,7,8,9,10,11`
```
GPIO 2: level=1 fsel=0 func=INPUT
GPIO 3: level=1 fsel=0 func=INPUT
GPIO 4: level=1 fsel=0 func=INPUT
GPIO 7: level=1 fsel=0 func=INPUT
GPIO 8: level=1 fsel=0 func=INPUT
GPIO 9: level=0 fsel=0 func=INPUT
GPIO 10: level=0 fsel=0 func=INPUT
GPIO 11: level=0 fsel=0 func=INPUT
```

```
GPFLSEL0: 00 000 000 000 001 001 000 000 000 100 100
              9   8   7           4   3   2
GPFLSEL1: 00 000 000 000 001 100 100 000 000 000 000
                                             11  10
```



# Pin Set/Clear Register Addresses

Register | Memory Address
-------------------------
GPSET0   | 0x7e20001c
GPCLR0   | 0x7e200028

