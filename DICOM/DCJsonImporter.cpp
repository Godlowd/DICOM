#include "DCJsonImporter.h"
#include "DCDicomFileModel.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
vector<DCDicomFileModel*> DCJsonImporter::genDcmFromJson(string filepath)
{
	vector<DCDicomFileModel*> result;
	QFile file(filepath.c_str());
	if (!file.open(QIODevice::ReadOnly))
	{
		return result;
	}
	QByteArray allData = file.readAll();
	file.close();

	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(allData, &jsonError);

	if (!document.isNull() && (jsonError.error == QJsonParseError::NoError)) {
		if (document.isArray()) {
			QJsonArray array = document.array();
			/// array中的每一个元素都是一个Dicom文件
			for (int index = 0; index < array.size(); index++) {
				DcmFileFormat fileFormat;
				string filename = "default-" + to_string(index)+ ".dcm";
				auto obj = array.at(index);
				if (obj.isObject()) {
					auto jsonObj = obj.toObject();
					QJsonObject::Iterator it;
					/// jsonObj中的每一个元素都是一个group
					for (it = jsonObj.begin(); it != jsonObj.end(); it++) {
						auto groupValue = it.value();
						/// groupValue应该也是键值对的集合
						if (groupValue.isObject()) {
							/// 存储在JSON中的group和element都是16进制
							/// 需要转换为10进制才能初始化DcmTag
							auto groupHexKey = it.key().toStdString();
							stringstream ss;
							ss << std::hex << groupHexKey;
							int groupDecKey;
							ss >> groupDecKey;

							auto groupObj = groupValue.toObject();
							QJsonObject::Iterator groupIt;
							for (groupIt = groupObj.begin(); groupIt != groupObj.end(); groupIt++) {
								auto elementHexKey = groupIt.key().toStdString();
								stringstream sss;
								sss << std::hex << elementHexKey;
								int elementDecKey;
								sss >> elementDecKey;

								auto elementValue = groupIt.value().toString().toStdString();

								DcmTag tagKey = DcmTag(groupDecKey, elementDecKey);
								bool status = fileFormat.getDataset()->putAndInsertString(tagKey, elementValue.c_str()).good();
								if (status) {
									int a;
								}
								else {
									cout << "tets" << endl;
								}
							}
						}
						else if (groupValue.isString()) {
							auto key = it.key();
							if (key == "filename")
								filename = groupValue.toString().toStdString();
						}
					}

					fileFormat.saveFile(filename.c_str(), EXS_LittleEndianImplicit);
					DCDicomFileModel *model = new DCDicomFileModel(fileFormat);
					result.push_back(model);
				}
				else {
					break;
				}
			}
		}
	}

	return result;
}
