#pragma once
#include <string>

class IResource
{
public:
	IResource(std::string const& filePath) : filePath(filePath) {};
	virtual ~IResource() = default;

	const std::string& GetFilePath() const { return filePath; };

protected:
	std::string filePath;
};

std::string readFile(std::string const&);
