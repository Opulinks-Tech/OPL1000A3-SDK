# Function
This example introduces how to set SPI master mode.

In this demo, SPI_2 was used to issue the command to query device ID.
    - The pin-assignment:
        - IO_02: SPI2_IO_1
        - IO_03: SPI2_IO_0
        - IO_04: SPI2_CLK
        - IO_05: SPI2_CS
When the slave device was W25Q80DV, the got ID should be "0xEF 0x40 0x14"

# Notes
- An external SPI slave device is required.