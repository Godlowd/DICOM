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
	DCM_PatientSize,
	DCM_PatientWeight
	};

	std::vector<DcmTagKey> STUDY_TAGS = {
		DCM_AccessionNumber,
		DCM_StudyID,
		DCM_StudyDate,
		DCM_StudyTime,
		DCM_ReferringPhysicianName,
		DCM_StudyDescription,
		DCM_PatientAge,
		DCM_Radiopharmaceutical
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

	std::vector<std::vector<DcmTagKey>> PREDEFINED_TAGS = {
		PATIENT_TAGS, STUDY_TAGS, SERIES_TAGS
	};
}
