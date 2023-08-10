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

  //---- Aproximacion de la integral
  integral = trapezoides(a,b,n);

  printf("Con n = %d trapezoides, nuestra aproximacion \n",n);
  printf("de la integral de %f a %f es = %.10f\n", a,b,integral);

  return 0;
}/*main*/

//------------------------------------------
// trapezoides
//
// Estimar la integral mediante sumas de Riemann
// Input: a,b,n
// Output: integral
//------------------------------------------
double trapezoides(double a, double b, int n) {
  double integral, h;
  int k;

  //---- Ancho de cada trapecio
  h = (b-a)/n;
  //---- Valor inicial de la integral (valores extremos)
  integral = (f(a) + f(b)) / 2.0;

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
    integral += f(a_local + k_local * h_local);
  }
  integral = integral * h;

  return integral;
}/*trapezoides*/

//------------------------------------------
// f
//
// Funcion a ser integrada
// Input: x
//------------------------------------------
double f(double x) {
  double return_val;

  return_val = x*x;

  return return_val;
}/*f*/
