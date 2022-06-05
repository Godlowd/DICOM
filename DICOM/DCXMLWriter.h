#pragma once
#include <vector>
#include <string>
#include <QDomElement>

using namespace std;
class DCDicomFileModel;
class DcmTagKey;
class DCXMLWriter
{
public:
	DCXMLWriter::DCXMLWriter(vector<DcmTagKey *> tagArray);
	void genXMLFromFileModel(vector<DCDicomFileModel*> models, string xmlPath);
	QDomElement * findElement(QDomElement root, string field, string value);

	void createTagNode(QDomElement * root, vector<DcmTagKey*> tagArray, DCDicomFileModel * model, QDomDocument * doc);

private:
	vector<DcmTagKey *> tagArray;
};

