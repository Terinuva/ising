#ifndef CURSOR_H_INCLUDED
#define CURSOR_H_INCLUDED

#include <stdio.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    #include <windows.h>  //For Cursor Control on Winows
#endif



/*Turns the cursor on/off*/
void ShowConsoleCursor(bool showFlag)
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
    #endif
}

/*Sets cursor to the top left position to overwrite terminal output*/
void InitCurs(){

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
	/*Initialise objects for cursor manipulation*/
    HANDLE hStdout;
    COORD destCoord;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    /*Sets coordinates of cursor to (0,0) */
    destCoord.X = 0;
    destCoord.Y = 0;
    SetConsoleCursorPosition(hStdout, destCoord);

    ShowConsoleCursor(false);
    #elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    printf("\033[1;1H");
    #endif
}

void Fullscreen(bool bl){
    if(bl){
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    #endif
    }
}





#endif // CURSOR_H_INCLUDED
