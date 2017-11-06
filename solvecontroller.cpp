#include "solvecontroller.h"
#include "functions.h"
#include "residual.h"
#include "plot3d.h"
#include "iterators.h"
#include "cljacobiiterator.h"

#include <QElapsedTimer>

SolveController::SolveController(size_t gridSize, double epsilon, std::unique_ptr<AbstractIterator> iterator, size_t stepIterations):
    m_gridSize(gridSize),
    m_epsilon(epsilon),
    m_iterator(std::move(iterator)),
    m_stepIterations(stepIterations)
{
}

void SolveController::solve()
{
    const double h = 1./m_gridSize;
    const double xOrigin = 0.;
    const double yOrigin = 1.;
    const GridValues::Origin origin = std::make_pair(xOrigin, yOrigin);

    GridValues fValues(m_gridSize + 1, origin, h);
    fValues.fillWithFunction(f);

    GridValues uValues(m_gridSize + 1, origin, h);
    uValues.fillBoundaryWithFunction(g);

    const double zeroResidual = residual(uValues, fValues).maxAbs();
    double currentResidual = zeroResidual;

    QElapsedTimer timer;
    timer.start();

    size_t iteration = 0;
    double previousEpsilon = 1.;
    double currentEpsilon = 1.;
    do
    {
        previousEpsilon = currentEpsilon;
        (*m_iterator)(uValues, fValues, m_stepIterations);
        currentResidual = residual(uValues, fValues).maxAbs();
        ++iteration;
        emit solution(uValues);
        emit message(QString("Итерация: ") + QString::number(iteration) + "\nОтношение невязки: "
                     + QString::number(currentResidual / zeroResidual)
                     + " Максимальный модуль невязки: " + QString::number(currentResidual) + "\n");
        currentEpsilon = currentResidual / zeroResidual;
    }
    while(iteration < 1 && currentEpsilon <= previousEpsilon && currentEpsilon > m_epsilon);

    emit message(QString("Время: ") + QString::number(timer.elapsed()) + "\n");
    emit finished();
}
