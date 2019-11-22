#define _GNU_SOURCE
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <syscall.h>
static char child_stack[1048576];

static int child_fn(void* arg) {
	unshare(CLONE_NEWNET);
	system("mount -t proc proc /proc --make-private");
	printf("New `net` Namespace:\n");
	system("ip link");
	printf("\n\n");
	system("sudo -S umount /proc");
	return 0;
}

int main() {
	printf("Original `net` Namespace:\n");
	system("ip link");
	printf("\n\n");
	pid_t child_pid = clone(child_fn, child_stack+1048576, CLONE_NEWPID| CLONE_NEWNET | SIGCHLD, NULL);
	waitpid(child_pid, NULL, 0);
	_exit(EXIT_SUCCESS);
}
