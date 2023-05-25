# BBLJ2.0
The second coming of the BehavioralBoxLabJackController
This project will be used to read in values (pure voltages) from a LabJack T7. 
Specifically, this code was designed to record data from mice initiating from a beam break (using a digital I/O) and a running wheel (using an analog I/O). 
It is important to note that a reading of 1.0 V is the resting state of the beam break, so a reading of 0.0 V indicates a signal
For the running wheel, a range of values from ~4.5 V - ~0.0 V indicates one full turn.
