#include <stdio.h>

int log_2(int x) {
    int result = 0;
    while (x > 1) {
        x >>= 1;
        result++;
    }
    return result;
}
