# Function
This example introduces how to set SPI with flash APIs.

In this demo, SPI_2 was used to issue the flash erase/write/read commands
    - The pin-assignment:
        - IO_02: SPI2_IO_1
        - IO_03: SPI2_IO_0
        - IO_04: SPI2_CLK
        - IO_05: SPI2_CS
    - slave device was W25Q80DV
You can see "Flash erase/read/write OK" in UART

# Notes
- An external SPI slave device is required.