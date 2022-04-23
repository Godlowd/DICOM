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
		DicomImage *dicomImg = new DicomImage(sourceFileName.c_str());
		DiJPEGPlugin plugin;
		plugin.setQuality(quality);
		plugin.setSampling(ESS_444);
		bool status = dicomImg->writePluginFormat(&plugin, "test111.jpeg");
		return status;
	}
	else {
		return false;
	}
}
