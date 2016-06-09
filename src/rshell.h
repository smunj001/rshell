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
                
                // Remove Comments
                for (unsigned int x = 0; x < input.size(); x++)
                {
                    if (input.at(x) == '#')
                    {
                        input = input.substr(0 , x);
                        x = input.size();
                    }
                }
            	parse(input, temp);//will need to impliment a parsing section for test command using brackets "[]"
                //at this point our parse only seperages cmds, args, and connectors if there is a space in between each of them
                    
                
               // cout << "just to see how quickly changes are being saved115" << endl;
                for ( unsigned i = 0; i < temp.size(); i++ )//I"m thinking we should make temp a 2-d vector. first bracket would be a specific set of commands inside a pair of parenthesis and the second pair of brackets would hold all the commands inside that pair of parenthesis
                {
                    if ( i == 0 )
                    {
                        //if first command is being executed
                        //conn.comm = temp[i + 1];
                        if ( (temp[i].cmd != "test") && (temp[i].cmd != "["))
                        {
                            if (temp[i].connect != "|")//if we're not piping
                            {
                            temp[i].execute();        
                            conn.last_executed = temp[i];
                            if(temp[i].executable == true)
                           // cout << "last_executed.executable = true " << endl;
                            temp[i].clear();
                            }
                            else if (temp[i].connect == "|")//still need to implement if a "test" command is going to be piped
                            {
                                conn.last_executed = temp[i];//.executable still has not value here
                                conn.comm = temp[i + 1];
                                conn.Pipe();
                                temp[i].clear();
                                i++;
                                temp[i].clear();
                            }
                        }
                        else//still need to come up with case if the test command gets piped into another
                        {
                            temp[i].test_exec();
                            conn.last_executed = temp[i];
                            
                            temp[i].clear();
                        }
                    }
                     else//if we're not executing the first command... i > 0
                    {
                        conn.comm = temp[i];
                         cont = true;
                        
                        // all of these are going to check if conn.comm is to be executed
                        //if it is to be executed then we also need to check if the next connector
                        //is a pipe. If it is then implement pipe.
                        //If not then continue to execute like a normal command
                        //implimentation for pipe will need to go inside each of these member functions
                        if (temp[i].connect != "|")//if we're not piping the current command
                        {
                            
                        if ( cont )
                        {
                            cont = conn.Semi();
                        }
                        
                        if ( cont )
                        {
                            cont = conn.Or();
                        }
                        
                        if ( cont )
                        {
                            cont = conn.And();
                        }
                        
                        temp[i].clear();
                        }
                        else if (temp[i].connect == "|")//if we need to pipe the current command
                        {
                            conn.last_executed = temp[i];
                            conn.comm = temp[i + 1];
                            conn.Pipe();
                            temp[i].clear();
                            i++;
                            temp[i].clear(); 
                        }
                    }//end of else statement... else=commands that are after the first index
                
                }//end of for loop for executing commands
         
            }//end of while
        }//end of void
        
};


#endif
