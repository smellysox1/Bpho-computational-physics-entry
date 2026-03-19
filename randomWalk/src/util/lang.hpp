#include <string>
#include <pugixml.hpp>

#include "logger.hpp"
#include "../global.hpp"

namespace lang {
    bool init(const std::string& lang);
    std::string getString(const std::string& identifier);

    inline pugi::xml_document doc;
    inline pugi::xml_node currentLangDocRoot;
}