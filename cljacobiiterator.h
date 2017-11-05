#ifndef CLJACOBIITERATOR_H
#define CLJACOBIITERATOR_H

#include "iterators.h"
#include <memory>

namespace cl
{
class Kernel;
class CommandQueue;
class Context;
}

class ClJacobiIterator: public AbstractIterator
{
public:
    ClJacobiIterator();
    virtual ~ClJacobiIterator();

    virtual void operator()(GridValues& uValues, const GridValues& fValues, size_t iterations = 1) const;
private:
    std::unique_ptr<cl::Context> m_context;
    std::unique_ptr<cl::Kernel> m_kernel;
    std::unique_ptr<cl::CommandQueue> m_CommandQueue;
};

#endif // CLJACOBIITERATOR_H
