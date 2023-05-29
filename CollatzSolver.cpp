#include <iostream>
#include <tuple>
#include <chrono>
#include <numeric>

void solve(int A, int B, int *result)
{
	if (A == 0)
	{
		result[0] = 0;
		result[1] = 1;
		return;
	}

	int xy[2];
	solve(B % A, A, xy);
	result[0] = (xy[1] - (B / A) * xy[0]);
	result[1] = xy[0];
}

void diophantSolution(int A, int B, int C, int *result)
{
	int coeff0 = 1;
	int coeff1 = 1;
	if (A < 0)
	{
		coeff0 = -1;
		A = abs(A);
	}

	if (B < 0)
	{
		coeff1 = -1;
		B = abs(B);
	}

	int solution[2];
	solve(A, B, solution);

	result[0] = coeff0 * (solution[0] - (solution[0] / B) * B);
	result[1] = coeff1 * (solution[1] - (solution[1] / A) * A);
	// return [solution[i] - (solution[i]/[B, A][i])*[B, A][i] for i in range(len(solution))]
}

class DiophantSolver
{
public:
	void operator()(int a, int b, int c)
	{
	}
};

int main()
{
	return 0;
}