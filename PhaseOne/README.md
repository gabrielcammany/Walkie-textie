# Phase One - The transmitter

As said before, this phase consists of creating the main transmitter that will be programmed in Assembler and with a Java Interface.

## Micro-controller

It will be responsible for receiving the information provided by the user interface from the serial channel. Using a PIC18F4321 will be able to manage the system together with 2 buttons (That will act as the buttons in the user interface) and 10 LEDs that will show the transmission state.

The problem using Radio Frequency is that, everybody who is on the same channel as the transmitter, can receive data, so there's a need to create some sort of identification for each receiver in order be able to target one. So, I've used a 3-bit numeric code for each one.

### RF protocol

Besides that, each receiver and emitter, must have the same RF protocol to be able to extract data from the channel. So I created the following protocol (using the [Manchester Code](https://en.wikipedia.org/wiki/Manchester_code) together with a 100bps transmission rate).

Manchester codification example
![Screenshot](PhaseOne/images/Manchester.PNG)

As the receivers are constantly listening to the channel, they need a synchronization sequence, so they know when they are receiving meaningful data or just noise.

![Screenshot](PhaseOne/images/RF.PNG)

Each frame of data will have 10 pulses together with a 15 ms 0 value gap, after the gap, it will receive a 8 bit value that both the emitter and receiver will have. 
After the synchronization is completed, the ID of the target receiver are the first bits of the sequence, so if it detects that the data is not targeted to him, it stops processing the frame.

### LEDs

The LEDs will have various states:
* 5Hz - When the message from the interface is saved correctly into the microcontroller.
* 10Hz - Once the message is send through RF.
* Progress bar - While sending the message through RF, each 10% of message sent will turn on a LED.

## Interface

To send a message, the user will have to write it through the following interface:

![Screenshot](PhaseOne/images/Interface.PNG)

The maximum length of the message will be 300 bytes, because of the restrictions of the micro controller. The data is going to be send using the serial port and the RS-232 protocol. On the other hand, the user will also be able to change the baudrate and the serial port of the computer.

## Sequence Diagram

The following image shows how the hole system works using a sequence diagram.

![Screenshot](PhaseOne/images/SequenceDiagram.PNG)

## Board

Finally, here is a picture of how it looks physically. This project was done in the same year, as [Hangman](https://github.com/gabrielcammany/Hangman), so I used the same board that was used to display the game state.

![Screenshot](PhaseOne/images/Board.jpg)
