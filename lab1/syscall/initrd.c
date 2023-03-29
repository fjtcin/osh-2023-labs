#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
int main() {
	char buf[34] = {0};
	// syscall(548, buf, k) will print the error message,
	// if and only if integer k is in [0, 32]
	if (syscall(548, buf, 33)) {
		puts("Error: Buffer length NOT enough");
		return -1;
	}
	printf("%s", buf);
	return 0;
}
