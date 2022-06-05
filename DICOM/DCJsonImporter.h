#pragma once
#include <vector>
#include <string>
using namespace std;
class DCDicomFileModel;
class DCJsonImporter
{
public:
	vector<DCDicomFileModel *> genDcmFromJson(string filepath, string rawDataPath = "", int xSize = 160, int ySize = 160, int zSize = 210);
};

