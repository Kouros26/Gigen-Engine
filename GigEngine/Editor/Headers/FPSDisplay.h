#pragma once
#include "Displayable.h"
#include "Watch.h"

class FPSDisplay : Displayable
{
public:

	FPSDisplay();
	~FPSDisplay() override;

	void Draw() override;
};
