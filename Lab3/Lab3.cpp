#include <iostream>
#include <iomanip>

typedef double System3r[3][4];

typedef double Matrix2x2[2][2];
typedef double Matrix3x3[3][3];
typedef double Vector3[3];

double det(Matrix3x3 matrix)
{
	return
		+ matrix[0][0] * matrix[1][1] * matrix[2][2]
		+ matrix[0][1] * matrix[1][2] * matrix[2][0]
		+ matrix[1][0] * matrix[2][1] * matrix[0][2]
		- matrix[2][0] * matrix[1][1] * matrix[0][2]
		- matrix[1][0] * matrix[0][1] * matrix[2][2]
		- matrix[0][0] * matrix[2][1] * matrix[1][2];
}

double det(Matrix2x2 matrix)
{
	return
		+ matrix[0][0] * matrix[1][1] 
		- matrix[1][0] * matrix[0][1];
}

void transponate(Matrix3x3 matrix)
{
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < y; x++)
		{
			double temp = matrix[y][x];
			matrix[y][x] = matrix[x][y];
			matrix[x][y] = temp;
		}
	}
}

void multiply(Matrix3x3 matrix, double value)
{
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			matrix[y][x] *= value;
		}
	}
}

void multiply(Matrix3x3 matrix, Vector3 column)
{
	Vector3 buffer = { column[0], column[1], column[2] };
	column[0] = matrix[0][0] * buffer[0] + matrix[0][1] * buffer[1] + matrix[0][2] * buffer[2];
	column[1] = matrix[1][0] * buffer[0] + matrix[1][1] * buffer[1] + matrix[1][2] * buffer[2];
	column[2] = matrix[2][0] * buffer[0] + matrix[2][1] * buffer[1] + matrix[2][2] * buffer[2];
}

void MatrixMethod(System3r system)
{
	Vector3 roots;
	Vector3 freeCoeffs = { system[0][3], system[1][3], system[2][3] };
	Matrix3x3 coeffs =
	{ { system[0][0], system[0][1], system[0][2] },
	  { system[1][0], system[1][1], system[1][2] },
	  { system[2][0], system[2][1], system[2][2] } };
	Matrix3x3 reversed;

	std::cout << "Matrix A:\n";
	std::cout << std::setw(5) << coeffs[0][0] << " " << std::setw(5) << coeffs[0][1] << " " << std::setw(5) << coeffs[0][2] << "\n";
	std::cout << std::setw(5) << coeffs[1][0] << " " << std::setw(5) << coeffs[1][1] << " " << std::setw(5) << coeffs[1][2] << "\n";
	std::cout << std::setw(5) << coeffs[2][0] << " " << std::setw(5) << coeffs[2][1] << " " << std::setw(5) << coeffs[2][2] << "\n";
	std::cout << "--------------\n";

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			int x0 = (x != 0) ? 0 : 1;
			int y0 = (y != 0) ? 0 : 1;
			int x1 = (x != 2) ? 2 : 1;
			int y1 = (y != 2) ? 2 : 1;

			Matrix2x2 minor =
			{ { coeffs[y0][x0], coeffs[y0][x1] } ,
			  { coeffs[y1][x0], coeffs[y1][x1] } };

			double minorD = ((y + x) % 2) ? -det(minor) : det(minor);
			reversed[y][x] = minorD;

			std::cout << "Minor[" << y << "][" << x << "]:\n";
			std::cout << std::setw(5) << minor[0][0] << " " << std::setw(5) << minor[0][1] << "\n";
			std::cout << std::setw(5) << minor[1][0] << " " << std::setw(5) << minor[1][1] << "\n";
			std::cout << "\n";
			std::cout << "Det = " << minorD << "\n";
			std::cout << "------------\n";
		}
	}
	std::cout << "Reversed:\n";
	std::cout << std::setw(10) << reversed[0][0] << " " << std::setw(10) << reversed[0][1] << " " << std::setw(10) << reversed[0][2] << "\n";
	std::cout << std::setw(10) << reversed[1][0] << " " << std::setw(10) << reversed[1][1] << " " << std::setw(10) << reversed[1][2] << "\n";
	std::cout << std::setw(10) << reversed[2][0] << " " << std::setw(10) << reversed[2][1] << " " << std::setw(10) << reversed[2][2] << "\n";
	std::cout << "--------------\n";

	transponate(reversed);

	std::cout << "Transponated reversed:\n";
	std::cout << std::setw(10) << reversed[0][0] << " " << std::setw(10) << reversed[0][1] << " " << std::setw(10) << reversed[0][2] << "\n";
	std::cout << std::setw(10) << reversed[1][0] << " " << std::setw(10) << reversed[1][1] << " " << std::setw(10) << reversed[1][2] << "\n";
	std::cout << std::setw(10) << reversed[2][0] << " " << std::setw(10) << reversed[2][1] << " " << std::setw(10) << reversed[2][2] << "\n";
	std::cout << "--------------\n";

	double d = det(coeffs);

	std::cout << "Det = " << d << "\n";

	multiply(reversed, 1 / d); 

	std::cout << "Multiplied reversed:\n";
	std::cout << std::setw(10) << reversed[0][0] << " " << std::setw(10) << reversed[0][1] << " " << std::setw(10) << reversed[0][2] << "\n";
	std::cout << std::setw(10) << reversed[1][0] << " " << std::setw(10) << reversed[1][1] << " " << std::setw(10) << reversed[1][2] << "\n";
	std::cout << std::setw(10) << reversed[2][0] << " " << std::setw(10) << reversed[2][1] << " " << std::setw(10) << reversed[2][2] << "\n";
	std::cout << "--------------\n";

	multiply(reversed, freeCoeffs);

	roots[0] = freeCoeffs[0];
	roots[1] = freeCoeffs[1];
	roots[2] = freeCoeffs[2];

	std::cout << "Root 1: " << roots[0] << ";\n";
	std::cout << "Root 2: " << roots[1] << ";\n";
	std::cout << "Root 3: " << roots[2] << ";\n";
}

