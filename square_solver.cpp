/* main.cpp simple square solver */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define COLOR_TEXT(STR, COLOR) "\033[" COLOR "m" STR "\033[0m"

#define ASSERT_ANOTHER_DO(condition, func) if (!condition) {printf("Assertion failed: " #condition "function %s, file %s, line %d", __FUNCTION__, __FILE__, __LINE__); func;}

#define DOUBLE       "%lg"
#define UNSIGNED_INT "%ud"
#define CHAR         "%c"
#define INT          "%d"

#define RED    "91"
#define GREEN  "92"
#define BLUE   "94"
#define YELLOW "33"

#define EPSILON 1e-6

const size_t MAX_LEN_COEF = 200;

const int COUNT_TESTS = 1000;

const char FILE_INPUT_NAME[]  = "input.txt";
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
error_code_e read_one_from_terminal  (void* const coef, const char* const type, const char* const message);
error_code_e read_one_from_file      (void* const coef, const char* const type, FILE* const file_input, int* const line_in_input_file);
error_code_e print_equation          (const coefficients_s* const coefs);

error_code_e linear_solver(const coefficients_s* const coefs, solutions_s* const sol);
error_code_e square_solver(const coefficients_s* const coefs, solutions_s* const sol);

error_code_e where_print_output      (const solutions_s* const sol);
error_code_e print_output_to_terminal(const solutions_s* const sol);
error_code_e print_output_to_file    (const solutions_s* const sol);

error_code_e run_all_tests           ();
error_code_e run_test_with_coefs     (const coefficients_s* const coefs);
error_code_e run_test_with_sol       (const solutions_s* const ref_sol);
error_code_e run_manual_test         (const coefficients_s* const coefs, const solutions_s* const ref_sol);
error_code_e check_answer            (const coefficients_s* const coefs, const solutions_s* const sol);
bool are_the_solution_correct        (const coefficients_s* const coefs, const double sol);
coefficients_s restoring_coefficients(const solutions_s* const sol);

error_code_e clean_buffer (FILE* const file_input);
bool is_equally           (const double lhs, const double rhs);
double get_random_number  ();
double fix_negative_zero  (const double num);



