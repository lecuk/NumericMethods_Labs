#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

class Polynomial
{
private:
	double* coefficients;
	int power;

	void resize(int newPower)
	{
		double* temp = new double[power + 1];
		for (int i = 0; i <= power; i++)
		{
			temp[i] = coefficients[i];
		}

		if (coefficients) delete[] coefficients;
		coefficients = new double[newPower + 1];

		if (newPower > power)
		{
			for (int i = 0; i <= power; i++)
			{
				coefficients[i] = temp[i];
			}

			for (int i = power + 1; i <= newPower; i++)
			{
				coefficients[i] = 0;
			}
		}
		else
		{
			for (int i = 0; i <= newPower; i++)
			{
				coefficients[i] = temp[i];
			}
		}

		delete[] temp;
		power = newPower;
	}

	void shorten()
	{
		int truePower = power;
		for (truePower; truePower >= 0; truePower--)
		{
			if (coefficients[truePower] != 0)
			{
				break;
			}
		}

		if (truePower < power)
		{
			resize(truePower);
		}
	}

	Polynomial(int _power = 0, double value = 0)
	{
		if (_power < 0) throw 0;

		power = _power;
		coefficients = new double[power + 1];
		for (int i = power; i >= 0; i--)
		{
			coefficients[i] = value;
		}
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

		power = _power;
		coefficients = new double[power + 1];
		for (int i = power; i >= 0; i--)
		{
			coefficients[i] = _coefficients[i];
		}
	}

	Polynomial(const Polynomial & original)
	{
		power = original.power;
		coefficients = new double[power + 1];
		for (int i = power; i >= 0; i--)
		{
			coefficients[i] = original.coefficients[i];
		}
	}

	Polynomial operator = (const Polynomial & original)
	{
		resize(original.power);
		for (int i = power; i >= 0; i--)
		{
			coefficients[i] = original.coefficients[i];
		}
		return *this;
	}

	~Polynomial()
	{
		delete[] coefficients;
	}

	double& operator [] (int i)
	{
		return coefficients[i];
	}

	int Power()
	{
		shorten();
		return power;
	}

