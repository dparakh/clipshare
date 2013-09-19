//----------------------------------------------------------------------------------
//
// Copyright (c) 2013 Devendra Parakh. All rights reserved.
//
//! \file	clipshare.mm
//!
//! The Clipboard Sharing application.
//!
//---------------------------------------------------------------------------------*/


#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>

using namespace std;

#define argc __argc
#define argv __argv

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
//int main (int argc, char * const argv[])
{
    
    if (argc < 2)
    {
        std::cout << "Format is: " << argv[0] << " <fileForClipShare>\n";
        return -1;
    }
    
    
    std::cout << "Will Dump Data to: " << argv[1] << "\n";

    string oldClipData;
    string oldFileData;

    while (true)
    {
        //NSString *dataOnBoard = [thePasteboard stringForType:NSStringPboardType];
        string dataOnBoard;
        if (OpenClipboard (0))
        {
            if (IsClipboardFormatAvailable(CF_TEXT))
            {
                HANDLE hClipText = GetClipboardData (CF_TEXT);
                if (hClipText)
                {
                    char* pClipText = (char *)GlobalLock(hClipText);
                    if (pClipText)
                    {
                        dataOnBoard = pClipText;
                        GlobalUnlock (hClipText);
                    }
                    
                }
            }
            CloseClipboard ();
        }
        
        if (dataOnBoard != oldClipData)
        {
            std::ofstream clipfile (argv[1]);
            clipfile << dataOnBoard;
            clipfile.close();
            oldClipData = dataOnBoard;
            oldFileData = oldClipData;
            std::cout << "Copied data to file from pasteboard\n";
        }
        
        std::ifstream clipfile(argv[1]);
        if (clipfile.is_open())
        {
            std::string filedata((std::istreambuf_iterator<char>(clipfile)),
                std::istreambuf_iterator<char>());
            if (filedata != oldFileData)
            {
                if (OpenClipboard(0))
                {
                    EmptyClipboard();
//                NSArray *objectsToCopy = [NSArray arrayWithObject:[NSString stringWithUTF8String:filedata.c_str()]];
//                [thePasteboard writeObjects:objectsToCopy];
                    HANDLE hFileData = GlobalAlloc (GMEM_SHARE, filedata.size()+1);
                    if (hFileData)
                    {
                        char *pFileData = (char *)GlobalLock (hFileData);
                        if (pFileData)
                        {
                            memcpy (pFileData, filedata.c_str(), filedata.size()+1);
                            GlobalUnlock (hFileData);
                        }
                        SetClipboardData (CF_TEXT, hFileData);
                        oldFileData = filedata;
                        std::cout << "Copied data to pasteboard from file\n";
                    }
                    CloseClipboard();
                }
            }
        }
        
        //sleep one second.
        //sleep(1);
        Sleep(1000);
    }

    return 0;
}
