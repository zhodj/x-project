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
            boost::gregorian::date_facet *df = new boost::gregorian::date_facet(format.c_str());
            std::ostringstream is;
            is.imbue(std::locale(is.getloc(), df));
            is << second_clock::local_time();
            return is.str();
        }
    }
}
#endif
