#pragma once
#include <string>
#include <vector>
using namespace std;
class DCDicomFileModel;
class DcmTagKey;
class DCJsonExporter
{
public:
	DCJsonExporter::DCJsonExporter(vector<DcmTagKey *> tagArray);
	void exportDcmAsJson(string filePath, vector<DCDicomFileModel*> fileArray);
private:
	vector<DcmTagKey *> tagArray;
};

