#pragma once
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include <vector>

typedef std::vector<DcmTagKey> TAG_ARRAY;

namespace CommonTag {
	std::vector<DcmTagKey> PATIENT_TAGS = {
	DCM_PatientName,
	DCM_PatientID,
	DCM_PatientBirthDate,
	DCM_PatientBirthTime,
	DCM_PatientSex,
	DCM_PatientWeight,
	DCM_PregnancyStatus
	};

	std::vector<DcmTagKey> STUDY_TAGS = {
		DCM_AccessionNumber,
		DCM_StudyID,
		DCM_StudyInstanceUID,
		DCM_StudyDate,
		DCM_StudyTime,
		DCM_ModalitiesInStudy,
		DCM_BodyPartExamined,
		DCM_StudyDescription,
		DCM_PatientAge
	};

	std::vector<DcmTagKey> SERIES_TAGS = {
		DCM_SeriesNumber,
		DCM_SeriesInstanceUID,
		DCM_Modality,
		DCM_SeriesDescription,
		DCM_SeriesDate,
		DCM_SeriesTime,
		DCM_ImagePositionPatient,
		DCM_ImageOrientationPatient,
		DCM_SliceThickness,
		DCM_SpacingBetweenSlices,
		DCM_SliceLocation,
		DCM_BodyPartExamined
	};
}
