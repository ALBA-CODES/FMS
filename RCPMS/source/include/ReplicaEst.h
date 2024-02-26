// Código retirado de https://www.johndcook.com/blog/skewness_kurtosis/

#ifndef __REPLICAEST_H__
#define __REPLICAEST_H__

#include <math.h>
#include <iostream>
#include <cmath>

class ReplicaEst
{
public:
    ReplicaEst();
    void Clear();
    void Push(double x);
    long long NumDataValues() const;
    double Mean() const;
    double Variance() const;
    double StandardDeviation() const;
    double Skewness() const;
    double Kurtosis() const;

    friend ReplicaEst operator+(const ReplicaEst a, const ReplicaEst b);
    ReplicaEst& operator+=(const ReplicaEst &rhs);

private:
    long long n;
    double M1, M2, M3, M4;
};

#endif
