#include <cmath>
#include <iostream>
#include <vector>

const double EPS = 1e-6;
const int PRECISION = 20;

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

struct Equation {
    Equation(const Matrix &a, const Column &b):
        a(a),
        b(b)
    {}

    Matrix a;
    Column b;
};

Equation ReadEquation() {
    int size;
    std::cin >> size;
    Matrix a(size, std::vector <double> (size, 0.0));
    Column b(size, 0.0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column)
            std::cin >> a[row][column];
        std::cin >> b[row];
    }
    return Equation(a, b);
}
Column SolveEquation(Equation equation) {
    Matrix &a = equation.a;
    Column &b = equation.b;
    int size = a.size();

    //Re-arrange the row such that diagonal always contain non-zero value
    for (int i=0; i< size; ++ i)
        if (a[i][i] == 0)
        {
            for (int j = i + 1; j < size; ++j)
            {
                if (a[j][i] != 0)
                {
                    //swap i & j break;
                    std::swap(a[i], a[j]);
                    std::swap(b[i], b[j]);
                }
            }

        }
    for (int col = 0; col < size; ++col)
    {
        if (a[col][col] != 1)  // Scale down the row
        {
            double scale = a[col][col];
            a[col][col] = 1;
            b[col] /= scale;
            for (int i = col + 1; i < size; ++i)
                a[col][i] /= scale;
        }
        for (int row = col + 1; row < size; ++row)
            if (a[row][col] != 0)
            {
                double scale = -a[row][col] / a[col][col]; // Find the scaling factor with above row
                for (int c = col; c < size; ++c)
                    a[row][c] += (a[col][c] * scale); // And then subtract such that the left diagnonal become 0
                b[row] += (b[col] * scale);
            }
    }
    // To compute solution start with last row which has only 1 non-zero entry and then iterate back
    for (int row = size - 1; row >= 0; --row)
    {
        double sum = 0;
        for (int col = size - 1; col >= 0; --col)
        {
            if (row == col)
            {
                b[row] -= sum;
                break;
            }
            sum += (a[row][col] * b[col]);
        }
    }
    return b;
}

void PrintColumn(const Column &column) {
    int size = column.size();
    std::cout.precision(PRECISION);
    for (int row = 0; row < size; ++row)
        std::cout << column[row] << std::endl;
}

int main() {
    Equation equation = ReadEquation();
    Column solution = SolveEquation(equation);
    PrintColumn(solution);
    return 0;
}
