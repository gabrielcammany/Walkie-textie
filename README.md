# RFtext

The proposed challenge of this project is the creation of a telecommunications network based on radio frequency links 
controlled by microcontrollers. The main idea is to have an central emitter that will comunicate with receivers sending text messages
wirelessly.

Therefore, the system consist of two well differentiated blocks that will respectively correspond to each of the two phases of
the project: the transmitter and receiver.

## [Phase One - Transmitter](PhaseOne/)
 
The first phase, consists of a PIC18F4321 microcontroller programmed in the assembler. This system is capable of (1) communicating with a computer through RS-232 serial communications protocol, (2) receiving a text message from the computer and (3) forwarding the message by radio frequency. The following figure shows a general diagram of Phase 1.

Regarding the user interface, to allow the user communicate with the transmitter, a Java interface is provided:


## [Phase Two - Receiver](PhaseTwo/)

The second phase of this project, has the hability receiving and displaying messages that have been sent from the transmitter. As you know, RF messages are sent at great distances and everyone who is on the way, can recieve it. In order to organizate each reciever, each one will have its own ID (a 3-bit code) that will allow it to identify the messages that are intended for itself.
