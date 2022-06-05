#include "DCXMLWriter.h"
#include <QFile>
#include "DCDicomFileModel.h"
#include <QDomDocument>
#include <QTextStream>
#include "Utils.h"

DCXMLWriter::DCXMLWriter(vector<DcmTagKey *> tagArray):tagArray(tagArray) {

}
void DCXMLWriter::genXMLFromFileModel(vector<DCDicomFileModel*> models, string xmlPath)
{
	QFile file(xmlPath.c_str());

	if (file.exists()) {
		return;
	}

	file.open(QIODevice::WriteOnly);
	QDomDocument doc;


	//写入xml头部
	QDomProcessingInstruction instruction; //添加处理命令
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);

	QDomElement root = doc.createElement("midware");
	doc.appendChild(root);
	for (int index = 0; index < models.size(); index++) {
		auto model = models.at(index);

		auto patientName = model->getStringForTag(DCM_PatientName);
		auto studyId = model->getStringForTag(DCM_StudyInstanceUID);
		auto seriesId = model->getStringForTag(DCM_SeriesInstanceUID);

		auto samePatientNode = this->findElement(root, "name", patientName);
		if (samePatientNode != nullptr) {
			auto sameStuydNode = this->findElement(*samePatientNode, "studyId", studyId);
			if (sameStuydNode != nullptr) {
				auto sameSeriesNode = this->findElement(*sameStuydNode, "seriesId", seriesId);
				if (sameSeriesNode != nullptr) {
					createTagNode(sameSeriesNode, tagArray, model, &doc);
				}
				else {
					QDomElement newSeriesNode = doc.createElement("Series");
					newSeriesNode.setAttribute("seriesId", stoi(seriesId));
					sameStuydNode->appendChild(newSeriesNode);
					createTagNode(&newSeriesNode, tagArray, model, &doc);
				}
			}
			else {
				QDomElement newStudyNode = doc.createElement("Study");
				newStudyNode.setAttribute("studyId", studyId.c_str());

				QDomElement newSeriesNode = doc.createElement("Series");
				newSeriesNode.setAttribute("seriesId", seriesId.c_str());

				newStudyNode.appendChild(newSeriesNode);
				samePatientNode->appendChild(newStudyNode);
				createTagNode(&newSeriesNode, tagArray, model, &doc);
			}
		}
		else {
			auto newPatientNode = doc.createElement("Patient");
			newPatientNode.setAttribute("name", patientName.c_str());

			QDomElement newStudyNode = doc.createElement("Study");
			newStudyNode.setAttribute("studyId", stoi(studyId));

			QDomElement newSeriesNode = doc.createElement("Series");
			newSeriesNode.setAttribute("seriesId", stoi(seriesId));

			newStudyNode.appendChild(newSeriesNode);
			newPatientNode.appendChild(newStudyNode);
			root.appendChild(newPatientNode);
			createTagNode(&newSeriesNode, tagArray, model, &doc);
		}
	}

	QTextStream out_stream(&file);
	doc.save(out_stream, 4); //缩进4格
	file.close();
}

QDomElement* DCXMLWriter::findElement(QDomElement root, string field, string value) {
	QDomElement childEle = root.firstChildElement();
	for (; !childEle.isNull(); childEle = childEle.nextSiblingElement()) {
		if (childEle.hasAttribute(field.c_str())) {
			if (childEle.attributeNode(field.c_str()).value() == value.c_str()) {
				return &childEle;
			}
		}
	}
	return nullptr;
}

void DCXMLWriter::createTagNode(QDomElement * root, vector<DcmTagKey *> tagArray, DCDicomFileModel * model, QDomDocument * doc) {
	auto instanceRootNode = doc->createElement("Instance");
	auto uid = model->getStringForTag(DCM_UID);
	instanceRootNode.setAttribute("uid", uid == "" ? "default-uid" : uid.c_str());
	for (auto tagKey : tagArray) {
		QDomElement tagEle = doc->createElement("tag");

		// 设置Group和Element
		auto groupAttr = doc->createAttribute("GroupId");
		auto group = Utils::decToHex(tagKey->getGroup());
		while (group.size() < 4)
		{
			group = "0" + group;
		}
		groupAttr.setValue(group.c_str());

		auto elementAttr = doc->createAttribute("ElementId");
		auto element = Utils::decToHex(tagKey->getElement());
		while (element.size() < 4)
		{
			element = "0" + element;
		}
		elementAttr.setValue(element.c_str());

		// 设置具体的值
		auto tagValue = model->getStringForTag(*tagKey);
		auto text = doc->createTextNode(tagValue.c_str());

		tagEle.setAttributeNode(groupAttr);
		tagEle.setAttributeNode(elementAttr);
		tagEle.appendChild(text);

		instanceRootNode.appendChild(tagEle);
	}

	root->appendChild(instanceRootNode);
}
