/***********************************************************************
/
/  GRID CLASS (PREPARE NORMALIZATION FOR MAGNETIC FIELDS)
/            
/
/  written by: Tom Abel
/  date:       June, 2011
/  modified1:
/
/  PURPOSE:
/
/  RETURNS:
/    SUCCESS or FAIL
/
************************************************************************/
 
#include <stdio.h>
#include "ErrorExceptions.h"
#include "macros_and_parameters.h"
#include "typedefs.h"
#include "global_data.h"
#include "Fluxes.h"
#include "GridList.h"
#include "ExternalBoundary.h"
#include "Grid.h"
 
int grid::PrepareAlfvenVelocityNormalization(double *v_rms, double *Volume)
{
 
  /* Return if this doesn't concern us. */
 
  if (ProcessorNumber != MyProcessorNumber)
      return SUCCESS;
 
  /* Find fields: density, total energy, velocity1-3. */
 
  int DensNum, GENum, Vel1Num, Vel2Num, Vel3Num, TENum, B1Num, B2Num, B3Num;
    fprintf(stderr, "GPREPARVN: \n");
  if (this->IdentifyPhysicalQuantities(DensNum, GENum, Vel1Num, Vel2Num,
				       Vel3Num, TENum, B1Num, B2Num, B3Num) == FAIL) {
    fprintf(stderr, "GPRFN: Error in IdentifyPhysicalQuantities.\n");
    ENZO_FAIL("");
  }


  int i, j, k, index;
  double MassFactor=1;
  for (int dim = 0; dim < GridRank; dim++)
    MassFactor *= CellWidth[dim][0];

  for (k = GridStartIndex[2]; k <= GridEndIndex[2]; k++)
    for (j = GridStartIndex[1]; j <= GridEndIndex[1]; j++)
      for (i = GridStartIndex[0]; i <= GridEndIndex[0]; i++) {
	index = GRIDINDEX_NOGHOST(i,j,k);
	for (int dim = 0; dim < GridRank; dim++) {
	  int bf = B1Num + dim;
	  (*v_rms) += BaryonField[bf][index]*BaryonField[bf][index]/BaryonField[DensNum][index]
	    *MassFactor;
	}
	(*Volume) += MassFactor;
      }

  
  return SUCCESS;
}
