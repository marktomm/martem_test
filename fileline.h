

#ifndef martem_test_fileline_h
#define martem_test_fileline_h
#include <stdio.h>
#include <string>
#include <boost/date_time.hpp>

using namespace std;

class FileLine {
    int lineNumber;
    string lineText;
    bool correctLine = true;
    boost::posix_time::ptime lineTime;
    
public:
    FileLine(int lineNr, string text);
    int getLineNumber() { return lineNumber; };
    string& getLineText() { return lineText; };
    bool isCorrectLine () { return correctLine; }
    boost::posix_time::ptime& getLineTime () { return lineTime; };

    void setIsCorrectLine (bool correct);

private:
    void setLineTime (boost::posix_time::ptime dateTime);
    boost::posix_time::ptime getDateTimeFromLineString(string& line);
};

#endif
