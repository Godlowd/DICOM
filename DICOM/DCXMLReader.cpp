#include "DCXMLReader.h"
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QFile>
#include "DCDicomFileModel.h"
#include <QMessageBox>
static const auto PATIENT_XML_TAG = "Patient";
static const auto STUDY_XML_TAG = "Study";
static const auto SERIES_XML_TAG = "Series";
static const auto INSTANCE_XML_TAG = "Instance";

vector<DCDicomFileModel*> DCXMLReader::genDcmFromXML(string filePath, string rawDataPath, int xSize, int ySize, int zSize)
{
	vector<DCDicomFileModel*> result;

	QFile file(filePath.c_str());

	if (file.open(QIODevice::ReadOnly)) {

		QDomDocument doc;
		if (!doc.setContent(&file)) {
			QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
			file.close();
			return result;
		}

		// 获得根节点
		QDomElement root = doc.documentElement();

		// 获取所有Patient节点
		QDomNodeList patientList = root.elementsByTagName(PATIENT_XML_TAG);


		/* 获取属性中的值 */
		for (int i = 0; i < patientList.count(); i++) {
			// 获取链表中的值
			QDomElement patientElement = patientList.at(i).toElement();

			QDomNodeList studyList = patientElement.elementsByTagName(STUDY_XML_TAG);

			for (int study = 0; study < studyList.count(); study++) {
				QDomElement studyElement = studyList.at(i).toElement();

				QDomNodeList seriesList = studyElement.elementsByTagName(SERIES_XML_TAG);

				for (int series = 0; series < seriesList.count(); series++) {
					QDomElement seriesElement = seriesList.at(i).toElement();

					QDomNodeList instanceList = seriesElement.elementsByTagName(INSTANCE_XML_TAG);

					for (int instance = 0; instance < instanceList.count(); instance++) {
						DcmFileFormat fileFormat;

						auto instanceElement = instanceList.at(instance).toElement();

						auto filename = "default" + std::to_string(instance) + ".dcm";
						if (instanceElement.hasAttribute("name")) {
							filename = instanceElement.attribute("name").toStdString();
						}

						QDomNodeList nodeList = instanceElement.childNodes();

						for (int tagIndex = 0; tagIndex < nodeList.count(); tagIndex++) {
							auto tagElement = nodeList.at(tagIndex).toElement();

							if (tagElement.hasAttribute("GroupId") && tagElement.hasAttribute("ElementId")) {
								auto group = tagElement.attribute("GroupId").toStdString();
								stringstream ss;
								ss << std::hex << group;
								int groupDecKey;
								ss >> groupDecKey;

								auto element = tagElement.attribute("ElementId").toStdString();
								stringstream sss;
								sss << std::hex << element;
								int elementDecKey;
								sss >> elementDecKey;

								auto value = tagElement.text().toStdString();

								DcmTag tagKey = DcmTag(groupDecKey, elementDecKey);
								bool status = fileFormat.getDataset()->putAndInsertString(tagKey, value.c_str()).good();


							}
						}

						fileFormat.saveFile(filename.c_str(), EXS_LittleEndianImplicit);
						DCDicomFileModel *model = new DCDicomFileModel(fileFormat);
						result.push_back(model);
					}
				}
			}
		}


		file.close();
	}

	return result;
}
