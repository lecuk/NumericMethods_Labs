#include <iostream>
#include <iomanip>

typedef double Matrix4x4[4][4];
typedef double Vector4[4];
typedef double Matrix3x3[3][3];
typedef double Vector3[3];

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

double NormalValueByRows(Matrix4x4 A)
{
	double maxSum = 0;
	for (int y = 0; y < 4; y++)
	{
		double sum = 0;
		for (int x = 0; x < 4; x++)
		{
			sum += fabs(A[y][x]);
		}
		if (sum > maxSum) maxSum = sum;
	}
	return maxSum;
}

double NormalValueByColumns(Matrix4x4 A)
{
	double maxSum = 0;
	for (int x = 0; x < 4; x++)
	{
		double sum = 0;
		for (int y = 0; y < 4; y++)
		{
			sum += fabs(A[y][x]);
		}
		if (sum > maxSum) maxSum = sum;
	}
	return maxSum;
}

double NormalValueBySqrt(Matrix4x4 A)
{
	double sum = 0;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			sum += A[y][x] * A[y][x];
		}
	}
	return sqrt(sum);
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

bool IsMatrixConvergent(Matrix4x4 A)
{
	double normalByRows = NormalValueByRows(A);
	double normalByCols = NormalValueByColumns(A);
	return (normalByRows < 1.0 || normalByCols < 1.0);
}

double NormalValueByRows(Matrix3x3 A)
{
	double maxSum = 0;
	for (int y = 0; y < 3; y++)
	{
		double sum = 0;
		for (int x = 0; x < 3; x++)
		{
			sum += fabs(A[y][x]);
		}
		if (sum > maxSum) maxSum = sum;
	}
	return maxSum;
}

double NormalValueByColumns(Matrix3x3 A)
{
	double maxSum = 0;
	for (int x = 0; x < 3; x++)
	{
		double sum = 0;
		for (int y = 0; y < 3; y++)
		{
			sum += fabs(A[y][x]);
		}
		if (sum > maxSum) maxSum = sum;
	}
	return maxSum;
}

double NormalValueBySqrt(Matrix3x3 A)
{
	double sum = 0;
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			sum += A[y][x] * A[y][x];
		}
	}
	return sqrt(sum);
}

bool IsMatrixConvergent(Matrix3x3 A)
{
	double normalByRows = NormalValueByRows(A);
	double normalByCols = NormalValueByColumns(A);
	return (normalByRows < 1.0 || normalByCols < 1.0);
}

size_t JacobiMethod(Matrix4x4 A, Vector4 B, Vector4 X[], size_t maxIterations, double eps)
{
	if (!IsMatrixConvergent(A))
	{
		std::cout << "Matrix is not convergent.\n";
		return 0;
	}

	std::cout << "Starting X:   ";
	for (int y = 0; y < 4; y++)
	{
		X[0][y] = B[y];
		std::cout << std::setw(10) << X[0][y] << " ";
	}
	std::cout << "\n\n";
	for (int i = 1; i < maxIterations; i++)
	{
		std::cout << "Iteration " << std::setw(2) << i << ": ";
		for (int y = 0; y < 4; y++)
		{
			double newX = 0;
			for (int x = 0; x < 4; x++)
			{
				newX += A[y][x] * X[i - 1][x];
			}
			X[i][y] = newX + B[y];
			std::cout << std::setw(10) << X[i][y] << " ";
		}
		std::cout << "\n";

		Vector4 delta =
		{
			X[i][0] - X[i - 1][0],
			X[i][1] - X[i - 1][1],
			X[i][2] - X[i - 1][2],
			X[i][3] - X[i - 1][3]
		};

		double normal = NormalValueBySqrt(delta, 4);
		if (normal < eps)
		{
			std::cout << "Delta = " << normal << " < " << eps << "\n";
			return i;
		}
		else
		{
			std::cout << "Delta = " << normal << " >= " << eps << "\n";
		}
		std::cout << "\n";
	}
	return maxIterations - 1;
}

void ConvertToAlphaBetaForm(Matrix3x3 A, Vector3 B)
{
	for (int y = 0; y < 3; y++)
	{
		double coefficient = A[y][y];
		for (int x = 0; x < 3; x++)
		{
			if (y == x)
			{
				A[y][x] = 0;
			}
			else
			{
				A[y][x] /= -coefficient;
			}
		}
		B[y] /= coefficient;
	}
}

