#ifndef ITERATORS_H
#define ITERATORS_H

#include "gridvalues.h"

#include <functional>

typedef std::function<void(GridValues&, const GridValues&)> FunctionalIterator;

class AbstractIterator
{
public:
    virtual void operator()(GridValues& uValues, const GridValues& fValues) const = 0;
    virtual ~AbstractIterator();
}; // class AbstractIterator

class ZeidelIterator: public AbstractIterator
{
public:
    virtual void operator()(GridValues& uValues, const GridValues& fValues) const;
protected:
    void iterate(const GridValues& uValues, const GridValues& fValues, GridValues& result) const;
}; // class ZeidelIterator

class JacobiIterator: public ZeidelIterator
{
public:
    virtual void operator()(GridValues& uValues, const GridValues& fValues) const;
}; // class JacobiIterator

class MultigridIterator: public AbstractIterator
{
public:
    MultigridIterator(size_t steps, const FunctionalIterator& simpleIterator = ZeidelIterator());
    virtual void operator()(GridValues& uValues, const GridValues& fValues) const;
private:
    void iterate(GridValues& uValues, const GridValues& fValues, size_t steps) const;

    const size_t m_steps;
    const FunctionalIterator m_simpleIterator;
}; // class MultigridIterator

#endif // ITERATORS_H
