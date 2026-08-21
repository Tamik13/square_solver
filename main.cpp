/* main.cpp simple square solver */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define DELTA 1e-8

enum Errors {
    SUCCESS = 0,
    READ_INPUT_ERROR = 1
};

enum Solutions {
    INIT_VALUE = -2,
    INFINITE_SOLUTION = -1,
    NO_SOLUTION = 0,
    ONE_SOLUTION = 1,
    TWO_SOLUTION = 2
};

Errors read_input(double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c);
Errors read_one(double* const pt_coef, const char* str_num);
void clean_buffer();

Solutions linear_solver(const double first_coef, const double second_coef, double* const pt_x);
Solutions square_solver(const double coef_a, const double coef_b, const double coef_c, double* const pt_x1, double* const pt_x2);
bool is_zero(const double num);
bool is_equally(const double lhs, const double rhs);

void print_output(const Solutions count_sol, const double first_sol, const double second_sol);



int main() {
    double coef_a = NAN, coef_b = NAN, coef_c = NAN;
    double first_sol = NAN, second_sol = NAN;
    Solutions count_sol = INIT_VALUE;

    read_input(&coef_a, &coef_b, &coef_c);

    count_sol = square_solver(coef_a, coef_b, coef_c, &first_sol, &second_sol);

    print_output(count_sol, first_sol, second_sol);

    return 0;
}


Errors read_input(double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c) {
    assert(pt_coef_a != NULL);
    assert(pt_coef_b != NULL);
    assert(pt_coef_c != NULL);

    assert(pt_coef_a != pt_coef_b);
    assert(pt_coef_b != pt_coef_c);
    assert(pt_coef_c != pt_coef_a);

    printf("Enter the coefficients of the square equation:\n");

    read_one(pt_coef_a, "first");

    read_one(pt_coef_b, "second");

    read_one(pt_coef_c, "thread");

    return SUCCESS;
}


Errors read_one(double* const pt_coef, const char* str_num) {
    printf("Enter %s coefficient: ", str_num);

    while (scanf("%lg", pt_coef) != 1) {
        clean_buffer();

        printf("Incorrect input\n"
               "Try again\n"
               "Enter %s coefficient: ", str_num);
    }

    clean_buffer();

    return SUCCESS;
}


void clean_buffer() {
    while (getchar() != '\n') continue;
}



Solutions linear_solver(const double first_coef, const double second_coef, double* const pt_x) {
    assert(isfinite(first_coef));
    assert(isfinite(second_coef));
    assert(pt_x != NULL);

    if (!is_equally(first_coef, 0)) {
        *pt_x = -second_coef / first_coef;
        return ONE_SOLUTION;
    } else if (is_equally(second_coef, 0)) {
        return INFINITE_SOLUTION;
    } else {
        return NO_SOLUTION;
    }
}


Solutions square_solver(const double coef_a, const double coef_b, const double coef_c, double* const pt_x1, double* const pt_x2) {
    assert(isfinite(coef_a));
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));

    assert(pt_x1 != NULL);
    assert(pt_x1 != NULL);
    assert(pt_x1 != pt_x2);

    if (is_equally(coef_a, 0)) {
        return linear_solver(coef_b, coef_c, pt_x1);
    }

    double discriminant = coef_b * coef_b - 4 * coef_a * coef_c;

    if (discriminant < 0) {
        return NO_SOLUTION;
    } else if (is_equally(discriminant, 0)) {
        *pt_x1 = -coef_b / (2 * coef_a);
        return ONE_SOLUTION;
    }

    *pt_x1 = (-coef_b + sqrt(discriminant)) / (2 * coef_a);
    *pt_x2 = (-coef_b - sqrt(discriminant)) / (2 * coef_a);

    return TWO_SOLUTION;
}


bool is_equally(const double lhs, const double rhs) {
    assert(isfinite(lhs));
    assert(isfinite(rhs));

    return abs(lhs - rhs) < DELTA;
}




void print_output(Solutions count_sol, const double first_sol, const double second_sol) {
    if (count_sol > 0) {
        assert(isfinite(first_sol));
    }

    if (count_sol > 1) {
        assert(isfinite(second_sol));
    }

    switch (count_sol) {
        case NO_SOLUTION:
            printf("No solution\n");
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
        case INIT_VALUE:
            printf("ERROR\n");
            return;
        default:
            printf("ERROR\n");
            return;
    }

    return;
}

/*
-D _DEBUG -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
*/
