#ifndef HELPER_XML_PARSER
#define HELPER_XML_PARSER

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml_print.hpp>

namespace helper {
    class Xmlparser {
    public:
        explicit Xmlparser(const std::string& xmlfile)
            :m_xmlfile(xmlfile)
        {
        }
    private:
        void parse()
        {
            using namespace rapidxml;
        }
        std::string getItem() {return item_;}
    private:
        std::string m_xmlfile;
    }
}
#endif
