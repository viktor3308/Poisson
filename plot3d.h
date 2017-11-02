#ifndef PLOT3D_H
#define PLOT3D_H

#include <QObject>
#include "gridvalues.h"

namespace QtDataVisualization
{
    class Q3DSurface;
    class QSurface3DSeries;
}

class Plot3D : public QObject
{
    Q_OBJECT
public:
    explicit Plot3D(QtDataVisualization::Q3DSurface *surface);
    ~Plot3D();

signals:

public slots:
    void setData(const GridValues& data);

private:
    QtDataVisualization::Q3DSurface* m_surface;
    QtDataVisualization::QSurface3DSeries* m_series[2];
    bool m_currentSeries;
};

Q_DECLARE_METATYPE(std::valarray<double>)

#endif // PLOT3D_H
