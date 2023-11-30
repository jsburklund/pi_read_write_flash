# Hardware Notes
Raspberry Pi pin out is version 2.0: left side starts with pins 2, 3, 4.

Information is extracted from:
 - https://bob.cs.sonoma.edu/IntroCompOrg-RPi/sec-gpio-pins.html
 - https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf

# Pin Out

| Pin | NAND | Description   | GPIO | Register | Pin    | GPIO-Bits |
| --- | ---- | ------------- | ---- | -------- | ------ | --------- |
| IO0 |   29 | Parallel IO   |   11 |  GPFSEL1 | FSEL11 | 5-3       |
| IO1 |   30 | Parallel IO   |   17 |  GPFSEL1 | FSEL17 | 23-21     |
| IO2 |   31 | Parallel IO   |   18 |  GPFSEL1 | FSEL18 | 26-24     |
| IO3 |   32 | Parallel IO   |   22 |  GPFSEL2 | FSEL22 | 8-6       |
| IO4 |   41 | Parallel IO   |   23 |  GPFSEL2 | FSEL23 | 11-9      |
| IO5 |   42 | Parallel IO   |   24 |  GPFSEL2 | FSEL24 | 14-12     |
| IO6 |   43 | Parallel IO   |   25 |  GPFSEL2 | FSEL25 | 17-15     |
| IO7 |   44 | Parallel IO   |   27 |  GPFSEL2 | FSEL27 | 23-21     |

| Pin | NAND | Description   | GPIO | Register | Pin    | GPIO-Bits |
| --- | ---- | ------------- | ---- | -------- | ------ | --------- |
|  RB |    7 | Ready/Busy    |   10 |  GPFSEL1 | FSEL10 | 2-0       |
|   R |    8 | Read Enable   |    4 |  GPFSEL0 |  FSEL4 | 14-12     |
|  CL |   16 | Command Latch |    7 |  GPFSEL0 |  FSEL7 | 23-21     |
|  AL |   17 | Address Latch |    8 |  GPFSEL0 |  FSEL8 | 26-24     |
|   W |   18 | Write Enable  |    9 |  GPFSEL0 |  FSEL9 | 29-27     |

# Function Select Register Addresses

| Register | Memory Address |
| -------- | -------------- |
| GPFSEL0  | 0x7e200000     |
| GPFSEL1  | 0x7e200004     |
| GPFSEL2  | 0x7e200008     |

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
GPFSEL0: 00 000 000 000 001 001 000 000 000 100 100
              9   8   7           4   3   2
GPFSEL1: 00 000 000 000 001 100 100 000 000 000 000
                 18  17                      11  10
GPFSEL2: 00 000 000 000 000 100 100 000 000 000 100
                     27      25  24  23  22

```


# Pin Set/Clear Register Addresses

| Register | Memory Address |
| -------- | -------------- |
| GPSET0   | 0x7e20001c     |
| GPCLR0   | 0x7e200028     |

