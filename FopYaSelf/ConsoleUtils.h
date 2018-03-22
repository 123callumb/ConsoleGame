/* 
**A simple library of console routines (from http://sunlightd.virtualave.net/Windows/FAQ.html) 
---------------------------------------
** Clrscr() - clear the screen, and reset cursor to (0, 0)
** Gotoxy(x, y) - move the text cursor to position (x, y)
** SelectBackColour(colour) - select a background colour from the colour constants list
** SelectTextColour(colour) - select a text colour from the colour constants list
** SetNewConsoleTitle(title) - set a new title for the console window
 
** NOTE:Assume that whenever you write text to the screen the attributes will apply to all text
** sent to the screen from that point until the next point at which you select different text 
** or background colour, and output text with the new attributes.
*/ 

#pragma once

#include <windows.h>
#include <stdio.h>

//colour constants for translation
const WORD clBlack       = 0;
const WORD clDarkRed     = 1;
const WORD clDarkGreen   = 2;
const WORD clDarkBlue    = 3;
const WORD clDarkCyan    = 4;
const WORD clDarkMagenta = 5;
const WORD clDarkYellow  = 6;
const WORD clDarkGrey    = 7;
const WORD clGrey        = 8;
const WORD clRed         = 9;
const WORD clGreen       = 10;
const WORD clBlue        = 11;
const WORD clCyan        = 12;
const WORD clMagenta     = 13;
const WORD clYellow      = 14;
const WORD clWhite       = 15;

//clear the screen, and reset cursor to (0, 0)
extern void ClearScreen();

//move the text cursor to position (x, y)
extern void GotoXY(int x, int y);

//puts a character at the position of the cursor
extern void PutCh(const char);

//select a background colour from the colour constants list
extern void SelectBackColour(int);

//select a text colour from the colour constants list
extern void SelectForeColour(int);

//select the text attributes
extern void SelectAttributes();

//get the heigth of the screen (console window)
extern int screenHeight();

//get the width of the screen (console window)
extern int screenWidth();

//set a new title for the console window
extern void SetNewConsoleTitle(const char*);