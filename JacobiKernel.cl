__kernel void JacobiKernel(
  __global const double* pOriginUMatrix,
  __global const double* pFMatrix,
  __global double* pResultUMatrix,
  double h2,
  int matrixStride)
  {
    int column = get_global_id(0);
    int row = get_global_id(1);

    if(column < matrixStride - 2)
    {
    pResultUMatrix[(row + 1) * (matrixStride) + column + 1] =
      1./4. *
      (
          pOriginUMatrix[row * (matrixStride) + column + 1] +
          pOriginUMatrix[(row + 1) * (matrixStride) + column] +
          pOriginUMatrix[(row + 1) * (matrixStride) + column + 2] +
          pOriginUMatrix[(row + 2) * (matrixStride) + column + 1] +
          pFMatrix[(row + 1) * (matrixStride) + column + 1] * h2
      );
      }
  }
