#pragma once
#include <string>

class IResource
{
public:
	virtual ~IResource() = default;
};

std::string readFile(std::string const&);
