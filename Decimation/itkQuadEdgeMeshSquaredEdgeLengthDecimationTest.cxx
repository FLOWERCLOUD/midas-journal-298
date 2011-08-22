/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshSquaredEdgeLengthDecimationTest.cxx,v $
  Language:  C++
  Date:      $Date: 2008-10-05 22:49:46 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <itkQuadEdgeMesh.h>
#include <itkVTKPolyDataReader.h>
#include <itkVTKPolyDataWriter.h>

#include <sstream>

#include "itkQuadEdgeMeshDecimationCriteria.h"
#include "itkQuadEdgeMeshSquaredEdgeLengthDecimation.h"

using namespace itk;

int main( int argc, char* argv[] )
{
  // ** ERROR MESSAGE AND HELP ** //
  if( argc < 3 )
    {
    std::cout << "Requires 3 argument: " << std::endl;
    std::cout << "1-Input file name " << std::endl;
    std::cout << "2-Number of Faces " << std::endl;
    std::cout << "3-Output file name " << std::endl;
    return EXIT_FAILURE;
    }

// ** TYPEDEF **
  typedef double                              CoordType;
  const unsigned int                          Dimension = 3;

  typedef QuadEdgeMesh< CoordType, Dimension >    MeshType;
  typedef VTKPolyDataReader< MeshType >           ReaderType;
  typedef VTKPolyDataWriter< MeshType >           WriterType;

  // ** READ THE FILE IN **
  ReaderType::Pointer reader = ReaderType::New( );
  reader->SetFileName( argv[1] );
  try
    {
    reader->Update( );
    }
  catch( itk::ExceptionObject & exp )
    {
    std::cerr << "Exception thrown while reading the input file " << std::endl;
    std::cerr << exp << std::endl;
    return EXIT_FAILURE;
    }

  MeshType::Pointer mesh = reader->GetOutput( );

  typedef NumberOfFacesCriterion< MeshType > CriterionType;
  typedef QuadEdgeMeshSquaredEdgeLengthDecimation< MeshType,
    MeshType, CriterionType > DecimationType;

  long N;
  std::stringstream ssout( argv[2] );
  ssout >>N;

  CriterionType::Pointer criterion = CriterionType::New();
  criterion->SetTopologicalChange( true );
  criterion->SetNumberOfElements( N );

  DecimationType::Pointer decimate = DecimationType::New();
  decimate->SetInput( mesh );
  decimate->SetCriterion( criterion );
  decimate->Update();

  // ** WRITE OUTPUT **
  WriterType::Pointer writer = WriterType::New( );
  writer->SetInput( decimate->GetOutput( ) );
  writer->SetFileName( argv[3] );
  writer->Update( );

  return EXIT_SUCCESS;

}
