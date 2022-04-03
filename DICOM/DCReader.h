#pragma once
#include <string>
class DCDicomFileModel;
class DCReader
{
public:
	std::string lastPath;
	void DCReader::readFromFile(std::string fileName, DCDicomFileModel *model);
private:

};

