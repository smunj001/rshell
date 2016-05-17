#ifndef COMMAND_H
#define COMMAND_H
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
//#include "rshell.h"
//#include "connector.h"

using namespace std;

class Command 
{
	private:
		char** args;
		
	public:
		string cmd;
		vector <string> arg;
		bool executable;
		string connect;
		int num;
		
        
		void execute()
		{
			num = arg.size();		
			//number of arguments + cmd + NULL
			// char* args[num];
			args = new char*[num + 2];
			// char* args = new char[num];
			
			args[0] = ( char* )cmd.c_str();
			
			for ( unsigned i = 1; i < arg.size() + 1; i++ )			
			{
				//i starts at 1 b/c cmd already at 0
				
				args[i] = ( char* )arg[i - 1].c_str();
				//arg[i-1] b/c we need to start at i=0 for vector<string> arg
			}
			
			/*This is the start to our forking process
				The fork is used so that we have a child copy that executes the
				necessary functions while the parent is not affected nor changed
			*/
			args[num - 1] = NULL;
			
			executable = true;
			
			pid_t pid = fork();
			
			// call and initialize the PID
			int status;

			if ( pid < 0 )
			{
				perror ( "Forking Failed\n" );
				exit ( EXIT_FAILURE );
			}
			else if ( pid > 0 )
			{
				if ( waitpid( pid, &status, 0 ) == -1 )
			  	{
			    	perror( "waitpid" );
			    	exit( EXIT_FAILURE );
				}
			}
			else if ( pid == 0 )
			{
			//	cout << "executable = true" << endl;
				
				if ( execvp ( args[0], args ) == -1 )
				{
					// cout << "executable = false(in child process" << endl;
					executable = false;
					perror ( "exec" );
					exit( EXIT_FAILURE );
			    }
			    exit(EXIT_SUCCESS);
	        }

}
		//syscalls fork, execvp, and waitpid.
		
		void clear()
		{
			cmd.clear();
			arg.clear();
			connect.clear();
		}

		void parse( string& whole_command, vector<Command>& cmds )
		{
			string input;//tempstring
			Command c;//tempcommand
			
			/*
				We will be parsing the command passed by the user. We need the 'input' 
				variable to store the whole line and then the object, of type command,
				will help us parse it and push into the 'cmds' vector. Esentially we 
				will be looking for &&, ||, ;, " ", "#" to help us determine what to do. 
			*/
			
			// Need a for loop to help us iterate thorugh the command
			
			for ( unsigned x = 0; x < whole_command.size(); x++ )
			{												
				if (whole_command.at(x) == ' ')
				{
					//Checking to see whether we have a space at the current index
					if( input.size() > 0 )
					{										
						if ( c.cmd.size() == 0 )
						{									
							c.cmd = input;					
							input.clear();					
							
							/*If the command is empty then we can set it to the 
								input and clear the input string */
						}
						else
						{									
							c.arg.push_back( input ); 		
							input.clear();				
							
							/* If it is not empty then it contains an argument 
								and must be pushed back to the vector of arg */
						}
					}
				}
				else
				{
					//Indicating that we do not have a space at current index
					
					if ( ( whole_command.at(x) == '|' ) && ( x != whole_command.length() - 1 ) )
					{										
						//First instance of the OR operator '|'
						if ( whole_command.at( x + 1 ) == '|' )
						{										
							// This is to check if we have two ||'s 
							if( c.connect.size() > 0 )
							{									
								cmds.push_back(c);			
								c.clear();						
								
							/*initially we must check if there is a value at the 
								current connetor
							  if so we must push that onto the vector and clear it. 
							*/
							}
							
							x++;					//increment value to next position
							c.connect = "||"; 		//assign the connector to the 'or' operation
							cmds.push_back(c);		//push the connector to the array
							c.clear();				//clear the command
						}
					}
					else if ( ( whole_command.at(x) == '&' ) && ( x != whole_command.size() - 1 )  )
					{										
						//First instance of the AND operator '&'
						if ( whole_command.at( x + 1 ) == '&')
						{										
							// This is to check if we have two &&'s 
							if ( c.connect.size() > 0 )
							{									
								cmds.push_back(c);				
								c.clear();						
								
								//initially we must check if there is a value at the current connetor
								//if so we must push that onto the vector and clear it. 
							}
							
							x++;						//increment value to next position
							c.connect = "&&";			//assign the connector to the 'and' operation
							cmds.push_back(c);			//push the connector to the array
							c.clear();					//clear the command
						}
					}
					else if ( whole_command.at(x) == '#' )
					{												
						x = whole_command.size();
						
						/*if we have a '#', indicating a comment then we must reassign 
							the value x to the length of the command 
						*/
					}
					else if ( whole_command.at(x) == ';' )
					{											// if the char is a semicolon
						c.connect = whole_command.at(x);		// save the connector as ;
						cmds.push_back(c);						// save the command
						c.clear();								// clear the temp command
						
						/*if we have a semicolon then we know that the connctor 
							is ';' and therefore we can push the whole command 
							onto the vector 
						*/
					}
					else
					{
						input += whole_command.at(x);
						//input will hold the whole command recieved
					}
				}
			}
			
			if ( input.size() > 0 )
			{												
				if ( c.cmd.size() != 0 )
				{										
					c.arg.push_back(input); 				
				}
				else
				{
					c.cmd = input;						
				}
			}
			cmds.push_back(c); 
			//push the rest
		}
};

#endif
