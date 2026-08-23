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
#define CHAR "%c"
#define INT "%d"

#define RED   "91"
#define GREEN "92"
#define BLUE "94"

#define EPSILON 1e-3

const char FILE_INPUT_NAME[] = "input.txt";
const char FILE_OUTPUT_NAME[] = "output.txt";

enum error_code_e {
    SUCCESS,
    READ_INPUT_ERROR,
    PRINT_OUTPUT_ERROR,
    NOT_EMPTY_BUFFER,
    OPEN_FILE_ERROR,
    ERROR_IN_TEST,
    ERROR_IN_MATH
};

enum count_solutions_e {
    INIT_VALUE = -2,
    INFINITE_SOLUTIONS,
    NO_SOLUTIONS,
    ONE_SOLUTION ,
    TWO_SOLUTIONS
};

struct coefficients_s {
    double coef_a, coef_b, coef_c;
};

struct solutions_s {
    double first_sol, second_sol;
    count_solutions_e count_sol;
};

error_code_e where_from_read_input   (coefficients_s* const coefs);
error_code_e read_input_from_terminal(coefficients_s* const coefs);
error_code_e read_input_from_file    (coefficients_s* const coefs);
error_code_e read_one_from_terminal  (void* const coef, const char* type, const char* str_num);
error_code_e read_one_from_file      (void* const coef, const char* const type, FILE* const file_input, int* const line_in_input_file);

error_code_e linear_solver(const coefficients_s coefs, solutions_s* const sol);
error_code_e square_solver(const coefficients_s coefs, solutions_s* const sol);

error_code_e where_print_output      (const solutions_s sol);
error_code_e print_output_to_terminal(const solutions_s sol);
error_code_e print_output_to_file    (const solutions_s sol);

error_code_e run_tests       ();
error_code_e run_one_test    (const coefficients_s coefs);
error_code_e check_answer    (const coefficients_s coefs, const solutions_s sol);
bool are_the_solution_correct(const coefficients_s coefs, const double sol);

bool is_equally          (const double lhs, const double rhs);
error_code_e clean_buffer(FILE* const file_input);
double get_random_number ();



