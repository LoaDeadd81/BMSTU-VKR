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
    double a, b;
public:
    explicit NormalDistribution(double a, double b) : dist(0, 0.3), a(a), b(b) {};

    double gen() override {
        double mean = (b + a) / 2, interval = (b - a) / 2;
        double num = dist(rd);
        if (num < -1) num = -1;
        if (num > 1) num = 1;
        double res = mean + interval * num;

        return res;
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
