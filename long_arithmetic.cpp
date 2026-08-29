#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>

#define ASSERT_FOR_ARR(ind, size)     \
    assert(0 <= ind && i < size)

#define LONG_ARTHM_LEN 100
#define BLOCK_MAX 999999999
#define COUNT_DIGIT_IN_BLOCK 9


struct long_arithmetic_int {
    unsigned int arr[LONG_ARTHM_LEN] = {0};
};

void int_to_long_arithmetic_int(const unsigned int num, long_arithmetic_int* const lng_num);

long_arithmetic_int sum_long_arithmetic_int           (const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2);
long_arithmetic_int multiplication_long_arithmetic_int(const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2);
void assigning_long_arithmetic_int(long_arithmetic_int* const lhs_lng_num, long_arithmetic_int* rhs_lng_num);
void reset_to_zero_long_arithmetic_int(long_arithmetic_int* const lng_num);

void read_long_arithmetic_int(long_arithmetic_int* const lng_int);
void print_long_arithmetic_int (const long_arithmetic_int* const lng_num);



int main() {
    long_arithmetic_int num1, num2;

    read_long_arithmetic_int(&num1);
//     int_to_long_arithmetic_int(BLOCK_MAX, &num2);
//
//     long_arithmetic_int new_num = multiplication_long_arithmetic_int(&num1, &num2);

    print_long_arithmetic_int(&num1);

    return 0;
}


void int_to_long_arithmetic_int(const unsigned int num, long_arithmetic_int* const lng_num) {
    assert(lng_num != NULL);

    lng_num->arr[0] = (unsigned long long)num;
}


long_arithmetic_int sum_long_arithmetic_int(const long_arithmetic_int* const lng_num1, const long_arithmetic_int* const lng_num2) {
    assert(lng_num1 != NULL);
    assert(lng_num2 != NULL);

    bool from_previous_block = 0;
    long_arithmetic_int lng_new_num;

    for (int i = 0; i < LONG_ARTHM_LEN; i++) {
        ASSERT_FOR_ARR(i, LONG_ARTHM_LEN);
        lng_new_num.arr[i] = lng_num1->arr[i] + lng_num2->arr[i] + from_previous_block;

        if (BLOCK_MAX - lng_num1->arr[i] < lng_num2->arr[i] + from_previous_block) {
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

        if (lng_num1->arr[i] == 0) {
            continue;
        }

        for (int j = 0; j + i < LONG_ARTHM_LEN; j++) {
            lng_new_num.arr[i + j] += (((unsigned long long)lng_num1->arr[i] * lng_num2->arr[j] + from_previous_block) % (BLOCK_MAX + 1));
            from_previous_block = ((unsigned long long)lng_num1->arr[i] * lng_num2->arr[j] + from_previous_block) / (BLOCK_MAX + 1);
        }
    }

    return lng_new_num;
}


void assigning_long_arithmetic_int(long_arithmetic_int* const lhs_lng_num, long_arithmetic_int* rhs_lng_num) {
    assert(lhs_lng_num != NULL);
    assert(rhs_lng_num != NULL);

    for (int i = 0; i < LONG_ARTHM_LEN; i++) {
        lhs_lng_num->arr[i] = rhs_lng_num->arr[i];
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

    while (lng_num->arr[i] == 0) i--;

    printf("%u", lng_num->arr[i]);
    i--;

    for (; i >= 0; i--) {
        printf("%09u", lng_num->arr[i]);
    }
}


void read_long_arithmetic_int(long_arithmetic_int* const lng_num) {
    assert(lng_num != 0);

    char symbol = '\0';
    char str_num[LONG_ARTHM_LEN * COUNT_DIGIT_IN_BLOCK + 1];
    size_t len_str_num = 0;
    int digit = 0;

    reset_to_zero_long_arithmetic_int(lng_num);

    while (isspace(symbol = getchar()) || symbol == '0') {
        continue;
    }

    str_num[len_str_num++] = symbol;

    while ('0' <= (symbol = getchar()) && symbol <= '9' && len_str_num < LONG_ARTHM_LEN * COUNT_DIGIT_IN_BLOCK) {
        str_num[len_str_num++] = symbol;
    }
    str_num[len_str_num--] = '\0';

    for (int i = len_str_num; i >= 0;) {
        unsigned int degree_of_ten = 1;

        for (int j = 0; j < COUNT_DIGIT_IN_BLOCK && i >= 0; j++, i--) {
            lng_num->arr[digit] += (str_num[i] - '0') * degree_of_ten;
            degree_of_ten *= 10;
        }

        digit++;
    }
}



