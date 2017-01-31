#include "stdafx.h"
#include "cSaveFile.h"

struct LevelHeader
{
    int   size;
    int   rows;
    int   cols;

};

struct GameLevel
{
};

// ----------------------------------------------------------------------------
cSaveFile::cSaveFile()
{
}

// ----------------------------------------------------------------------------
cSaveFile::~cSaveFile()
{
}

// ----------------------------------------------------------------------------
void    cSaveFile::save(const std::string& strFile)
{
    FILE * f = fopen(strFile.c_str(), "w+t");
    fclose(f);
}

// ----------------------------------------------------------------------------
void cSaveFile::load()
{

}