int main(int argc, char* argv[]) {
    coefficients_s coefs = {.coef_a = NAN, .coef_b = NAN, .coef_c = NAN};
    solutions_s sol = {.count_sol = INIT_VALUE, .first_sol = NAN, .second_sol = NAN};

    if (argc != 1 && strcmp(argv[1], "test") == 0) {

        run_tests();

        return EXIT_FAILURE;
    }

    if (where_from_read_input(&coefs) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return EXIT_FAILURE;
    }

    if (square_solver(coefs, &sol) == ERROR_IN_MATH) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return EXIT_FAILURE;
    }

    if (where_print_output(sol) == PRINT_OUTPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


error_code_e where_from_read_input(coefficients_s* coefs) {
    assert(coefs != NULL);

    int variant_input;

    read_one_from_terminal(&variant_input, CHAR,
            "If you want read input from terminal - press T\n"
            "else, if you want read input from file - press F\n");

    error_code_e error_code = SUCCESS;
    do {
        switch (variant_input) {
            case 'T':
                error_code = read_input_from_terminal(coefs);
                if (error_code == READ_INPUT_ERROR) {
                    printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
                }

                break;
            case 'F':
                error_code = read_input_from_file(coefs);
                if (error_code == READ_INPUT_ERROR) {
                    printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
                }

                break;
            default:
                read_one_from_terminal(&variant_input, CHAR,
                "If you want print output to terminal - press T\n"
                "else, if you want print output to file - press F\n");
        }
    } while (variant_input != 'T' && variant_input != 'F');

    return error_code;
}


error_code_e read_input_from_terminal(coefficients_s* coefs) {
    assert(coefs != NULL);

    printf("Enter the coefficients_s of the square equation:\n");

    read_one_from_terminal(&(coefs->coef_a), DOUBLE, "Enter first coefficient: ");

    read_one_from_terminal(&(coefs->coef_b), DOUBLE, "Enter second coefficient: ");

    read_one_from_terminal(&(coefs->coef_c), DOUBLE, "Enter thread coefficient: ");

    return SUCCESS;
}


error_code_e read_input_from_file(coefficients_s* const coefs) {
    assert(coefs != NULL);

    FILE* file_input = fopen(FILE_INPUT_NAME, "r");
    int line_in_input_file = 1;

    if (!file_input) {
        printf(COLOR_TEXT("Error occured while opening ", RED) "%s\n", FILE_INPUT_NAME);
        printf("%s:%d" COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return OPEN_FILE_ERROR;
    }


    if (read_one_from_file(&(coefs->coef_a), DOUBLE, file_input, &line_in_input_file) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        fclose(file_input);
        return READ_INPUT_ERROR;
    }

    if (read_one_from_file(&(coefs->coef_b), DOUBLE, file_input, &line_in_input_file) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        fclose(file_input);
        return READ_INPUT_ERROR;
    }

    if (read_one_from_file(&(coefs->coef_c), DOUBLE, file_input, &line_in_input_file) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        fclose(file_input);
        return READ_INPUT_ERROR;
    }

    if (fclose(file_input)) {
        printf(COLOR_TEXT("Error occured while closing ", RED) "%s\n", FILE_INPUT_NAME);
        printf("%s:%d" COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return OPEN_FILE_ERROR;
    }

    return SUCCESS;
}


error_code_e read_one_from_terminal(void* const coef, const char* type ,const char* str_num) {
    assert(coef != NULL);
    assert(type != NULL);
    assert(str_num != NULL);

    bool is_num_read = false;
    printf("%s", str_num);

    while ((is_num_read = (bool)scanf(type, coef)) != 1 || clean_buffer(stdin)) {
        if(!is_num_read ) {
            clean_buffer(stdin);
        }

        printf(COLOR_TEXT(
                "Incorrect input\n"
                "Try again\n", RED)
                "%s", str_num);
    }

    return SUCCESS;
}


error_code_e read_one_from_file(void* const coef, const char* const type, FILE* const file_input, int* const line_in_input_file) {
    assert(coef != NULL);
    assert(type != NULL);
    assert(file_input != NULL);

    if (fscanf(file_input, type, coef) != 1 || clean_buffer(file_input)) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", FILE_INPUT_NAME, *line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    (*line_in_input_file)++;

    return SUCCESS;
}



error_code_e linear_solver(const coefficients_s coefs, solutions_s* const sol) {
    assert(isfinite(coefs.coef_b));
    assert(isfinite(coefs.coef_c));

    assert(sol != NULL);

    if (!is_equally(coefs.coef_b, 0)) {
        sol->first_sol = -coefs.coef_c / coefs.coef_b;
        sol->count_sol = ONE_SOLUTION;
        return SUCCESS;
    } else if (is_equally(coefs.coef_c, 0)) {
        sol->count_sol = INFINITE_SOLUTIONS;
        return SUCCESS;
    } else {
        sol->count_sol = NO_SOLUTIONS;
        return SUCCESS;
    }
}


error_code_e square_solver(const coefficients_s coefs, solutions_s* const sol) {
    assert(isfinite(coefs.coef_a));
    assert(isfinite(coefs.coef_b));
    assert(isfinite(coefs.coef_c));

    assert(sol != NULL);

    if (is_equally(coefs.coef_a, 0)) {
        if (linear_solver(coefs, sol) == ERROR_IN_MATH) {

            printf("%s:%d" COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            return ERROR_IN_MATH;
        }
        return SUCCESS;
    }

    double discriminant = coefs.coef_b * coefs.coef_b - 4 * coefs.coef_a * coefs.coef_c;

    if (discriminant < 0) {
        sol->count_sol = NO_SOLUTIONS;
        return SUCCESS;
    } else if (is_equally(discriminant, 0)) {
        sol->first_sol = -coefs.coef_b / (2 * coefs.coef_a);
        sol->count_sol = ONE_SOLUTION;
        return SUCCESS;
    }

    sol->first_sol = (-coefs.coef_b + sqrt(discriminant)) / (2 * coefs.coef_a);
    sol->second_sol = (-coefs.coef_b - sqrt(discriminant)) / (2 * coefs.coef_a);

    sol->count_sol = TWO_SOLUTIONS;
    return SUCCESS;
}



error_code_e where_print_output(const solutions_s sol) {
    if (sol.count_sol > 0) {
        assert(isfinite(sol.first_sol));
    }

    if (sol.count_sol > 1) {
        assert(isfinite(sol.second_sol));
    }

    int variant_output = 0;

    read_one_from_terminal(&variant_output, CHAR,
            "If you want print output to terminal - press T\n"
            "else, if you want print output to file - press F\n");

    error_code_e error_code = SUCCESS;

    do {
        switch (variant_output) {
            case 'T':
                error_code = print_output_to_terminal(sol);
                if (error_code == PRINT_OUTPUT_ERROR) {
                    printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
                }
                break;

            case 'F':
                error_code = print_output_to_file(sol);
                if (error_code == PRINT_OUTPUT_ERROR) {
                    printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
                }
                break;

            default:
                read_one_from_terminal(&variant_output, CHAR,
                "If you want print output to terminal - press T\n"
                "else, if you want print output to file - press F\n");
        }
    } while (variant_output != 'T' && variant_output != 'F');

    return error_code;

}


error_code_e print_output_to_terminal(const solutions_s sol) {
    if (sol.count_sol > 0) {
        assert(isfinite(sol.first_sol));
    }

    if (sol.count_sol > 1) {
        assert(isfinite(sol.second_sol));
    }

    switch (sol.count_sol) {
        case NO_SOLUTIONS:
            printf(COLOR_TEXT("No solution\n", GREEN));
            break;

        case ONE_SOLUTION:
            printf(COLOR_TEXT("One solution: %lg\n", GREEN), sol.first_sol);
            break;

        case TWO_SOLUTIONS:
            printf(COLOR_TEXT("Two solution: %lg %lg\n", GREEN), sol.first_sol, sol.second_sol);
            break;

        case INFINITE_SOLUTIONS:
            printf(COLOR_TEXT("Infinite solution\n", GREEN));
            break;

        case INIT_VALUE:
            printf(COLOR_TEXT("INIT VALUE IN COUNT_SOL ERROR\n", RED));
            printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            return PRINT_OUTPUT_ERROR;

        default:
            printf(COLOR_TEXT("SWITCH ERROR\n", RED));
            printf("%s:%d  " COLOR_TEXT("Error in ", RED) "\n %s", __FILE__, __LINE__, __FUNCTION__);
            return PRINT_OUTPUT_ERROR;
    }

    return SUCCESS;
}


error_code_e print_output_to_file(const solutions_s sol) {
    if (sol.count_sol > 0) {
        assert(isfinite(sol.first_sol));
    }

    if (sol.count_sol > 1) {
        assert(isfinite(sol.second_sol));
    }

    FILE* const file_output = fopen(FILE_OUTPUT_NAME, "w");
    if (!file_output) {
        printf(COLOR_TEXT("Error occured while opening ", RED) "%s\n", FILE_OUTPUT_NAME);
        printf("%s:%d" COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return OPEN_FILE_ERROR;
    }


    switch (sol.count_sol) {
        case NO_SOLUTIONS:
            fprintf(file_output, "No solution\n");

            break;

        case ONE_SOLUTION:
            fprintf(file_output, "One solution: x1 = %lg\n", sol.first_sol);

            break;

        case TWO_SOLUTIONS:
            fprintf(file_output, "Two solution: x1 = %lg x2 = %lg\n", sol.first_sol, sol.second_sol);

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

    if (fclose(file_output)) {
        printf(COLOR_TEXT("Error occured while closing ", RED) "%s\n", FILE_OUTPUT_NAME);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return OPEN_FILE_ERROR;
    }

    printf("Done\n");

    return SUCCESS;
}



error_code_e run_tests() {
    unsigned int count_tests = 0;
    read_one_from_terminal(&count_tests, UNSIGNED_INT, "Enter count tests\n");

    srand(time_t());

    for (unsigned int i = 0; i < count_tests; i++) {
        const coefficients_s coefs = {.coef_a = get_random_number(), .coef_b = get_random_number(), .coef_c = get_random_number()};

        if (run_one_test(coefs) == ERROR_IN_TEST) {
            printf("%s:%d  " COLOR_TEXT("Error in", RED) " %s\n", __FILE__, __LINE__, __FUNCTION__);
            return ERROR_IN_TEST;
        }
    }

    return SUCCESS;
}


error_code_e run_one_test(const coefficients_s coefs) {
    assert(isfinite(coefs.coef_a));
    assert(isfinite(coefs.coef_b));
    assert(isfinite(coefs.coef_c));

    solutions_s sol = {.first_sol = NAN, .second_sol = NAN, .count_sol = INIT_VALUE};
    square_solver(coefs, &sol);

    if (check_answer(coefs, sol) == ERROR_IN_TEST) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
        return ERROR_IN_TEST;
    }

    return SUCCESS;
}


double get_random_number() {
    return (double)(rand() % 1000000) / (rand() % 5 + 1);
}


error_code_e check_answer(const coefficients_s coefs, const solutions_s sol) {
    assert(isfinite(coefs.coef_a));
    assert(isfinite(coefs.coef_b));
    assert(isfinite(coefs.coef_c));

    switch (sol.count_sol) {

        case ONE_SOLUTION:
            if (are_the_solution_correct(coefs, sol.first_sol)) {
                printf( "Error during testing\n"
                        "Wrong answer\n"
                        "The found roots: x = %lg\n"
                        "a = %lg b = %lg c = %lg\n"
                        "%s:%d  "  COLOR_TEXT("Error in", RED) " %s\n", sol.first_sol, coefs.coef_a, coefs.coef_b, coefs.coef_c,
                        __FILE__, __LINE__, __FUNCTION__);
                return ERROR_IN_TEST;
            }
            break;

        case TWO_SOLUTIONS:
            if (are_the_solution_correct(coefs, sol.first_sol) || are_the_solution_correct(coefs, sol.second_sol)) {
                printf( "Error during testing\n"
                        "Wrong answer\n"
                        "The found roots: x1 = %lg x2 = %lg\n"
                        "a = %lg b = %lg c = %lg\n"
                        "%s:%d  Error in %s\n", sol.first_sol, sol.second_sol, coefs.coef_a, coefs.coef_b, coefs.coef_c, __FILE__, __LINE__, __FUNCTION__);
                return ERROR_IN_TEST;
            }
            break;

        case INIT_VALUE:
        case INFINITE_SOLUTIONS:
        case NO_SOLUTIONS:
        default:
            return SUCCESS;
    }

    return SUCCESS;
}


bool are_the_solution_correct(const coefficients_s coefs, const double sol) {
    assert(isfinite(coefs.coef_a));
    assert(isfinite(coefs.coef_b));
    assert(isfinite(coefs.coef_c));
    assert(isfinite(sol));

    return !is_equally(coefs.coef_a * sol * sol + coefs.coef_b * sol + coefs.coef_c, 0);
}



error_code_e clean_buffer(FILE* const file_input) {
    assert(file_input != NULL);

    error_code_e error_code = SUCCESS;

    while (fgetc(file_input) != '\n') {
        error_code = NOT_EMPTY_BUFFER;
    }

    return error_code;
}

bool is_equally(const double lhs, const double rhs) {
    assert(isfinite(lhs));
    assert(isfinite(rhs));

    return fabs(lhs - rhs) < EPSILON;
}
