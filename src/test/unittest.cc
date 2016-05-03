#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <sstream>
using namespace boost::posix_time;

std::string getNowByFormat(const std::string& format)
{
    static std::locale loc(std::wcout.getloc(),
                           new time_facet(format.c_str()));
    std::basic_stringstream<char> wss;
    wss.imbue(loc);
    ptime now = second_clock::universal_time();
    wss << now;
    return wss.str();
}

int main()
{
    std::string ws = getNowByFormat("%Y%m%d");
    std::cout << ws << std::endl;

    return 0;
}
