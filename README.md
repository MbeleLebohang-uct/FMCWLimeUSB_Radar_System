PP_Prac4
NOTE: shrek_test.pgm is used for testing, therefore it may not be modified.

# Compile
To compile the source code, run:
$make

# Run
To run the code, you have to compile it first. this will produce the huffencode executable which can be ran by:

$./imageops <options>

for more information on <options>, read the PDF on this assignment. Please not that if incorrect options are
given the program will abort.

# Testing
To compile the executable file for testing use:
$make test 

To run the tests use:
$./imageopstest

if you want to see each asssertion results, add the -s flag:
$./imageopstest -s

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

