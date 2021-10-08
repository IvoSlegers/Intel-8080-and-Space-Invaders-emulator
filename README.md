# Intel 8080 microprocessor and Space Invaders emulator
A accurate emulator of the Intel 8080 microprocessor.

This project consists of two parts.

### Intel 8080 emulator

A accurate emulator of the Intel 8080 microprocessor. 

Passes the following four 8080 diagnostic tests:
  * 8080EXM.COM
  * 8080PRE.COM
  * CPUTEST.COM
  * TST8080.COM
  
To run these tests start the application with the -d command-line option. 

A small  debugging application is implemented in the console window which allows the user to run a program step by step or set breakpoints.

### Space Invaders emulator

A fully functional emulator of the 1978 arcade game [Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders) which was designed to run on a Intel 8080 based system.

Implements an emulator of the video, audio and input components of the arcade cabinet that interact with the processor.

Runs the unmodified original .ROM file.
