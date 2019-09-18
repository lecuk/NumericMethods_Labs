#include <iostream>
#include <iomanip>

typedef double(*function)(double*, int);

double f1(double* x, int size)
{
	return cos(x[0] + 0.5) - x[1] - 2;
}

double f2(double* x, int size)
{
	return sin(x[1]) - 2 * x[0] - 1;
}

double phi1(double* x, int size)
{
	return (sin(x[1]) - 1.0) / 2.0;
}

double phi2(double* x, int size)
{
	return (cos(x[0] + 0.5) - 2);
}

double f1d1(double* x, int size)
{
	return -sin(x[0] + 0.5);
}

double f1d2(double* x, int size)
{
	return -1;
}

double f2d1(double* x, int size)
{
	return -2;
}

double f2d2(double* x, int size)
{
	return cos(x[1]);
}

double d_dx(function f, double* x, int size, int x0)
{
	constexpr double delta = 0.000001;
	double* dx = new double[size];
	for (int i = 0; i < size; i++)
	{
		dx[i] = x[i];
	}
	x[x0] += delta;
	double y = (f(dx, size) - f(x, size)) / delta;
	delete[] dx;
	return y;
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

double NormalValueBySqrt(double* B, size_t size)
{
	double sum = 0;
	for (int y = 0; y < size; y++)
	{
		sum += B[y] * B[y];
	}
	return sqrt(sum);
}

void IterationMethod(function* phi, double* X, int size, double eps)
{
	double* dX = new double[size];
	double* X0 = new double[size];
	for (int i = 0; i < size; i++)
	{
		X[i] = 0.0;
		dX[i] = 0.0;
		X0[i] = 0.0;
	}
	PrintMatrix(X, 1, size);

	int iterations = 0;
	while (iterations < 150)
	{
		for (int i = 0; i < size; i++)
		{
			X0[i] = X[i];
		}
		for (int i = 0; i < size; i++)
		{
			X[i] = phi[i](X0, size);
			dX[i] = X0[i] - X[i];
		}
		PrintMatrix(X, 1, size);

		if (NormalValueBySqrt(dX, size) < eps) break;

		iterations++;
	}
	delete[] X0;
	delete[] dX;
}

//Using Doolittle's Method
void SolveSystemByLU(double* A, double* B, double* X, size_t size)
{
	double* L = new double[size * size];
	memset(L, 0, sizeof(double) * size * size);
	double* U = new double[size * size];
	memset(U, 0, sizeof(double) * size * size);
	memset(X, 0, sizeof(double) * size);

	for (int i = 0; i < size; i++)
	{
		L[i * size + i] = 1;
	}

	// L - U - L - U - ...
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < i; j++)
		{
			double alpha = A[i * size + j];
			for (int k = 0; k < j; k++)
			{
				alpha -= L[i * size + k] * U[k * size + j];
			}
			L[i * size + j] = alpha / U[j * size + j];
		}

		for (int j = i; j < size; j++)
		{
			double alpha = A[i * size + j];
			for (int k = 0; k < i; k++)
			{
				alpha -= L[i * size + k] * U[k * size + j];
			}
			U[i * size + j] = alpha;
		}
	}

	// solve for L
	for (int i = 0; i < size; i++)
	{
		double alpha = 0;
		for (int k = 0; k <= i; k++)
		{
			alpha += L[i * size + k] * X[k];
		}
		X[i] = B[i] - alpha;
	}

	// solve for U
	for (int i = size - 1; i >= 0; i--)
	{
		double alpha = 0;
		for (int k = i + 1; k < size; k++)
		{
			alpha += U[i * size + k] * X[k];
		}
		X[i] = (X[i] - alpha) / U[i * size + i];
	}

	delete[] L;
	delete[] U;
}

void NewtonMethod(function* f, function* df, double* X, int size, double eps)
{
	double* J = new double[size * size]; //Jacobi matrix
	double* dX = new double[size];
	double* B = new double[size];
	for (int i = 0; i < size * size; i++)
	{
		J[i] = 0.0;
	}
	for (int i = 0; i < size; i++)
	{
		X[i] = 1.0;
		B[i] = 0.0;
	}
	PrintMatrix(X, 1, size);

	int iterations = 0;
	while (iterations < 50)
	{
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				J[y * size + x] = df[y * size + x](X, size);
				//J[y * size + x] = d_dx(f[y], X, size, x); //doesn't work(
			}
			B[y] = -f[y](X, size);
		}

		SolveSystemByLU(J, B, dX, size);

		if (NormalValueBySqrt(dX, size) < eps) break;

		for (int y = 0; y < size; y++)
		{
			X[y] += dX[y];
		}
		PrintMatrix(X, 1, size);
		iterations++;
	}
	delete[] J;
	delete[] B;
	delete[] dX;
}

int main()
{
	function system[2] = { f1, f2 };
	function phis[2] = { phi1, phi2 };
	function derivatives[2 * 2] =
	{
		f1d1, f1d2,
		f2d1, f2d2
	};
	double X[2] = { 0 };
	std::cout << "Iteration method:\n";
	IterationMethod(phis, X, 2, 0.0001);
	std::cout << "---------------------------------------\n";
	std::cout << "Newton method:\n";
	NewtonMethod(system, derivatives, X, 2, 0.0001);
	return 0;
}