#pragma once

#include <string>
#include <vector>
#include "cPos.h"

typedef enum
{
    TID_PlayerHealth,
    TID_Inventory,
    TID_Message0,
    TID_Message1,
    TID_Message2,
    TID_End
} eTextID;

struct _TextElement
{
    cPos            pos;
    std::string     text;
    void            init(std::string str, int x, int y);
    void            set(std::string str);
};

class cStatus
{
public:
	cStatus(const cPos& posStatus);
    ~cStatus();
    void			cStatus::init();
	void			display();

private:
	cPos			origin;
	std::vector<_TextElement>	  vElements;
    bool            bDirty;
};

