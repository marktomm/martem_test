
#include "fileline.h"
#include <string>
#include <boost/date_time.hpp>

using namespace std;


FileLine::FileLine (int lineNr, string text) {
    lineNumber = lineNr;
    lineText = text;
    
    setLineTime(getDateTimeFromLineString(lineText));
}

void FileLine::setIsCorrectLine (bool correct) {
    correctLine = correct;
}

void FileLine::setLineTime (boost::posix_time::ptime dateTime) {
    lineTime = dateTime;
}

boost::posix_time::ptime FileLine::getDateTimeFromLineString(string& line) {
    try {
        string datetime = line.substr(0, 23);
        return boost::posix_time::time_from_string(datetime);
    }catch (int e) {
        setIsCorrectLine(false);
        return boost::date_time::min_date_time;
    }
}