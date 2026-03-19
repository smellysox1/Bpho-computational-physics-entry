#include "lang.hpp"

bool lang::init(const std::string& lang) {
    pugi::xml_parse_result result = doc.load_file((global::basePath + "resources/lang/" + lang + ".xml").c_str());

    if (result.status != pugi::status_ok) {
        WC2_ERROR("Pugi error: ", result.description());
        return false;
    }

    currentLangDocRoot = doc.child("Strings");

    if (!currentLangDocRoot) {
        WC2_ERROR("No Strings node in lang file");
        return false;
    }

    WC2_LOG("Language '", lang, "' successfully initialised.");

    return true;
}

std::string lang::getString(const std::string& id) {
    pugi::xml_node stringNode = currentLangDocRoot.find_child_by_attribute("String", "id", id.c_str());

    if (stringNode)
        return std::string(stringNode.child_value());
    else {
        WC2_WARN("String ID not found: ", id);
        return "Error";
    }
}