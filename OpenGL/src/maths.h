#pragma once

const double PI = 3.14159265358979323846;
const double e = 2.71828182845904523536;
int factorial(int num);
int nPr(int n, int r);
int nCr(int n, int r);
int sumX(int start, int end);
double intPow(double base, int exponent);
double root(double value, int exponent, int iterations);
double ln(double value, int iterations);
double log(double base, double value, int iterations);
double calcExp(double x, int iterations);
double reduce(double x);
double sin(double x, int iterations);
double cos(double x, int iterations);
double tan(double x, int iterations);
double sinh(double x, int iterations);
double cosh(double x, int iterations);
double tanh(double x, int iterations);
double arcSinh(double x, int iterations);
double arcCosh(double x, int iterations);
double arcTanh(double x, int iterations);
double arcSin(double x, int iterations);
double arcCos(double x, int iterations);
double arcTan(double x, int iterations);
double doublePow(double base, double exponent, int iterations);
double toRadians(double degrees);
double toDegrees(double radians);