	double CalculateValue(double x)
	{
		double polyX = 1;
		double y = 0;
		for (int i = 0; i <= power; i++)
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

Polynomial GetDerivative(Polynomial& P)
{
	Polynomial Pd = Polynomial(P);
	for (int i = 0; i < Pd.Power(); i++)
	{
		Pd[i] = Pd[i + 1];
		Pd[i] *= i + 1;
		Pd[i + 1] = 0.0;
	}
	
	return Pd;
}

Polynomial CreateLagrangePolynomial(int count, double* X, double* Y)
{
	Polynomial P = Polynomial::Zeroes();
	for (int i = 0; i < count; i++)
	{
		Polynomial Pi = Polynomial::One();

		for (int j = 0; j < count; j++)
		{
			if (i != j)
			{
				double pair[2] = { -X[j], 1 };
				Polynomial Pj = Polynomial(1, pair);
				Pi *= Pj;

				Pi /= X[i] - X[j];
				std::cout << "P[" << i << "][" << j << "] = " << Pi.ToString() << "\n";
			}
		}
		P += Pi * Y[i];
		std::cout << "P[" << i << "] = " << P.ToString() << "\n";
	}

	return P;
}

Polynomial CreateNewtonPolynomial(int count, double* X, double* Y)
{
	double* coeffs = new double[count];
	double* coeffsCopy = new double[count];
	for (int i = 0; i < count; i++)
	{
		coeffs[i] = Y[i];
	}

	Polynomial result = Polynomial::One() * Y[0];

	for (int i = 1; i < count; i++)
	{
		Polynomial member = Polynomial::One();
		for (int j = 0; j < count; j++)
		{
			coeffsCopy[j] = coeffs[j];
		}
		for (int j = 0; j < count - i; j++)
		{
			coeffs[j] = (coeffsCopy[j + 1] - coeffsCopy[j]) / (X[j + i] - X[j]);
			std::cout << "C[" << j << "] = " << coeffs[j] << "\n";
		}
		member *= coeffs[0];

		for (int j = 0; j < i; j++)
		{
			double pair[2] = { -X[j], 1 };
			member *= Polynomial(1, pair);
		}

		result += member;
		std::cout << member.ToString() << "\n";
		std::cout << result.ToString() << "\n";
	}

	delete[] coeffs;
	delete[] coeffsCopy;
	return result;
}

void ShowDiffTable(int count, double* X, double* Y)
{
	double* diff = new double[count];
	for (int i = 0; i < count; i++)
	{
		diff[i] = Y[i];
	}
	double* temp = new double[count];

	std::cout << "|" << std::setw(5) << "x:";
	for (int i = 0; i < count; i++)
	{
		std::cout << std::setw(12) << X[i] << " ";
	}
	std::cout << "|\n";

	for (int y = 0; y < count; y++)
	{
		std::cout << "|" << std::setw(5) << ("y" + std::to_string(y) + ":");
		for (int x = 0; x < count - y; x++)
		{
			std::cout << std::setw(12) << diff[x] << " ";
		}
		for (int x = count - y; x < count; x++)
		{
			std::cout << std::setw(12) << "-" << " ";
		}
		for (int i = 0; i < count - y; i++)
		{
			temp[i] = diff[i];
		}
		for (int x = 0; x < count - y - 1; x++)
		{
			diff[x] = temp[x + 1] - temp[x];
		}
		std::cout << "|\n";
	}

	delete[] diff;
	delete[] temp;
}

std::string BinToHex(std::string binary)
{
	const char* hexChar = "0123456789abcdef";

	int length;
	while ((length = binary.length()) % 4)
	{
		binary.insert(binary.begin(), '0');
	}

	std::string hex = std::string(length / 4, '0');

	for (int i = (length - 1) / 4; i >= 0; --i) {
		int pow2 = 1;
		int sum = 0;
		for (int j = 3; j >= 0; --j)
		{
			if (binary[i * 4 + j] == '1') {
				sum += pow2;
			}
			pow2 = pow2 << 1;
		}
		hex[i] = hexChar[sum];
	}

	return hex;
}

int main()
{
	double X1[] = { 2, 4, 6 };
	double Y1[] = { -3, 5, 21 };

	double X[] = { 0.45, 0.46, 0.47, 0.48, 0.49, 0.50, 0.51, 0.52, 0.53, 0.54 };
	double Y[] = { 20.19, 19.61, 18.94, 18.17, 17.30, 16.31, 15.19, 13.94, 12.55, 10.99 };
	double x1 = 0.455;
	double x2 = 0.535;

	std::cout << "Difference table:\n";
	ShowDiffTable(10, X, Y);
	std::cout << "\n";

	std::cout << "Lagrange polynomial creation\n";
	Polynomial lagrange = CreateLagrangePolynomial(9, X, Y);
	std::cout << "Result: " << lagrange.ToString() << "\n\n";

	std::cout << "Newton polynomial creation\n";
	Polynomial newton = CreateNewtonPolynomial(9, X, Y);
	std::cout << "Result: " << newton.ToString() << "\n\n";

	std::cout << "P(x1) = " << lagrange.CalculateValue(x1) << "\n";
	std::cout << "P(x2) = " << newton.CalculateValue(x2) << "\n";

	Polynomial P = Polynomial(3, new double[4] { 1, 2, 3, 4 });
	std::cout << "P = " << P.ToString() << "\n";
	Polynomial Pd1 = GetDerivative(P);
	std::cout << "P' = " << Pd1.ToString() << "\n";
	Polynomial Pd2 = GetDerivative(Pd1);
	std::cout << "P'' = " << Pd2.ToString() << "\n";
	Polynomial Pd3 = GetDerivative(Pd2);
	std::cout << "P''' = " << Pd3.ToString() << "\n";
	Polynomial Pd4 = GetDerivative(Pd3);
	std::cout << "P'''' = " << Pd4.ToString() << "\n";

	return 0;
}