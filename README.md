# RGBcontroller
RGB LED controller written for a video production studio at the University of Sussex.
This is a partial showcase of the project (hence the lack of commits) as the main version 
is on a private repo for confidentiality.
This public repo is to showcase the basic main aspects of the project and does not cover the project in its entirety.

## Project Overview

Project is written in:

lua for a JustMacros script.

c++ for the arduino code.

The project controls strips of powerful led lights which are red green and blue.

A command is sent from a lua function (which in this case is used in
JustMacros) over the network using UDP for the transport protocol.
This function can be passed an RGB value of any colour along with a frequency
code for Pulse Width Modulation of the led's.

## invocation:

ledLightColourControl(red, green, blue, frequency)

E.G
ledLightColourControl(255, 67, 100, 3)

## Frequency codes:

The frequency code is connected to a set value for a divisor which controls
the amount of time the led is on/off using pwm.
codes:

1 = 1

2 = 8

3 = 64

4 = 256

5 = 1024
