#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.1415926
#define populationSize 50
#define generation 700
#define muRate 0.1
#define xoRate 0.5
#define chromoSize 9

char population[populationSize][10];
double getRandomDouble() {
	return rand() % 10000 / 10000.00;
}

void initPopulation() {
	for (int i = 0; i <populationSize ; i++)
		for (int j  = 0;  j<chromoSize; j++)
		{
			double flag = getRandomDouble();
			if (flag >= 0.5)
				population[i][j] = '1';
			else
				population[i][j] = '0';
		}
}

double decode(char* gene) {

	int len = 0;
	while (gene[len] != '\0')
		len++;
	
	double sum = 0;
	for (int i = 0; i < len; i++) {
		if (gene[i] - '0' == 1)
			sum = sum + pow(2, len - i - 1);
	}
	
	return sum * 3/511-1;
}
double fit(double x) {
	return x * sin(10 * PI*x) + 2;
}

int selectMemberUsingRoultteWheel() {
	double totalNum = 0;
	double wheel[populationSize];
	double temp[populationSize];
	for (int x = 0; x < populationSize; x++) {
		double show = decode(population[x]);
		temp[x] = fit(show);
		totalNum = totalNum + temp[x];
	}
	for (int i = 0; i < populationSize; i++)
	{
		wheel[i] = temp[i] / totalNum;
	}
	double rand = getRandomDouble();
	double partialSum = 0;
	for (int index = 0; index < populationSize; index++) {
		partialSum = partialSum + wheel[index];
		if (partialSum > rand) {
			return index;
		}
	}
	return -1;

}
void evolvePopulation() {
	char newPopulation[populationSize][10];
	int a = -1, b = -1;
	int winner, loser;
	for (int i = 0; i < populationSize; i++) {
		do {
			a = selectMemberUsingRoultteWheel();
			b = selectMemberUsingRoultteWheel();
		} while (a == b);
		if (fit(decode(population[a])) > fit(decode(population[b]))) {
			winner = a;
			loser = b;
		}
		else
		{
			winner = b;
			loser = a;
		}
	
		int tempChrome[10];
		for (int kk = 0; kk < chromoSize; kk++) {
			tempChrome[kk] = population[winner][kk];
			
		}
		if (getRandomDouble() < xoRate) {
			do {
				a = rand() % chromoSize;
				b = rand() % chromoSize;
			} while (a == b);
			if (a < b) {
				for (int tt = a; tt < b; tt++) {
					tempChrome[tt] = population[loser][tt];
				}
			}
			else
				for (int tt = b; tt < a; tt++) {
					tempChrome[tt] = population[loser][tt];
				}
		}
		if (getRandomDouble() < muRate) {
			int muRateNum = rand() % chromoSize;
			if (tempChrome[muRateNum] - '0' == 0)
				tempChrome[muRateNum] = '1';
			else
				tempChrome[muRateNum] = '0';
		}
		for (int kk = 0; kk < chromoSize; kk++) {
			newPopulation[i][kk] =tempChrome[kk];

		}
	}
	for(int i = 0; i < populationSize; i++)
		for (int kk = 0; kk < chromoSize; kk++) {
			population[i][kk] = newPopulation[i][kk];
		}
}

int getFittestMem() {
	double temp = fit(decode(population[0]));
	int index = 0;
	for (int x = 1; x < populationSize; x++) {
		double yy = fit(decode(population[x]));
		if (temp <yy ) {
			index = x;
			temp = yy;
		}
	}
	return index;
}
void displayResult() {
	int fittest = getFittestMem();
	printf("  %s  \n", population[fittest]);
	printf("  %f   %f  \n", decode(population[fittest]), fit(decode(population[fittest])));
	
}
void runGA() {
	for (int u = 0; u < generation; u++) {
		printf("Current generation is : %d", u);
		displayResult();
		evolvePopulation();
	}

}
int main() {

	initPopulation();
	runGA();
	getchar();
	return 0;
}