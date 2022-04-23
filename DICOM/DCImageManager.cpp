#include "DCImageManager.h"
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmjpeg/djencode.h"
#include "dcmtk/dcmjpeg/djrplol.h"

bool DCImageManager::losslessDefaultCompress(const char * sourceFileName, const char * dstFileName)
{
	DJEncoderRegistration::registerCodecs(); // register JPEG codecs
	DcmFileFormat fileformat;
	OFCondition status;
	if (fileformat.loadFile(sourceFileName).good())
	{
		DcmDataset *dataset = fileformat.getDataset();
		DcmItem *metaInfo = fileformat.getMetaInfo();
		DJ_RPLossless params; // codec parameters, we use the defaults

		// this causes the lossless JPEG version of the dataset to be created
		if (dataset->chooseRepresentation(EXS_JPEGProcess14SV1, &params).good() &&
			dataset->canWriteXfer(EXS_JPEGProcess14SV1))
		{
			// store in lossless JPEG format
			status = fileformat.saveFile(dstFileName, EXS_JPEGProcess14SV1);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
	DJEncoderRegistration::cleanup(); // deregister JPEG codecs

	return status.good();
}

bool DCImageManager::deCompress(const char * sourceFileName, const char * dstFileName)
{
	DJEncoderRegistration::registerCodecs(); // register JPEG codecs
	DcmFileFormat fileformat;
	OFCondition status;
	if (fileformat.loadFile(sourceFileName).good())
	{
		DcmDataset *dataset = fileformat.getDataset();
		DcmItem *metaInfo = fileformat.getMetaInfo();
		DJ_RPLossless params; // codec parameters, we use the defaults

		// this causes the lossless JPEG version of the dataset to be created
		if (dataset->chooseRepresentation(EXS_LittleEndianImplicit, &params).good() &&
			dataset->canWriteXfer(EXS_LittleEndianImplicit))
		{
			// store in lossless JPEG format
			status = fileformat.saveFile(dstFileName, EXS_LittleEndianImplicit);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
	DJEncoderRegistration::cleanup(); // deregister JPEG codecs

	return status.good();
}
