# Function
This example introduces how to set PWMs

OPL1000 supports two configuration methods, one is simple mode, the other one is complex mode.

In this demo, 
    - The pin-assignment:
        - IO18: PWM_5 in simple mode. (Duty is 50%, 3 KHz)
        - IO19: PWM_4 in simple mode. (Duty is 20%, 10 KHz)
        - IO20: PWM_3 in complex mode. (Duty is 80%, 100 KHz)

# Notes
- All PWM ports should use same clock source. 
- Refer Doc\OPL1000_peripheral_PWM_application_notes.pdf to know more detailed application notes. 