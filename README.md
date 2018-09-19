## RaptorWatch
FMCW radar system implemented in LimeSDR-USB for detection and tracking of birds.

# Compile
To compile the source code, run:
$make

# Run
To run the code, you have to compile it first. this will produce the huffencode executable which can be ran by:

$./fmcwradar <options>

Please note that if incorrect options are given the program will abort.

# Testing
To compile the executable file for testing use:
$make test 

To run the tests use:
$./fmcwradartest

if you want to see each asssertion results, add the -s flag:
$./fmcwradartest -s

but this might results in slower test runs. :(

IMPORTANT!
The test take long time to finish, so run them and get some coffee. :)
alternatively, you can comment all the SECTIONS of the tests and leave the one you want to run. or find a way to specify which section you want to run.
:).

Sorry you said you want the tests right :). 
This happens only when you add the -s flag or other reporting flags.

# Clean
The following command will do basic cleaning.

$make clean

