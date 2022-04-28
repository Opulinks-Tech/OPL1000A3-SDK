# Function
This example introduces how to set I2C slave mode.

In this demo,
    - The pin-assignment:
        - IO_10: clock(I2C_SCL)
        - IO_11: data signal(I2C_SDA)
    - Address is 7bit with 0x51

A simple example is provided to received and display data from I2C master device.
    - write cmd: updated current index
    - read cmd: got current index then +1
Format:
[ START|Addr|R/W cmd][ 8 bits Data|STOP ]

# Notes
- An external I2C master device is required.
- The 10K-Ohm resistance pull-up sources were need for both IO10 and IO11