

#ifndef martem_test_fileline_h
#define martem_test_fileline_h
#include <stdio.h>
#include <string>

using namespace std;

class FileLine {
    int lineNumber;
    string lineText;
    
public:
    FileLine(int lineNr, string text);
    int getLineNumber() { return lineNumber; };
    string& getLineText() { return lineText; };
    
};

#endif
