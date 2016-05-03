#ifndef HELPER_COMMON
#define HELPER_COMMON
#include <boost/date_time/local_time/local_time.hpp>
using namespace boost::posix_time;

namespace helper {
    namespace Common {
        template<typename T>
        struct deleter_for_array
        {
            void operator()(T const * p)
            {
                delete[] p;
            }
        };
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
    }
}
#endif
