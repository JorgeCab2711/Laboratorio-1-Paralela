/*----------------------------------------------
 * riemann.c - calculo de area bajo la curva
 *----------------------------------------------
 * Sumas de Riemann para calcular la integral f(x)
 *
 * Date:  2021-09-22
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define A 1
#define B 40
#define N 10e6

double f(double x);     //La funcion a integrar
double trapezoides(double a, double b, int n);

int main(int argc, char* argv[]) {
  double integral;
  double a=A, b=B;
  int n=N, thread_count=1;
  double h;

  if(argc > 1) {
    a = strtol(argv[1], NULL, 10);
    b = strtol(argv[2], NULL, 10);
    thread_count = strtol(argv[3], NULL, 10);
  }

  integral = trapezoides(a,b,n);

  printf("Con n = %d trapezoides, nuestra aproximacion \n",n);
  printf("de la integral de %f a %f es = %.10f\n", a,b,integral);

  return 0;
}/*main*/

double trapezoides(double a, double b, int n) {
  double integral, h, suma_local, *suma_global;
  int k;

  h = (b-a)/n;
  integral = (f(a) + f(b)) / 2.0;
  suma_global = malloc(sizeof(double));
  *suma_global = 0.0;

  int thread_id, num_threads, n_local, k_local;
  double a_local, b_local, h_local;

  h_local = h;
  thread_id = omp_get_thread_num();
  num_threads = omp_get_num_threads();
  n_local = n / num_threads;
  a_local = a + thread_id * n_local * h_local;
  b_local = a_local + n_local * h_local;

  #pragma omp parallel for reduction(+:integral) num_threads(thread_count)
  for(k_local = 0; k_local < n_local; k_local++) {
    suma_local += f(a_local + k_local * h_local);
  }

  #pragma omp critical
  {
    *suma_global += suma_local;
  }
  integral = *suma_global * h;

  free(suma_global);
  return integral;
}/*trapezoides*/

double f(double x) {
  double return_val;

  return_val = x*x;

  return return_val;
}/*f*/
