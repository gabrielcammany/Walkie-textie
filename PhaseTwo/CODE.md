
# Cooperative Programming

Cooperative Programing is a way of structuring the code so for instance a microcontroller with only one execution thread is able to execute tasks as if they where executed simultaneously.

The main idea is to structure the project in what is called [Abstract data type (ADT)](https://en.wikipedia.org/wiki/Abstract_data_type). That are basically modules that contain data and functions to manipulate these, with the condition that this data will only be accessible through this functions. 

We separate ADTs in two ways:
* Simple ADTs, they dont do any logic through time, so they they objective is just to provide an interface to use some particular I/O for example. 
* Logic ADTs, this ones have tasks that need to be executed constantly so we create what we call Motors. They are basically small state machines that execute tasks every time the condition to go to the next state is met.

## ADTs diagram

![Screenshot](images/TADDiagram.PNG)

This is the actual representation of a ADT diagram. Where the different modules connect to a specific peripheral (Boxes) and its most importat variables (Semi-closed boxes). The arrows connecting each variable/pheripheral represent the direction of the data flow.

Each connection to the other ADTs have a number specifing the interface together with the direction of the access. All the interfaces are explained in what we call the Dictionary, where each function used by the direction of the arrow, has its PRE and POST conditions.

[Dictionary of the ADT diagram](Dictionary.c)

On the other hand, arrows like one in the Timer, defines a interruption together with the time it occures.  

The idea of execution is that there's the main loop, where is constantlly calling all the motors available so each one can execute a small task, this produces the effect of acting as they are actually performing tasks simultaneously.

### Main Loop
![Screenshot](images/main.PNG)

On the [full report](Report.pdf) it is available all the motors of each logic ADT (In catalan though). But as an example here is the most important and complex motor of this project (from the RF ADT).

### RF Motor
![Screenshot](images/RFMotor.PNG)

### UART Motor
At the same time, they can be really simple, like the SIO (UART).

![Screenshot](images/UART.PNG)


