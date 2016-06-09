#ifndef CONNECTOR_H
#define CONNECTOR_H

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
//#include "rshell.h"
#include "command.h"

using namespace std;


class Connector
{
    public:

      Command last_executed;//also contains bool of whether this command was an executable or not
      Command comm;
        
        
        bool Semi()
        {
            // cout << "Semi()\n";
            if ( last_executed.connect == ";" )
            {
                if ((comm.cmd != "test") && (comm.cmd != "["))
                {
                  comm.execute();
                  last_executed = comm;
                  return false;
                }
                else
                {
                  comm.test_exec();
                  last_executed = comm;
                  return false;
                }
            }
            else
            {
                return true; //continue execution
            }
        }
        
        bool Or()
        {
            // if (last_executed.executable == true)
            //     cout << "executable is true here" << endl;
            if ( ( last_executed.connect == "||" ) && ( last_executed.executable == false ) )
            {
                if ( (comm.cmd != "test") && (comm.cmd != "["))
                {
                comm.execute();
                last_executed = comm;
                return false;
                //Reference: ls ||...||..||...&& echo
                /* First check the connector,
                    Next need to keep track of the previously executed command
                        If previously executed command is true then 'flag' it
                */
                }
                else
                {
                  comm.test_exec();
                  last_executed = comm;
                  return false;
                }
            }
            else if ( ( last_executed.connect == "||" ) && ( last_executed.executable == true ) )
            {
                last_executed.connect = comm.connect;
                return false;
            }
            else
            {
                return true;
            }
        }
        
        bool And()
        {
            if ( ( last_executed.connect == "&&" ) && ( last_executed.executable == true ) )
            {
                //last_executed.connect will eventually be different than the 
                //connect currently be tested
                if ((comm.cmd != "test") && (comm.cmd != "["))
                {
                comm.execute();
                last_executed = comm;
                return false;
                }
                else
                {
                  comm.test_exec();
                  last_executed = comm;
                  return false;
                }
              // sec_comm = false;
            }
            else if ( ( last_executed.connect == "&&" ) && ( last_executed.executable == false ) )
            {
                last_executed.connect = comm.connect; //obtain next connector
                return false;
            }
            
            return true;
        }
        
        void Pipe()
        {
            if ( last_executed.connect == "|" ) //might have to create a error check to see if commnad to be piped is actually an executable
            {

                
                int returnValue = fork();
                if ( returnValue < 0)
                {
                     perror ( "Forking Failed\n" );
				     exit ( EXIT_FAILURE );
                }
                else if ( returnValue == 0)//child process
                {
                    int pfd[2];
                    pipe(pfd);
                    int newChild = fork();
                    if ( newChild < 0)
                    {
				      perror ( "Forking Failed\n" );
				      exit ( EXIT_FAILURE );
			        }
                    else if ( newChild == 0)//child of the child proccess
                    {
                        close(pfd[0]);
                        close(1);
                        dup(pfd[1]);
                        close(pfd[1]);
                        last_executed.beingpiped = true;
                        last_executed.execute(); 
                        exit(EXIT_SUCCESS);
                    }
                    else if (newChild > 0)//parent of the child's child... aka child proccess
                    {
                        wait(NULL);
                        close(pfd[1]);
                        close(0);
                        dup(pfd[0]);
                        close(pfd[0]);
                        comm.beingpiped = true;
                        comm.execute();
                    }
                    exit(EXIT_SUCCESS);
                }
                else if (returnValue > 0)
                {
                    wait(NULL);
                    last_executed = comm;
                    
                }
            }
        }
};

#endif
