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
#include <sys/stat.h>
//#include "rshell.h"
//#include "connector.h"

using namespace std;

class Command 
{
	private:
		char** args;//args is the character pointer array of the vector string "arg"
		
	public:
		string cmd;
		vector <string> arg;
		bool executable;
		string connect;
		unsigned num;
		
        
		void execute()
		{
			
			num = arg.size() + 2;		
			args = new char*[num];
			
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
			
			if (cmd == "exit")
			{
				clear();
				exit(0);
			}
			
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

        }//end of void execute()
		
		void test_exec()
		{
			struct stat sb;
			
			if (cmd == "[")
			{
				num = arg.size() - 1; 
				// "-1" because we're not going to count "]" as an argument	
				// but that's only if cmd is "["
			}
			else //else if you're using the actual word "test" as a command
			{
				num = arg.size();
			}
			
			args = new char*[num];
			
			for ( unsigned i = 0; i < num ; i++ )			
			{
				args[i] = ( char* )arg[i].c_str();
			}
			
			//stat returns 0 if true, 1 if false;
			if ( arg[0] == "-d")
			{
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
					if (stat(args[1], &sb) == -1)
					  {
					  	//perror("stat");
					  	cout << "(FALSE)" << endl;
					  	executable = false;
					  	exit(EXIT_FAILURE);
					  }
					else if (stat(args[1], &sb) == 0)  
					{
						if (S_ISDIR(sb.st_mode))
						{
							cout << "(TRUE)" << endl;
							executable = true;
							exit (EXIT_SUCCESS);
						}
						else
						{
							cout << "(FALSE)" << endl;
							executable = false;
							exit (EXIT_FAILURE);
						}
					}
					else if (stat(args[1], &sb) == 1)
					{
						cout << "(FALSE)" << endl;
						executable = false;
						exit (EXIT_FAILURE);
					}
				}
			
			}//end of "-d" flag if statement
			
			else if ( arg[0] == "-f")
			{
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
				  	if (stat(args[1], &sb) == -1)
					  {
					  //	perror("stat");
					    cout << "(FALSE)" << endl;
					    executable = false;
					  	exit(EXIT_FAILURE);
					  }
					else if (stat(args[1], &sb) == 0)
					{
						if (S_ISREG(sb.st_mode))
						{
						cout << "(TRUE)" << endl;
						executable = true;
						exit (EXIT_SUCCESS);
						}
						else
						{
						cout << "(FALSE)" << endl;
						executable = false;
						exit (EXIT_FAILURE);
						}
					}
					else if (stat(args[1], &sb) == 1)
					{
						cout << "(FALSE)" << endl;
						executable = false;
						exit (EXIT_FAILURE);
					}
				}
			}//end of "-f" flag if statement
			
			else if ( arg[0] != "-f" && arg[0] != "-d")
			//which means arg[0] could either be "-e" or no flag at all in which 
			//case "-e" is used anyway because it is the default
			{
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
					if (arg[0] != "-e") //if the "-e" flag was not used
					{
						if (stat(args[0], &sb) == -1)
						{
							cout << "This is where the error is happening when there's no flag." << endl;
							cout << "(FALSE)" << endl;
							executable = false;
							//perror("stat");
							exit(EXIT_FAILURE);
						}
						else if (stat(args[0], &sb) == 0)//if path is a file or directory
						{
							cout << "(TRUE)" << endl;
							executable = true;
							exit (EXIT_SUCCESS);
						}
						else if (stat(args[0], &sb) == 1)//if path is not file or directory
						{
							cout << "(FALSE)" << endl;
							executable = false;
							exit (EXIT_FAILURE);
						}
					}
					else // if "-e" flag IS used
					{
						if (stat(args[1], &sb) == -1)
						{
						  
							//perror("stat");
							cout << "(FALSE)" << endl;
							executable = false;
							exit(EXIT_FAILURE);
						}
						else if (stat(args[1], &sb) == 0)
						{
							cout << "(TRUE)" << endl;
							executable = true;
							exit (EXIT_SUCCESS);
						}
						else if (stat(args[1], &sb) == 1)
						{
							cout << "(FALSE)" << endl;
							executable = false;
							exit (EXIT_FAILURE);
						}
					}
				}
			}//end of "-e" flag statement
			
			//will first start off by writing code for the regular "test" command
			//we need to see what type of flag it uses
			//then we need to test the string of the path to the file/directory
		}//end of test_exec()
		
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
							if ( c.connect.size() > 0 )
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
					
					//START NEW IMPLEMENTATION / HW3
					else if ( whole_command.at(x) == '(' )
					{
						//Checks for the Parenthesis / Precedence Order of Operations
						/* Do not need to check for ')' specifically because '(' is required
							for ')' */
					
						bool isParen = true;
						int numOParen = 0;//open parenthesis
						int numCParen = 0;//close parenthesis
						unsigned y = x; //assign y the current position
	       				
	      				//Count Open and Close ( )
	      				if(isParen)
	      				{
				            do
				            {
				                if ( whole_command.at(y) == '(' ) 
				                {
				                    numOParen++;
				                }
				                else if ( whole_command.at(y) == ')' ) 
				                {
				                    numCParen++;
				                }
				                y++;
				            }while ( y < whole_command.size() );
				            
				            //Check the count to ensure balanced or print error
				            if ( numOParen - numCParen != 0 )
				            {
					            if ( (numOParen - numCParen) > 0 ) 
					            {
					                cout << "Syntax error near unexpected token ')'" << endl;
					                return;
					            }
					            else if ((numOParen - numCParen) < 0)
					            {
					            	 cout << "Syntax error near unexpected token '('" << endl;
					            	 return;
					            }
				            }
				            
				            isParen = false;
	      				}
			            
						/* We only want to do this once for each command, doing it 
							multiple times is a waste because result will be the same */
							
						// Time to push the operation onto our vector
						else
			            {
			            	c.connect = whole_command.at(x);
			            	cmds.push_back(c);
			            	c.clear();
			            }
						
						
					}
					
					//END NEW IMPLEMENTATION / HW3
					else
					{
						input += whole_command.at(x);
						//input will hold the whole command recieved
					}
					
				}//end of if not a space
			}//end of for loop
			
			if ( input.size() > 0 )
			{												
				if ( c.cmd.size() != 0 ) // if cmd is not empty
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