size_t SeidelMethod(Matrix3x3 A, Vector3 B, Vector3 X[], size_t maxIterations, double eps)
{
	if (!IsMatrixConvergent(A))
	{
		std::cout << "Matrix is not convergent.\n";
		//return 0;
	}

	std::cout << "Starting X:   ";
	for (int y = 0; y < 3; y++)
	{
		X[0][y] = B[y];
		std::cout << std::setw(10) << X[0][y] << " ";
	}
	std::cout << "\n\n";
	for (int i = 1; i < maxIterations; i++)
	{
		std::cout << "Iteration " << std::setw(2) << i << ": ";
		for (int y = 0; y < 3; y++)
		{
			double newX = 0;
			for (int x = 0; x < y; x++)
			{
				newX += A[y][x] * X[i][x];
			}
			for (int x = y; x < 3; x++)
			{
				newX += A[y][x] * X[i - 1][x];
			}
			X[i][y] = newX + B[y];
			std::cout << std::setw(10) << X[i][y] << " ";
		}
		std::cout << "\n";

		Vector3 delta =
		{
			X[i][0] - X[i - 1][0],
			X[i][1] - X[i - 1][1],
			X[i][2] - X[i - 1][2]
		};

		double normal = NormalValueBySqrt(delta, 3);
		if (normal < eps)
		{
			std::cout << "Delta = " << normal << " < " << eps << "\n";
			return i;
		}
		else
		{
			std::cout << "Delta = " << normal << " >= " << eps << "\n";
		}
		std::cout << "\n";
	}
	return maxIterations - 1;
}

size_t SeidelMethod(Matrix4x4 A, Vector4 B, Vector4 X[], size_t maxIterations, double eps)
{
	if (!IsMatrixConvergent(A))
	{
		std::cout << "Matrix is not convergent.\n";
		//return 0;
	}

	std::cout << "Starting X:   ";
	for (int y = 0; y < 4; y++)
	{
		X[0][y] = B[y];
		std::cout << std::setw(10) << X[0][y] << " ";
	}
	std::cout << "\n\n";
	for (int i = 1; i < maxIterations; i++)
	{
		std::cout << "Iteration " << std::setw(2) << i << ": ";
		for (int y = 0; y < 4; y++)
		{
			double newX = 0;
			for (int x = 0; x < y; x++)
			{
				newX += A[y][x] * X[i][x];
			}
			for (int x = y; x < 4; x++)
			{
				newX += A[y][x] * X[i - 1][x];
			}
			X[i][y] = newX + B[y];
			std::cout << std::setw(10) << X[i][y] << " ";
		}
		std::cout << "\n";

		Vector4 delta =
		{
			X[i][0] - X[i - 1][0],
			X[i][1] - X[i - 1][1],
			X[i][2] - X[i - 1][2],
			X[i][3] - X[i - 1][3]
		};

		double normal = NormalValueBySqrt(delta, 4);
		if (normal < eps)
		{
			std::cout << "Delta = " << normal << " < " << eps << "\n";
			return i;
		}
		else
		{
			std::cout << "Delta = " << normal << " >= " << eps << "\n";
		}
		std::cout << "\n";
	}
	return maxIterations - 1;
}

int main()
{ 
	//x1 =  0.10*x1 - 0.07*x2 + 0.38*x3 - 0.21*x4 - 0.81
	//x2 = -0.22*x1 + 0.08*x2 + 0.11*x3 + 0.33*x4 - 0.64
	//x3 =  0.51*x1 - 0.07*x2 + 0.09*x3 - 0.11*x4 + 1.71
	//x4 =  0.33*x1 - 0.41*x2                     - 1.21
	Matrix4x4 A = 
	{
		+0.10, -0.07, +0.38, -0.21,
		-0.22, +0.08, +0.11, +0.33, 
		+0.51, -0.07, +0.09, -0.11,
		+0.33, -0.41, +0.00, +0.00
	};
	Vector4 B =
	{
		-0.81,
		-0.64,
		+1.71,
		-1.21
	};
	Vector4 X[40] = { 0 };
	Matrix3x3 A2 = 
	{
		+1, +2, +3,
		+2, -1, -2,
		+3, +2, -1
	};
	Matrix3x3 A3 =
	{
		+1.84, +2.25, -2.53,
		+2.32, +2.60, -2.82,
		+1.83, +2.06, +2.24
	};
	Vector3 B2 =
	{
		+1,
		+2,
		-5
	};
	Vector3 B3 =
	{
		-6.09,
		-6.98,
		-5.52
	};
	Vector3 X2[40] = { 0 };

	std::cout << "Jacobi method: \n";
	size_t jacobiIterations = JacobiMethod(A, B, X, 40, 0.001);
	std::cout << "---------------------------------------------------------------\n";
	std::cout << "Seidel method: \n";
	size_t seidelIterations = SeidelMethod(A, B, X, 40, 0.001);

	/*
	std::cout << "A and B matrices: \n";
	PrintMatrix(*A2, 3, 3);
	PrintMatrix(B2, 1, 3);
	ConvertToAlphaBetaForm(A2, B2);
	std::cout << "Alpha and Beta matrices: \n";
	PrintMatrix(*A2, 3, 3);
	PrintMatrix(B2, 1, 3);
	size_t seidelIterations = SeidelMethod(A2, B2, X2, 40, 0.001);
	*/

	std::cout << "Press ENTER to exit\n";
	std::getchar();
	return 0;
}
