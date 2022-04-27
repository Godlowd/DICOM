#include "DCImageConvertManager.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmjpeg/dipijpeg.h"
bool DCImageConvertManager::convertToBMP(string sourceFileName, string dstFileName, int quality)
{
	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(sourceFileName.c_str());
	if (status.good()) {
		double winWidth, winCenter;
		fileformat.getDataset()->findAndGetFloat64(DCM_WindowWidth, winWidth);
		fileformat.getDataset()->findAndGetFloat64(DCM_WindowCenter, winCenter);

		DicomImage *dicomImg = new DicomImage(sourceFileName.c_str());
		dicomImg->setWindow(winCenter, winWidth);
		DiJPEGPlugin plugin;
		plugin.setQuality(quality);
		plugin.setSampling(ESS_444);
		bool status = dicomImg->writePluginFormat(&plugin, dstFileName.c_str());
		return status;
	}
	else {
		return false;
	}
}
