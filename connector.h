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
                // cout << "semi" << endl;
                comm.execute();
                last_executed = comm;
                return false;
            }
            else
            return true; //continue execution
        }
        
        bool Or()
        {
          //  cout << "Or()\n";
        //   if ( last_executed.executable == false)
        //   cout << "executable = false(Or statement)" << endl;
        //   else
        //   cout << "executable = false" << endl;
        // last_executed.executable = true;
            if ( ( last_executed.connect == "||" ) && ( last_executed.executable == false ) )//program enters this if statement even if executable == true
            {
                // cout << "exec next command" << endl;
                comm.execute();
                last_executed = comm;
               // cout << "return false";
                return false;
                //Reference: ls ||...||..||...&& echo
                /* First check the connector,
                    Next need to keep track of the previously executed command
                        If previously executed command is true then 'flag' it
                */
            }
            else if ( ( last_executed.connect == "||" ) && ( last_executed.executable == true ) )
            {
                // cout << "dont' execute next" << endl;
                last_executed.connect = comm.connect;
                return true;
            }
            else
            return true;
        }
        
        bool And()
        {
        //   cout << "And()\n";e
            if ( ( last_executed.connect == "&&" ) && ( last_executed.executable == true ) )
            {
                //last_executed.connect will eventually be different than the 
                //connect currently be tested
                comm.execute();
                last_executed = comm;
                return false;
              // sec_comm = false;
            }
            else if ( ( last_executed.connect == "&&" ) && ( last_executed.executable == false ) )
            {
                last_executed.connect = comm.connect; //obtain next connector
                return true;
            }
            
            return true;
        }
};

#endif