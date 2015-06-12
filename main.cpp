#include "fileline.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/date_time.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/shared_ptr.hpp>
#include <stdlib.h> 				// atoi

using namespace std;

int main(int argc, char* argv[])
{
    namespace bpt = boost::posix_time;
  
    if (argc < 2) {
        cout << "Input file missing" << std::endl;
        return 1;
    }
    else {
        ifstream file (argv[1]);
        if ( !file.is_open() ) {
            cout<<"Input file missing" << std::endl;
            return 1;
        }
        else {
            bool stats = false;
            bool list = false;
            bool all = false;
	    bool hangs = false;
	    
	    unsigned int msecs = 500;
            
            if (argc > 2) {
                string arg(argv[2]);
         
                if (arg == "--list" || arg == "-list") {
                    list = true;
                }
                else if (arg == "--all" || arg == "-all") {
                    all = true;
                }
                else if (arg == "--hangs-check") {
		    hangs = true;
		    if(argc > 3) {
			msecs = atoi(argv[3]);
		    } 
		}
                else {
                    stats = true;
                }
            }
            else {
                stats = true;
            }
            
            string line;
            bpt::ptime correctTime (boost::date_time::min_date_time);
            std::list<FileLine> fileLinesList;
            int lineNr = 0;
            while (std::getline(file, line)) {
                lineNr++;
                FileLine fileLine = FileLine(lineNr, line);
                bpt::ptime& currentTime = fileLine.getLineTime();
		
		if(hangs) {
		    /* Use correctTime as prevTime */
		    if(correctTime == boost::date_time::min_date_time) {
			correctTime = currentTime;
		    }
		    else {
			boost::shared_ptr<bpt::time_period> interval(new bpt::time_period(correctTime, currentTime));
			
			if(interval->is_null())
			    interval.reset(new bpt::time_period(currentTime, correctTime));
			
			if(interval->length() > bpt::millisec(msecs))
			  fileLine.setLongDelay();
			  
			correctTime = currentTime;
		    }
		      
		}
		else {
		    if (correctTime > currentTime) {
			fileLine.setIsCorrectLine(false);
		    }
		    else {
			correctTime = currentTime;
		    }
		}
		

                fileLinesList.push_back(fileLine);
            }
            
            if (stats == true || all == true) {
                cout << "Number of lines:" << std::endl;
                cout << fileLinesList.size() << std::endl;
            
                cout << "Invalid time stamps at line(s):" << std::endl;
                int incorrectLineBeginNumber = fileLinesList.front().getLineNumber();
                int previousLineNumber = incorrectLineBeginNumber;
                bool incorrectLines = false;
                for(std::list<FileLine>::iterator i = fileLinesList.begin();i != fileLinesList.end(); i++) {
                    int lineNumber = (*i).getLineNumber();
                    if (!(*i).isCorrectLine()) {
                        if (incorrectLines == false) {
                            incorrectLineBeginNumber = lineNumber;
                            incorrectLines = true;
                        }
                    }
                    else {
                        if (incorrectLines == true) {
                            if (incorrectLineBeginNumber == previousLineNumber) {
                                cout << incorrectLineBeginNumber << std::endl;
                            }
                            else {
                                cout << incorrectLineBeginNumber << "-" << previousLineNumber << std::endl;
                            }
                            incorrectLines = false;
                        }
                    }

                    previousLineNumber = lineNumber;
                }
            }
            
            if (all == true || list == true) {
                
                cout << "Lines with invalid time stamps:" << std::endl;
                FileLine& previousLine = fileLinesList.front();
                bool incorrectLines = false;
                for(std::list<FileLine>::iterator i = fileLinesList.begin();i != fileLinesList.end(); i++) {
                    int lineNumber = (*i).getLineNumber();
                    if (!(*i).isCorrectLine()) {
                        if (incorrectLines == false) {
                            incorrectLines = true;
                            
                            cout << "---" << std::endl;
                            cout << "[" << previousLine.getLineNumber() << "] " << previousLine.getLineText()  << std::endl;
                        }
                        cout << "[" << lineNumber << "] " << (*i).getLineText()  << std::endl;
                    }
                    else {
                        if (incorrectLines == true) {
                            cout << "[" << (*i).getLineNumber() << "] " << (*i).getLineText()  << std::endl;
                            incorrectLines = false;
                        }
                    }
                    previousLine = (*i);
                }

            }
            
            if (hangs == true) {
	      FileLine& previousLine = fileLinesList.front();
                bool incorrectLines = false;
                for(std::list<FileLine>::iterator i = fileLinesList.begin();i != fileLinesList.end(); i++) {
                    int lineNumber = (*i).getLineNumber();
                    if ((*i).isLongDelay()) {
			cout << "---" << std::endl;
			cout << "[" << previousLine.getLineNumber() << "] " << previousLine.getLineText()  << std::endl;
                        cout << "[" << lineNumber << "] " << (*i).getLineText()  << std::endl;
                    }
                    previousLine = (*i);
                }
	    }
        }
    }
    
    return 0;
}

