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
        
        Connector();
        
        void Semi()
        {
           // cout << "Semi()\n";
            if (last_executed.connect == ";")
            {
                comm.execute();
                last_executed = comm;
            }
        }
        void Or()
        {
           // cout << "Or()\n";
            if (last_executed.connect == "||" && last_executed.executable == false)
            {
                comm.execute();
                last_executed = comm;
                //Reference: ls ||...||..||...&& echo
                /* First check the connector,
                    Next need to keep track of the previously executed command
                        If previously executed command is true then 'flag' it
                */
            }
            else if (last_executed.connect == "||" && last_executed.executable == true)
            {
                last_executed.connect = comm.connect;
            }
        }
        void And()
        {
         //   cout << "And()\n";
            if (last_executed.connect == "&&" && last_executed.executable)
            {
                //last_executed.connect will eventually be different than the connect currently be tested
                comm.execute();
                last_executed = comm;
              // sec_comm = false;
            }
            else if(last_executed.connect == "&&" && last_executed.executable == false)
            {
                last_executed.connect = comm.connect; //obtain next connector
            }
        }
    
};

#endif
