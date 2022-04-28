# Function
This example introduces how to set I2C master mode.

In this demo,
    - The pin-assignment:
        - IO_10: I2C_SCL
        - IO_11: I2C_SDA
    - The slave device was EEPROM 24AA128 with address 0x7
Implement an EEPROM-write followed with one EEPROM-read per second, and those two data should be matched

# Notes
- An external I2C slave device is required.
- The 10K-Ohm resistance pull-up sources were need for both IO10 and IO11