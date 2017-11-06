#include "cljacobiiterator.h"

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <iostream>
#include <fstream>

ClJacobiIterator::ClJacobiIterator()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for(const cl::Platform& platform : platforms)
    {
        std::string platformName{platform.getInfo<CL_PLATFORM_NAME>()};
        platformName.pop_back();
        if(platformName == std::string("NVIDIA CUDA"))
        {
            try
            {
                std::cout << "Name: " << platform.getInfo<CL_PLATFORM_NAME>();
                std::cout << " Vendor: " << platform.getInfo<CL_PLATFORM_VENDOR>();                
                std::cout << std::endl;

                std::vector<cl::Device> devices;
                platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
                m_context.reset(new cl::Context(devices));

                m_CommandQueue.reset(new cl::CommandQueue(*m_context, devices[0]));

                std::cout << " Local memory size: " << devices[0].getInfo<CL_DEVICE_LOCAL_MEM_SIZE>()  << std::endl;

                std::ifstream sourceFile("JacobiKernel.cl");
                std::string sourceCode(std::istreambuf_iterator<char>(sourceFile),(std::istreambuf_iterator<char>()));

                cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));
                cl::Program program = cl::Program(*m_context, source);
                program.build(devices);
                m_kernel.reset(new cl::Kernel(program, "JacobiKernel"));
            }
            catch(cl::Error error)
            {
                std::cout << error.what() << "(" << error.err() << ")" << std::endl;
            }
        }
    }
}

ClJacobiIterator::~ClJacobiIterator()
{
}

void ClJacobiIterator::operator()(GridValues& uValues, const GridValues& fValues, size_t iterations) const
{
    try
    {
        const int GRID_STRIDE = fValues.size();
        const int THREADS_STRIDE = GRID_STRIDE - 2;

        cl::Buffer clUValues1 = cl::Buffer(*m_context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
                                            (GRID_STRIDE) * (GRID_STRIDE) * sizeof(double),
                                            &(uValues.value(0,0)));

        cl::Buffer clUValues2 = cl::Buffer(*m_context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
                                            (GRID_STRIDE) * (GRID_STRIDE) * sizeof(double),
                                            &(uValues.value(0,0)));

        cl::Buffer* currentInputBufferPointer = &clUValues1;
        cl::Buffer* currentOutputBufferPointer = &clUValues2;

        double* fValuesPointer = const_cast<double*>(&(fValues.values()[0]));

        cl::Buffer clFValues = cl::Buffer(*m_context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                                          ((GRID_STRIDE) * (GRID_STRIDE)) * sizeof(double),
                                          fValuesPointer);

        const double h2 = pow(uValues.realStep(), 2);

        for(size_t iteration = 0; iteration < iterations; ++iteration)
        {
            const size_t TREADS_COUNT = THREADS_STRIDE + 1 <= 1024 ? THREADS_STRIDE + 1 : 1024u;
            //const size_t TREADS_COUNT = 128;

            int iArg = 0;
            m_kernel->setArg(iArg++, *currentInputBufferPointer);
            m_kernel->setArg(iArg++, clFValues);
            m_kernel->setArg(iArg++, *currentOutputBufferPointer);
            m_kernel->setArg(iArg++, TREADS_COUNT * sizeof(double), NULL);
            m_kernel->setArg(iArg++, h2);
            m_kernel->setArg(iArg++, GRID_STRIDE);

            //CL_INVALID_WORK_DIMENSION

            m_CommandQueue->enqueueNDRangeKernel(*m_kernel, cl::NullRange, cl::NDRange(THREADS_STRIDE + 1, THREADS_STRIDE), cl::NDRange(TREADS_COUNT, 1));

            //m_CommandQueue->
            //m_CommandQueue->finish();
            std::swap(currentInputBufferPointer, currentOutputBufferPointer);
        }
        m_CommandQueue->finish();


        //В конце цикла swap!
        m_CommandQueue->enqueueReadBuffer(*currentInputBufferPointer, CL_TRUE, 0,
                                          ((GRID_STRIDE) * (GRID_STRIDE)) * sizeof(double),
                                          &(uValues.value(0,0)));
    }
    catch(cl::Error error)
    {
        std::cout << error.what() << "(" << error.err() << ")" << std::endl;
    }
}
