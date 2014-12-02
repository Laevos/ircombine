//TODO: Everything
#include "logfile.h"
#include <iostream>
#include <fstream>
#include <stdlib.h> //exit, EXIT_FAILURE, EXIT_SUCCESS

//Define error codes
#define MISSING_LOG -1

using namespace std;

//Sort function
void sortLog(Logfile a, Logfile b, ofstream & fout);

int main(int argc, char * argv[])
{

#ifndef LOGFILE_H //Something wacky is going on
    return MISSING_LOG;
#endif

    ifstream log1, log2;
    ofstream outlog;
    string FAIL_SYNTAX = "SYNTAX: ircombine INPUTFILE1.log INPUTFILE2.log [-o OUTPUTFILE.log]\n";

    if (argc < 3 || argc == 4)//Check for at least two input files; reject if less than two or exactly three (a third file with no command)
    {
            cout << FAIL_SYNTAX;
            exit(EXIT_FAILURE);
    }
    else if (argc >= 5) //If the user supplies more arguments, they're ignored
    {
        string in3 = argv[3];
        if (in3 != "-o") //Invalid third argument
        {
            cout << FAIL_SYNTAX;
            exit(EXIT_FAILURE);
        }
        else
        {
            outlog.open(argv[4]);//Open output file with named user selection
            if (!outlog.good())//Failed to create output file
            {
                cout << "Could not open " << argv[4] << " for writing.\n";
                exit(EXIT_FAILURE);
            }
        }
    }

    //If no name given, make default name
    if (!outlog.is_open())
    {
        cout << "No output file specified, using default name \'ircombineOutput.log\'.\n";
        outlog.open("ircombineOutput.log");
        if (!outlog.good())//Failed to create output file
        {
            cout << "Could not open ircombineOutput.log for writing.\n";
            exit(EXIT_FAILURE);
        }
    }

    //Open input logs
    log1.open(argv[1]);
    if (!log1.good())
        {
            cout << "Could not open " << argv[1] << " for reading.\n";
            exit(EXIT_FAILURE);
        }
    log2.open(argv[2]);
    if (!log2.good())
    {
        cout << "Could not open " << argv[2] << " for reading.\n";
        exit(EXIT_FAILURE);
    }

    //Create the two logfiles to compare.
    Logfile msgs1, msgs2;
    std::string buf;

    //Fill the two logfiles
    while (getline(log1, buf))
    {
        logEntry a(buf);
        msgs1.msgs.push_back(a);
    }

    while (getline(log2, buf))
    {
        logEntry a(buf);
        msgs2.msgs.push_back(a);
    }

    //Done with the log files, close
    log1.close();
    log2.close();

    //Give dates to all the messages in the two Logfiles
    msgs1.giveAbsDate();
    msgs2.giveAbsDate();

    sortLog(msgs1, msgs2, outlog);

    return 0;
}

void sortLog(Logfile a, Logfile b, ofstream &fout)
{
    /* Basically what we need to do is compare the starting
     * date for each log. If one is earlier than another,
     * then it stands to reason that all of its included
     * messages will be earlier than all of the included
     * messages in the other log. If the two dates are equal,
     * then we need to go in and compare the individual
     * messages, making sure that everything is in the right
     * order.
     */

    //Let's make two integers that'll go through each list and keep track of the current position
    unsigned int ap(0), bp(0);

    while (ap < a.msgs.size() && bp < b.msgs.size())
    {
        //First case: Both entries are LOG_OPEN, LOG_DAY, or a mix of LOG_OPEN and LOG_DAY
        if ((a.msgs[ap].msgType == LOG_OPEN && b.msgs[bp].msgType == LOG_OPEN) ||
            (a.msgs[ap].msgType == LOG_DAY && b.msgs[bp].msgType == LOG_DAY)   ||
            (a.msgs[ap].msgType == LOG_OPEN && b.msgs[bp].msgType == LOG_DAY)  ||
            (a.msgs[ap].msgType == LOG_DAY && b.msgs[bp].msgType == LOG_OPEN))
        {
            //Compare the day
            if (a.msgs[ap].absDate < b.msgs[bp].absDate) //a has an earlier date
            {
                do
                {
                    fout << a.msgs[ap].msgContent << endl;
                    ap++;
                } while (a.msgs[ap].msgType == LOG_MSG || a.msgs[ap].msgType == LOG_CLOSE); //If there's no day change, LOG_MSG and LOG_CLOSE must be on the same day.
            }
            else if (a.msgs[ap].absDate > b.msgs[bp].absDate) //b has an earlier date
            {
                do
                {
                    fout << b.msgs[bp].msgContent << endl;
                    bp++;
                } while (b.msgs[bp].msgType == LOG_MSG || b.msgs[bp].msgType == LOG_CLOSE);
            }
            else //Same date, now we're in trouble.
            {
                //Let's compare the time of the two days.
                if (a.msgs[ap].msgTime < b.msgs[bp].msgTime)//a has an earlier time
                {
                    fout << a.msgs[ap].msgContent << endl;
                    ap++;
                }
                else if (a.msgs[ap].msgTime > b.msgs[bp].msgTime)//b has an earlier time
                {
                    fout << b.msgs[bp].msgContent << endl;
                    bp++;
                }
                else //Same time, just put both of those messages in
                {
                    fout << a.msgs[ap].msgContent << endl << b.msgs[bp].msgContent << endl;
                    ap++;
                    bp++;
                }
            }
        }

        //Other case: Any other combination, really.
        else
        {
            //Just compare the two entries with the > and < operators, defined in logfile.h
            if (a.msgs[ap] < b.msgs[bp]) //Message a comes before b
            {
                fout << a.msgs[ap].msgContent << endl;
                ap++;
            }
            else if (a.msgs[ap] > b.msgs[bp]) //Message b comes before a
            {
                fout << b.msgs[bp].msgContent << endl;
                bp++;
            }
            else //Messages occur at the same time
            {
                fout << a.msgs[ap].msgContent << endl << b.msgs[bp].msgContent << endl;
                ap++;
                bp++;
            }
        }
    }

    //Now, if there are any other lines to be written in either log, do so.
    while (ap < a.msgs.size())
    {
        fout << a.msgs[ap].msgContent << endl;
        ap++;
    }
    while (bp < b.msgs.size())
    {
        fout << b.msgs[bp].msgContent << endl;
        bp++;
    }
}
