/*******************************************************************
* logfile.h
*
* Class definition for a log file. Needs to have a sort function,
* a list of each message along with its time stamp. Need to decide
* the format of this. Would it be better for us to use a vector of
* strings? Clearly we need multiple of whatever it is that's being
* stored, since we'll need to check the dates, but then maybe we
* could use a vector or dynamic array of structures of the given
* type? Yeah, let's try that.
*
* Huge problem here: This doesn't account for users who have changed
* the format of their logfile in ANY WAY. This makes the usefulness
* of this program somewhat limited. I guess we'll see if it's still
* functional for us, and then consider sharing it with others.
*
*******************************************************************/
//Header definition
#ifndef LOGFILE_H
#define LOGFILE_H
#endif

//Message type definitions.
#define LOG_OPEN  0 //Message is of the form --- Log opened DAY MTH ## HH:MM:SS YYYY
#define LOG_MSG   1 //Message is of the form HH:MM:SS[USERNAME] [MESSAGE]
#define LOG_DAY   2 //Message is of the form --- Day changed DAY MTH ## YYYY
#define LOG_CLOSE 3 //Message is of the form --- Log opened DAY MTH ## HH:MM:SS YYYY
#define LOG_INVALID -1 //Something is wrong


#include <string>
#include <vector>
#include <cctype> //isdigit
#include <stdexcept> //Throwing errors, see http://stdcxx.apache.org/doc/stdlibref/2-3.html

class logEntry
{
public:
    //Constructors
    logEntry(std::string a);
    logEntry();
    //Data
    std::string msgTime;
    std::string msgContent;
    int msgType;

    int absDate;//To be set by Logfile
    //Functions
    int stoi(); //Convert HH:MM:SS to HHMMSS
    int date(); //Convert Day Month Year to YYYYMMDD
    int mtoi(std::string m); //Convert MTH to MM
    //Check message status
    bool isMsg() {return this->msgType == LOG_MSG;}
    bool isDay() {return this->msgType == LOG_DAY;}
    bool isOpen() {return this->msgType == LOG_OPEN;}
    bool isClose() {return this->msgType == LOG_CLOSE;}
    /* These might need to be made more robust. Don't try a comparison on
     * something that is a LOG_INVALID. (Done) Also need to overload
     * stoi to get the numbers out of the string without the semicolons. (Done)
     */
    void operator = (logEntry& a) {this->msgTime = a.msgTime; this->msgContent = a.msgContent; this->msgType = a.msgType;}
    bool friend operator == (logEntry& a, logEntry& b) {return a.stoi() == b.stoi();}
    bool friend operator != (logEntry& a, logEntry& b) {return a.stoi() != b.stoi();}
    bool friend operator > (logEntry& a, logEntry& b) {return a.stoi() > b.stoi();}
    bool friend operator >= (logEntry& a, logEntry& b) {return a.stoi() >= b.stoi();}
    bool friend operator < (logEntry& a, logEntry& b) {return a.stoi() < b.stoi();}
    bool friend operator <= (logEntry& a, logEntry& b) {return a.stoi() <= b.stoi();}

};


//Function definitions
logEntry::logEntry()//Tried to create a log entry without passing a string
{
    this->msgTime = "";
    this->msgContent = "";
    this->msgType = LOG_INVALID;
}

logEntry::logEntry(std::string a) //Constructor for a standard log entry
{
    if (isdigit(a[0]))//First character is a digit, must be a message
    {
        this->msgTime = a.substr(0,8);
        this->msgContent = a;
        this->msgType = LOG_MSG;
    }
    else if (a.substr(0,1) == "-") //First character is a -, must be a day change or open/close.
    {
        if (a.substr(4,1) == "D") //LOG_DAY
        {
            this->msgTime = "00:00:00";
            this->msgContent = a;
            this->msgType = LOG_DAY;
            this->absDate = this->date();
        }
        else if (a.substr(8,1) == "o") //LOG_OPEN
        {
            this->msgTime = a.substr(26,8);
            this->msgContent = a;
            this->msgType = LOG_OPEN;
            this->absDate = this->date();
        }
        else if (a.substr(8,1) == "c")//LOG_CLOSE
        {
            this->msgTime = a.substr(26,8);
            this->msgContent = a;
            this->msgType = LOG_CLOSE;
            this->absDate = this->date();
        }
        else //LOG_INVALID that somehow wasn't caught.
        {
            logEntry();
        }
    }
    else //Not a recognized log type.
    {
        logEntry();
    }
}

