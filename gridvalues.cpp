#include "gridvalues.h"
#include "functions.h"
#include <assert.h>

GridValues::GridValues():
    m_gridSide(0),
    m_firstX(0),
    m_firstY(0),
    m_realStep(0)
{
}

GridValues::GridValues(size_t gridSide, const Origin &origin, double realStep):
    m_gridSide(gridSide),
    m_firstX(origin.first),
    m_firstY(origin.second),
    m_realStep(realStep),
    m_values(gridSide * gridSide)
{
    static bool typeRegistered = false;
    if(!typeRegistered)
    {
        qRegisterMetaType<GridValues>();
        typeRegistered = true;
    }
    fillWithFunction(zero);
}

GridValues& GridValues::operator=(GridValues&& other)
{
  if(this == &other)
      return *this;

  assert(equalSizeOriginAndStep(*this, other));
  m_values = std::move(other.m_values);
  return *this;
}

size_t GridValues::size() const
{
    return m_gridSide;
}

double GridValues::realStep() const
{
    return m_realStep;
}

GridValues::Origin GridValues::origin() const
{
    return std::make_pair(m_firstX, m_firstY);
}

double GridValues::value(size_t row, size_t column) const
{
    assert(row < m_gridSide);
    assert(column < m_gridSide);
    return m_values[row * m_gridSide + column];
}

double &GridValues::value(size_t row, size_t column)
{
    assert(row < m_gridSide);
    assert(column < m_gridSide);
    return m_values[row * m_gridSide + column];
}

const std::valarray<double> &GridValues::values() const
{
    return m_values;
}

QVector3D GridValues::pointAt(size_t row, size_t column) const
{
    //Origin вверху слева
    return QVector3D(m_firstX + m_realStep * column,
                     m_firstY - m_realStep * row,
                     value(row, column));
}

void GridValues::fillWithFunction(double (*function)(double, double))
{
    double currentX = m_firstX;
    double currentY = m_firstY;
    for(size_t rowIndex = 0; rowIndex < m_gridSide; ++rowIndex)
    {
        currentX = m_firstX;
        for(size_t columnIndex = 0; columnIndex < m_gridSide; ++columnIndex)
        {
            value(rowIndex, columnIndex) = function(currentX, currentY);
            currentX += m_realStep;
        }
        currentY -= m_realStep;
    }
}

void GridValues::fillBoundaryWithFunction(double (*function)(double, double))
{
    double currentX = m_firstX;
    double currentY = m_firstY;
    for(size_t rowIndex = 0; rowIndex < m_gridSide; ++rowIndex)
    {
        currentX = m_firstX;
        for(size_t columnIndex = 0; columnIndex < m_gridSide; ++columnIndex)
        {
            bool isBorder = false;
            if( rowIndex == 0 || rowIndex == m_gridSide - 1)
                isBorder = true;
            else
            {
                if(columnIndex == 0 || columnIndex == m_gridSide - 1)
                    isBorder = true;
            }

            if(isBorder)
                value(rowIndex, columnIndex) = function(currentX, currentY);
            currentX += m_realStep;
        }
        currentY -= m_realStep;
    }
}

double GridValues::maxAbs() const
{
    return std::abs(m_values).max();
}

GridValues GridValues::up() const
{
    assert(m_gridSide > 0);
    const size_t upGridSide = m_gridSide * 2 - 1;
    const double upGridRealStep = m_realStep / 2.;
    GridValues upGrid(upGridSide, origin(), upGridRealStep);

    for(size_t rowIndex = 0; rowIndex < upGridSide; ++rowIndex)
        for(size_t columnIndex = 0; columnIndex < upGridSide; ++columnIndex)
        {
            if(rowIndex % 2 == 0)
            {
               if(columnIndex  % 2 == 0)
                   upGrid.value(rowIndex, columnIndex) = value(rowIndex / 2, columnIndex / 2);
               else
                   upGrid.value(rowIndex, columnIndex) =
                           0.5 *
                           (value(rowIndex / 2, columnIndex / 2) +
                            value(rowIndex / 2, columnIndex / 2 + 1));
            }
            else
            {
                if(columnIndex  % 2 == 0)
                {
                    upGrid.value(rowIndex, columnIndex) =
                            0.5 *
                            (value(rowIndex / 2, columnIndex / 2) +
                             value(rowIndex / 2 + 1, columnIndex / 2));
                }
                else
                    upGrid.value(rowIndex, columnIndex) =
                            0.25 *
                            (value(rowIndex / 2, columnIndex / 2) +
                             value(rowIndex / 2 + 1, columnIndex / 2) +
                             value(rowIndex / 2, columnIndex / 2 + 1) +
                             value(rowIndex / 2 + 1, columnIndex / 2 + 1));
            }
        }

    return upGrid;
}

GridValues GridValues::down() const
{
    GridValues downGrid = zeroDownGrid();

    auto downGridIterator = std::begin(downGrid.m_values);
    for(size_t rowIndex = 0; rowIndex < m_gridSide; rowIndex += 2)
        for(size_t columnIndex = 0; columnIndex < m_gridSide; columnIndex += 2)
        {
            *downGridIterator = value(rowIndex, columnIndex);
            downGridIterator++;
        }

    return downGrid;
}

GridValues GridValues::zeroDownGrid() const
{
    assert(m_gridSide > 4);
    assert((m_gridSide - 1) % 2 == 0);
    const size_t downGridSide = (m_gridSide - 1) / 2 + 1;
    const double downGridRealStep = m_realStep * 2.;
    GridValues zeroDownGrid(downGridSide, origin(), downGridRealStep);

    return zeroDownGrid;
}

GridValues& GridValues::operator+=(const GridValues &other)
{
    //undefined behavior
    assert(this != &other);
    assert(equalSizeOriginAndStep(*this, other));

    m_values += other.values();

    return *this;
}

GridValues& GridValues::operator*=(double value)
{
    m_values *= value;
    return *this;
}

bool equalSizeOriginAndStep(const GridValues& firstValues, const GridValues& secondValues)
{
    static const double EPSILON = .00001;
    if(firstValues.size() != secondValues.size() || fabs(firstValues.realStep() - secondValues.realStep()) > EPSILON)
        return false;

    const GridValues::Origin firstOrigin = firstValues.origin();
    const GridValues::Origin secondOrigin = secondValues.origin();

    return firstOrigin.first == secondOrigin.first && firstOrigin.second == secondOrigin.second;
}
