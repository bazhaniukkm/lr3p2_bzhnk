#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

// Vyhidna funkcii
double f(double x) {
    return -pow(x, 4) - 5 * pow(x, 3) - 0.25 * pow(x, 2) + 2;
}

// Pohidna funkcii
double df(double x) {
    return -4 * pow(x, 3) - 15 * pow(x, 2) - 0.5 * x;
}

// Diya pry perevyschenni limitu iteracii
int ask_continue() {
    int choice;
    printf("\nDosyagnuto limitu iteracii. Oberit diyu:\n");
    printf("1 dodaty sche iteracii\n");
    printf("2 prodovzhyty do zbizhnosti\n");
    printf("3 zupynyty i povernuty promizhnyi rezultat\n");
    printf("Vash vybir: ");
    if (scanf("%d", &choice) != 1) return 3;
    return choice;
}

// Metod polovynnoho dilennya
int bisection(double a, double b, double tol, int maxIter, int debug,
              double *root, double *froot, double *time_used) {
    if (f(a) * f(b) >= 0) return -1;
    clock_t start = clock();
    int iter = 0, origMax = maxIter;
    double fa = f(a), fb = f(b);
    while (1) {
        iter++;
        double c = (a + b) / 2.0;
        double fc = f(c);
        if (debug) printf("iter %d: x=%.8f f=%.8e\n", iter, c, fc);
        if (fabs(fc) <= tol || (b - a) / 2.0 <= tol) {
            *root = c; *froot = fc;
            *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
            return iter;
        }
        if (fa * fc < 0) { b = c; fb = fc; } else { a = c; fa = fc; }
        if (iter >= maxIter) {
            int choice = ask_continue();
            if (choice == 1) maxIter += origMax;
            else if (choice == 2) maxIter = INT_MAX / 2;
            else {
                *root = c; *froot = fc;
                *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
                return -2;
            }
        }
    }
}

// Metod hord
int secant(double x0, double x1, double tol, int maxIter, int debug,
           double *root, double *froot, double *time_used) {
    clock_t start = clock();
    int iter = 0, origMax = maxIter;
    double f0 = f(x0), f1 = f(x1);

    while (1) {
        iter++;
        if (fabs(f1 - f0) < 1e-14) {
            *root = x1;
            *froot = f1;
            *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
            return -3;
        }

        double x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        double f2 = f(x2);

        if (debug) {
            printf("iter %d: x=%.8f f=%.8e\n", iter, x2, f2);
        }

        if (fabs(x2 - x1) <= tol || fabs(f2) <= tol) {
            *root = x2;
            *froot = f2;
            *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
            return iter;
        }

        x0 = x1; f0 = f1;
        x1 = x2; f1 = f2;

        if (iter >= maxIter) {
            *root = x1;
            *froot = f1;
            *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
            return -2;
        }
    }
}
int main(void) {
    double root, froot, time_used;
    int iters;

    iters = bisection(-3, -2, 1e-6, 100, 1, &root, &froot, &time_used);
    printf("\nBisection: root = %.8f, f(root) = %.8e, iters = %d, time = %.6f s\n",
           root, froot, iters, time_used);

    iters = secant(-3, -2, 1e-6, 100, 1, &root, &froot, &time_used);
    printf("\nSecant: root = %.8f, f(root) = %.8e, iters = %d, time = %.6f s\n",
           root, froot, iters, time_used);

    return 0;
}
