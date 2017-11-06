__kernel void JacobiKernel(
  __global const double* pOriginUMatrix,
  __global const double* pFMatrix,
  __global double* pResultUMatrix,
  __local double* pLocalUValues,
  double h2,
  int matrixStride)
  {
    int localColumn = get_local_id(0);
    int localUValuesStride = get_local_size(0);
    int lastLocalColumn = localUValuesStride - 1;

    int localRow = get_local_id(1);
    int lastLocalRow = get_local_size(1) - 1;

    int globalColumn = get_global_id(0);
    int lastGlobalColumnRow = matrixStride - 2;

    int globalRow = get_global_id(1);

    if(lastLocalColumn > 0 && globalColumn == lastGlobalColumnRow - 1)
        --lastLocalColumn;

    if(lastLocalRow > 0 && globalRow == lastGlobalColumnRow - 1)
        --lastLocalRow;

    if(globalColumn < lastGlobalColumnRow && globalRow < lastGlobalColumnRow)
    {
        pLocalUValues[localRow * localUValuesStride + localColumn] = pOriginUMatrix[(globalRow + 1) * (matrixStride) + globalColumn + 1];
        barrier(CLK_LOCAL_MEM_FENCE);

        double topUValue;
        if(localRow == 0)
            topUValue = pOriginUMatrix[globalRow * (matrixStride) + globalColumn + 1];
        else
            topUValue = pLocalUValues[(localRow - 1) * localUValuesStride + localColumn];

        double leftUValue;
        if(localColumn == 0)
            leftUValue = pOriginUMatrix[(globalRow + 1) * (matrixStride) + globalColumn];
        else
            leftUValue = pLocalUValues[localRow * localUValuesStride + localColumn - 1];

        double rightUValue;
        if(localColumn == lastLocalColumn)
            rightUValue = pOriginUMatrix[(globalRow + 1) * (matrixStride) + globalColumn + 2];
        else
            rightUValue = pLocalUValues[localRow * localUValuesStride + localColumn + 1];

        double bottomUValue;
        if(localRow == lastLocalRow)
            bottomUValue = pOriginUMatrix[(globalRow + 2) * (matrixStride) + globalColumn + 1];
        else
            bottomUValue = pLocalUValues[(localRow + 1) * localUValuesStride + localColumn];

        pResultUMatrix[(globalRow + 1) * (matrixStride) + globalColumn + 1] =
          1./4. * (topUValue + leftUValue + rightUValue + bottomUValue +
                        pFMatrix[(globalRow + 1) * (matrixStride) + globalColumn + 1] * h2);

    }

//    if(globalColumn < lastGlobalColumnRow && globalRow < lastGlobalColumnRow)
//    {
//        double topUValue;
//            topUValue = pOriginUMatrix[globalRow * (matrixStride) + globalColumn + 1];

//        double leftUValue;
//            leftUValue = pOriginUMatrix[(globalRow + 1) * (matrixStride) + globalColumn];

//        double rightUValue;
//            rightUValue = pOriginUMatrix[(globalRow + 1) * (matrixStride) + globalColumn + 2];

//        double bottomUValue;
//            bottomUValue = pOriginUMatrix[(globalRow + 2) * (matrixStride) + globalColumn + 1];

//        pResultUMatrix[(globalRow + 1) * (matrixStride) + globalColumn + 1] =
//          1./4. * (topUValue + leftUValue + rightUValue + bottomUValue +
//                        pFMatrix[(globalRow + 1) * (matrixStride) + globalColumn + 1] * h2);

//    }





//    int column = get_global_id(0);
//    int row = get_global_id(1);
//    int lastDummyColumn = matrixStride - 2;

//    int localColumn = get_local_id(0);
//    int lastLocalColumn = get_local_size(0) - 1;

//    if(column < lastDummyColumn)
//    {
//        pLocalUValues[localColumn] = pOriginUMatrix[(row + 1) * (matrixStride) + column + 1];
//        barrier(CLK_LOCAL_MEM_FENCE);

//        if(localColumn > 0 && localColumn < lastLocalColumn && column < (lastDummyColumn - 1))
//        {
//            pResultUMatrix[(row + 1) * (matrixStride) + column + 1] =
//              native_divide
//              (
//                  pOriginUMatrix[row * (matrixStride) + column + 1] +

////                  pOriginUMatrix[(row + 1) * (matrixStride) + column] +
////                  pOriginUMatrix[(row + 1) * (matrixStride) + column + 2] +
//                  pLocalUValues[localColumn - 1] +
//                  pLocalUValues[localColumn + 1] +

//                  pOriginUMatrix[(row + 2) * (matrixStride) + column + 1] +
//                  pFMatrix[(row + 1) * (matrixStride) + column + 1] * h2, 4.
//              );
//        }
//        else
//        {
//            if(localColumn == 0)
//            {
//                pResultUMatrix[(row + 1) * (matrixStride) + column + 1] =
//                  native_divide
//                  (
//                      pOriginUMatrix[row * (matrixStride) + column + 1] +
//                      pOriginUMatrix[(row + 1) * (matrixStride) + column] +

////                      pOriginUMatrix[(row + 1) * (matrixStride) + column + 2] +
//                      pLocalUValues[localColumn + 1] +

//                      pOriginUMatrix[(row + 2) * (matrixStride) + column + 1] +
//                      pFMatrix[(row + 1) * (matrixStride) + column + 1] * h2, 4.
//                  );
//            }
//            else
//            {
//                pResultUMatrix[(row + 1) * (matrixStride) + column + 1] =
//                  native_divide
//                  (
//                      pOriginUMatrix[row * (matrixStride) + column + 1] +

////                      pOriginUMatrix[(row + 1) * (matrixStride) + column] +
//                      pLocalUValues[localColumn - 1] +

//                      pOriginUMatrix[(row + 1) * (matrixStride) + column + 2] +
//                      pOriginUMatrix[(row + 2) * (matrixStride) + column + 1] +
//                      pFMatrix[(row + 1) * (matrixStride) + column + 1] * h2, 4.
//                  );
//            }
//        }
//    }
////    else
////        barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);
  }
