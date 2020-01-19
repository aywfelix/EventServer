#include <string>
#include <iostream>
#include "libconfig++.hpp"

using namespace libconfig;

class LibConfig{
public:
	LibConfig() {  }
	~LibConfig() {  }
    static LibConfig& Instance()
    {
        static LibConfig instance;
        return instance;
    }

    bool loadcfg(std::string& file)
    {
        try
        {
            cfg.readFile(file);
        }
        catch(const FileIOException &e)
        {
            std::cerr << "I/O error while reading file."<<e.what()<< std::endl;
            return false;
        }
        catch(const ParseException &e)
        {
            std::cerr << "Parse error at " << e.getFile() << ":" << e.getLine()
                    << " - " << e.getError() << std::endl;
            return false;
        } 
		return true;
    }
    template<typename Type>
    void Lookup(const std::string &section, Type& value)
    {
        value = cfg.lookup(section);
    }

    const Setting& Lookup(const std::string &section)
    {
        return cfg.lookup(section);
    }

	const Setting& GetRoot()
	{
		return cfg.getRoot();
	}

	void Clear()
	{
		cfg.clear();
	}
private:
    Config cfg;
};