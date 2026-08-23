/* main.cpp simple square solver */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define COLOR_TEXT(STR, COLOR) "\033[" COLOR "m" STR "\033[0m"

#define DOUBLE "%lg"
#define UNSIGNED_INT "%ud"

#define RED "91"
#define GREEN "92"
#define BLUE "94"

#define DELTA 1e-3

const char file_input_name[] = "input.txt";
const char file_output_name[] = "output.txt";

int line_in_input_file = 1;
int line_in_output_file = 1;

enum Error_Code {
    SUCCESS,
    READ_INPUT_ERROR,
    PRINT_OUTPUT_ERROR,
    NOT_EMPTY_BUFFER,
    OPEN_FILE_ERROR,
    ERROR_IN_TEST
};

enum Count_Solutions {
    INIT_VALUE = -2,
    INFINITE_SOLUTIONS,
    NO_SOLUTIONS,
    ONE_SOLUTION ,
    TWO_SOLUTIONS
};


Error_Code read_input               (double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c);
Error_Code read_input_from_terminal (double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c);
Error_Code read_one_from_terminal   (void* const pt_coef,     const char* tupe,        const char* str_num);
Error_Code read_input_from_file     (double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c);
Error_Code read_one_from_file       (double* const pt_coef,   FILE* file_input);

Count_Solutions linear_solver       (const double coef_b, const double coef_c, double* const pt_x);
Count_Solutions square_solver       (const double coef_a, const double coef_b, const double coef_c, double* const pt_x1, double* const pt_x2);

Error_Code print_output             (const Count_Solutions count_sol, const double first_sol, const double second_sol);
Error_Code print_output_to_terminal (const Count_Solutions count_sol, const double first_sol, const double second_sol);
Error_Code print_output_to_file     (const Count_Solutions count_sol, const double first_sol, const double second_sol);

Error_Code run_one_test             (const double coef_a, const double coef_b, const double coef_c);
Error_Code check_answer             (const double coef_a, const double coef_b, const double coef_c, double first_sol, double second_sol, const Count_Solutions count_sol);
Error_Code run_tests();

bool is_equally                     (const double lhs, const double rhs);
Error_Code clean_buffer             (FILE* file_input);
double get_random_number();



int main(int args, char* argv[]) {
    double coef_a = NAN, coef_b = NAN, coef_c = NAN;
    double first_sol = NAN, second_sol = NAN;
    Count_Solutions count_sol = INIT_VALUE;

    if (args != 1 && strcmp(argv[1], "test") == 0) {
        srand(time_t());

        run_tests();

        return 0;
    }

    if (read_input(&coef_a, &coef_b, &coef_c) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return 0;
    }

    count_sol = square_solver(coef_a, coef_b, coef_c, &first_sol, &second_sol);

    if (print_output(count_sol, first_sol, second_sol) == PRINT_OUTPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return 0;
    }

    return 0;
}


Error_Code read_input(double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c) {
    assert(pt_coef_a != NULL);
    assert(pt_coef_b != NULL);
    assert(pt_coef_c != NULL);

    assert(pt_coef_a != pt_coef_b);
    assert(pt_coef_b != pt_coef_c);
    assert(pt_coef_c != pt_coef_a);

    printf("If you want read input from terminal - press T\n"
           "else, if you want read input from file - press F\n");

    int variant_input = getchar();

    clean_buffer(stdin);

    Error_Code error = SUCCESS;

    switch (variant_input) {
        case 'T':
            error = read_input_from_terminal(pt_coef_a, pt_coef_b, pt_coef_c);
            if (error == READ_INPUT_ERROR) {
                printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            }
            break;
        case 'F':
            error = read_input_from_file(pt_coef_a, pt_coef_b, pt_coef_c);
            if (error == READ_INPUT_ERROR) {
                printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            }
            break;
        default:
            printf(COLOR_TEXT("Incorrect input", RED) "\n");
            printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            error = READ_INPUT_ERROR;
    }


    return error;
}


Error_Code read_input_from_terminal(double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c) {
    assert(pt_coef_a != NULL);
    assert(pt_coef_b != NULL);
    assert(pt_coef_c != NULL);

    assert(pt_coef_a != pt_coef_b);
    assert(pt_coef_b != pt_coef_c);
    assert(pt_coef_c != pt_coef_a);

    printf("Enter the coefficients of the square equation:\n");

    read_one_from_terminal(pt_coef_a, DOUBLE, "Enter first coefficient: ");

    read_one_from_terminal(pt_coef_b, DOUBLE, "Enter second coefficient: ");

    read_one_from_terminal(pt_coef_c, DOUBLE, "Enter thread coefficient: ");

    return SUCCESS;
}


