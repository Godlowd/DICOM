#include "DCJsonImporter.h"
#include "DCDicomFileModel.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "RawDataReader.h"
vector<DCDicomFileModel*> DCJsonImporter::genDcmFromJson(string filepath, string rawDataPath, int xSize, int ySize, int zSize)
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

					if (rawDataPath != "") {
						RawDataReader *rawDataReader = new RawDataReader(xSize, ySize, zSize);
						auto rawData = rawDataReader->readRawDataFromFile(rawDataPath);

						auto biggestValue = rawDataReader->getBiggestValue();

						int16_t* imagePixel = new int16_t[xSize * ySize]();

						for (int row = 0; row < xSize; row++) {
							for (int col = 0; col < ySize; col++) {
								imagePixel[row * ySize + col] = 2;
								// imagePixel[row * ySize + col] = static_cast<int16_t>(rawData[zSize / 2 * xSize * ySize + row * ySize + col] / biggestValue);
							}
						}
						//fill DcmDataset
						fileFormat.getDataset()->putAndInsertString(DCM_SliceThickness, "0.5");
						fileFormat.getDataset()->putAndInsertString(DCM_PixelSpacing, "0.5\\0.5");
						fileFormat.getDataset()->putAndInsertString(DCM_PixelRepresentation, "1");
						fileFormat.getDataset()->putAndInsertUint16(DCM_ImageIndex, 1);
						fileFormat.getDataset()->putAndInsertString(DCM_InstanceNumber, "1");//图像码：辨识图像的号码.
						fileFormat.getDataset()->putAndInsertUint16(DCM_SamplesPerPixel, 1);
						fileFormat.getDataset()->putAndInsertUint16(DCM_NumberOfSlices, 200);
						fileFormat.getDataset()->putAndInsertUint16(DCM_Rows, static_cast<uint16_t>(xSize));
						fileFormat.getDataset()->putAndInsertUint16(DCM_Columns, static_cast<uint16_t>(ySize));
						fileFormat.getDataset()->putAndInsertUint16(DCM_PlanarConfiguration, 0);
						fileFormat.getDataset()->putAndInsertUint16(DCM_BitsAllocated, 16);
						fileFormat.getDataset()->putAndInsertUint16(DCM_BitsStored, 16);
						fileFormat.getDataset()->putAndInsertUint16(DCM_HighBit, 15);
						fileFormat.getDataset()->putAndInsertOFStringArray(DCM_PhotometricInterpretation, "MONOCHROME2");
						fileFormat.getDataset()->putAndInsertString(DCM_SpecificCharacterSet, "ISO_IR 192");//增加编码格式为utf-8

						fileFormat.getDataset()->putAndInsertUint8Array(DCM_PixelData, reinterpret_cast<Uint8*>(imagePixel), xSize * ySize * 2);
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
