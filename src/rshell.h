#ifndef RSHELL_H
#define RSHELL_H
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include "command.h"
#include "connector.h"

using namespace std;

    /*Objective: need to get user credentials and run it
        Credentials can be obtained through the functions (gethostname) and set through (sethostname)
        int gethostname(char *name, size_t len);
        int sethostname(const char *name, size_t len); 
    */

class Rshell : public Command
{
    protected:
    
        string input;
        vector<Command> temp;
		    char username[100];
		    char hostname[100];
        Connector conn;
        
        //these are the protected member variables of the Rshell Class
        //We need a vector of commands to be able to parse the input we
        //recieve from the user.
        
    public:
     	
        Rshell() 
        {
         
            input = "";
		        int x = 0;
		    
		        while (x < 100)
		        {
		          username[x] = 0;
		          hostname[x] = 0;
		          x++;
		        }
  		    getlogin_r(username, 100);
  		    gethostname(hostname, 100);
		    
        }
        
        void getInput()
        {

		while (true) 
		//for some reason this while loop exits as soon as one of the commands executes
		{
	                cout << username << "@" << hostname << "$ ";  //This is the command prompt that will be printed
	                getline(cin, input, '\n'); //Take in the whole input from the user
	               
	                if (input == "exit" || input == "EXIT") //if the input is exit then terminate
	                {
	                    exit(EXIT_FAILURE);
	                }
	                
	                parse(input, temp);
	                	 //at this point our parse only seperages cmds, args, and connectors if there is a space in between each of them
	                    
	                for (int i = 0; i < temp.size(); i++)
	                {
	                        if (i == 0)
	                        {
	                            //if first command is being executed
	                            //conn.comm = temp[i + 1];
	                            temp[i].execute();        
	                            conn.last_executed = temp[i];
	
	                    
	                            temp[i].clear();
	                        }
	                        else
	                        {
	                            conn.comm = temp[i];
	                            conn.Semi();
	                            conn.Or();
	                            conn.And();
	                            temp[i].clear();
	                        }
	                
	                }

        	}//end of while
    	}//end of void

        
};
