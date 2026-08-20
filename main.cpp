/* main.cpp simple square solver */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

enum Solutions {
    INFINITE_SOLUTION = -1,
    NO_SOLUTION = 0,
    ONE_SOLUTION = 1,
    TWO_SOLUTION = 2
};

#define MAX_LEN_COEF 50

int linear_solver(const double first_coef, const double second_coef, double* const pt_x);
int square_solver(const double coef_a, const double coef_b, const double coef_c, double* const pt_x1, double* const pt_x2);
void print_equation(const double coef_a, const double coef_b, const double coef_c);

int main() {
    double coef_a = NAN, coef_b = NAN, coef_c = NAN;
    double first_sol = NAN, second_sol = NAN;
    int count_solution = -1;

    printf("Enter the coefficients of the square equation:\n");

    printf("Enter first coefficient: ");
    assert(scanf("%lg", &coef_a) == 1);
    while (getchar() != '\n') continue;

    printf("Enter second coefficient: ");
    assert(scanf("%lg", &coef_b) == 1);
    while (getchar() != '\n') continue;

    printf("Enter third coefficient: ");
    assert(scanf("%lg", &coef_c) == 1);
    while (getchar() != '\n') continue;

    print_equation(coef_a, coef_b, coef_c);

    count_solution = square_solver(coef_a, coef_b, coef_c, &first_sol, &second_sol);

    switch (count_solution) {
        case NO_SOLUTION:
            printf("No solution");
            break;
        case ONE_SOLUTION:
            printf("One solution: %lg\n", first_sol);
            break;
        case TWO_SOLUTION:
            printf("Two solution: %lg %lg\n", first_sol, second_sol);
            break;
        case INFINITE_SOLUTION:
            printf("Infinite solution\n");
            break;
        default:
            printf("ERROR");
            return 0;
    }

    return 0;
}


int linear_solver(const double first_coef, const double second_coef, double* const pt_x) {
    assert(isfinite(first_coef));
    assert(isfinite(second_coef));
    assert(pt_x != NULL);

    if (first_coef != 0) {
        *pt_x = -second_coef / first_coef;
        return ONE_SOLUTION;
    } else if (second_coef == 0) {
        return INFINITE_SOLUTION;
    } else {
        return NO_SOLUTION;
    }
}


int square_solver(const double coef_a, const double coef_b, const double coef_c, double* const pt_x1, double* const pt_x2) {
    assert(isfinite(coef_a));
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));

    assert(pt_x1 != NULL);
    assert(pt_x1 != NULL);
    assert(pt_x1 != pt_x2);

    if (coef_a == 0) {
        return linear_solver(coef_b, coef_c, pt_x1);
    }

    double discriminant = coef_b * coef_b - 4 * coef_a * coef_c;

    if (discriminant < 0) {
        return NO_SOLUTION;
    } else if (discriminant == 0) {
        *pt_x1 = -coef_b / (2 * coef_a);
        return ONE_SOLUTION;
    }

    *pt_x1 = (-coef_b + sqrt(discriminant)) / (2 * coef_a);
    *pt_x2 = (-coef_b - sqrt(discriminant)) / (2 * coef_a);

    return TWO_SOLUTION;
}

void print_equation(const double coef_a, const double coef_b, const double coef_c) {
    assert(isfinite(coef_a));
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));

    if (coef_a == 0 && coef_b == 0 && coef_c == 0) {
        printf("0 = 0\n");
        return;
    }

    char str_coef_a[MAX_LEN_COEF] = "";
    char str_coef_b[MAX_LEN_COEF] = "";
    char str_coef_c[MAX_LEN_COEF] = "";

    char sign_a[4] = "";
    char sign_b[4] = " + ";
    char sign_c[4] = " + ";

    if (coef_a < 0) {
        sign_a[0] = '-';
    }

    if (coef_b == 0) {
        sign_b[0] = '\0';
    } else if (coef_b < 0) {
        snprintf(sign_b, 4, "%s", " - ");
    }

    if (coef_c == 0) {
        sign_c[0] = '\0';
    } else if (coef_c < 0) {
        snprintf(sign_c, 4, "%s", " - ");
    }

    if (coef_a) {
        int size_a = 0;
        if (coef_a != 1) {
            snprintf(str_coef_a, MAX_LEN_COEF, "%lg", abs(coef_a));
            size_a = strlen(str_coef_a);
        }
        snprintf(str_coef_a + size_a, MAX_LEN_COEF - size_a, "%s", "x^2");
    }

    if (coef_b) {
        int size_b = 0;
        if (coef_b != 1) {
            snprintf(str_coef_b, MAX_LEN_COEF, "%lg", abs(coef_b));
            size_b = strlen(str_coef_b);
        }
        snprintf(str_coef_b + size_b, MAX_LEN_COEF - size_b, "%s", "x");
    }

    if (coef_c) {
        snprintf(str_coef_c, MAX_LEN_COEF, "%lg", abs(coef_c));
    }

    printf("%s%s%s%s%s%s = 0\n", sign_a, str_coef_a, sign_b, str_coef_b, sign_c, str_coef_c);
}
