
#include "fileline.h"
#include <string>
#include <boost/date_time.hpp>

using namespace std;


FileLine::FileLine (int lineNr, string text)
: longDelay(false)
{
    lineNumber = lineNr;
    lineText = text;
    
    setLineTime(getDateTimeFromLineString(lineText));
}

void FileLine::setIsCorrectLine (bool correct) {
    correctLine = correct;
}

void FileLine::setLongDelay() {
    longDelay = true;
}

void FileLine::setLineTime (boost::posix_time::ptime dateTime) {
    lineTime = dateTime;
}

boost::posix_time::ptime FileLine::getDateTimeFromLineString(string& line) {
    try {
        if (line.size() >= 23) {
            string datetime = line.substr(0, 23);
            return boost::posix_time::time_from_string(datetime);
        }
        else {
            setIsCorrectLine(false);
            return boost::date_time::min_date_time;
        }
    } catch (std::exception& e) {
        setIsCorrectLine(false);
        return boost::date_time::min_date_time;
    }
    
}