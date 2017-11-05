#include "iterators.h"
#include "residual.h"

#include <assert.h>
#include <utility>

AbstractIterator::~AbstractIterator()
{
}

void ZeidelIterator::iterate(const GridValues &uValues, const GridValues &fValues, GridValues &result) const
{
    assert(equalSizeOriginAndStep(uValues, fValues));
    assert(equalSizeOriginAndStep(uValues, result));
    const size_t matrixSize = uValues.size();

    const double h2 = pow(uValues.realStep(), 2);

    for(size_t rowIndex = 0; rowIndex < matrixSize; ++rowIndex)
    {
        for(size_t columnIndex = 0; columnIndex < matrixSize; ++columnIndex)
        {
            if(rowIndex != 0 && rowIndex != matrixSize - 1 &&
                    columnIndex != 0 && columnIndex != matrixSize - 1)
            {

                result.value(rowIndex, columnIndex) =
                        1./4. *
                        (uValues.value(rowIndex - 1, columnIndex) +
                         uValues.value(rowIndex + 1, columnIndex) +
                         uValues.value(rowIndex, columnIndex - 1) +
                         uValues.value(rowIndex, columnIndex + 1) +
                         fValues.value(rowIndex, columnIndex) * h2);
            }
        }
    }
}

void ZeidelIterator::operator()(GridValues &uValues, const GridValues &fValues, size_t) const
{
    iterate(uValues, fValues, uValues);
}

void JacobiIterator::operator()(GridValues &uValues, const GridValues &fValues, size_t iterations) const
{
    GridValues anotherUValues(uValues);

    GridValues* currentInput = &uValues;
    GridValues* currentOutput = &anotherUValues;

    for(size_t iteration = 0; iteration < iterations; ++ iteration)
    {
        iterate(*currentInput, fValues, *currentOutput);
        std::swap(currentInput, currentOutput);
    }
    if(currentInput != &uValues)
        uValues = std::move(anotherUValues);
}

MultigridIterator::MultigridIterator(size_t steps, const FunctionalIterator &simpleIterator):
    m_steps(steps),
    m_simpleIterator(simpleIterator)
{
}

void MultigridIterator::operator()(GridValues &uValues, const GridValues &fValues, size_t) const
{
    iterate(uValues, fValues, m_steps);
}

void MultigridIterator::iterate(GridValues &uValues, const GridValues &fValues, size_t steps) const
{
    m_simpleIterator(uValues, fValues);

    if(!steps)
        return;

    GridValues residualValues = residual(uValues, fValues);

    GridValues errorValues = uValues.zeroDownGrid();

    iterate(errorValues, residualValues.down() *= -1., steps - 1);
    uValues += errorValues.up();

    m_simpleIterator(uValues, fValues);
}
