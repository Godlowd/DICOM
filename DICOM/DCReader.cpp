#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "DCReader.h"
#include "DCDicomFileModel.h"

void DCReader::readFromFile(std::string fileName, DCDicomFileModel *model)
{

	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(fileName.c_str());
	if (status.good()) {
		this->lastPath = fileName;
		model->setDataSet(fileformat.getDataset());
		OFString patientName;

		if (fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientName).good()) {
			std::cout << "Patient's Name: " << patientName << std::endl;
		}

		if (model->getValueForTag(DCM_PatientName)) {
			std::cout << "Patient's Name: " << patientName << std::endl;
		}
	}
	else {

	}
}
