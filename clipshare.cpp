#include <iostream>
#include <fstream>
#include <windows>



int main (int argc, char * const argv[])
{
    
    if (argc < 2)
    {
        std::cout << "Format is: " << argv[0] << " <fileForClipShare>\n";
        return -1;
    }
    
    
    std::cout << "Will Dump Data to: " << argv[1] << "\n";

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
