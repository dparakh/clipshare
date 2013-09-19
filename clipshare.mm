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
#include <Cocoa/Cocoa.h>


class DPAutoReleasePool {
public:
    DPAutoReleasePool() : m_Pool(0)  { m_Pool = [[NSAutoreleasePool alloc] init];}
    ~DPAutoReleasePool () {if (m_Pool) [m_Pool drain];}
private:
    NSAutoreleasePool *m_Pool;
};

static void daemonize(void)
{
    pid_t pid, sid;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        printf ("pid < 0\n");
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        printf ("pid > 0\n");
        exit(EXIT_SUCCESS);
    }

    /* At this point we are executing as the child process */
    printf ("In Child\n");

#if 0
    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
#endif //0

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
        printf ("Could not cd /\n");
        exit(EXIT_FAILURE);
    }

    printf ("About to reopen standard files\n");
    
    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}


int main (int argc, char * const argv[])
{
    
    if (argc < 2)
    {
        std::cout << "Format is: " << argv[0] << " <fileForClipShare>\n";
        return -1;
    }
    
    
    std::cout << "Will Dump Data to: " << argv[1] << "\n";

    daemonize();

    DPAutoReleasePool thePool;
    NSPasteboard *thePasteboard = [NSPasteboard generalPasteboard];
    NSString *oldClipData = nil;
    std::string oldFileData;

    while (true)
    {
        NSString *dataOnBoard = [thePasteboard stringForType:NSStringPboardType];
        if ((nil != dataOnBoard) && (![dataOnBoard isEqual:oldClipData]))
        {
            std::ofstream clipfile (argv[1]);
            clipfile << [dataOnBoard UTF8String];
            clipfile.close();
            [oldClipData release];
            oldClipData = [dataOnBoard retain];
            oldFileData = [oldClipData UTF8String];
            std::cout << "Copied data to file from pasteboard\n";
        }
        
        std::ifstream clipfile(argv[1]);
        if (clipfile.is_open())
        {
            std::string filedata((std::istreambuf_iterator<char>(clipfile)),
                std::istreambuf_iterator<char>());
            if (filedata != oldFileData)
            {
                [thePasteboard clearContents];
                NSArray *objectsToCopy = [NSArray arrayWithObject:[NSString stringWithUTF8String:filedata.c_str()]];
                [thePasteboard writeObjects:objectsToCopy];
                oldFileData = filedata;
                std::cout << "Copied data to pasteboard from file\n";
            }
        }
        
        //sleep one second.
        sleep(1);
    }

    [oldClipData release];

    return 0;
}
