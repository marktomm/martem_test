#include "fileline.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/date_time.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;

int main(int argc, char* argv[])
{
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
         
            if (argc > 2) {
                string arg(argv[2]);
         
                if (arg == "--list" || arg == "-list") {
                    list = true;
                }
                else if (arg == "--all" || arg == "-all") {
                    all = true;
                }
                else {
                    stats = true;
                }
            }
            else {
                stats = true;
            }
         
            string line;
            boost::posix_time::ptime correctTime (boost::date_time::min_date_time);
            std::list<FileLine> invalidLineNumbers;
            int lineNr = 0;
            while (std::getline(file, line)) {
                lineNr++;
                boost::posix_time::ptime currentTime;
                
                try {
                    string datetime = line.substr(0, 23);
                    currentTime = boost::posix_time::time_from_string(datetime);
                }
                catch (int e) {
                    invalidLineNumbers.push_back(FileLine(lineNr, line));
                    continue;
                }

                if (correctTime > currentTime) {
                    invalidLineNumbers.push_back(FileLine(lineNr, line));
                }
                else {
                    correctTime = currentTime;
                }
            }
            
            if (stats == true || all == true) {
                cout << "Number of lines:" << std::endl;
                cout << invalidLineNumbers.size() << std::endl;
            
                cout << "Invalid time stamps at line(s):" << std::endl;
                int beginLineNumber = invalidLineNumbers.front().getLineNumber();
                int previousLineNumber = beginLineNumber;
                for(std::list<FileLine>::iterator i = invalidLineNumbers.begin();i != invalidLineNumbers.end(); i++) {
                    int lineNumber = (*i).getLineNumber();
                    if ((lineNumber - previousLineNumber) > 1) {
                        cout << beginLineNumber << "-" << previousLineNumber << std::endl;
                        beginLineNumber = lineNumber;
                        previousLineNumber = lineNumber;
                    }
                    else {
                        previousLineNumber = lineNumber;
                    }
                }
            }
            
            if (all == true || list == true) {
                
                cout << "Lines with invalid time stamps:" << std::endl;
                int beginLineNumber = invalidLineNumbers.front().getLineNumber();
                int previousLineNumber = beginLineNumber;
                for(std::list<FileLine>::iterator i = invalidLineNumbers.begin(); i != invalidLineNumbers.end(); i++) {
                    int lineNumber = (*i).getLineNumber();
                    string& lineText =  (*i).getLineText();
                    
                    if ((lineNumber - previousLineNumber) > 1) {
                        cout << "---" << std::endl;
                        beginLineNumber = lineNumber;
                        previousLineNumber = lineNumber;
                    }
                    else {
                        previousLineNumber = lineNumber;
                    }
                    cout << "[" << lineNumber << "] " << lineText  << std::endl;
                }
            }
        }
    }
    
    return 0;
}

