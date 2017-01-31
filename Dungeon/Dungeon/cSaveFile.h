#pragma once

#include <string>

class cSaveFile
{
public:
    cSaveFile();
    ~cSaveFile();

    void    save(const std::string& strFile);
    void    load();
private:
};

