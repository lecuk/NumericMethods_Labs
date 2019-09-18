#include <iostream>
#include <iomanip>

typedef double Matrix3x3[3][3];
typedef double Vector3[3];

#define DEBUG

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

double* TransponateMatrix(const double* A, size_t w, size_t h)
{
	double* result = new double[w * h];
	for (size_t y = 0; y < h; y++)
	{
		for (size_t x = 0; x < w; x++)
		{
			result[x * h + y] = A[y * w + x];
		}
	}
	return result;
}

double* MultiplyMatrix(const double* A, const double* B, size_t w1, size_t h1, size_t w2, size_t h2)
{
	if (w1 != h2) return NULL;

	double* result = new double[w2 * h1];

	for (size_t y1 = 0; y1 < h1; y1++)
	{
		for (size_t x1 = 0; x1 < w2; x1++)
		{
			result[y1 * w2 + x1] = 0.0;
		}
	}

	for (size_t y1 = 0; y1 < h1; y1++)
	{
		for (size_t x2 = 0; x2 < w2; x2++)
		{
			for (size_t i = 0; i < w1; i++)
			{
				result[y1 * w2 + x2] += A[y1 * w1 + i] * B[i * w2 + x2];
			}
		}
	}

	return result;
}

void ConvertToNormalForm(const double* A, const double* B, size_t w, size_t h, double* resultA, double* resultB)
{
	double* AT = TransponateMatrix(A, w, h);
	double* AT_A = MultiplyMatrix(AT, A, h, w, w, h);
	double* AT_B = MultiplyMatrix(AT, B, h, w, 1, h);
	for (size_t y = 0; y < w; y++)
	{
		resultB[y] = AT_B[y];
		for (size_t x = 0; x < w; x++)
		{
			resultA[y * w + x] = AT_A[y * w + x];
		}
	}
	delete[] AT;
	delete[] AT_A;
	delete[] AT_B;
}

void SolveBySquareRootMethod(const double* A, const double* B, double* X, size_t n)
{
	double* L = new double[n * n];
	for (int i = 0; i < n * n; i++)
	{
		L[i] = 0;
	}

	for (int y = 0; y < n; y++)
	{
		for (int x = 0; x < y; x++)
		{
			double sum = 0;
			for (int k = 0; k < y; k++)
			{
				sum += L[y * n + k] * L[x * n + k];
			}
			L[y * n + x] = (A[y * n + x] - sum) / L[x * n + x];
		}

		double sum = 0;
		for (int k = 0; k < y; k++)
		{
			sum += L[y * n + k] * L[y * n + k];
		}
		L[y * n + y] = sqrt(A[y * n + y] - sum);
	}

	double* L_T = TransponateMatrix(L, n, n);

#ifdef DEBUG
	std::cout << "Matrix L:\n";
	PrintMatrix(L, n, n);
	std::cout << "\n";

	std::cout << "Matrix L_T:\n";
	PrintMatrix(L_T, n, n);
	std::cout << "\n";
#endif

	double* Y = new double[n];
	for (int i = 0; i < n; i++)
	{
		Y[i] = 0;
	}

	// solve for L
	for (int y = 0; y < n; y++)
	{
		double sum = 0;
		for (int x = 0; x < y; x++)
		{
			sum += L[y * n + x] * Y[x];
		}
		Y[y] = (B[y] - sum) / L[y * n + y];
	}

	// solve for L_T
	for (int y = n - 1; y >= 0; y--)
	{
		double sum = 0;
		for (int x = y + 1; x < n; x++)
		{
			sum += L_T[y * n + x] * X[x];
		}
		X[y] = (Y[y] - sum) / L_T[y * n + y];
	}

	delete[] L;
	delete[] L_T;
	delete[] Y;
}

void LUMethod(double* A, double* B, double* X, size_t n)
{
	double* L = new double[n * n];
	double* U = new double[n * n];

	for (int i = 0; i < n * n; i++)
	{
		L[i] = 0;
		U[i] = 0;
	}

	std::cout << "A matrix: \n";
	std::cout << "--------------------------------------\n";
	PrintMatrix(A, n, n);

	for (int i = 0; i < n; i++)
	{
		X[i] = 0;
		L[i * n + i] = 1;
	}

	// L - U - L - U - ...
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			double alpha = A[i * n + j];
			for (int k = 0; k < j; k++)
			{
				alpha -= L[i * n + k] * U[k * n + j];
			}
			L[i * n + j] = alpha / U[j * n + j];
		}

		for (int j = i; j < n; j++)
		{
			double alpha = A[i * n + j];
			for (int k = 0; k < i; k++)
			{
				alpha -= L[i * n + k] * U[k * n + j];
			}
			U[i * n + j] = alpha;
		}
	}

	// solve for L
	for (int i = 0; i < n; i++)
	{
		double alpha = 0;
		for (int k = 0; k <= i; k++)
		{
			alpha += L[i * n + k] * X[k];
		}
		X[i] = B[i] - alpha;
	}

	// solve for U
	for (int i = n - 1; i >= 0; i--)
	{
		double alpha = 0;
		for (int k = i + 1; k < n; k++)
		{
			alpha += U[i * n + k] * X[k];
		}
		X[i] = (X[i] - alpha) / U[i * n + i];
	}

	std::cout << "--------------------------------------\n\n";
	std::cout << "L matrix: \n";
	std::cout << "--------------------------------------\n";
	PrintMatrix(L, n, n);
	std::cout << "--------------------------------------\n\n";
	std::cout << "U matrix: \n";
	std::cout << "--------------------------------------\n";
	PrintMatrix(U, n, n);
	std::cout << "--------------------------------------\n\n";

	delete[] L;
	delete[] U;
}

int main()
{
	double A[6 * 3] = 
	{
		+1, +1, -1,
		+2, +2, +2,
		+1, -1, +3,
		+3, +1, -1,
		+1, +2, +3,
		+4, -1, +1
	};
	double A2[6 * 3] =
	{
		1, 0, 0,
		1, 1, 0,
		1, 1, 1,
		0, 1, 0,
		0, 1, 1,
		0, 0, 1
	};

	double B[6] =
	{
		+8,
		-3,
		+5,
		+6,
		+4,
		-1
	};
	double B2[6] =
	{
		+1.4,
		-1.5,
		+3.2,
		+0.6,
		+4.3,
		+4.2
	};

	double N[3 * 3],  C[3],  X[3];
	double N2[3 * 3], C2[3], X2[3];
	ConvertToNormalForm(A, B, 3, 6, N, C);
	//ConvertToNormalForm(A2, B2, 3, 6, N2, C2);

	std::cout << "Matrix A:\n";
	PrintMatrix(A, 3, 6);
	std::cout << "\n";

	std::cout << "Matrix B:\n";
	PrintMatrix(B, 1, 6);
	std::cout << "\n";

	std::cout << "Matrix N:\n";
	PrintMatrix(N, 3, 3);
	std::cout << "\n";

	std::cout << "Matrix C:\n";
	PrintMatrix(C, 1, 3);
	std::cout << "\n";

	SolveBySquareRootMethod(N, C, X, 3);

	std::cout << "Roots:\n";
	PrintMatrix(X, 1, 3);
	std::cout << "\n";
}