#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define N 5

double *X = NULL;
double *R = NULL;
int TERMS = 0;

void *TaskCode(void *argument)
{
	int i = *((int*)argument);
	double x = X[i];
	double value = x;
	double numer = x * x * x;
	double denom = 6.; // 3!
	int sign = -1;

	for(int j=1; j<=TERMS; j++) {
		value += (double)sign * numer / denom;
		numer *= x * x;
		denom *= (2.*(double)j+2.) * (2.*(double)j+3.);
		sign *= -1;
	}
	R[i] = value;

	return NULL;
}

void sinx_taylor(int num_elements, int terms, double* x, double* result)
{
	int *args = (int *)malloc(num_elements*sizeof(int));
	pthread_t *threads = (pthread_t *)malloc(num_elements*sizeof(pthread_t));
	X = (double *)malloc(num_elements*sizeof(double));
	R = (double *)malloc(num_elements*sizeof(double));
	for(int i=0; i<num_elements; i++) X[i] = x[i];
	TERMS = terms;


	for(int i=0; i<num_elements; i++) {
		args[i] = i;
		pthread_create(&threads[i], NULL, TaskCode, (void *) &args[i]);
	}

	for(int i=0; i<num_elements; i++) {
		pthread_join(threads[i], NULL);
		result[i] = R[i];
	}

}

int main()
{
	double x[N] = {0, M_PI/6.f, M_PI/3.f, M_PI/2.f, 0.134};
	double res[N];

	sinx_taylor(N, 3, x, res);

	for(int i=0; i<N; i++) {
		printf("sin(%.2f) by Taylor series = %f\n", x[i], res[i]);
		printf("sin(%.2f) = %f\n", x[i], sin(x[i]));
	}

	return 0;
}

