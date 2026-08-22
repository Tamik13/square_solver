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

const char file_input_name[] = "input.txt";
const char file_output_name[] = "output.txt";

int line_in_input_file = 1;
int line_in_output_file = 1;

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
Errors read_one(double* const pt_coef, FILE* file_input);
Errors clean_buffer(FILE* file_input);

Solutions linear_solver(const double first_coef, const double second_coef, double* const pt_x);
Solutions square_solver(const double coef_a, const double coef_b, const double coef_c, double* const pt_x1, double* const pt_x2);
bool is_equally(const double lhs, const double rhs);

void print_output(const Solutions count_sol, const double first_sol, const double second_sol);


int main() {
    double coef_a = NAN, coef_b = NAN, coef_c = NAN;
    double first_sol = NAN, second_sol = NAN;
    Solutions count_sol = INIT_VALUE;

    if (read_input(&coef_a, &coef_b, &coef_c) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return 0;
    }

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

    FILE* file_input = fopen(file_input_name, "r");

    if (!file_input) {
        printf(COLOR_TEXT("Error occured while opening %s\n", RED), file_input_name);
        return OPEN_FILE_ERROR;
    }

    if (read_one(pt_coef_a, file_input) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    if (read_one(pt_coef_b, file_input) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    if (read_one(pt_coef_c, file_input)) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    fclose(file_input);

    return SUCCESS;
}


Errors read_one(double* const pt_coef, FILE* file_input) {
    if (fscanf(file_input, "%lg", pt_coef) != 1 || clean_buffer(file_input)) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    line_in_input_file++;

    return SUCCESS;
}


Errors clean_buffer(FILE* file_input) {
    Errors error = SUCCESS;

    while (fgetc(file_input) != '\n') {
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

    FILE* file_output = fopen(file_output_name, "w");

    switch (count_sol) {

        case NO_SOLUTION:
            fprintf(file_output, "No solution\n");
            break;

        case ONE_SOLUTION:
            fprintf(file_output, "One solution: x1 = %lg\n", first_sol);
            break;

        case TWO_SOLUTION:
            fprintf(file_output, "Two solution: x1 = %lg x2 = %lg\n", first_sol, second_sol);
            break;

        case INFINITE_SOLUTION:
            fprintf(file_output, "Infinite solution\n");
            break;

        case INIT_VALUE:
            fprintf(file_output, "INIT VALUE IN COUNT_SOL ERROR\n");
            fclose(file_output);
            return;

        default:
            fprintf(file_output, "SWITCH ERROR\n");
            fclose(file_output);
            return;
    }

    line_in_output_file++;

    fclose(file_output);
    return;
}
