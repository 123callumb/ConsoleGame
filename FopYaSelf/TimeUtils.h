//Routines that get the system date and time and produce them as a formatted strings
//
//	GetSystemTime: get time from system
//	TimeToString: convert the time to a string in 24-h digital clock format (00:00:00)
//	GetTime: return the current time in a string format (00:00:00)
//	GetSystemDate: get date from system
//	DateToString: convert the date to a string in format (dd/mm/yyyy)
//	GetDate: return the current date in format (dd/mm/yyyy)
//
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>

// TODO: get rid
using namespace std;

// TODO: Return a struct instead of output parameters?
//get time from system
extern void GetSystemTime(int& hrs, int& mins, int& secs);

//convert the time to a string in 24-h digital clock format (00:00:00)
extern string TimeToString(int h, int m, int s);

//return the current time in a string format
extern string GetTime();

//get date from system
extern void GetSystemDate(int& day, int& month, int& year);

//convert the date to a string in format (dd/mm/yyyy)
extern string DateToString(int day, int month, int year);

//return the current date in a string format
extern string GetDate();