#ifndef HELPER_COMMON
#define HELPER_COMMON

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

        std::string getNowByFormat(std::string format)
        {
            boost::gregorian::date_facet *df = new boost::gregorian::date_facet(format);
            std::ostringstream is;
            is.imbue(std::locale(is.getloc(), df));
            is << second_clock::local_time() << std::endl;
            return is.str();
        }
    }
}
#endif
