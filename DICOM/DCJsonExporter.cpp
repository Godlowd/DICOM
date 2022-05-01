#include "DCJsonExporter.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "DCDicomFileModel.h"
#include "Utils.h"

DCJsonExporter::DCJsonExporter(vector<DcmTagKey*> tagArray):tagArray(tagArray)
{
}

void DCJsonExporter::exportDcmAsJson(string filePath, vector<DCDicomFileModel *>fileArray) {
	QJsonObject *root;
	QJsonArray *array = new QJsonArray();
	for (auto file : fileArray) {
		/// 一个tagPair中包含了一个dcm文件中所有感兴趣的tag的集合
		/// 这些集合以分组的方式(group)变成json文件中不同的字段
		/// 键值是groupId，值是所有elementId以及对应的value的集合
		QJsonObject tagPair;
		string lastGroup = "";
		QJsonObject lastGroupPair;
		for (int index = 0; index < tagArray.size(); index++) {
			auto tagKey = tagArray.at(index);
			auto group = Utils::decToHex(tagKey->getGroup());
			auto element = Utils::decToHex(tagKey->getElement());
			auto value = file->getStringForTag(*tagKey);

			// 当一个group中的所有element均遍历,将lastGroupPair添加到tagPair中
			if (group != lastGroup) {
				if (index != 0)
					tagPair.insert(lastGroup.c_str(), lastGroupPair);
				lastGroupPair = QJsonObject();
				lastGroup = group;
			}

			lastGroupPair.insert(element.c_str(), value.c_str());

			// 如果是最后一个tag
			if (index == tagArray.size() - 1)
				tagPair.insert(group.c_str(), lastGroupPair);
		}
		array->append(tagPair);
	}

	QJsonDocument document;
	document.setArray(*array);
	QByteArray byteArray = document.toJson(QJsonDocument::Indented);

	QFile file(filePath.c_str());
	if (!file.open(QIODevice::ReadWrite))
		return;
	file.write(byteArray);
	file.close();
}
