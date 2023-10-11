#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define N 4
#define MAXLINE 100


void sinx_taylor(int num_elements, int terms, double* x, double* result)
{
       int fd[2*N];
       char message[MAXLINE] , line[MAXLINE];
       int length;
       for(int i=0; i<num_elments; i++) {
	       int* p_fd = &fd[2*i];
	       pipe(p_fd);
       }


	int pid;
	for(int i=0; i<num_elements; i++) {
                int* p_fd = &fd[2*i];
		if((pid = fork()) = 0){
			close(fd[0]);
	    	int my_id = i;
		double value = x[i];
		double numer = x[i] * x[i] * x[i];
		double denom = 6.; // 3!
		int sign = -1;

		for(int j=1; j<=terms; j++) {
			value += (double)sign * numer / denom;
			numer *= x[i] * x[i];
			denom *= (2.*(double)j+2.) * (2.*(double)j+3.);
			sign *= -1;
		}
		result[i] = value;

		exit(my_id);
	}
}
 int stat;
 for(int i =0; i<num_elements; i++){
        wait(stat);
	int child_id = stat >> 8;
	read(fd[2*child_id], line, MAXLINE);
	
	double res = atof(line);
	result[child_id] = res;
 }

}

int main()
{
double x[N] = {0, M_PI/6., M_PI/3. , 0. 134

}