Error_Code read_one_from_terminal(void* const pt_coef, const char* type ,const char* str_num) {
    bool is_num_read = false;
    printf("%s", str_num);

    while ((is_num_read = (bool)scanf(type, pt_coef)) != 1 || clean_buffer(stdin)) {
        if(!is_num_read ) {
            clean_buffer(stdin);
        }

        printf(COLOR_TEXT(
                "Incorrect input\n"
                "Try again\n", RED)
                "Enter %s coefficient: ", str_num);
    }

    return SUCCESS;
}


Error_Code read_input_from_file(double* const pt_coef_a, double* const pt_coef_b, double* const pt_coef_c) {
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

    if (read_one_from_file(pt_coef_a, file_input) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    if (read_one_from_file(pt_coef_b, file_input) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    if (read_one_from_file(pt_coef_c, file_input)) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    fclose(file_input);

    return SUCCESS;
}


Error_Code read_one_from_file(double* const pt_coef, FILE* file_input) {
    if (fscanf(file_input, "%lg", pt_coef) != 1 || clean_buffer(file_input)) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", file_input_name, line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    line_in_input_file++;

    return SUCCESS;
}


Error_Code clean_buffer(FILE* file_input) {
    Error_Code error = SUCCESS;

    while (fgetc(file_input) != '\n') {
        error = NOT_EMPTY_BUFFER;
    }

    return error;
}


Count_Solutions linear_solver(const double coef_b, const double coef_c, double* const pt_x) {
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));
    assert(pt_x != NULL);

    if (!is_equally(coef_b, 0)) {
        *pt_x = -coef_c / coef_b;
        return ONE_SOLUTION;
    } else if (is_equally(coef_c, 0)) {
        return INFINITE_SOLUTIONS;
    } else {
        return NO_SOLUTIONS;
    }
}


Count_Solutions square_solver(const double coef_a, const double coef_b, const double coef_c, double* const pt_x1, double* const pt_x2) {
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
        return NO_SOLUTIONS;
    } else if (is_equally(discriminant, 0)) {
        *pt_x1 = -coef_b / (2 * coef_a);
        return ONE_SOLUTION;
    }

    *pt_x1 = (-coef_b + sqrt(discriminant)) / (2 * coef_a);
    *pt_x2 = (-coef_b - sqrt(discriminant)) / (2 * coef_a);

    return TWO_SOLUTIONS;
}


bool is_equally(const double lhs, const double rhs) {
    assert(isfinite(lhs));
    assert(isfinite(rhs));

    return fabs(lhs - rhs) < DELTA;
}


Error_Code print_output(const Count_Solutions count_sol, const double first_sol, const double second_sol) {
    if (count_sol > 0) {
        assert(isfinite(first_sol));
    }

    if (count_sol > 1) {
        assert(isfinite(second_sol));
    }

    printf("If you want print output to terminal - press T\n"
           "else, if you want print output to file - press F\n");

    int variant_output = getchar();

    clean_buffer(stdin);

    Error_Code error = SUCCESS;

    switch (variant_output) {
        case 'T':
            error = print_output_to_terminal(count_sol, first_sol, second_sol);
            if (error == PRINT_OUTPUT_ERROR) {
                printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            }
            break;
        case 'F':
            error = print_output_to_file(count_sol, first_sol, second_sol);
            if (error == PRINT_OUTPUT_ERROR) {
                printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            }
            break;
        default:
            printf(COLOR_TEXT("Incorrect output", RED) "\n");
            printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            error = PRINT_OUTPUT_ERROR;
    }


    return error;

}


