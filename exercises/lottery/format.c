#include <stdio.h>
#include <sys/types.h>

void vuln(char* str, u_int32_t secret1, u_int32_t secret2, u_int32_t secret3) {
    printf(str);
}

int main(int argc, char* argv[]) {
    vuln(argv[1],19148,60837,31436);
    return 0;
}

