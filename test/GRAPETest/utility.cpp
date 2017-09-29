
#include "QtCore"
#include "utility.h"
#include <fstream>
#include <iostream>

using namespace std;

//--------------------------------------------------------------------------------------------------
bool compare_files(string f1, string f2)
{    
    ifstream sf1(f1);
    ifstream sf2(f2);

    if (!sf1.is_open())
        cout << "could not open file: " << f1 << endl;

    if (!sf2.is_open())
        cout << "could not open file: " << f2 << endl;

    if (!sf1.is_open() || !sf2.is_open())
            return false;

    string line1, line2;

    while ( getline(sf1,line1) && getline(sf2,line2) )
    {
        // find the third colon
        size_t pos;
        pos = find_nth(line1, 3, ':') + 1;
        line1 = line1.substr(pos+1);
        pos = find_nth(line2, 3, ':') + 1;
        line2 = line2.substr(pos+1);

        // remove new line characters
        line1.erase(std::remove(line1.begin(), line1.end(), '\n'), line1.end());
        line1.erase(std::remove(line1.begin(), line1.end(), '\r'), line1.end());
        line2.erase(std::remove(line2.begin(), line2.end(), '\n'), line2.end());
        line2.erase(std::remove(line2.begin(), line2.end(), '\r'), line2.end());

        // compare lines
        if (line1.compare(line2) != 0)
            return false;
    }

    return true;
}
//--------------------------------------------------------------------------------------------------
size_t find_nth(const string& s, size_t nth, char val)
{
    std::size_t pos = -1;
    unsigned occurrence = 0;

    while (occurrence < nth){
        pos = s.find(val, pos+1);
        if(pos != string::npos)
            ++occurrence;
        else
            return -1;
    }

    return pos;
}
//--------------------------------------------------------------------------------------------------


