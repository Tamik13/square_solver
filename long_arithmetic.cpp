#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>

#define ASSERT_FOR_ARR(arr, ind)     \
    assert(0 <= ind && ind < (long long)(sizeof(arr) / sizeof(arr[ind])));

#define LONG_ARTHM_LEN 100
#define BLOCK_MAX 999999999
#define COUNT_DIGIT_IN_BLOCK 9

enum sign_e {
    PLUS = 0,
    MINUS = 1
};

struct long_arithmetic_int {
    unsigned int digits[LONG_ARTHM_LEN] = {0};
    bool sign = false;
};

void int_to_long_arithmetic_int(const int num, long_arithmetic_int* const lng_num);

long_arithmetic_int sum_long_arithmetic_int           (const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2);
long_arithmetic_int multiplication_long_arithmetic_int(const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2);
long_arithmetic_int subtraction_long_arithmetic_int   (const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2);
bool is_equally_long_arithmetic_int                   (const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2);
bool is_less_long_arithmetic_int                      (const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2);
void assigning_long_arithmetic_int                    (long_arithmetic_int* const lhs_lng_num, long_arithmetic_int* rhs_lng_num);
void reset_to_zero_long_arithmetic_int                (long_arithmetic_int* const lng_num);

void read_long_arithmetic_int(long_arithmetic_int* const lng_int);
void print_long_arithmetic_int (const long_arithmetic_int* const lng_num);



int main() {
    long_arithmetic_int num1, num2;

    read_long_arithmetic_int(&num1);
    read_long_arithmetic_int(&num2);

    // print_long_arithmetic_int(&num1);
    // printf("\n");
    // print_long_arithmetic_int(&num2);
    // printf("\n");

    num1 = multiplication_long_arithmetic_int(&num1, &num2);

    print_long_arithmetic_int(&num1);

    return 0;
}


void int_to_long_arithmetic_int(const int num, long_arithmetic_int* const lng_num) {
    assert(lng_num != NULL);

    ASSERT_FOR_ARR(lng_num->digits, 0);

    lng_num->digits[0] = (unsigned int)num;

    if (num < 0) {
        lng_num->sign = true;
    }
}


long_arithmetic_int sum_long_arithmetic_int(const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2) {
    assert(lng_num1 != NULL);
    assert(lng_num2 != NULL);

    bool from_previous_block = 0;
    long_arithmetic_int lng_new_num;

    for (int i = 0; i < LONG_ARTHM_LEN; i++) {
        ASSERT_FOR_ARR(lng_num1->digits, i);
        ASSERT_FOR_ARR(lng_num2->digits, i);

        lng_new_num.digits[i] = lng_num1->digits[i] + lng_num2->digits[i] + from_previous_block;

        if (BLOCK_MAX - lng_num1->digits[i] < lng_num2->digits[i] + from_previous_block) {
            from_previous_block = 1;
        } else {
            from_previous_block = 0;
        }
    }
    return lng_new_num;
}


long_arithmetic_int multiplication_long_arithmetic_int(const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2) {
    assert(lng_num1 != NULL);
    assert(lng_num2 != NULL);

    long_arithmetic_int lng_new_num;

    for (int i = 0; i < LONG_ARTHM_LEN; i++) {
        unsigned int from_previous_block = 0;

        ASSERT_FOR_ARR(lng_num1->digits, i);

        if (lng_num1->digits[i] == 0) {
            continue;
        }

        for (int j = 0; j + i < LONG_ARTHM_LEN; j++) {
            ASSERT_FOR_ARR(lng_new_num.digits, i + j);
            ASSERT_FOR_ARR(lng_num1->digits, i);
            ASSERT_FOR_ARR(lng_num2->digits, j);

            unsigned int tmp = lng_new_num.digits[i + j];
            lng_new_num.digits[i + j] = (unsigned int)(((unsigned long long)lng_num1->digits[i] * lng_num2->digits[j] + from_previous_block + tmp) % (BLOCK_MAX + 1));
            from_previous_block = (unsigned int)(((unsigned long long)lng_num1->digits[i] * lng_num2->digits[j] + from_previous_block + tmp) / (BLOCK_MAX + 1));
        }
    }

    lng_new_num.sign = lng_num1->sign ^ lng_num2->sign;

    return lng_new_num;
}


