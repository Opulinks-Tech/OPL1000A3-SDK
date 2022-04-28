# Function
This example introduces how to set GPIOs.

In this demo, the IO_02 was configured as GPO (General Purpose Output) pin, and set high/low periodically.

The IO_03 ~ IO_05 were configured as GPI (General Purpose Input)
    - IO_03 : Detect falling edge
    - IO_04 : Detect rising edge
    - IO_05 : Detect level
You can connect IO_02 to IO_03 ~ IO_05 to check when callback log happen
    - Falling: happened when IO_02 changed to LOW
    - Rising : happened when IO_02 changed to HIGH
    - Level  : happened when IO_02 changed

# Notes
- None