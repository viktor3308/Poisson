__kernel void JacobiKernel(
  __global const double* pOriginUMatrix,
  __global const double* pFMatrix,
  __global double* pResultUMatrix,
  __local double* pLocalUValues,
  double h2,
  int matrixStride)
  {
    int column = get_global_id(0);
    int row = get_global_id(1);
    int lastDummyColumn = matrixStride - 2;

    int localColumn = get_local_id(0);
    int lastLocalColumn = get_local_size(0) - 1;

    if(column < lastDummyColumn)
    {
        pLocalUValues[localColumn] = pOriginUMatrix[(row + 1) * (matrixStride) + column + 1];
        barrier(CLK_LOCAL_MEM_FENCE);

        if(localColumn > 0 && localColumn < lastLocalColumn && column < (lastDummyColumn - 1))
        {
            pResultUMatrix[(row + 1) * (matrixStride) + column + 1] =
              native_divide
              (
                  pOriginUMatrix[row * (matrixStride) + column + 1] +

//                  pOriginUMatrix[(row + 1) * (matrixStride) + column] +
//                  pOriginUMatrix[(row + 1) * (matrixStride) + column + 2] +
                  pLocalUValues[localColumn - 1] +
                  pLocalUValues[localColumn + 1] +

                  pOriginUMatrix[(row + 2) * (matrixStride) + column + 1] +
                  pFMatrix[(row + 1) * (matrixStride) + column + 1] * h2, 4.
              );
        }
        else
        {
            if(localColumn == 0)
            {
                pResultUMatrix[(row + 1) * (matrixStride) + column + 1] =
                  native_divide
                  (
                      pOriginUMatrix[row * (matrixStride) + column + 1] +
                      pOriginUMatrix[(row + 1) * (matrixStride) + column] +

//                      pOriginUMatrix[(row + 1) * (matrixStride) + column + 2] +
                      pLocalUValues[localColumn + 1] +

                      pOriginUMatrix[(row + 2) * (matrixStride) + column + 1] +
                      pFMatrix[(row + 1) * (matrixStride) + column + 1] * h2, 4.
                  );
            }
            else
            {
                pResultUMatrix[(row + 1) * (matrixStride) + column + 1] =
                  native_divide
                  (
                      pOriginUMatrix[row * (matrixStride) + column + 1] +

//                      pOriginUMatrix[(row + 1) * (matrixStride) + column] +
                      pLocalUValues[localColumn - 1] +

                      pOriginUMatrix[(row + 1) * (matrixStride) + column + 2] +
                      pOriginUMatrix[(row + 2) * (matrixStride) + column + 1] +
                      pFMatrix[(row + 1) * (matrixStride) + column + 1] * h2, 4.
                  );
            }
        }
    }
//    else
//        barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
  }
