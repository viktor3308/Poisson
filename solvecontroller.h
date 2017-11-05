#ifndef SOLVECONTROLLER_H
#define SOLVECONTROLLER_H

#include <QObject>
#include "gridvalues.h"
#include "iterators.h"

#include <memory>

class Plot3D;

class SolveController: public QObject
{
    Q_OBJECT

public:
    SolveController(size_t gridSize, double epsilon, std::unique_ptr<AbstractIterator> iterator, size_t stepIterations);

signals:
    /*!
     * \brief Выдает решение на текушей итерации
     * \param data Данные решения. При передаче в другой поток Qt скопирует массив не смотря на ссылку
     * так что все ок.
     * \param h Шаг сетки
     */
    void solution(const GridValues& values) const;

    /*!
     * \brief Выдает сообщения о ходе выполнения
     * \param message
     */
    void message(const QString& message) const;

    /*!
     * \brief Сигнал окончания решения
     */
    void finished() const;

public slots:

    /*!
     * \brief Приступить к решению
     */
    void solve();

private:
    const size_t m_gridSize;
    const double m_epsilon;
    //const FunctionalIterator m_iterator;
    const std::unique_ptr<AbstractIterator> m_iterator;
    const size_t m_stepIterations;
};

#endif // SOLVECONTROLLER_H
