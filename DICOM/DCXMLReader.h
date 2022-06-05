#pragma once

#include <vector>
#include <string>

using namespace std;
class DCDicomFileModel;
class DCXMLReader
{
public:
	vector<DCDicomFileModel *> genDcmFromXML(string filePath, string rawDataPath = "", int xSize = 160, int ySize = 160, int zSize = 210);
};

