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
        bool cont;
        
        //these are the protected member variables of the Rshell Class
        //We need a vector of commands to be able to parse the input we
        //recieve from the user.
        
    public:
        
        Rshell() 
        {
            //constructor
            
            input = "";
		    int x = 0;
		    cont = true;
		    
		    while ( x < 100 )
		    {
		        //initialize the vectors
		        username[x] = 0;
		        hostname[x] = 0;
		        x++;
		    }
		    
		    getlogin_r(username, 100);
		    gethostname(hostname, 100);
		    //this is to get and set hostname and username
        }
        
        void getInput()
        {

            while (true) 
            {
                if ( !temp.empty() )
                {
                    temp.resize(0);
                }
                
                if ( !arg.empty() )
                {
                    arg.resize(0);
                }
                
                cout << username << "@" << hostname << "$ ";  
                //This is the command prompt that will be printed
                
                getline(cin, input, '\n'); 
                //Take in the whole input from the user
               
                if ( input == "exit" || input == "EXIT" ) 
                {
                    exit(EXIT_FAILURE);
                    //if the input is exit then terminate
                }
                
            	parse(input, temp);
                //at this point our parse only seperages cmds, args, and connectors if there is a space in between each of them
                    
                    
                for ( unsigned i = 0; i < temp.size(); i++ )
                {
                    if ( i == 0 )
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
                         cont = true;
                        
                        if ( cont )
                        {
                            cout << "executable1 " ;
                            cont = conn.Semi();
                        }
                        
                        if ( cont )
                        {
                            cout << "executable2 " ;
                            cont = conn.Or();
                        }
                        
                        if ( cont )
                        {
                            cout << "executable3 " ;
                            cont = conn.And();
                        }
                        
                        temp[i].clear();
                    }
                
                }
         
            }//end of while
        }//end of void
};


#endif
