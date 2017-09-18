#include <sys/wait.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int
filter(char *input, size_t inlen, char *cmd, char **outputo, size_t *outleno)
{
	char *output;
	ssize_t outlen;
	ssize_t outalloc = 4096;
	pid_t pid;
	sigset_t mask, orig_mask;
	int r;

	sigemptyset(&mask);
	sigaddset(&mask, SIGPIPE);
	sigprocmask(SIG_BLOCK, &mask, &orig_mask);

	outlen = 0;
	output = malloc(outalloc);
	if (!output)
		goto fail;

	int pipe0[2];
	int pipe1[2];

	if (pipe(pipe0) != 0 || pipe(pipe1) != 0)
		goto fail;

	int got = fcntl(pipe0[1], F_GETFL);
	if (got > 0)
		fcntl(pipe0[1], F_SETFL, got | O_NONBLOCK);

	char *argv[] = { "/bin/sh", "-c", cmd, (char *)0 };

	if (!(pid = fork())) {
		dup2(pipe0[0], 0);
		close(pipe0[1]);
		close(pipe0[0]);

		dup2(pipe1[1], 1);
		close(pipe1[0]);
		close(pipe1[1]);

		execvp(argv[0], argv);
		exit(-1);
	}
	close(pipe0[0]);
	close(pipe1[1]);

	if (pid < 0) {
		close(pipe0[1]);
		close(pipe1[0]);
		goto fail;
	}

	struct pollfd fds[2];

	fds[0].fd = pipe1[0];
	fds[0].events = POLLIN | POLLHUP;
	fds[1].fd = pipe0[1];
	fds[1].events = POLLOUT;

	while ((fds[0].fd >= 0 || fds[1].fd >= 0) &&
	    poll(fds, sizeof fds / sizeof fds[0], -1) >= 0) {
		if (fds[0].revents & POLLIN) {
			if (outlen + 512 > outalloc) {
				outalloc *= 2;
				if (outalloc < 0)
					exit(-1);
				output = realloc(output, outalloc);
				if (!output)
					exit(-1);
			}
			ssize_t ret = read(fds[0].fd, output + outlen, 512);
			if (ret > 0)
				outlen += ret;
			else
				close(fds[0].fd);
		} else if (fds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
			fds[0].fd = -1;
		}

		if (fds[1].revents & POLLOUT) {
			ssize_t ret = write(fds[1].fd, input, inlen);
			if (ret > 0) {
				input += ret;
				inlen -= ret;
			}
			if (ret <= 0 && errno == EAGAIN) {
				/* ignore */
			} else if (ret <= 0 || inlen == 0)
				close(fds[1].fd);
		} else if (fds[1].revents & (POLLERR | POLLHUP | POLLNVAL)) {
			fds[1].fd = -1;
		}
	}

	// ok to fail when closed already
	close(pipe0[1]);
	close(pipe1[0]);

	int status;
	waitpid(pid, &status, 0);
	r = WEXITSTATUS(status);

	*outputo = output;
	*outleno = outlen;

	if (0) {
fail:
		*outputo = 0;
		*outleno = 0;
		free(output);
		r = -1;
	}

	sigpending(&mask);
	if (sigismember(&mask, SIGPIPE)) {
		int sig;
		sigwait(&mask, &sig);
	}
	sigprocmask(SIG_SETMASK, &orig_mask, 0);

	return r;
}

#ifdef TEST
int
main()
{
	char *input = "foo\nbar\nbaz";
	int e;

	char *output;
	size_t outlen;

	e = filter(input, strlen(input), "rev;exit 2", &output, &outlen);

	fwrite(output, 1, outlen, stdout);
	printf("%ld -> %d\n", outlen, e);

	return 0;
}
#endif
