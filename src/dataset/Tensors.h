/////////////////////////////////////////////////////////////////////////////
// Name:            Tensors.h
// Author:          Imagicien ->LAMIRANDE-NADEAU Julien & NAZRATI R�da<-
// Creation Date:   10/07/2009
//
// Description: Tensors class.
//
// Last modifications:
//      by : Imagicien - 12/11/2009
/////////////////////////////////////////////////////////////////////////////
#ifndef TENSORS_H_
#define TENSORS_H_

#include "datasetInfo.h"
#include "Glyph.h"
#include "../misc/lic/TensorField.h"
#include "../misc/Fantom/FVector.h"

using namespace std;

class Tensors : public Glyph
{

public:
    // Constructor/Destructor
	Tensors( DatasetHelper* i_datasetHelper );
	~Tensors();

    // From DatasetInfo
    void draw();

    // Functions
    bool loadNifti( wxString i_fileName );

private:
    
    // From Glyph
    bool createStructure ( vector< float >& i_fileFloatData );
    void drawGlyph       ( int i_zVoxel, int i_yVoxel, int i_xVoxel, AxisType i_axis = AXIS_UNDEFINED );
    void freeArrays      ( bool i_VBOActivated );
    void setScalingFactor( float i_scalingFactor );
    // Functions    
    void setTensorInfo( FTensor i_FTensor, vector< FMatrix > &o_matrixVector, vector< float > &o_FAVector );
    
    // Variables
    vector< float   >  m_tensorsFA;        // All the tensors's FA values.
    vector< FMatrix >  m_tensorsMatrix;    // All the tensors's matrix.    
};

#endif /* TENSORS_H_ */
