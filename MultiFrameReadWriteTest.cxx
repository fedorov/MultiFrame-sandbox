/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include <iostream>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDCMTKImageIO.h"
#include "itkGDCMImageIO.h"
#include "itkSubtractImageFilter.h"
#include "itkStatisticsImageFilter.h"

template <class T> bool testReadWrite(char* inputName){

  typedef itk::ImageFileReader<T> ReaderType;
  typedef itk::ImageFileWriter<T> WriterType;

  typename ReaderType::Pointer dcmtkreader = ReaderType::New();
  typename ReaderType::Pointer gdcmreader = ReaderType::New();
  dcmtkreader->SetImageIO(itk::DCMTKImageIO::New());
  gdcmreader->SetImageIO(itk::GDCMImageIO::New());
  dcmtkreader->SetFileName(inputName);
  gdcmreader->SetFileName(inputName);

	bool dcmtkFailed = 0, gdcmFailed = 0;
  try
    {
    dcmtkreader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "exception in DCMTK file reader" << std::endl;
    std::cerr << e << std::endl;
		dcmtkFailed = 1;
    }

  try
    {
    gdcmreader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "exception in GDCM file reader" << std::endl;
    std::cerr << e << std::endl;
		gdcmFailed = 1;
    }

	if(dcmtkFailed && gdcmFailed){
		return false;
	}

  typename T::Pointer im1 = dcmtkreader->GetOutput();
  typename T::Pointer im2 = gdcmreader->GetOutput();

  typename WriterType::Pointer dcmtkwriter = WriterType::New();
  dcmtkwriter->SetInput(im1);
	dcmtkwriter->SetFileName("dcmtk_result.nrrd");

  typename WriterType::Pointer gdcmwriter = WriterType::New();
  gdcmwriter->SetInput(im1);
	gdcmwriter->SetFileName("gdcm_result.nrrd");

  try
    {
    dcmtkwriter->Update();
		gdcmwriter->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cerr << "exception in file writer" << std::endl;
    std::cerr << e << std::endl;
    return false;
    }
  catch(...)
    {
    return false;
    }

	return true;
}

int main(int argc, char *argv[])
{
  PARSE_ARGS;

  typedef itk::Image<unsigned short, 3>   ImageType3d;
  typedef itk::Image<unsigned short, 4>   ImageType4d;

	if(!testReadWrite<ImageType3d>(inputFileName.c_str())){
		std::cerr << "Reading 3d image failed" << std::endl;
	}

	if(!testReadWrite<ImageType4d>(inputFileName.c_str())){
		std::cerr << "Reading 4d image failed" << std::endl;
	}

  return EXIT_SUCCESS;
}
