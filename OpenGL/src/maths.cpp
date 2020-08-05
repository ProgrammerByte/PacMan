#include "maths.h"

//PI = 3.14159265358979323846;
//e = 2.71828182845904523536;

int factorial(int num) {
    int result = 1;
    for (int i = 2; i <= num; i++) {
        result *= i;
    }
    return result;
}

int nPr(int n, int r) {
    return factorial(n) / factorial(n - r);
}

int nCr(int n, int r) {
    return nPr(n, r) / factorial(r);
}

int sumX(int start, int end) {
    int result = 0;
    for (int i = start; i <= end; i++) {
        result += i;
    }
    return result;
}

double binomialExpansion(double x, double n, int iterations) { // |x| < 1 is assumed to be true, calculates (1 + x)^n
    double result = 1;
    double currentVal = 1;
    for (int i = 0; i < iterations; i++) {
        currentVal *= (double)(x * (n - i)) / (i + 1);
        if (currentVal == 0) {
            break;
        }
        result += currentVal;
    }



    return result;
}

double intPow(double base, int exponent) { //for integer exponents
    double result = 1;
    int exponentModulus = (exponent > 0) ? exponent:-exponent;
    for (int i = 0; i < exponentModulus; i++) {
        result *= base;
    }
    if (exponent > 0) {
        return result;
    }
    else {
        return (double)1 / result;
    }   
}

double root(double value, int exponent, int iterations) { //in this case exponent refers to the nth root
    if (value <= 0) { //if number is invalid
        return 0;
    }
    double x = value - 1;
    int multiplier = 1;
    int exponentModulus = (exponent > 0) ? exponent:-exponent;
    while (x > 1) {
        multiplier *= 10;
        x = ((double)value / (intPow(multiplier, exponentModulus))) - 1;
    }
    double result = binomialExpansion(x, (double)1 / exponentModulus, iterations) * multiplier;
    if (exponent > 0) {
        return result;
    }
    else {
        return (double)1 / result;
    }
}

double ln(double value, int iterations) { //value > 0 is assumed
    int lneCount = 0; //uses the rule ln(ab) = ln(a) + ln(b) hence ln(ea) = 1 + ln(a)
    while (value > 2) { //until valid
        lneCount += 1;
        //value = root(value, 2, iterations); this was very inefficient compared to the new rule
        value /= e;
    }

    double result = 0;
    if (value != 1) {
        double x = value - 1;
        double currentX = -1;
        for (int i = 1; i <= iterations; i++) { //applying Maclaurin series
            currentX *= -x;
            result += (double)currentX / i;
        }
    }
    return result + lneCount;
}

double log(double base, double value, int iterations) {
    return (double)ln(value, iterations) / ln(base, iterations);
}

//isSin is true whenever sin or sinh are called
//multiplier is -1 for sin and cos, and 1 for everything else (using Maclaurin series)
double exp(double x, int iterations, bool skip, bool isSin, int multiplier) { //skip is used for trigonometric and hyperbolic functions
    double result = 0;
    double currentVal = 1;
    for (int i = 1; i <= iterations; i++) {
        if (skip == false || (i + isSin) % 2 == 1) {
            result += currentVal;
            currentVal *= multiplier;
        }

        currentVal *= (double)x / i;
        if (currentVal == 0) {
            break;
        }
    }
    return result;
}

double calcExp(double x, int iterations) { //calculates e^x and is used for other methods
    return exp(x, iterations, false, false, 1);
}

double reduce(double x) { //reduces the value of x by 2PI until it can no longer be done, done in order to keep calculations accurate in trigonometric functions
    double period = 2 * PI;
    while (x >= period) {
        x -= period;
    }
    while (x <= -period) {
        x += period;
    }
    return x;
}

double sin(double x, int iterations) {
    return exp(reduce(x), iterations, true, true, -1);
}

double cos(double x, int iterations) {
    return exp(reduce(x), iterations, true, false, -1);
}

double tan(double x, int iterations) {
    return (double)sin(reduce(x), iterations) / cos(reduce(x), iterations);
}

double sinh(double x, int iterations) {
    return exp(x, iterations, true, true, 1);
}

