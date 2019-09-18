
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <iomanip>

typedef double(*function)(double);
typedef double(*integrationMethod)(function, double, double, int);

double test_f(double x)
{
	return 0.5 * x * x;
}

double f(double x)
{
	double ln = log(1.0 + fabs(x));
	return ln * ln;
}

double RectangleLeftIntegration(function f, double a, double b, int n)
{
	double h = (b - a) / n;
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		double x = a + h * i;
		sum += f(x);
	}
	return sum * h;
}

double RectangleRightIntegration(function f, double a, double b, int n)
{
	double h = (b - a) / n;
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		double x = a + h * i;
		sum += f(x + h);
	}
	return sum * h;
}

double RectangleCenterIntegration(function f, double a, double b, int n)
{
	double h = (b - a) / n;
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		double x = a + h * i;
		sum += f(x + h / 2.0);
	}
	return sum * h;
}

double TrapezoidIntegration(function f, double a, double b, int n)
{
	double h = (b - a) / n;
	double sum = 0;
	for (int i = 0; i < n; i++)
	{
		double x = a + h * i;
		sum += (f(x) + f(x + h)) / 2.0;
	}
	return sum * h;
}

double SimpsonIntegration(function f, double a, double b, int n)
{
	if (n % 2) throw "n should be even";
	
	double h = (b - a) / n;
	double sum = 0;
	for (int i = 0; i < n / 2; i++)
	{
		double x_e = a + h * (i * 2);
		double x_o = a + h * (i * 2 + 1);
		sum += 2.0 * f(x_e);
		sum += 4.0 * f(x_o);
	}
	sum -= f(a);
	sum += f(b);
	return sum * h / 3.0;
}

double IntegrateWithAutoN(integrationMethod I, function f, double a, double b, double eps)
{
	int n = 16;

	double I1, I2 = 0;

	I1 = I(f, a, b, n);

	do
	{
		I2 = I1;
		n = n << 1;
		I1 = I(f, a, b, n);

	} while (fabs(I2 - I1) >= eps);

	std::cout << "N = " << n << "\n";
	return I1;
}

int main()
{
	double a = -1, b = 2;
	double eps = 0.001;

	std::cout << "----- MANUAL N = 64 -----\n\n";

	double I_rl =   RectangleLeftIntegration(f, a, b, 64);
	std::cout << "Rectangle left integration =   " << I_rl << "\n";

	double I_rr =  RectangleRightIntegration(f, a, b, 64);
	std::cout << "Rectangle right integration =  " << I_rr << "\n";

	double I_rc = RectangleCenterIntegration(f, a, b, 64);
	std::cout << "Rectangle center integration = " << I_rc << "\n";

	double I_t  =       TrapezoidIntegration(f, a, b, 64);
	std::cout << "Trapezoid integration =        " << I_t << "\n";

	double I_s  =         SimpsonIntegration(f, a, b, 64);
	std::cout << "Simpson integration =          " << I_s  << "\n";

	std::cout << "\n\n----- AUTO N -----\n\n";

	double I_arl =   IntegrateWithAutoN(RectangleLeftIntegration, f, a, b, eps);
	std::cout << "Rectangle left integration (auto N) =   " << I_arl << "\n\n";

	double I_arr =  IntegrateWithAutoN(RectangleRightIntegration, f, a, b, eps);
	std::cout << "Rectangle right integration (auto N) =  " << I_arr << "\n\n";

	double I_arc = IntegrateWithAutoN(RectangleCenterIntegration, f, a, b, eps);
	std::cout << "Rectangle center integration (auto N) = " << I_arc << "\n\n";

	double  I_at =       IntegrateWithAutoN(TrapezoidIntegration, f, a, b, eps);
	std::cout << "Trapezoid integration (auto N) =        " << I_at  << "\n\n";

	double  I_as =         IntegrateWithAutoN(SimpsonIntegration, f, a, b, eps);
	std::cout << "Simpson integration (auto N) =          " << I_as  << "\n\n";
	
	return 0;
}