#ifndef GRIDVALUES_H
#define GRIDVALUES_H

#include <valarray>
#include <QMetaType>
#include <QVector3D>

class GridValues
{
public:
    typedef std::pair<double, double> Origin;

    /*!
     * \brief Конструктор по умолчанию для qRegisterMetaType
     */
    GridValues();

    /*!
     * \brief Конструктор квадратной сетки со значениями
     * \param gridSide размер стороны сетки
     * \param origin реальные координаты для первого значения сетки (вверху слева)
     * \param realStep реальный шаг точек сетки
     */
    GridValues(size_t gridSide, const Origin& origin, double realStep);

    size_t size() const;
    double realStep() const;
    Origin origin() const;
    double value(size_t row, size_t column) const;
    double& value(size_t row, size_t column);

    const std::valarray<double>& values() const;

    /*!
     * \brief QVector3D с реальных значений x,y,z для визуализации
     * \param row строка
     * \param column столбец
     * \return QVector3D с реальными значениями
     */
    QVector3D pointAt(size_t row, size_t column) const;
    void fillWithFunction(double(*function)(double, double));
    void fillBoundaryWithFunction(double(*function)(double, double));
    double maxAbs() const;

    GridValues up() const;
    GridValues down() const;

    GridValues zeroDownGrid() const;

    GridValues& operator+=(const GridValues& other);

    GridValues& operator*=(double value);

private:
    const size_t m_gridSide;
    const double m_firstX;
    const double m_firstY;
    const double m_realStep;
    std::valarray<double> m_values;
};

/*!
 * \brief Проверка двух сеток на соответствие размеров, начальных координат и шага
 * \param firstValues первая сетка
 * \param secondValues вторая сетка
 * \return true если размеры, начальные координаты и шаг сеток совпадают
 */
bool equalSizeOriginAndStep(const GridValues& firstValues, const GridValues& secondValues);

Q_DECLARE_METATYPE(GridValues)

#endif // GRIDVALUES_H
