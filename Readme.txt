-----------------------------------------------------------------------------------------------------

CS203 LAB 2
SHIVAM PRASAD
2017CSB1108

-----------------------------------------------------------------------------------------------------
The goal of this assignment was to convert a given SOP expression to its reduced form.
Task A is designed to convert any expression (both POS and SOP) to its canonical form.
The output from taskA.cpp is stored in a file output.txt.
Task B is designed specifically for reducing canonical SOP expression to reduced.
If the output.txt file contains only minterm expressions then the reduced form of
the minterms will get stored in outputB.txt.
The user can also give console input of the minterms to get output in console for
taskB.cpp
-----------------------------------------------------------------------------------------------------

INSTRUCTIONS FOR EXECUTION:

1 ) After unzipping the all the files to the same folder give the "make" command in the terminal

2 ) Save your input in the input.txt file ( the input should be in the same format as already given)

3 ) Give "./A" command to get the output stored in the output.txt file
	You need to tell if the maxterms need to be shown for POS input
	If maxterm output is shown then it won't be converted to reduced SOP in task B

4 ) Give "./B" command to execute task B. 
	You will be asked if you want to read input from the previously generated file or give custom input.
	In case of file input, output will be displayed on the outputB.txt file.
	In case of user input, output will be displayed in the terminal.

5) Give "make clean" command to clean the previous build.

------------------------------------------------------------------------------------------------------