bool is_equally_long_arithmetic_int(const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2) {
    assert(lng_num1 != NULL);
    assert(lng_num2 != NULL);

    for (int i = 0; i < LONG_ARTHM_LEN; i++) {
        ASSERT_FOR_ARR(lng_num1->digits, i);
        ASSERT_FOR_ARR(lng_num2->digits, i);

        if (lng_num1->digits[i] != lng_num2->digits[i]) {
            return false;
        }
    }

    return true;
}


bool is_less_long_arithmetic_int(const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2) {
    assert(lng_num1 != NULL);
    assert(lng_num2 != NULL);

    if (is_equally_long_arithmetic_int(lng_num1, lng_num2)) {
        return false;
    }

    for (int i = 0; i < LONG_ARTHM_LEN; i++) {
        ASSERT_FOR_ARR(lng_num1->digits, i);
        ASSERT_FOR_ARR(lng_num2->digits, i);

        if (lng_num1->digits[i] > lng_num2->digits[i]) {
            return false;
        }
    }

    return true;
}


void assigning_long_arithmetic_int(long_arithmetic_int* const lhs_lng_num, long_arithmetic_int* rhs_lng_num) {
    assert(lhs_lng_num != NULL);
    assert(rhs_lng_num != NULL);

    for (int i = 0; i < LONG_ARTHM_LEN; i++) {
        ASSERT_FOR_ARR(lhs_lng_num->digits, i);
        ASSERT_FOR_ARR(rhs_lng_num->digits, i);

        lhs_lng_num->digits[i] = rhs_lng_num->digits[i];
    }
}


void reset_to_zero_long_arithmetic_int(long_arithmetic_int* const lng_num) {
    assert(lng_num != NULL);

    long_arithmetic_int zero_num;

    assigning_long_arithmetic_int(lng_num, &zero_num);
}



void print_long_arithmetic_int(const long_arithmetic_int* const lng_num) {
    assert(lng_num != NULL);

    int i = LONG_ARTHM_LEN - 1;
    long_arithmetic_int lng_zero;

    if (is_equally_long_arithmetic_int(lng_num, &lng_zero)) {
        return;
    }

    if (lng_num->sign) {
        printf("-");
    }

    ASSERT_FOR_ARR(lng_num->digits, i);
    while (lng_num->digits[i] == 0) {
        ASSERT_FOR_ARR(lng_num->digits, i);;
        i--;
    }

    ASSERT_FOR_ARR(lng_num->digits, i);
    printf("%u", lng_num->digits[i]);
    i--;

    for (; i >= 0; i--) {
        ASSERT_FOR_ARR(lng_num->digits, i);
        printf("%09u", lng_num->digits[i]);
    }
}


void read_long_arithmetic_int(long_arithmetic_int* const lng_num) {
    assert(lng_num != 0);

    int symbol = '\0';
    char str_num[LONG_ARTHM_LEN * COUNT_DIGIT_IN_BLOCK + 1];
    size_t len_str_num = 0;
    int digit = 0;

    reset_to_zero_long_arithmetic_int(lng_num);

    while (isspace(symbol = getchar()) || symbol == '0') {
        continue;
    }

    if (isdigit(symbol)) {
        ASSERT_FOR_ARR(str_num, len_str_num);
        str_num[len_str_num++] = (char)symbol;
    } else if (symbol == '-') {
        lng_num->sign = true;
    }

    while (isdigit(symbol = getchar()) && len_str_num < LONG_ARTHM_LEN * COUNT_DIGIT_IN_BLOCK) {
        ASSERT_FOR_ARR(str_num, len_str_num);
        str_num[len_str_num++] = (char)symbol;
    }

    ASSERT_FOR_ARR(str_num, len_str_num);
    str_num[len_str_num--] = '\0';

    for (int i = (int)len_str_num; i >= 0;) {
        unsigned int degree_of_ten = 1;

        for (int j = 0; j < COUNT_DIGIT_IN_BLOCK && i >= 0; j++, i--) {
            ASSERT_FOR_ARR(lng_num->digits, digit);
            ASSERT_FOR_ARR(str_num, i);

            lng_num->digits[digit] += (unsigned int)(str_num[i] - '0') * degree_of_ten;
            degree_of_ten *= 10;
        }

        digit++;
    }
}