Error_Code print_output_to_terminal(Count_Solutions count_sol, const double first_sol, const double second_sol) {
    if (count_sol > 0) {
        assert(isfinite(first_sol));
    }

    if (count_sol > 1) {
        assert(isfinite(second_sol));
    }

    switch (count_sol) {
        case NO_SOLUTIONS:
            printf(COLOR_TEXT("No solution\n", GREEN));
            break;

        case ONE_SOLUTION:
            printf(COLOR_TEXT("One solution: %lg\n", GREEN), first_sol);
            break;

        case TWO_SOLUTIONS:
            printf(COLOR_TEXT("Two solution: %lg %lg\n", GREEN), first_sol, second_sol);
            break;

        case INFINITE_SOLUTIONS:
            printf(COLOR_TEXT("Infinite solution\n", GREEN));
            break;

        case INIT_VALUE:
            printf(COLOR_TEXT("INIT VALUE IN COUNT_SOL ERROR\n", RED));
            printf(COLOR_TEXT("%s:%d  Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            return PRINT_OUTPUT_ERROR;

        default:
            printf(COLOR_TEXT("SWITCH ERROR\n", RED));
            printf(COLOR_TEXT("%s:%d  Error in ", RED) "\n %s", __FILE__, __LINE__, __FUNCTION__);
            return PRINT_OUTPUT_ERROR;
    }

    return SUCCESS;
}

Error_Code print_output_to_file(Count_Solutions count_sol, const double first_sol, const double second_sol) {
    if (count_sol > 0) {
        assert(isfinite(first_sol));
    }

    if (count_sol > 1) {
        assert(isfinite(second_sol));
    }

    FILE* file_output = fopen(file_output_name, "w");

    switch (count_sol) {

        case NO_SOLUTIONS:
            fprintf(file_output, "No solution\n");
            break;

        case ONE_SOLUTION:
            fprintf(file_output, "One solution: x1 = %lg\n", first_sol);
            break;

        case TWO_SOLUTIONS:
            fprintf(file_output, "Two solution: x1 = %lg x2 = %lg\n", first_sol, second_sol);
            break;

        case INFINITE_SOLUTIONS:
            fprintf(file_output, "Infinite solution\n");
            break;

        case INIT_VALUE:
            printf(COLOR_TEXT("INIT VALUE IN COUNT_SOL ERROR\n", RED));
            printf(COLOR_TEXT("%s:%d  Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

            fclose(file_output);
            return PRINT_OUTPUT_ERROR;

        default:
            printf(COLOR_TEXT("SWITCH ERROR\n", RED));
            printf(COLOR_TEXT("%s:%d  Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

            fclose(file_output);
            return PRINT_OUTPUT_ERROR;
    }

    line_in_output_file++;

    fclose(file_output);
    return SUCCESS;
}


Error_Code run_tests() {
    unsigned int count_tests = 0;

    read_one_from_terminal(&count_tests, UNSIGNED_INT, "Enter count tests\n");

    for (unsigned int i = 0; i < count_tests; i++) {
        const double coef_a = get_random_number(), coef_b = get_random_number(), coef_c = get_random_number();

        if (run_one_test(coef_a, coef_b, coef_c) == ERROR_IN_TEST) {
            printf("%s:%d  " COLOR_TEXT("Error in", RED) " %s\n", __FILE__, __LINE__, __FUNCTION__);
            return ERROR_IN_TEST;
        }
    }

    return SUCCESS;
}


Error_Code run_one_test(const double coef_a, const double coef_b, const double coef_c) {
    double first_sol = NAN, second_sol = NAN;
    Count_Solutions count_sol = square_solver(coef_a, coef_b, coef_c, &first_sol, &second_sol);

    if (check_answer(coef_a, coef_b, coef_c, first_sol, second_sol, count_sol) == ERROR_IN_TEST) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
        return ERROR_IN_TEST;
    }

    return SUCCESS;
}


double get_random_number() {
    return rand() % 1000000;
}


Error_Code check_answer(const double coef_a, const double coef_b, const double coef_c, double first_sol, double second_sol, const Count_Solutions count_sol) {
    assert(isfinite(coef_a));
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));

    switch (count_sol) {

        case ONE_SOLUTION:
            if (!is_equally(coef_a * first_sol * first_sol + coef_b * first_sol + coef_c, 0)) {
                printf( "Error during testing\n"
                        "Wrong answer\n"
                        "The found roots: x = %lg\n"
                        "a = %lg b = %lg c = %lg\n"
                        "%s:%d  "  COLOR_TEXT("Error in", RED) " %s\n", first_sol, coef_a, coef_b, coef_c, __FILE__, __LINE__, __FUNCTION__);
                return ERROR_IN_TEST;
            }
            break;

        case TWO_SOLUTIONS:
            if (!is_equally(coef_a * first_sol * first_sol + coef_b * first_sol + coef_c, 0) || !is_equally(coef_a * second_sol * second_sol + coef_b * second_sol + coef_c, 0)) {
                printf( "Error during testing\n"
                        "Wrong answer\n"
                        "The found roots: x1 = %lg x2 = %lg\n"
                        "a = %lg b = %lg c = %lg\n"
                        "%s:%d  Error in %s\n", first_sol, second_sol, coef_a, coef_b, coef_c, __FILE__, __LINE__, __FUNCTION__);
                return ERROR_IN_TEST;
            }
            break;
        default:
            return SUCCESS;
    }

    return SUCCESS;
}
