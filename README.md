Names: Sai Munjeti (861155045)
       John Reyes  (861026916)
       
CS 100 - Software Construction

#RSHELL (Terminal)

Overview
--------
The program RSHELL is an open ended project which is a reproduction of a command shell or a temrinal. It will print a command prompt (i.e $) and read in a commands on a single line. One will be able to execute common commands like (ls, cd, echo, mkdir, etc...) along with flags. The commands will have the form: cmd = executable[argumentList][connector cmd] and can be separated by three connectors ( &&, ||, ;). 


Execution
------
*      $ git clone https://github.com/smunj001/rshell.git
*      $ cd rshell
*      $ git checkout hw2
*      $ make
*      $ bin/rshell


Notes / Bugs Encountered
-----------
* HW 2 is the foundation of the terminal where the user will be able to enter basic commands such as: echo Hello and RSHELL will react according to the command.
* The boolean values when running the or was wrong.
* Memory allocation was not properly assigned, therefore we had trouble using the connectors and executing the arguments.

Current Bugs
-----------
* The comments works sometimes and in certain scenarios. //Fixed
* We have not been able to implement the precedence operation so we took it out of the program
* Few bugs while using dup with redirection
* Piping changed the value of executable to true when it should be false after child process exits. We're able to resolve the issue but could not identify why or where the value was being changed.
