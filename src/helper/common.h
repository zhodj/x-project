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
    }
}
#endif
