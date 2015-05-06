
#include "fileline.h"
#include <string>

using namespace std;


FileLine::FileLine (int lineNr, string text) {
    lineNumber = lineNr;
    lineText = text;
}
