#include "DCJsonExporter.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "DCDicomFileModel.h"
DCJsonExporter::DCJsonExporter(vector<DcmTagKey*> tagArray):tagArray(tagArray)
{
}

void DCJsonExporter::exportDcmAsJson(string filePath, vector<DCDicomFileModel *>fileArray) {
	QJsonObject *root;
	QJsonArray *array = new QJsonArray();
	for (auto file : fileArray) {
		QJsonObject *tagPair = new QJsonObject();
		int lastGroup = -1;
		QJsonObject *lastGroupPair(nullptr);
		for (int index = 0; index < tagArray.size(); index++) {
			auto tagKey = tagArray.at(index);
			auto group = tagKey->getGroup();
			auto element = tagKey->getElement();
			auto value = file->getStringForTag(*tagKey);

			if (group != lastGroup) {
				if (nullptr != lastGroupPair)
					tagPair->insert(to_string(group).c_str(), *lastGroupPair);
				lastGroupPair = new QJsonObject();
				lastGroup = group;
			}

			lastGroupPair->insert(to_string(element).c_str(), value.c_str());

			// 如果是最后一个tag
			if (index == tagArray.size() - 1)
				tagPair->insert(to_string(group).c_str(), *lastGroupPair);
		}
		array->insert((*array).end(), *tagPair);
	}

	QJsonDocument document;
	document.setArray(*array);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);

	QFile file(filePath.c_str());
	if (!file.open(QIODevice::ReadWrite))
		return;
	file.write(byteArray);
	file.close();
}