int logEntry::stoi()//Convert HH:MM:SS to HHMMSS; TODO: Make this more generic. Search the string for valid HH:MM:SS
{
    if (this->isMsg())
    {
        std::string h,m,s;
        h = this->msgTime.substr(0,2);
        m = this->msgTime.substr(3,2);
        s = this->msgTime.substr(6,2);
        return ((std::stoi(h) * 10000) + (std::stoi(m) * 100) + (std::stoi(s)));
    }
    else if (this->isDay())
    {
            return 0;//00:00:00 => 000000
    }
    else if (this->isOpen() || this->isClose()) //Opening and closing have time at the same position
    {
        std::string h,m,s;
        h = this->msgTime.substr(0,2);
        m = this->msgTime.substr(3,2);
        s = this->msgTime.substr(6,2);
        return ((std::stoi(h) * 10000) + (std::stoi(m) * 100) + (std::stoi(s)));
    }
    else //LOG_INVALID
    {
        throw std::invalid_argument("Attempted conversion of LOG_INVALID message to time");
    }
}
int logEntry::date() //Convert to YYYYMMDD; TODO: Make this more generic. Search the string for valid YMD.
{
    if (!this->isDay() && !this->isOpen() && !this->isClose())
    {
        throw std::invalid_argument("Attempted conversion of LOG_INVALID message to date");
    }

    else if (this->isDay()) //LOG_DAY
    {
        int d, m, y;
        d = std::stoi(this->msgContent.substr(24,2));
        m = mtoi(this->msgContent.substr(20,3));
        y = std::stoi(this->msgContent.substr(27,4));

        return (y * 10000) + (m * 100) + d;
    }
    else //LOG_OPEN or LOG_CLOSE
    {
        int d, m, y;
        d = std::stoi(this->msgContent.substr(23,2));
        m = mtoi(this->msgContent.substr(19,3));
        y = std::stoi(this->msgContent.substr(35,4));

        return (y * 10000) + (m * 100) + d;
    }

}
int logEntry::mtoi(std::string m)
{
    if (m == "Jan")
        return 1;
    else if (m == "Feb")
        return 2;
    else if (m == "Mar")
        return 3;
    else if (m == "Apr")
        return 4;
    else if (m == "May")
        return 5;
    else if (m == "Jun")
        return 6;
    else if (m == "Jul")
        return 7;
    else if (m == "Aug")
        return 8;
    else if (m == "Sep")
        return 9;
    else if (m == "Oct")
        return 10;
    else if (m == "Nov")
        return 11;
    else if (m == "Dec")
        return 12;
    else //Invalid month
        throw std::invalid_argument("Invalid month passed to mtoi");

}

class Logfile
{
	public:
        std::vector<logEntry> msgs;
        void giveAbsDate();
    private:
};

void Logfile::giveAbsDate()
{
    /* Get the date from the most recent day change or LOG_OPEN.
     * We're assuming here that a message must follow either a
     * LOG_OPEN or a LOG_CLOSE. Therefore the final absolute date
     * should read YYYYMMDDHHMMSS. However, that's longer than
     * could even be stored in an unsigned long int. Therefore,
     * maybe we should just give dates to messages, and compare
     * times if needed. Yes, let's go with that.
     *
     * Give a YYYYMMDD to every message.
     */

    //The first message in every log must be a LOG_OPEN. Otherwise, there's something wrong.
    if (this->msgs[0].msgType != LOG_OPEN)
    {
        throw std::invalid_argument("Log does not begin with LOG_OPEN");
    }

    //Get the initial date
    int date = this->msgs[0].date();

    //Assign a date to every message of type LOG_MSG
    for (unsigned int i = 0; i < this->msgs.size(); i++)
    {
        if (this->msgs[i].msgType == LOG_MSG)
        {
            this->msgs[i].absDate = date;
            //std::printf("Added date to message.\n");
        }
        else if (this->msgs[i].msgType == LOG_DAY || this->msgs[i].msgType == LOG_OPEN)//LOG_CLOSE should always be followed by LOG_OPEN
            date = this->msgs[i].date();
    }
}
