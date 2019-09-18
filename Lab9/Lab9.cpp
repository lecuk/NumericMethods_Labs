#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

class Polynomial
{
private:
	std::vector<double> coefficients;

	Polynomial(int power = 0, double value = 0)
	{
		if (power < 0) throw 0;

		coefficients = std::vector<double>(power, value);
	}

public:
	static Polynomial Zeroes(int _power = 0)
	{
		return Polynomial(_power, 0.0);
	}

	static Polynomial Ones(int _power = 0)
	{
		return Polynomial(_power, 1.0);
	}

	static Polynomial One()
	{
		return Polynomial(0, 1.0);
	}

	Polynomial(int _power, double* _coefficients)
	{
		if (_power < 0) throw 0;

		coefficients = std::vector<double>(_power);
		for (int i = _power; i >= 0; i--)
		{
			coefficients[i] = _coefficients[i];
		}
	}

	Polynomial(const Polynomial & original)
	{
		coefficients = original.coefficients;
	}

	Polynomial operator = (const Polynomial & original)
	{
		coefficients = original.coefficients;
		return *this;
	}

	~Polynomial()
	{
		coefficients.clear();
	}

	double operator [] (int i)
	{
		return coefficients[i];
	}

	int Power()
	{
		for (int i = coefficients.size() - 1; i >= 0; --i)
		{
			if (coefficients[i] != 0) return i;
		}
		return 0;
	}

	double CalculateValue(double x)
	{
		double polyX = 1;
		double y = 0;
		for (int i = 0; i <= Power(); i++)
		{
			y += coefficients[i] * polyX;
			polyX *= x;
		}
		return y;
	}

	std::string ToString()
	{
		std::stringstream stream = std::stringstream();
		bool notZeroItems = false;

		int power = Power();
		if (power >= 2)
		{
			if (coefficients[power] != 0)
			{
				if (coefficients[power] == 1.0)
				{
					stream << std::setprecision(4) << "x^" << power;
				}
				else if (coefficients[power] == -1.0)
				{
					stream << std::setprecision(4) << "-x^" << power;
				}
				else
				{
					stream << std::setprecision(4) << coefficients[power] << "x^" << power;
				}
				notZeroItems = true;
			}

			for (int i = power - 1; i >= 2; i--)
			{
				if (notZeroItems)
				{
					if (coefficients[i] > 0)
					{
						if (coefficients[i] == 1.0)
						{
							stream << " + " << std::setprecision(4) << "x^" << i;
						}
						else
						{
							stream << " + " << std::setprecision(4) << coefficients[i] << "x^" << i;
						}
					}
					else if (coefficients[i] < 0)
					{
						if (coefficients[i] == -1.0)
						{
							stream << " - " << std::setprecision(4) << "x^" << i;
						}
						else
						{
							stream << " - " << std::setprecision(4) << -coefficients[i] << "x^" << i;
						}
					}
				}
				else
				{
					if (coefficients[i] != 0)
					{
						if (coefficients[i] == 1.0)
						{
							stream << std::setprecision(4) << "x^" << i;
						}
						else if (coefficients[i] == -1.0)
						{
							stream << std::setprecision(4) << "-x^" << i;
						}
						else
						{
							stream << std::setprecision(4) << coefficients[i] << "x^" << i;
						}
						notZeroItems = true;
					}
				}
			}
		}

		if (power >= 1)
		{
			if (notZeroItems)
			{
				if (coefficients[1] > 0)
				{
					if (coefficients[1] == 1.0)
					{
						stream << " + " << std::setprecision(4) << "x";
					}
					else
					{
						stream << " + " << std::setprecision(4) << coefficients[1] << "x";
					}
				}
				else if (coefficients[1] < 0)
				{
					if (coefficients[1] == -1.0)
					{
						stream << " - " << std::setprecision(4) << "x";
					}
					else
					{
						stream << " - " << std::setprecision(4) << -coefficients[1] << "x";
					}
				}
			}
			else
			{
				if (coefficients[1] != 0)
				{
					if (coefficients[1] == 1.0)
					{
						stream << std::setprecision(4) << "x";
					}
					else if (coefficients[1] == -1.0)
					{
						stream << std::setprecision(4) << "-x";
					}
					else
					{
						stream << std::setprecision(4) << coefficients[1] << "x";
					}
					notZeroItems = true;
				}
			}
		}

		if (notZeroItems)
		{
			if (coefficients[0] > 0)
			{
				stream << " + " << std::setprecision(4) << coefficients[0];
			}
			else if (coefficients[0] < 0)
			{
				stream << " - " << std::setprecision(4) << -coefficients[0];
			}
		}
		else
		{
			if (coefficients[0] != 0)
			{
				stream << std::setprecision(4) << coefficients[0];
				notZeroItems = true;
			}
		}

		if (!notZeroItems) stream << 0;

		return stream.str();
	}

	friend Polynomial operator + (Polynomial A, Polynomial B)
	{
		//make A >= B
		if (A.power < B.power)
		{
			Polynomial temp = A;
			A = B;
			B = temp;
		}

		Polynomial result = Polynomial(A);

		for (int i = B.power; i >= 0; i--)
		{
			result[i] += B[i];
		}

		return result;
	}

	friend Polynomial operator += (Polynomial& A, Polynomial B)
	{
		if (A.power < B.power) A.resize(B.power);

		for (int i = B.power; i >= 0; i--)
		{
			A[i] += B[i];
		}

		return A;
	}

