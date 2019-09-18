#include <iostream>
#include <iomanip>
#include <cmath>

double f(double x)
{
	//tg(0.5x + 0.1) = x^2			| - x^2
	//tg(0.5x + 0.1) - x^2 = 0

	return tan(0.5 * x + 0.1) - x * x;
}

double phi(double x)
{
	//tg(0.5x + 0.1) - x^2 = 0		| / x
	//tg(0.5x + 0.1) / x - x = 0	| + x
	//tg(0.5x + 0.1) / x = x

	return tan(0.5 * x + 0.1) / x;
}

double d_dx(double(*f)(double), double x)
{
	constexpr double delta = 0.000001;
	return (f(x + delta) - f(x)) / delta;
}

double f1d(double x)
{
	return d_dx(f, x);
}

double f2d(double x)
{
	return d_dx(f1d, x);
}

void swap(double* a, double* b)
{
	double t = *a;
	*a = *b;
	*b = t;
}

double BisectionMethod(double a, double b, double eps)
{
	double x = (a + b) / 2;
	int iterations = 0;

	std::cout << "Bisection method: \n";
	while (fabs(f(x)) > eps)
	{
		if (!isfinite(x))
		{
			std::cout << "f(x) at point x = " << x << " has no value. Returning NaN...\n";
			return NAN;
		}
		std::cout << "Iteration " << std::setw(3) << iterations << ": x = " << std::setw(10) << x << "; f(x) = " << std::setw(10) << f(x) << "\n";
		if (f(a) * f(x) > 0) a = x;
		else b = x;

		x = (a + b) / 2;
		iterations++;
	}

	std::cout << "Total iterations: " << std::setw(3) << iterations << "; x = " << std::setw(10) << x << "; f(x) = " << std::setw(10) << f(x) << "\n";
	return x;
}

double ChordMethod(double a, double b, double eps)
{
	std::cout << f(a) << "\n";
	std::cout << f2d(a) << "\n";

	if (f(a) * f2d(a) < 0)
	{	
		swap(&a, &b);
	}

	double x = a; //a is moving
	int iterations = 0;

	std::cout << "Chord method: \n";
	while (fabs(f(x)) > eps)
	{
		if (!isfinite(x))
		{
			std::cout << "f(x) at point x = " << x << " has no value. Returning NaN...\n";
			return NAN;
		}
		std::cout << "Iteration " << std::setw(3) << iterations << ": x = " << std::setw(10) << x << "; f(x) = " << std::setw(10) << f(x) << "\n";

		x -= ((f(x) * (b - x)) / (f(b) - f(x)));
		iterations++;
	}

	std::cout << "Total iterations: " << std::setw(3) << iterations << "; x = " << std::setw(10) << x << "; f(x) = " << std::setw(10) << f(x) << "\n";
	return x;
}

double NewtonMethod(double a, double b, double eps)
{
	double x = (f(a) * f2d(a) > 0) ? a : b;
	int iterations = 0;
	
	std::cout << "Newton method: \n";
	while (fabs(f(x)) > eps)
	{
		if (!isfinite(x))
		{
			std::cout << "f(x) at point x = " << x << " has no value. Returning NaN...\n";
			return NAN;
		}
		std::cout << "Iteration " << std::setw(3) << iterations << ": x = " << std::setw(10) << x << "; f(x) = " << std::setw(10) << f(x) << "\n";

		x -= f(x) / f1d(x);
		iterations++;
	}

	std::cout << "Total iterations: " << std::setw(3) << iterations << "; x = " << std::setw(10) << x << "; f(x) = " << std::setw(10) << f(x) << "\n";
	return x;
}

double IterationMethod(double a, double b, double eps)
{
	double x = (f(a) * f2d(a) > 0) ? a : b;
	int iterations = 0;

	std::cout << "Iteration method: \n";
	while (fabs(f(x)) > eps)
	{
		if (!isfinite(x))
		{
			std::cout << "f(x) at point x = " << x << " has no value. Returning NaN...\n";
			return NAN;
		}
		std::cout << "Iteration " << std::setw(3) << iterations << ": x = " << std::setw(10) << x << "; f(x) = " << std::setw(10) << f(x) << "\n";
		
		x = phi(x);
		iterations++;
	}

	std::cout << "Total iterations: " << std::setw(3) << iterations << "; x = " << std::setw(10) << x << "; f(x) = " << std::setw(10) << f(x) << "\n";
	return x;
}


int main()
{
	std::cout << std::left; //left justification
	int type = 0;
	double a, b, eps;
	char exitCond;

	do
	{
		std::cout << "Type a, b (a < b):\n";
		while (1)
		{
			if (!(std::cin >> a >> b))
			{
				std::cout << "Invalid input: not floating-point numbers.\n";
				std::cin.clear();
				std::cin.sync();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else if (a >= b)
			{
				std::cout << "Invalid range: a >= b.\n";
			}
			else if (!isfinite(f(a)) || !isfinite(f(b)))
			{
				std::cout << "Invalid range: Either f(a) or f(b) are infinity or NaN\n";
			}
			else if (f(a) * f(b) > 0 && f1d(a) * f1d(b) > 0)
			{
				std::cout << "Invalid range: There are no roots or there are more then one root.\n";
			}
			else break;
		}

		std::cout << "Enter Epsilon (accuracy):\n";
		while (1)
		{
			if (!(std::cin >> eps))
			{
				std::cout << "Invalid input: not a floating-point number.\n";
				std::cin.clear();
				std::cin.sync();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else if (eps <= 0)
			{
				std::cout << "Invalid Epsilon, should be > 0.\n";
			}
			else break;
		}

		std::cout << "Enter approximation method (1 - bisectional, 2 - chord, 3 - newton, 4 - iterational, 5 - all):\n";
		std::cin >> type;
		switch (type)
		{
		case 1:
		{
			BisectionMethod(a, b, eps);
			break;
		}
		case 2:
		{
			ChordMethod(a, b, eps);
			break;
		}
		case 3:
		{
			NewtonMethod(a, b, eps);
			break;
		}
		case 4:
		{
			IterationMethod(a, b, eps);
			break;
		}
		case 5:
		{
			BisectionMethod(a, b, eps);
			std::cout << "-------------------------------------------------------\n";
			ChordMethod(a, b, eps);
			std::cout << "-------------------------------------------------------\n";
			NewtonMethod(a, b, eps);
			std::cout << "-------------------------------------------------------\n";
			IterationMethod(a, b, eps);
			break;
		}
		default:
		{
			std::cout << "Unknown type, selecting default (bisection method)\n";
			BisectionMethod(a, b, eps);
			break;
		}
		}
		std::cout << "Press ENTER to try again, or any other key to exit\n";
		std::getchar();
		exitCond = std::getchar();
	} while (exitCond == '\n');
	return 0;
}
