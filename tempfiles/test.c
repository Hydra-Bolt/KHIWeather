#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <integer>\n", argv[0]);
        return 1; // Exit with an error code
    }

    char *str_num = argv[1];
    int converted_num = atoi(str_num);

    // Display the result
    printf("String: %s\n", str_num);
    printf("Integer: %d\n", converted_num);

    return 0;
}
