#include <iostream>
#include <sstream>
#include <iomanip>

typedef double (*function)(double x);
typedef double (*diffFunction)(double x, double y);

double df(double x, double y)
{
	return x + sin(y) / sqrt(3.0);
}

void PrintMatrix(const double* A, size_t w, size_t h)
{
	std::cout << "|-";
	for (int x = 0; x < w; x++)
	{
		std::cout << "-----------";
	}
	std::cout << "|\n";
	for (int y = 0; y < h; y++)
	{
		std::cout << "| ";
		for (int x = 0; x < w; x++)
		{
			std::cout << std::setw(10) << A[y * w + x] << " ";
		}
		std::cout << "|\n";
	}
	std::cout << "|-";
	for (int x = 0; x < w; x++)
	{
		std::cout << "-----------";
	}
	std::cout << "|\n";
}

void DifferentiateByEulerMethod(const diffFunction f, const double x0, const double y0, const double xn, const int n, double *X, double *Y)
{
	double h = (xn - x0) / (n - 1);
	X[0] = x0;
	Y[0] = y0;
	for (int i = 1; i < n; i++)
	{
		X[i] = x0 + h * i;
		Y[i] = Y[i - 1] + h * f(X[i - 1], Y[i - 1]);
	}
}

void DifferentiateByRungeKuttaMethod(const diffFunction f, const double x0, const double y0, const double xn, const int n, double *X, double *Y)
{
	double h = (xn - x0) / (n - 1);
	X[0] = x0;
	Y[0] = y0;
	for (int i = 1; i < n; i++)
	{
		double k1 = h * f(X[i - 1]          , Y[i - 1]           );
		double k2 = h * f(X[i - 1] + h / 2.0, Y[i - 1] + k1 / 2.0);
		double k3 = h * f(X[i - 1] + h / 2.0, Y[i - 1] + k2 / 2.0);
		double k4 = h * f(X[i - 1] + h      , Y[i - 1] + k3      );

		double dy = (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;

		X[i] = x0 + h * i;
		Y[i] = Y[i - 1] + dy;
	}
}

void DifferentiateByAdamsMethod(const diffFunction f, const double x0, const double y0, const double xn, const int n, double *X, double *Y)
{
	double h = (xn - x0) / (n - 1);
	DifferentiateByRungeKuttaMethod(f, x0, y0, x0 + h * 4.0, 4, X, Y);
	double h_5 = h * h * h * h * h;
	double *dY = new double[n];
	for (int k = 0; k < 5; k++)
	{
		//Calculate derivative
		for (int i = 0; i < 4; i++)
		{
			dY[i] = f(X[i], Y[i]);
		}
		//Prediction
		for (int i = 4; i < n; i++)
		{
			X[i] = x0 + h * i;
			//double y_5 = f(X[i - 1], f(X[i - 1], f(X[i - 1], f(X[i - 1], f(X[i - 1], Y[i - 1])))));
			double y_5 = 0;
			Y[i] = Y[i - 1] + h / 24.0 * (55.0 * dY[i - 1] - 59 * dY[i - 2] + 37.0 * dY[i - 3] - 9.0 * dY[i - 4]) + 251.0 / 720.0 * h_5 * y_5;
			dY[i] = f(X[i], Y[i]);
		}
		//Correction
		for (int i = 3; i < n; i++)
		{
			//double y_5 = f(X[i], f(X[i], f(X[i], f(X[i], f(X[i], Y[i])))));
			double y_5 = 0;
			Y[i] = Y[i - 1] + h / 24.0 * (9.0 * dY[i] + 19.0 * dY[i - 1] - 5.0 * dY[i - 2] + dY[i - 3]) + 19.0 / 720.0 * h_5 * y_5;
		}
	}
	delete[] dY;
}

#define N 100

int main()
{
	double Xe[N] = { 0 };
	double Ye[N] = { 0 };
	DifferentiateByEulerMethod(df, 1.1, 1.5, 2.1, N, Xe, Ye);

	std::cout << "N = " << N << "\n\n";

	std::cout << "X:\n";
	PrintMatrix(Xe, 5, N / 5);


	std::cout << "Euler Y:\n";
	PrintMatrix(Ye, 5, N / 5);
	//std::cout << Ye[N - 1] << "\n\n";

	double Xr[N] = { 0 };
	double Yr[N] = { 0 };
	DifferentiateByRungeKuttaMethod(df, 1.1, 1.5, 2.1, N, Xr, Yr);

	std::cout << "R.Kut. Y:\n";
	PrintMatrix(Yr, 5, N / 5);
	//std::cout << Yr[N - 1] << "\n\n";

	double Xa[N] = { 0 };
	double Ya[N] = { 0 };
	DifferentiateByAdamsMethod(df, 1.1, 1.5, 2.1, N, Xa, Ya);

	std::cout << "Adams Y:\n";
	PrintMatrix(Ya, 5, N / 5);
	//std::cout << Ya[N - 1] << "\n\n";

	return 0;
}