double cosh(double x, int iterations) {
    return exp(x, iterations, true, false, 1);
}

double tanh(double x, int iterations) {
    return (double)sinh(x, iterations) / cosh(x, iterations);
}

double arcSinh(double x, int iterations) { //all x
    return ln(x + root(x * x + 1, 2, iterations), iterations);
}

double arcCosh(double x, int iterations) { //has two solutions, 2nd solution is the negative of the returned value. Only works when x >= 1.
    if (x >= 1) {
        return ln(x + root(x * x - 1, 2, iterations), iterations);
    }
    else {
        return -1000000; //invalid
    }
}

double arcTanh(double x, int iterations) {// -1 < x < 1
    if (x > -1 && x < 1) {
        return 0.5 * ln((x + 1) / (1 - x), iterations);
    }
    else {
        return -1000000; //invalid
    }
}

double iterativeSquareRoot(double currentVal, int iterations, int count) { //this method is very computationally expensive due to the iterative square roots
    int multiplier = 1;
    for (int i = 0; i < count; i++) { //if number is too large then cancellation errors occur (number becomes too large)
        multiplier *= 2;
        currentVal = (1 - root(1 - currentVal, 2, iterations)) / 2;
    }
    return multiplier * root(currentVal, 2, iterations);
}

double arcSin(double x, int iterations) {
    double result = iterativeSquareRoot(x * x, iterations, 6);
    if (x >= 0) {
        return result;
    }
    else {
        return -result;
    }
}

double arcCos(double x, int iterations) {
    return 2 * iterativeSquareRoot((1 - x) / 2, iterations, 5);
}

double arcTan(double x, int iterations) {
    double tanSquared = x * x;
    double result = iterativeSquareRoot(tanSquared / (1 + tanSquared), iterations, 6);
    if (result >= 0) {
        return result;
    }
    else {
        return -result;
    }
}

//base^exponent = e^exponent*ln(base)
double doublePow(double base, double exponent, int iterations) { //when the exponent is a decimal value
    if (base == 0) {
        return 0;
    }
    else if (exponent == 0) { //prevents unnecessary operations
        return 1;
    }
    return calcExp(exponent * ln(base, iterations), iterations);
}

double toRadians(double degrees) {
    return (degrees * PI) / 180;
}

double toDegrees(double radians) {
    return (radians * 180) / PI;
}

//int main() {
 //   cout << std::setprecision(10);

    //the following are all test lines - all tests have been successful!
    //cout << factorial(5) << endl;
    //cout << nPr(10, 5) << endl;
    //cout << nCr(10, 5) << endl;
    //cout << sumX(10, 100) << endl;
    //cout << intPow(4, -5) << endl;
    //cout << (double)binomialExpansion(0.5, 0.5, 10) << endl;

    //cout << root(1564, -7, 10000000) << endl; //many iterations are performed to give a very high level of accuracy

    //simplifyFraction(intPow(2, 5), intPow(2, 8));
    //quadraticEquation(-1, 4, 1);

    //cout << calcExp(5.6, 10000) << endl;
    //cout << sin(-PI / 2, 1000000) << endl;
    //cout << cos(-0.6, 1000000) << endl;
    //cout << tan(17 * e, 1000000) << endl; //as the value of x increases, the accuracy of this calculation decreases
    //cout << sinh(7 * PI, 1000000) << endl;
    //cout << cosh(-PI, 1000000) << endl;
    //cout << tanh(-15, 1000000) << endl;
    //cout << arcSinh(-45.6, 1000000) << endl;
    //cout << arcCosh(17.2, 1000000) << endl;
   // cout << arcTanh(-0.999999, 1000000) << endl;
    //cout << arcSin(-100, 1000000) << endl;
    //cout << arcCos(0.5, 1000000) << endl;
    //cout << arcTan(12, 1000000) << endl;

    //cout << toRadians(69.43) << endl;

    //cout << ln(50 * e, 1000000) << endl;

    //cout << log(1294.192384, 193.122465, 1000000) << endl;

    //cout << doublePow(25, 0.5, 1000000) << endl;
    //cout << doublePow(PI, e, 1000000) << endl;

//    return 0;
//}