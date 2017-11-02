#include "plot3d.h"
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <assert.h>

namespace
{
    size_t getMatrixSize(const std::valarray<double>& matrixValues)
    {
        const size_t matrixSize = static_cast<size_t>(sqrt(matrixValues.size()));
        assert( matrixSize*matrixSize == matrixValues.size());
        return matrixSize;
    }
}

Plot3D::Plot3D(QtDataVisualization::Q3DSurface *surface) :
    QObject(surface),
    m_surface(surface),
    m_currentSeries(0)
{
    qRegisterMetaType<std::valarray<double>>();

    m_surface->axisX()->setLabelFormat("X %.2f");
    m_surface->axisZ()->setLabelFormat("Y %.2f");
    m_surface->axisY()->setLabelFormat("Z %.2f");
    m_surface->axisZ()->setRange(0, 1);
    m_surface->axisX()->setRange(0, 1);
    m_surface->axisY()->setRange(0, 1);

    m_series[0] = new QtDataVisualization::QSurface3DSeries();
    m_series[1] = new QtDataVisualization::QSurface3DSeries();

    QLinearGradient gradient;
    gradient.setColorAt(0.0, Qt::black);
    gradient.setColorAt(0.1, Qt::blue);
    gradient.setColorAt(0.6, Qt::red);
    gradient.setColorAt(1.0, Qt::yellow);
    m_series[0]->setBaseGradient(gradient);
    m_series[0]->setColorStyle(QtDataVisualization::Q3DTheme::ColorStyleRangeGradient);
    m_series[1]->setBaseGradient(gradient);
    m_series[1]->setColorStyle(QtDataVisualization::Q3DTheme::ColorStyleRangeGradient);

    m_surface->addSeries(m_series[m_currentSeries]);
}

Plot3D::~Plot3D()
{
    delete m_series[!m_currentSeries];
}

void Plot3D::setData(const GridValues &data)
{
    const size_t matrixSize = data.size();
    QtDataVisualization::QSurfaceDataArray *dataArray = new QtDataVisualization::QSurfaceDataArray;

    for(size_t rowIndex = 0; rowIndex < matrixSize; ++rowIndex)
    {
        QtDataVisualization::QSurfaceDataRow *dataRow = new QtDataVisualization::QSurfaceDataRow;
        for(size_t columnIndex = 0; columnIndex < matrixSize; ++columnIndex)
        {
           const QVector3D& dataValue = data.pointAt(rowIndex, columnIndex);
           //Y и Z поменяны для визуализации
           *dataRow << QVector3D(dataValue.x(), dataValue.z(), dataValue.y());
        }
        *dataArray << dataRow;
    }
    m_series[!m_currentSeries]->dataProxy()->resetArray(dataArray);
    m_surface->removeSeries(m_series[m_currentSeries]);
    m_currentSeries = !m_currentSeries;
    m_surface->axisY()->setRange(data.values().min(), data.values().max());
    m_surface->addSeries(m_series[m_currentSeries]);
}
