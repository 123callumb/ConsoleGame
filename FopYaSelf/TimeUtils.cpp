#include "TimeUtils.h"

//get time from system
void GetSystemTime(int& hrs, int& mins, int& secs) 
{
	//set to system time
	time_t now(time(0));
	struct tm t;
	localtime_s(&t, &now);
	hrs  = t.tm_hour;
	mins = t.tm_min;
	secs = t.tm_sec;
}

//convert the time to a string in 24-h digital clock format (00:00:00)
string TimeToString(int h, int m, int s)
{ 
    ostringstream os;
    const char prev(os.fill ('0'));
    os << setw(2) << h << ":"
       << setw(2) << m << ":"
       << setw(2) << s;
    os.fill(prev);

    return os.str();
}

//return the current time in a string format
string GetTime() 
{ 	
	int hrs, mins, secs;
	GetSystemTime(hrs, mins, secs);

	return TimeToString(hrs, mins, secs);
}

//get date from system
void GetSystemDate(int& day, int& month, int& year)
{ 
	time_t now(time(0));
	struct tm t;
	localtime_s(&t, &now);
	day = t.tm_mday;
	month = t.tm_mon + 1;
	year = t.tm_year + 1900;
}

//convert the date to a string in format (dd/mm/yyyy)
string DateToString(int day, int month, int year) 
{
	ostringstream os;
    const char prev(os.fill ('0'));
    os << setw(2) << day << "/"
       << setw(2) << month << "/"
       << setw(4) << year;
    os.fill(prev);

    return os.str();
}

//return the current date in a string format
string GetDate() 
{ 	
	int day, month, year;
	GetSystemDate(day, month, year);
	
	return DateToString(day, month, year);
}