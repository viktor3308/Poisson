#include "residual.h"

#include <assert.h>

GridValues residual(const GridValues &uValues, const GridValues &fValues)
{
    assert(equalSizeOriginAndStep(uValues, fValues));

    const size_t matrixSize = uValues.size();
    GridValues result(matrixSize, uValues.origin(), uValues.realStep());

    const double h2 = pow(uValues.realStep(), 2);

    for(size_t rowIndex = 0; rowIndex < matrixSize; ++rowIndex)
    {
        for(size_t columnIndex = 0; columnIndex < matrixSize; ++columnIndex)
        {
            if(rowIndex != 0 && rowIndex != matrixSize - 1 &&
               columnIndex != 0 && columnIndex != matrixSize - 1)
            {

                result.value(rowIndex, columnIndex) =
                        - (uValues.value(rowIndex, columnIndex + 1)
                            - 2. * uValues.value(rowIndex, columnIndex)
                            + uValues.value(rowIndex, columnIndex - 1))
                        / h2
                        - (uValues.value(rowIndex + 1, columnIndex)
                            - 2. * uValues.value(rowIndex, columnIndex)
                            + uValues.value(rowIndex - 1, columnIndex))
                        / h2
                        - fValues.value(rowIndex, columnIndex);
            }
        }
    }

    return result;
}
