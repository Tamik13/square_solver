/* main.cpp simple square solver */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define COLOR_TEXT(STR, COLOR) "\033[" COLOR "m" STR "\033[0m"

#define RED "91"
#define GREEN "92"
#define BLUE "94"

#define DELTA 1e-8

enum Errors {
    SUCCESS = 0,
    READ_INPUT_ERROR = 1,
    NOT_EMPTY_BUFFER = 2,
    OPEN_FILE_ERROR = 3
};

enum Solutions {
    INIT_VALUE = -2,
    INFINITE_SOLUTION = -1,
    NO_SOLUTION = 0,
    ONE_SOLUTION = 1,
    TWO_SOLUTION = 2
};

Errors read_input(double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c);
Errors read_one(double* const pt_coef, FILE* file_in);
Errors clean_buffer(FILE* file_in);

Solutions linear_solver(const double first_coef, const double second_coef, double* const pt_x);
Solutions square_solver(const double coef_a, const double coef_b, const double coef_c, double* const pt_x1, double* const pt_x2);
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

    char file_in_name[] = "input.txt";
    FILE* file_in = fopen(file_in_name, "r");

    if (!file_in) {
        printf("Error occured while opening file\n");
        return OPEN_FILE_ERROR;
    }

    read_one(pt_coef_a, file_in);

    read_one(pt_coef_b, file_in);

    read_one(pt_coef_c, file_in);

    fclose(file_in);

    return SUCCESS;
}


Errors read_one(double* const pt_coef, FILE* file_in) {
    if (fscanf(file_in, "%lg", pt_coef) != 1 || clean_buffer(file_in)) {
        printf(COLOR_TEXT("Incorrect input\n", RED));

        exit(EXIT_FAILURE);
    }

    return SUCCESS;
}


Errors clean_buffer(FILE* file_inp) {
    Errors error = SUCCESS;
    char ch = '\0';

    fscanf(file_inp, "%c", &ch);

    while (ch != '\n') {
        fscanf(file_inp, "%c", &ch);

        error = NOT_EMPTY_BUFFER;
    }

    return error;
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

    char file_out_name[] = "output.txt";
    FILE* file_out = fopen(file_out_name, "w");

    switch (count_sol) {

        case NO_SOLUTION:
            fprintf(file_out, "No solution\n");
            break;

        case ONE_SOLUTION:
            fprintf(file_out, "One solution: %lg\n", first_sol);
            break;

        case TWO_SOLUTION:
            fprintf(file_out, "Two solution: %lg %lg\n", first_sol, second_sol);
            break;

        case INFINITE_SOLUTION:
            fprintf(file_out, "Infinite solution\n");
            break;

        case INIT_VALUE:
            fprintf(file_out, "INIT VALUE IN COUNT_SOL ERROR\n");
            fclose(file_out);
            return;

        default:
            fprintf(file_out, "SWITCH ERROR\n");
            fclose(file_out);
            return;
    }

    fclose(file_out);
    return;
}