	friend Polynomial operator - (Polynomial A, Polynomial B)
	{
		//make A >= B
		if (A.power < B.power)
		{
			Polynomial temp = A;
			A = B;
			B = temp;
		}

		Polynomial result = A;

		for (int i = B.power; i >= 0; i--)
		{
			result[i] -= B[i];
		}

		return result;
	}

	friend Polynomial operator -= (Polynomial& A, Polynomial B)
	{
		if (A.power < B.power) A.resize(B.power);

		for (int i = B.power; i >= 0; i--)
		{
			A[i] -= B[i];
		}

		return A;
	}

	friend Polynomial operator * (Polynomial A, Polynomial B)
	{
		int power = A.power + B.power;
		Polynomial result = Polynomial::Zeroes(power);

		for (int i = A.power; i >= 0; i--)
		{
			for (int j = B.power; j >= 0; j--)
			{
				result[i + j] += A[i] * B[j];
			}
		}

		return result;
	}

	friend Polynomial operator *= (Polynomial& A, Polynomial B)
	{
		int power = A.power + B.power;
		Polynomial temp = Polynomial::Zeroes(power);

		for (int i = A.power; i >= 0; i--)
		{
			for (int j = B.power; j >= 0; j--)
			{
				temp[i + j] += A[i] * B[j];
			}
		}

		A = Polynomial(temp);
		return A;
	}

	friend Polynomial operator * (Polynomial A, double multiplier)
	{
		Polynomial result = Polynomial(A);

		for (int i = A.power; i >= 0; i--)
		{
			result[i] *= multiplier;
		}

		return result;
	}

	friend Polynomial operator / (Polynomial A, double divisor)
	{
		Polynomial result = Polynomial(A);

		for (int i = A.power; i >= 0; i--)
		{
			result[i] /= divisor;
		}

		return result;
	}

	friend Polynomial operator *= (Polynomial& A, double multiplier)
	{
		for (int i = A.power; i >= 0; i--)
		{
			A[i] *= multiplier;
		}

		return A;
	}

	friend Polynomial operator /= (Polynomial& A, double divisor)
	{
		for (int i = A.power; i >= 0; i--)
		{
			A[i] /= divisor;
		}

		return A;
	}
};

void PrintMatrix(double* matrix, size_t w, size_t h)
{
	for (int y = 0; y < h; y++)
	{
		std::cout << "| ";
		for (int x = 0; x < w; x++)
		{
			std::cout << std::setw(12) << matrix[y * w + x] << " ";
		}
		std::cout << "|\n";
	}
}

void SolveByLUMethod(double* A, double* B, double* X, size_t n)
{
	double* L = new double[n * n];
	double* U = new double[n * n];

	//std::cout << "A matrix: \n";
	//PrintMatrix(A, n, n);

	//std::cout << "B matrix: \n";
	//PrintMatrix(B, 1, n);

	for (int i = 0; i < n * n; i++)
	{
		L[i] = 0;
		U[i] = 0;
	}

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

	delete[] L;
	delete[] U;
}

Polynomial CreateSqrtPolynomial(int power, int count, double* X, double* Y)
{
	int size = power + 1;
	int sumCount = size * 2 - 1;
	double* sumsA = new double[sumCount];
	for (int i = 0; i < sumCount; i++)
	{
		sumsA[i] = 0;
	}

	double* B = new double[size];
	for (int i = 0; i < size; i++)
	{
		B[i] = 0;
	}

	double* copyX = new double[count];
	for (int i = 0; i < count; i++)
	{
		copyX[i] = 1;
	}

	for (int i = 0; i < sumCount; i++)
	{
		for (int j = 0; j < count; j++)
		{
			sumsA[i] += copyX[j];
		}

		if (i < size)
		{
			for (int j = 0; j < count; j++)
			{
				B[i] += Y[j] * copyX[j];
			}
		}

		for (int j = 0; j < count; j++)
		{
			copyX[j] *= X[j];
		}
	}

	double* A = new double[size * size];

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			A[y * size + x] = sumsA[y + x];
		}
	}

	std::cout << "A:\n";
	PrintMatrix(A, size, size);

	std::cout << "B:\n";
	PrintMatrix(B, 1, size);

	double* coeffs = new double[size];
	SolveByLUMethod(A, B, coeffs, size);

	Polynomial poly = Polynomial(power, coeffs);

	delete[] sumsA;
	delete[] A;
	delete[] B;
	delete[] copyX;
	delete[] coeffs;

	return poly;
}

int main()
{
	double X1[] = { -3, -1, 0, 1, 3 };
	double Y1[] = { -4, -0.8, 1.6, 2.3, 1.5 };

	double X[] = { 4.03, 4.08, 4.16, 4.23, 4.26, 4.33 };
	double Y[] = { 2.8, 2.94, 3.2, 3.38, 3.53, 3.75 };

	std::cout << "|" << std::setw(12) << "X" << "|" << std::setw(12) << "Y" << "|\n";
	for (int i = 0; i < 5; i++)
	{
		std::cout << "|" << std::setw(12) << X1[i] << "|" << std::setw(12) << Y1[i] << "|\n";
	}
	std::cout << "\n1st power: \n";
	std::cout << CreateSqrtPolynomial(1, 5, X1, Y1).ToString() << "\n";
	std::cout << "\n2nd power: \n";
	std::cout << CreateSqrtPolynomial(2, 5, X1, Y1).ToString() << "\n";
	std::cout << "\n3rd power: \n";
	std::cout << CreateSqrtPolynomial(3, 5, X1, Y1).ToString() << "\n";

	return 0;
}