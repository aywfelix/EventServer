#include <string>
#include <fstream>
#include <streambuf>
#include "JsonConfig.h"

std::unique_ptr<JsonConfig> g_pConfig = nullptr;

bool JsonConfig::Load(const char *jsonFile)
{
    Json::Reader fileReader;
    std::ifstream ifs(jsonFile);
    if(ifs.good())
    {
        std::string str((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
		if (fileReader.parse(str, m_Root, true))
		{
			return true;
		}
        return false;
    }
    return false;
}
