#pragma once
#include <vector>
#include <string>
using namespace std;
class DCDicomFileModel;
class DCJsonImporter
{
public:
	vector<DCDicomFileModel *> genDcmFromJson(string filepath);
};

