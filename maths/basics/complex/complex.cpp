#include <complex>
#include <iostream>

typedef std::complex<double> Complex;

int main() {
    Complex z1(1.0, 2.0);
    Complex z2 = std::conj(z1);

    std::cout << z2 << std::endl;

    double z2_r = z2.real();
    double z2_i = z2.imag();

    return 0;
}