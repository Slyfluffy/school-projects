# INSTRUCTIONS FOR RPSSERVER

I built both applications with Xcode so that will be the easiest option. That
should be a simple download of the rpsServer file as well as the Xcode project.
Verify that your scheme (accessed via “edit scheme”) is passing in port number
as an argument. If not, set that to include port number. Then simply build.

## NON-Xcode RPSServerInstructions

If you are not using Xcode, you will need to include a makefile. Simply setup
makefile to use the rpsServer file and then make it. You should be good to go!
Test the program by entering “a.out” followed by a port number”.

## INSTRUCTIONS FOR RPSCLIENT

Similar to rpsServer, Xcode will be the easiest way. Verify that the scheme is
passing in your parameters (“localhost” or IP address depending on your test as
well as the port number). You’ll only need to build afterwards.

## NON-XCODE RPSclient Instructions

Similar to rpsServer Non-xcode builds.
If you are not using Xcode, you will need to include a makefile. Simply setup
makefile to use the rpsClient file and then make it. You should be good to go!
Test the program by entering “a.out” followed by address and port number”.

## OTHER

If you want to setup using Visual Studio or other c++ IDE, you may do so as well.
I'm unsure of which one you will choose but do use the arguments that are
required.
