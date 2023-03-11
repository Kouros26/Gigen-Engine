#pragma once
#include <string>

class IResource
{
public:
	IResource(const std::string& filePath) : filePath(filePath) {};
	virtual ~IResource() = default;

	[[nodiscard]] const std::string& GetFilePath() const { return filePath; };

protected:
	std::string filePath;
};

std::string readFile(std::string const&);
