# Function
This example introduces how to set HAL_TMR.

In this demo, the one-second timer was set and the follower timers values were observed
    - Hal_tmr      : unit: 1us, the only one HW had callback function
    - Hal_tick     : unit: 1/XTAL, default 22MHz
    - Hal_sleeptmr : unit: 1/32K-XTAL, default 32KHz
A FreeRTOS OsTick (unit: 1 ms) was also referenced in example.

# Notes
- Refer OPL1000 HDK materials to know more information about timer.
