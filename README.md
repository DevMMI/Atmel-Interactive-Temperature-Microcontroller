# Project
An Atmel microcontroller that regulates it's own temperature using a cooler and a heater based on information from an Analog
Temperature sensor and a desired temperature controller by the user

The goal of this project for me was to familiarize myself with pin based operations, I aimed to avoid using prebuilt functions in order to maximize my learning but I am aware that alot of operations can be abstracted.

# Requirements
Utilizing an Atmel SAML21 microcontroller (i.e. a SAM L21 Xplained Pro Evaluation Kit). The application should 
  Log debug data (i.e. printf()) over RS-232 to be displayed on a host computer
  
  Read the temperature from an analog temperature sensor (i.e. a MCP9700)
  
  Allow the user to increment/decrement the desired temperature using two buttons
  
  Turn a heater on/off using a GPIO pin when the temperature falls below desired
  
  Turn an AC unit on/off using a GPIO pin when the temperature rises above desired
  

# To Run
The run was simulated on an Atmega324P Simulator, the run is contained in the Simulator folder, Atmel Studio was used