int main(int argc, char* argv[]) {
    coefficients_s coefs = {.coef_a = NAN, .coef_b = NAN, .coef_c = NAN};
    solutions_s sol = {.count_sol = INIT_VALUE, .first_sol = NAN, .second_sol = NAN};

    if (argc != 1 && strcmp(argv[1], "test") == 0) {

        if (run_all_tests() == ERROR_IN_TEST) {
            printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    if (where_from_read_input(&coefs) == READ_INPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return EXIT_FAILURE;
    }

    if (print_equation(&coefs) == PRINT_OUTPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return EXIT_FAILURE;
    }

    if (square_solver(&coefs, &sol) == ERROR_IN_MATH) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return EXIT_FAILURE;
    }

    if (where_print_output(&sol) == PRINT_OUTPUT_ERROR) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


error_code_e where_from_read_input(coefficients_s* coefs) {
    assert(coefs != NULL);

    int variant_input;
    error_code_e error_code = SUCCESS;
    bool is_input_read = false;

    read_one_from_terminal(&variant_input, CHAR,
            "If you want read input from terminal - " COLOR_TEXT("press T\n", YELLOW)
            "else, if you want read input from file - " COLOR_TEXT("press F\n", YELLOW));



    while (!is_input_read) {
        switch (variant_input) {
            case 'T':
                error_code = read_input_from_terminal(coefs);
                is_input_read = true;
                if (error_code == READ_INPUT_ERROR) {
                    printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
                }

                break;
            case 'F':
                error_code = read_input_from_file(coefs);
                is_input_read = true;
                if (error_code == READ_INPUT_ERROR) {
                    printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
                }

                break;
            default:
                printf(COLOR_TEXT("Incorrect input\n", RED));
                read_one_from_terminal(&variant_input, CHAR,
                "If you want read input from terminal - " COLOR_TEXT("press T\n", YELLOW)
                "else, if you want read input from file - " COLOR_TEXT("press F\n", YELLOW));
        }
    }

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


error_code_e read_one_from_terminal(void* const coef, const char* const type ,const char* message) {
    assert(coef != NULL);
    assert(type != NULL);
    assert(message != NULL);

    error_code_e error_code = SUCCESS;
    bool is_num_read = false;
    printf("%s", message);

    while ((is_num_read = (bool)scanf(type, coef)) != 1 || clean_buffer(stdin)) {
        if(!is_num_read ) {
            clean_buffer(stdin);
        }


        printf(COLOR_TEXT("Incorrect input\n", RED));
        printf("%s", message);
    }

    return error_code;
}


error_code_e read_one_from_file(void* const coef, const char* const type, FILE* const file_input, int* const line_in_input_file) {
    assert(coef != NULL);
    assert(type != NULL);
    assert(file_input != NULL);
    assert(line_in_input_file != NULL);

    if (fscanf(file_input, type, coef) != 1 || clean_buffer(file_input)) {
        printf("%s:%d  " COLOR_TEXT("Incorrect input", RED) "\n", FILE_INPUT_NAME, *line_in_input_file);
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return READ_INPUT_ERROR;
    }

    (*line_in_input_file)++;

    return SUCCESS;
}


error_code_e print_equation(const coefficients_s* const coefs) {
    assert(isfinite(coefs->coef_a));
    assert(isfinite(coefs->coef_b));
    assert(isfinite(coefs->coef_c));

    if (is_equally(coefs->coef_a, 0) && is_equally(coefs->coef_b, 0)) {
        printf("%lg = 0\n", coefs->coef_c);
        return SUCCESS;
    }

    char str_coef_a[MAX_LEN_COEF] = "";
    char str_coef_b[MAX_LEN_COEF] = "";
    char str_coef_c[MAX_LEN_COEF] = "";

    char sign_a[4] = "";
    char sign_b[4] = " + ";
    char sign_c[4] = " + ";

    if (coefs->coef_a < 0) {
        sign_a[0] = '-';
    }

    if (is_equally(coefs->coef_b, 0)) {
        sign_b[0] = '\0';
    } else if (coefs->coef_b < 0 && !is_equally(coefs->coef_a, 0)) {
        sign_b[2] = '-';
    } else if (coefs->coef_b < 0 && is_equally(coefs->coef_a, 0)) {
        sign_b[0] = '-';
        sign_c[1] = '\0';
    } else if (coefs->coef_b > 0 && is_equally(coefs->coef_a, 0)) {
        sign_b[0] = '\0';
    }

    if (is_equally(coefs->coef_c, 0)) {
        sign_c[0] = '\0';
    } else if (coefs->coef_c < 0) {
        sign_c[2] = '-';
    }

    if (!is_equally(coefs->coef_a, 0)) {
        size_t size_a = 0;
        if (!is_equally(coefs->coef_a, 1)) {
            snprintf(str_coef_a, MAX_LEN_COEF, "%lg", abs(coefs->coef_a));
            size_a = strlen(str_coef_a);
        }
        snprintf(str_coef_a + size_a, MAX_LEN_COEF - size_a, "%s", "x^2");
    }

    if (!is_equally(coefs->coef_b, 0)) {
        size_t size_b = 0;
        if (!is_equally(coefs->coef_b, 1)) {
            snprintf(str_coef_b, MAX_LEN_COEF, "%lg", abs(coefs->coef_b));
            size_b = strlen(str_coef_b);
        }
        snprintf(str_coef_b + size_b, MAX_LEN_COEF - size_b, "%s", "x");
    }

    if (!is_equally(coefs->coef_c, 0)) {
        snprintf(str_coef_c, MAX_LEN_COEF, "%lg", abs(coefs->coef_c));
    }

    printf("%s%s%s%s%s%s = 0\n", sign_a, str_coef_a, sign_b, str_coef_b, sign_c, str_coef_c);
    return SUCCESS;
}



error_code_e linear_solver(const coefficients_s* const coefs, solutions_s* const sol) {
    assert(isfinite(coefs->coef_b));
    assert(isfinite(coefs->coef_c));

    assert(sol != NULL);

    if (!is_equally(coefs->coef_b, 0)) {
        sol->first_sol = fix_negative_zero(-coefs->coef_c / coefs->coef_b);
        sol->count_sol = ONE_SOLUTION;
        return SUCCESS;
    } else if (is_equally(coefs->coef_c, 0)) {
        sol->count_sol =  INFINITE_SOLUTIONS;
        return SUCCESS;
    } else {
        sol->count_sol = NO_SOLUTIONS;
        return SUCCESS;
    }
}


error_code_e square_solver(const coefficients_s* const coefs, solutions_s* const sol) {
    assert(isfinite(coefs->coef_a));
    assert(isfinite(coefs->coef_b));
    assert(isfinite(coefs->coef_c));

    assert(sol != NULL);

    if (is_equally(coefs->coef_a, 0)) {
        if (linear_solver(coefs, sol) == ERROR_IN_MATH) {

            printf("%s:%d" COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
            return ERROR_IN_MATH;
        }
        return SUCCESS;
    }

    double discriminant = coefs->coef_b * coefs->coef_b - 4 * coefs->coef_a * coefs->coef_c;
    double sqrt_discriminant = sqrt(discriminant);

    if (discriminant < 0) {
        sol->count_sol = NO_SOLUTIONS;
        return SUCCESS;
    } else if (is_equally(discriminant, 0)) {
        sol->first_sol = fix_negative_zero(-coefs->coef_b / (2 * coefs->coef_a));
        sol->count_sol = ONE_SOLUTION;
        return SUCCESS;
    }

    sol->first_sol = fix_negative_zero((-coefs->coef_b + sqrt_discriminant) / (2 * coefs->coef_a));
    sol->second_sol = fix_negative_zero((-coefs->coef_b - sqrt_discriminant) / (2 * coefs->coef_a));

    sol->count_sol = TWO_SOLUTIONS;
    return SUCCESS;
}



error_code_e where_print_output(const solutions_s* const sol) {
    if (sol->count_sol > 0) {
        assert(isfinite(sol->first_sol));
    }

    if (sol->count_sol > 1) {
        assert(isfinite(sol->second_sol));
    }

    int variant_output = 0;
    error_code_e error_code = SUCCESS;
    bool is_input_read = false;

    read_one_from_terminal(&variant_output, CHAR,
            "If you want print output to terminal - " COLOR_TEXT("press T\n", YELLOW)
            "else, if you want print output to file - " COLOR_TEXT("press F\n", YELLOW));

    while (!is_input_read) {
        switch (variant_output) {
            case 'T':
                error_code = print_output_to_terminal(sol);
                is_input_read = true;
                if (error_code == PRINT_OUTPUT_ERROR) {
                    printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
                }
                break;

            case 'F':
                error_code = print_output_to_file(sol);
                is_input_read = true;
                if (error_code == PRINT_OUTPUT_ERROR) {
                    printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
                }
                break;

            default:
                printf(COLOR_TEXT("Incorrect input\n", RED));
                read_one_from_terminal(&variant_output, CHAR,
                "If you want print output to terminal - " COLOR_TEXT("press T\n", YELLOW)
                "else, if you want print output to file - " COLOR_TEXT("press F\n", YELLOW));
        }
    }

    return error_code;

}


error_code_e print_output_to_terminal(const solutions_s* const sol) {
    if (sol->count_sol > 0) {
        assert(isfinite(sol->first_sol));
    }

    if (sol->count_sol > 1) {
        assert(isfinite(sol->second_sol));
    }

    switch (sol->count_sol) {
        case NO_SOLUTIONS:
            printf(COLOR_TEXT("No solution\n", GREEN));
            break;

        case ONE_SOLUTION:
            printf(COLOR_TEXT("One solution: x = %lg\n", GREEN), sol->first_sol);
            break;

        case TWO_SOLUTIONS:
            printf(COLOR_TEXT("Two solution: x1 = %lg x2 = %lg\n", GREEN), sol->first_sol, sol->second_sol);
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


error_code_e print_output_to_file(const solutions_s* const sol) {
    if (sol->count_sol > 0) {
        assert(isfinite(sol->first_sol));
    }

    if (sol->count_sol > 1) {
        assert(isfinite(sol->second_sol));
    }

    FILE* const file_output = fopen(FILE_OUTPUT_NAME, "w");
    if (!file_output) {
        printf(COLOR_TEXT("Error occured while opening ", RED) "%s\n", FILE_OUTPUT_NAME);
        printf("%s:%d" COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);

        return OPEN_FILE_ERROR;
    }


    switch (sol->count_sol) {
        case NO_SOLUTIONS:
            fprintf(file_output, "No solution\n");

            break;

        case ONE_SOLUTION:
            fprintf(file_output, "One solution: x = %lg\n", sol->first_sol);

            break;

        case TWO_SOLUTIONS:
            fprintf(file_output, "Two solution: x1 = %lg x2 = %lg\n", sol->first_sol, sol->second_sol);

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

    printf(COLOR_TEXT("Done\n", GREEN));

    return SUCCESS;
}



error_code_e run_all_tests() {
    unsigned int count_fail_tests = 0;

    srand(time_t());

    for (int i = 0; i < COUNT_TESTS; i++) {
        const coefficients_s coefs = {.coef_a = get_random_number(), .coef_b = get_random_number(), .coef_c = get_random_number()};
        if (run_test_with_coefs(&coefs) == ERROR_IN_TEST) {
            count_fail_tests++;
        }
    }


    for (int i = 0; i < COUNT_TESTS; i++) {
        const solutions_s sol = {.count_sol = TWO_SOLUTIONS, .first_sol = get_random_number(), .second_sol = get_random_number()};
        if (run_test_with_sol(&sol) == ERROR_IN_TEST) {
            count_fail_tests++;
        }
    }


    coefficients_s coefs_tests[] = {
        {.coef_a = NAN, .coef_b = 2, .coef_c = 1}, // coefs_tests[0]
        {.coef_a = 0, .coef_b = 0, .coef_c = 0}, // coefs_tests[1]
        {.coef_a = 0, .coef_b = 0, .coef_c = 5}  // coefs_tests[2]
    };
    size_t size_coefs_tests = sizeof(coefs_tests) / sizeof(coefs_tests[0]);

    solutions_s sol_tests[] = {
        {.count_sol = ONE_SOLUTION, .first_sol = -1, .second_sol = NAN},        // sol_tests[0]
        {.count_sol = INFINITE_SOLUTIONS, .first_sol = NAN, .second_sol = NAN}, // sol_tests[1]
        {.count_sol = NO_SOLUTIONS, .first_sol = NAN, .second_sol = NAN}        // sol_tests[2]
    };
    size_t size_sol_tests = sizeof(sol_tests) / sizeof(sol_tests[0]);

    assert(size_coefs_tests == size_sol_tests);

    for (size_t i = 0; i < size_coefs_tests; i++) {
        assert(0 <= i && i < size_coefs_tests);
        assert(0 <= i && i < size_sol_tests);

        if (run_manual_test(&coefs_tests[i], &sol_tests[i]) == ERROR_IN_TEST) {
            count_fail_tests++;
        }
    }

    if (count_fail_tests == 0) {
        printf(COLOR_TEXT("Count fail tests: %u\n", GREEN), count_fail_tests);
    } else {
        printf(COLOR_TEXT("Count fail tests: %u\n", RED), count_fail_tests);
    }

    return SUCCESS;
}


error_code_e run_test_with_coefs(const coefficients_s* const coefs) {
    assert(isfinite(coefs->coef_a));
    assert(isfinite(coefs->coef_b));
    assert(isfinite(coefs->coef_c));


    solutions_s sol = {.first_sol = NAN, .second_sol = NAN, .count_sol = INIT_VALUE};
    square_solver(coefs, &sol);

    if (check_answer(coefs, &sol) == ERROR_IN_TEST) {
        printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n", __FILE__, __LINE__, __FUNCTION__);
        return ERROR_IN_TEST;
    }

    return SUCCESS;
}


error_code_e run_test_with_sol(const solutions_s* const ref_sol) {
    assert(isfinite(ref_sol->first_sol));
    assert(isfinite(ref_sol->second_sol));

    const coefficients_s coefs = restoring_coefficients(ref_sol);

    solutions_s sol = {.count_sol = INIT_VALUE, .first_sol = NAN, .second_sol = NAN};

    square_solver(&coefs, &sol);

    if ((!is_equally(sol.first_sol, ref_sol->first_sol) || !is_equally(sol.second_sol, ref_sol->second_sol))
        && (!is_equally(sol.second_sol, ref_sol->first_sol) || !is_equally(sol.first_sol, ref_sol->second_sol))) {
        printf(COLOR_TEXT(
            "Error during testing\n"
            "Wrong answer\n", RED)
            "Expected:        x1 = %lg x2 = %lg\n"
            "The found roots: x1 = %lg x2 = %lg\n"
            "a = %lg b = %lg c = %lg\n",
            ref_sol->first_sol, ref_sol->second_sol, sol.first_sol, sol.second_sol, coefs.coef_a, coefs.coef_b, coefs.coef_c);
        return ERROR_IN_TEST;
    }

    return SUCCESS;
}


error_code_e run_manual_test(const coefficients_s* const coefs, const solutions_s* const ref_sol) {
    ASSERT_ANOTHER_DO(isfinite(coefs->coef_a), exit(EXIT_FAILURE));
    assert(isfinite(coefs->coef_b));
    assert(isfinite(coefs->coef_c));

    if (ref_sol->count_sol > 0) {
        assert(isfinite(ref_sol->first_sol));
    }

    if (ref_sol->count_sol > 1) {
        assert(isfinite(ref_sol->second_sol));
    }

    solutions_s sol = {.count_sol = INIT_VALUE, .first_sol = NAN, .second_sol = NAN};

    square_solver(coefs, &sol);

    if (is_equally(sol.count_sol, ref_sol->count_sol) && is_equally(sol.first_sol, ref_sol->first_sol) && is_equally(sol.second_sol, ref_sol->second_sol)) {
        return SUCCESS;
    } else {
        switch (ref_sol->count_sol) {
            case ONE_SOLUTION:
                printf( COLOR_TEXT("Error during testing\n"
                        "Wrong answer\n", RED)
                        "The found roots: x = %lg\n"
                        "a = %lg b = %lg c = %lg\n", sol.first_sol, coefs->coef_a, coefs->coef_b, coefs->coef_c);
                return ERROR_IN_TEST;

            case TWO_SOLUTIONS:
                printf(COLOR_TEXT(
                    "Error during testing\n"
                    "Wrong answer\n", RED)
                    "Expected:        x1 = %lg x2 = %lg\n"
                    "The found roots: x1 = %lg x2 = %lg\n"
                    "a = %lg b = %lg c = %lg\n",
                    ref_sol->first_sol, ref_sol->second_sol, sol.first_sol, sol.second_sol, coefs->coef_a, coefs->coef_b, coefs->coef_c);
                return ERROR_IN_TEST;

            case INFINITE_SOLUTIONS:
                printf( COLOR_TEXT("Error during testing\n"
                        "Wrong answer\n", RED)
                        "Expected: Infinite solution\n"
                        "Count found roots: %d\n", sol.count_sol);
                return ERROR_IN_TEST;

            case NO_SOLUTIONS:
                printf( COLOR_TEXT("Error during testing\n"
                        "Wrong answer\n", RED)
                        "Expected: No solutions solution\n"
                        "Count found roots: %d\n", sol.count_sol);
                return ERROR_IN_TEST;

            case INIT_VALUE:
            default:
                printf("Error in manual tests\n");
                printf("%s:%d  " COLOR_TEXT("Error in ", RED) "%s\n",  __FILE__, __LINE__, __FUNCTION__);
                return ERROR_IN_TEST;
        }
    }

    return SUCCESS;
}


error_code_e check_answer(const coefficients_s* const coefs, const solutions_s* const sol) {
    assert(isfinite(coefs->coef_a));
    assert(isfinite(coefs->coef_b));
    assert(isfinite(coefs->coef_c));

    if (sol->count_sol > 0) {
        assert(isfinite(sol->first_sol));
    }

    if (sol->count_sol > 1) {
        assert(isfinite(sol->second_sol));
    }

    switch (sol->count_sol) {

        case ONE_SOLUTION:
            if (are_the_solution_correct(coefs, sol->first_sol)) {
                printf( COLOR_TEXT("Error during testing\n"
                        "Wrong answer\n", RED)
                        "The found roots: x = %lg\n"
                        "a = %lg b = %lg c = %lg\n"
                        "%s:%d  "  COLOR_TEXT("Error in", RED) " %s\n", sol->first_sol, coefs->coef_a, coefs->coef_b, coefs->coef_c,
                        __FILE__, __LINE__, __FUNCTION__);
                return ERROR_IN_TEST;
            }
            break;

        case TWO_SOLUTIONS:
            if (are_the_solution_correct(coefs, sol->first_sol) || are_the_solution_correct(coefs, sol->second_sol)) {
                printf( COLOR_TEXT("Error during testing\n"
                        "Wrong answer\n", RED)
                        "The found roots: x1 = %lg x2 = %lg\n"
                        "a = %lg b = %lg c = %lg\n"
                        "%s:%d  " COLOR_TEXT("Error in", RED) " %s\n",
                        sol->first_sol, sol->second_sol, coefs->coef_a, coefs->coef_b, coefs->coef_c, __FILE__, __LINE__, __FUNCTION__);
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


bool are_the_solution_correct(const coefficients_s* const coefs, const double sol) {
    assert(isfinite(coefs->coef_a));
    assert(isfinite(coefs->coef_b));
    assert(isfinite(coefs->coef_c));
    assert(isfinite(sol));

    return !is_equally(coefs->coef_a * sol * sol + coefs->coef_b * sol + coefs->coef_c, 0);
}


coefficients_s restoring_coefficients(const solutions_s* const sol) {
    assert(isfinite(sol->first_sol));
    assert(isfinite(sol->second_sol));

    coefficients_s coef = {.coef_a = 1, .coef_b = -(sol->first_sol + sol->second_sol), .coef_c = sol->first_sol * sol->second_sol};

    return coef;
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
    if (isnan(lhs) && isnan(rhs)) {
        return true;
    }

    assert(isfinite(lhs));
    assert(isfinite(rhs));

    return fabs(lhs - rhs) < EPSILON;
}


double get_random_number() {
    return (double)(rand() % 1000000) / (rand() % 5 + 1);
}


double fix_negative_zero(const double num) {
    assert(isfinite(num));

    return (num == -0) ? 0 : num;
}
