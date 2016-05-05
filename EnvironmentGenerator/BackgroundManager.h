
#pragma once

#include "Environment.h"


class BackgroundManager
{
public:
	BackgroundManager();
	~BackgroundManager();

	void update();

private:
	SectorPosition currentPosition;
};

