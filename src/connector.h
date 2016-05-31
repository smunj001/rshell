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
            if ( ( last_executed.connect == "||" ) && ( last_executed.executable == false ) )
            //program enters this if statement even if executable == true
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
};

#endif
