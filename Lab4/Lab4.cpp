#include <iostream>
#include <iomanip>

void swapRow(double* matrix, size_t w, size_t h, size_t y1, size_t y2)
{
	for (int x = 0; x < w; x++)
	{
		double temp = matrix[y1 * w + x];
		matrix[y1 * w + x] = matrix[y2 * w + x];
		matrix[y2 * w + x] = temp;
	}
}

void PrintMatrix(double* matrix, size_t w, size_t h)
{
	for (int y = 0; y < h; y++)
	{
		std::cout << "| ";
		for (int x = 0; x < w; x++)
		{
			std::cout << std::setw(10) << matrix[y * w + x] << " ";
		}
		std::cout << "|\n";
	}
}

void GaussMethod(double* A, double* B, double* X, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		double max = 0;
		size_t maxI = i;
		for (int y = i; y < size; y++)
		{
			if (fabs(A[y * size + i]) > max)
			{
				max = fabs(A[y * size + i]);
				maxI = y;
			}
		}

		swapRow(A, size, size, i, maxI);

		for (int y = i + 1; y < size; y++)
		{
			double multiplier = A[y * size + i] / A[i * size + i];
			for (int x = 0; x < size; x++)
			{
				A[y * size + x] -= A[i * size + x] * multiplier;
			}
			B[y] -= B[i] * multiplier;
			std::cout << "i = " << i << ", y = " << y << ".\n";
			PrintMatrix(A, size, size);
			PrintMatrix(B, 1, size);
		}
	}

	for (int y = size - 1; y >= 0; y--)
	{
		double sum = B[y];
		std::cout << "Free coeff[" << y << "] = " << sum << "\n";
		for (int x = size - 1; x > y; x--)
		{
			sum -= X[x] * A[y * size + x];
			std::cout << "Roots[" << x << "] = " << X[x] << ", Coeff[" << x << "] = " << A[y * size + x] << "\n";
		}

		X[y] = sum / A[y * size + y];
		std::cout << std::setw(10) << sum << " / " << A[y * size + y] << " = " << X[y] << "\n";
	}
}

//Using Doolittle's Method
void LUMethod(double* A, double* B, double* X, size_t size)
{
	double* L = new double[size * size];
	memset(L, 0, sizeof(double) * size * size);
	double* U = new double[size * size];
	memset(U, 0, sizeof(double) * size * size);
	memset(X, 0, sizeof(double) * size);

	std::cout << "A matrix: \n";
	std::cout << "--------------------------------------\n";
	PrintMatrix(A, size, size);

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

	std::cout << "--------------------------------------\n\n";
	std::cout << "L matrix: \n";
	std::cout << "--------------------------------------\n";
	PrintMatrix(L, size, size);
	std::cout << "--------------------------------------\n\n";
	std::cout << "U matrix: \n";
	std::cout << "--------------------------------------\n";
	PrintMatrix(U, size, size);
	std::cout << "--------------------------------------\n\n";

	delete[] L;
	delete[] U;
}

//not working yet.
void TridiagonalMethod(double* A, double* B, double* X, size_t size)
{
	double *M_alpha = new double[size - 1];
	double *M_beta = new double[size];

	int i = 0;
	double divisor = 1;
	M_alpha[i] = -A[i * size + 1] / A[i * size + 0];
	M_beta[i] = B[0] / A[i * size + 0];
	for (i = 1; i < size - 1; ++i)
	{
		divisor = (A[i * size + i] + A[i * size + i - 1] * M_alpha[i - 1]);
		M_alpha[i] = -A[i * size + i + 1] / divisor;
		M_beta[i] = (B[i] - A[i * size + i] * M_beta[i - 1]) / divisor;
	}
	i = size - 1;
	divisor = (A[i * size + i] + A[i * size + i - 1] * M_alpha[i - 1]);
	M_beta[i] = (B[i] - A[i * size + i] * M_beta[i - 1]) / divisor;

	X[i] = M_beta[i];
	for (i = i - 1; i >= 0; --i)
	{
		X[i] = M_alpha[i] * X[i + 1] + M_beta[i];
	}

	delete[] M_alpha;
	delete[] M_beta;
}

int main()
{
	double A[3 * 3] =
	{
		+0.13, -0.14, -2.00,
		+0.75, +0.18, -0.77,
		+0.28, -0.17, +0.39
	};
	double B[3] =
	{
		 +0.15,
		 +0.11,
		 +0.12
	};
	double X1[3] = { 0 };
	double X2[3] = { 0 };
	double TA[4 * 4] =
	{
		+5, +3, +0, +0,
		+3, +6, +1, +0,
		+0, +1, +4, -2,
		+0, +0, +1, -3
	};
	double TB[8] =
	{
		+8,
		+10,
		+3,
		-2
	};
	double X3[4] = { 0 };

	GaussMethod(A, B, X1, 3);
	std::cout << "x1 = " << X1[0] << "\n";
	std::cout << "x2 = " << X1[1] << "\n";
	std::cout << "x3 = " << X1[2] << "\n";
	std::cout << "c1 = " << X1[0] * A[0 * 3 + 0] + X1[1] * A[0 * 3 + 1] + X1[2] * A[0 * 3 + 2] << " == " << B[0] << "\n";
	std::cout << "c2 = " << X1[0] * A[1 * 3 + 0] + X1[1] * A[1 * 3 + 1] + X1[2] * A[1 * 3 + 2] << " == " << B[1] << "\n";
	std::cout << "c3 = " << X1[0] * A[2 * 3 + 0] + X1[1] * A[2 * 3 + 1] + X1[2] * A[2 * 3 + 2] << " == " << B[2] << "\n";

	LUMethod(A, B, X2, 3);
	std::cout << "x1 = " << X2[0] << "\n";
	std::cout << "x2 = " << X2[1] << "\n";
	std::cout << "x3 = " << X2[2] << "\n";
	std::cout << "c1 = " << X2[0] * A[0 * 3 + 0] + X2[1] * A[0 * 3 + 1] + X2[2] * A[0 * 3 + 2] << " == " << B[0] << "\n";
	std::cout << "c2 = " << X2[0] * A[1 * 3 + 0] + X2[1] * A[1 * 3 + 1] + X2[2] * A[1 * 3 + 2] << " == " << B[1] << "\n";
	std::cout << "c3 = " << X2[0] * A[2 * 3 + 0] + X2[1] * A[2 * 3 + 1] + X2[2] * A[2 * 3 + 2] << " == " << B[2] << "\n";

	TridiagonalMethod(TA, TB, X3, 3);
	std::cout << "x1 = " << X3[0] << "\n";
	std::cout << "x2 = " << X3[1] << "\n";
	std::cout << "x3 = " << X3[2] << "\n";
	std::cout << "x3 = " << X3[3] << "\n";
	
	std::cout << "Press ENTER to exit\n";
	std::getchar();
	return 0;
}