void KramerMethod(System3r system)
{
	Vector3 roots;
	Vector3 freeCoefs = { system[0][3], system[1][3], system[2][3] };

	Matrix3x3 coeffs =
	{ { system[0][0], system[0][1], system[0][2] },
	  { system[1][0], system[1][1], system[1][2] },
	  { system[2][0], system[2][1], system[2][2] } };
	double d = det(coeffs);

	std::cout << "Matrix A:\n";
	std::cout << std::setw(5) << coeffs[0][0] << " " << std::setw(5) << coeffs[0][1] << " " << std::setw(5) << coeffs[0][2] << "\n";
	std::cout << std::setw(5) << coeffs[1][0] << " " << std::setw(5) << coeffs[1][1] << " " << std::setw(5) << coeffs[1][2] << "\n";
	std::cout << std::setw(5) << coeffs[2][0] << " " << std::setw(5) << coeffs[2][1] << " " << std::setw(5) << coeffs[2][2] << "\n";
	std::cout << "\n";
	std::cout << "Det: " << d << ";\n";
	std::cout << "--------------\n";

	Matrix3x3 coeffs1 =
	{ { freeCoefs[0], system[0][1], system[0][2] },
	  { freeCoefs[1], system[1][1], system[1][2] },
	  { freeCoefs[2], system[2][1], system[2][2] } };
	double d1 = det(coeffs1);

	std::cout << "Matrix A1:\n";
	std::cout << std::setw(5) << coeffs1[0][0] << " " << std::setw(5) << coeffs1[0][1] << " " << std::setw(5) << coeffs1[0][2] << "\n";
	std::cout << std::setw(5) << coeffs1[1][0] << " " << std::setw(5) << coeffs1[1][1] << " " << std::setw(5) << coeffs1[1][2] << "\n";
	std::cout << std::setw(5) << coeffs1[2][0] << " " << std::setw(5) << coeffs1[2][1] << " " << std::setw(5) << coeffs1[2][2] << "\n";
	std::cout << "\n";
	std::cout << "Det1: " << d1 << ";\n";
	std::cout << "--------------\n";

	Matrix3x3 coeffs2 =
	{ { system[0][0], freeCoefs[0], system[0][2] },
	  { system[1][0], freeCoefs[1], system[1][2] },
	  { system[2][0], freeCoefs[2], system[2][2] } };
	double d2 = det(coeffs2);

	std::cout << "Matrix A2:\n";
	std::cout << std::setw(5) << coeffs2[0][0] << " " << std::setw(5) << coeffs2[0][1] << " " << std::setw(5) << coeffs2[0][2] << "\n";
	std::cout << std::setw(5) << coeffs2[1][0] << " " << std::setw(5) << coeffs2[1][1] << " " << std::setw(5) << coeffs2[1][2] << "\n";
	std::cout << std::setw(5) << coeffs2[2][0] << " " << std::setw(5) << coeffs2[2][1] << " " << std::setw(5) << coeffs2[2][2] << "\n";
	std::cout << "\n";
	std::cout << "Det2: " << d2 << ";\n";
	std::cout << "--------------\n";

	Matrix3x3 coeffs3 =
	{ { system[0][0], system[0][1], freeCoefs[0] },
	  { system[1][0], system[1][1], freeCoefs[1] },
	  { system[2][0], system[2][1], freeCoefs[2] } };
	double d3 = det(coeffs3);

	std::cout << "Matrix A3:\n";
	std::cout << std::setw(5) << coeffs3[0][0] << " " << std::setw(5) << coeffs3[0][1] << " " << std::setw(5) << coeffs3[0][2] << "\n";
	std::cout << std::setw(5) << coeffs3[1][0] << " " << std::setw(5) << coeffs3[1][1] << " " << std::setw(5) << coeffs3[1][2] << "\n";
	std::cout << std::setw(5) << coeffs3[2][0] << " " << std::setw(5) << coeffs3[2][1] << " " << std::setw(5) << coeffs3[2][2] << "\n";
	std::cout << "\n";
	std::cout << "Det3: " << d3 << ";\n";
	std::cout << "--------------\n";

	roots[0] = d1 / d;
	roots[1] = d2 / d;
	roots[2] = d3 / d;

	std::cout << "Root 1: " << roots[0] << ";\n";
	std::cout << "Root 2: " << roots[1] << ";\n";
	std::cout << "Root 3: " << roots[2] << ";\n";
}

int main()
{
	System3r system =
	{
	{ +0.13, -0.14, -2.00, +0.15 },
	{ +0.75, +0.18, -0.77, +0.11 },
	{ +0.28, -0.17, +0.39, +0.12 }
	};

	std::cout << std::left; //left justification

	MatrixMethod(system);

	std::cout << "\n\n";

	KramerMethod(system);

	std::cout << "Press ENTER to exit\n";
	std::getchar();
	return 0;
}