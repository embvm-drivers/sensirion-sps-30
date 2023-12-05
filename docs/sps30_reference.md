# SPS-30 Reference Notes

## Pinout

1. VDD (5V)
2. RX (UART) or SDA (I2C)
3. TX (UART) or SCL (I2C)
4. SEL
5. GND

On our local devices, the line colors are:

1. Red
2. White
3. Purple
4. Green
5. Black

## Communication Modes

The device can talk over UART or I2C. When the SEL line is pulled to ground before/during sensor power up, the I2C interface is selected. Otherwise, UART is used.

### UART Settings

- Baud rate: 115200
- Data bits: 8
- Parity: none
- Stop bit: 1

In UART mode, the pinout is:

1. VDD (5V)
2. RX (Controller's TX line)
3. TX (Controller's RX line)
4. SEL (NC for UART)
5. GND

### I2C Settings

- Address: 0x69
- Clock speed: 100kHz
- Clock stretching not used

In I2C mode, the pinout is:

1. VDD (5V)
2. SDA (I2C)
3. SCL (I2C)
4. SEL (connect to GND)
5. GND

SCL and SDA are open drain I/O and should be connected toe xternal pullups.

## Reference Documents

- [Datasheet](https://sensirion.com/media/documents/8600FF88/616542B5/Sensirion_PM_Sensors_Datasheet_SPS30.pdf)
