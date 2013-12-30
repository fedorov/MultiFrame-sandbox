// STL includes
#include <iostream>
#include <string>


//DCMTK includes
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/ofstd/ofuuid.h"

#include "dcmultiFixupCLP.h"

#include "uidRoots.h"

int main(int argc, char** argv)
{
	PARSE_ARGS;
	DcmFileFormat fileformat;


	OFCondition status = fileformat.loadFile(inputFileName.c_str());
	if(status.good()){
		DcmMetaInfo *meta = fileformat.getMetaInfo();
		DcmDataset *dcm = fileformat.getDataset();

		// Set the correct class
		// LegacyEnhanced not recognized by David's tools
		if(useLegacy){
		  dcm->putAndInsertString(DCM_SOPClassUID, "1.2.840.10008.5.1.4.1.1.4.4");
				// UID_LegacyConvertedEnhancedMRImageStorage);
				// not supported in the Slicer version of DCMTK
		  meta->putAndInsertString(DCM_MediaStorageSOPClassUID, "1.2.840.10008.5.1.4.1.1.4.4");
				// UID_LegacyConvertedEnhancedMRImageStorage);
		}
	
		dcm->putAndInsertString(DCM_ImplementationClassUID, 
				QIICRTestImplementationClassUID);
		dcm->putAndInsertString(DCM_InstanceCreatorUID, 
				QIICRTestInstanceCreatorUID);
		
		OFUUID uid;
		OFString uidStr;

		uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		dcm->putAndInsertString(DCM_SOPInstanceUID, uidStr.c_str());
		meta->putAndInsertString(DCM_MediaStorageSOPInstanceUID, uidStr.c_str());

		uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		dcm->putAndInsertString(DCM_StudyInstanceUID, uidStr.c_str());

		uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		dcm->putAndInsertString(DCM_SeriesInstanceUID, uidStr.c_str());

		uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		dcm->putAndInsertString(DCM_FrameOfReferenceUID, uidStr.c_str());

		dcm->findAndDeleteElement(DCM_ReferencedRawDataSequence);

		DcmItem *seq, *item;
		dcm->findAndGetSequenceItem(DCM_DimensionOrganizationSequence, item);
		uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		item->putAndInsertString(DCM_DimensionOrganizationUID, uidStr.c_str());

		for(int i=0;i<3;i++){
		  dcm->findAndGetSequenceItem(DCM_DimensionIndexSequence, item, i);
		  item->putAndInsertString(DCM_DimensionOrganizationUID, uidStr.c_str());
		}

		fileformat.saveFile(outputFileName.c_str(), EXS_LittleEndianExplicit);

	} else {
		std::cerr << "Error: cannot read DICOM file (" << status.text() << ")" << std::endl;
	}
	return 0;
}
