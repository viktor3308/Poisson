#ifndef FUNCTIONS_H
#define FUNCTIONS_H

inline double f(double x, double y)
{
    return -6. * (pow(x, 2) * y * (pow(x, 2) + 2. * pow(y, 2)));
}

inline double g(double x, double y)
{
    return pow(x, 4) * pow(y, 3);
    //return sin(x)*cos(y);
}

inline double zero(double, double)
{
    return 0.;
}

#endif // FUNCTIONS_H
