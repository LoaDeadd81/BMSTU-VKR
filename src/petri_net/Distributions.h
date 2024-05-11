#ifndef SRC_DISTRIBUTIONS_H
#define SRC_DISTRIBUTIONS_H

#include <random>

using namespace std;

class BaseDistribution {
public:
    virtual double gen() = 0;
};

class UniformDistribution : public BaseDistribution {
private:
    random_device rd;
    uniform_real_distribution<> dist;
public:
    UniformDistribution(double a, double b) : dist(a, b) {};

    double gen() override {
        return dist(rd);
    }
};

class StudentDistribution : public BaseDistribution {
private:
    random_device rd;
    student_t_distribution<> dist;
public:
    explicit StudentDistribution(double n) : dist(n) {};

    double gen() override {
        return dist(rd);
    }
};

class NormalDistribution : public BaseDistribution {
private:
    random_device rd;
    normal_distribution<> dist;
public:
    explicit NormalDistribution(double mean, double sigma) : dist(mean, sigma) {};

    double gen() override {
        return dist(rd);
    }
};

class ImmediateDistribution : public BaseDistribution {
public:
    double gen() override {
        return 0;
    }
};

class ConstDistribution : public BaseDistribution {
private:
    double val;
public:
    explicit ConstDistribution(double val) : val(val) {};
public:
    double gen() override {
        return val;
    }
};

#endif
