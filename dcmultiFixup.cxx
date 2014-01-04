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

    meta->putAndInsertString(DCM_ImplementationClassUID, "1.2.276.0.7230010.3.0.3.6.1");
	
		dcm->putAndInsertString(DCM_InstanceCreatorUID, 
				QIICRTestInstanceCreatorUID);
		
		OFUUID uid;
		OFString uidStr;

		uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		dcm->putAndInsertString(DCM_SOPInstanceUID, uidStr.c_str());
		meta->putAndInsertString(DCM_MediaStorageSOPInstanceUID, uidStr.c_str());

		//uid.generate();
		//uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		//dcm->putAndInsertString(DCM_StudyInstanceUID, uidStr.c_str());

		DcmItem *seq, *item, *item2, *item3;
    // can't find the reason for this sequence existence!
    // -->
		status = dcm->findAndGetSequenceItem(DCM_ReferencedRawDataSequence, item);
    uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		item->putAndInsertString(DCM_StudyInstanceUID, uidStr.c_str());

		item->findAndGetSequenceItem(DCM_ReferencedSeriesSequence, item2);
		uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		item2->putAndInsertString(DCM_SeriesInstanceUID, uidStr.c_str());

		item2->findAndGetSequenceItem(DCM_ReferencedSOPSequence, item3);
    uid.generate();
		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
		item3->putAndInsertString(DCM_ReferencedSOPInstanceUID, uidStr.c_str());
    // <--

		status = dcm->findAndGetSequenceItem(DCM_DimensionOrganizationSequence, item);
    if(status.good()){
  		uid.generate();
  		uid.toString(uidStr, OFUUID::ER_RepresentationOID);
  		item->putAndInsertString(DCM_DimensionOrganizationUID, uidStr.c_str());
  
  		for(int i=0;i<3;i++){
  		  status = dcm->findAndGetSequenceItem(DCM_DimensionIndexSequence, item, i);
        if(status.good())
    		  item->putAndInsertString(DCM_DimensionOrganizationUID, uidStr.c_str());
  		}
    }

    // initialize AnatomicRegionSequence in the shared attributes
    dcm->findOrCreateSequenceItem(DCM_SharedFunctionalGroupsSequence, item);
    item->findOrCreateSequenceItem(DCM_FrameAnatomySequence, item2);
    item2->findOrCreateSequenceItem(DCM_AnatomicRegionSequence, item3);
    item3->putAndInsertString(DCM_CodeValue, "T-9200B");
    item3->putAndInsertString(DCM_CodingSchemeDesignator, "SRT");
    item3->putAndInsertString(DCM_CodeMeaning, "Prostate");

    // make sure PulseSequenceName is not longer than 16 characters
    OFString pulseSequenceName;
    status = dcm->findAndGetOFString(DCM_PulseSequenceName, pulseSequenceName);
    if(status.good()){
      pulseSequenceName[16] = '\0';
      dcm->putAndInsertString(DCM_PulseSequenceName, pulseSequenceName.c_str());
    }

		status = fileformat.saveFile(outputFileName.c_str(), EXS_LittleEndianExplicit);
    if(!status.good()){
      std::cerr << "Failed to save the output file" << std::endl;
    }

	} else {
		std::cerr << "Error: cannot read DICOM file (" << status.text() << ")" << std::endl;
	}
	return 0;
}
