#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

// f(x) = -x^4 - 5x^3 - 0.25x^2 + 2
double f(double x) {
    double x2 = x * x;
    double x3 = x2 * x;
    double x4 = x2 * x2;
    return -x4 - 5.0 * x3 - 0.25 * x2 + 2.0;
}

// metod polovynnoho dilennya
int bisection(double a, double b, double tol, int maxIter, int debug,
              double *root, double *froot, double *time_used) {
    double fa = f(a), fb = f(b);
    if (fa * fb >= 0.0) return -1;

    clock_t start = clock();
    int iter = 0;

    while (iter < maxIter) {
        iter++;
        double c = 0.5 * (a + b);
        double fc = f(c);

        if (debug) {
            printf("Bisection iter %d: x=%.8f f=%.8e\n", iter, c, fc);
        }

        if (fabs(fc) <= tol || (b - a) / 2.0 <= tol) {
            *root = c;
            *froot = fc;
            *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
            return iter;
        }

        if (fa * fc < 0.0) { b = c; fb = fc; }
        else              { a = c; fa = fc; }
    }

    *root = 0.5 * (a + b);
    *froot = f(*root);
    *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
    return -2;
}

// Metod hord
int secant(double x0, double x1, double tol, int maxIter, int debug,
           double *root, double *froot, double *time_used) {
    clock_t start = clock();
    int iter = 0;
    double f0 = f(x0), f1 = f(x1);

    while (iter < maxIter) {
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
            printf("Secant iter %d: x=%.8f f=%.8e\n", iter, x2, f2);
        }

        if (fabs(x2 - x1) <= tol || fabs(f2) <= tol) {
            *root = x2;
            *froot = f2;
            *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
            return iter;
        }

        x0 = x1; f0 = f1;
        x1 = x2; f1 = f2;
    }

    *root = x1;
    *froot = f1;
    *time_used = (double)(clock() - start) / CLOCKS_PER_SEC;
    return -2;
}

int main(void) {
    int method;
    double a, b, tol;
    int maxIter, debug;
    double root, froot, time_used;
    int iters;

    printf("=== Root Finding Program ===\n");
    printf("Choose method:\n");
    printf("1 - Bisection\n");
    printf("2 - Secant\n");
    printf("Enter your choice: ");
    scanf("%d", &method);

    printf("Enter interval [a b]: ");
    scanf("%lf %lf", &a, &b);

    printf("Enter tolerance: ");
    scanf("%lf", &tol);

    printf("Enter max iterations: ");
    scanf("%d", &maxIter);

    printf("Enable debug output? (0 - no, 1 - yes): ");
    scanf("%d", &debug);

    if (method == 1) {
        iters = bisection(a, b, tol, maxIter, debug, &root, &froot, &time_used);
        if (iters > 0)
            printf("\nBisection result:\nRoot = %.8f\nf(root) = %.8e\nIterations = %d\nTime = %.6f s\n",
                   root, froot, iters, time_used);
        else if (iters == -1)
            printf("\nError: f(a) and f(b) have same sign. No root isolation.\n");
        else
            printf("\nBisection stopped after max iterations. Approximate root = %.8f\n", root);
    }
    else if (method == 2) {
        iters = secant(a, b, tol, maxIter, debug, &root, &froot, &time_used);
        if (iters > 0)
            printf("\nSecant result:\nRoot = %.8f\nf(root) = %.8e\nIterations = %d\nTime = %.6f s\n",
                   root, froot, iters, time_used);
        else if (iters == -3)
            printf("\nError: denominator too small. Secant method failed.\n");
        else
            printf("\nSecant stopped after max iterations. Approximate root = %.8f\n", root);
    }
    else {
        printf("Invalid method selected.\n");
    }

    return 0;
}
