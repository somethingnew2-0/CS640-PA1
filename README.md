CS640-PA1
=========
Created by: Peter Collins (pcollins) & Cory Groom (groom)

For this programming assignment you will write two pieces of code: the "pinger" and the "reflector". The pinger will send a specified series of packets via UDP to the reflector. The reflector will receive these packets, delay them for a specified period of time and then either return them via UDP to the pinger or drop them. The pinger will produce an output based on the returned packets on the round trip times for each returned packet.
