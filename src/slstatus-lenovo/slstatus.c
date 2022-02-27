/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>

#include "arg.h"
#include "slstatus.h"
#include "util.h"

struct arg {
	const char *(*func)();
	const char *fmt;
	const char *args;
	const long interval;
	char* buf;
	struct timespec last_time;
	int failures;
};

char buf[1024];
static int done;
static Display *dpy;

#include "config.h"

static void
terminate(const int signo)
{
	(void)signo;

	done = 1;
}

static void
difftimespec(struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec - b->tv_sec - (a->tv_nsec < b->tv_nsec);
	res->tv_nsec = a->tv_nsec - b->tv_nsec +
	               (a->tv_nsec < b->tv_nsec) * 1E9;
}

static void
usage(void)
{
	die("usage: %s [-s]", argv0);
}

static void
refresh(const int signo)
{
	(void)signo;
}

static char status[MAXLEN];
static char res_buf[MAXLEN];
static size_t i;
static int sflag, ret;
const char* res;

static int
should_update(struct arg* arg) {
	if (arg->interval == 0)
		return 1;

	struct timespec diff, now;
	long ms;

	clock_gettime(CLOCK_MONOTONIC, &now);
	difftimespec(&diff, &now, &arg->last_time);
	ms = diff.tv_sec * 1000 + diff.tv_nsec / 1e6;
	
	if (ms < arg->interval)
		return 0;

	arg->last_time = now;
	return 1;
}

static void
prepare_bar()
{
	char* cur = res_buf;
	char* end = res_buf + sizeof(res_buf);

	for (i = 0; i < LEN(args); i++) {
		if (cur + COMMAND_MAXLEN <= end && args[i].interval > 0) {
			args[i].buf = cur;
			cur += COMMAND_MAXLEN;
		}
		else
			args[i].buf = NULL;

		args[i].failures = MAX_FAILURES + 1;
	}
}

static void
update_bar()
{
	size_t len;
	status[0] = '\0';
	for (i = len = 0; i < LEN(args); i++) {
		if (should_update(&args[i])) {
			if (!(res = args[i].func(args[i].args))) {
				args[i].failures++;
				res = unknown_str;
			}
			else
				args[i].failures = 0;

			if (args[i].buf)
				strncpy(args[i].buf, res, COMMAND_MAXLEN);
		}
		else if (!(res = args[i].buf))
			res = unknown_str;

		if (args[i].failures <= MAX_FAILURES) {
			if ((ret = esnprintf(status + len, sizeof(status) - len,
							args[i].fmt, res)) < 0) {
				break;
			}
			len += ret;
		}
	}

	// ugly hack
	if (status[0] == '|')
		memmove(status, status + 1, sizeof(status) - 1);

	if (sflag) {
		puts(status);
		fflush(stdout);
		if (ferror(stdout))
			die("puts:");
	} else {
		if (XStoreName(dpy, DefaultRootWindow(dpy), status)
				   < 0) {
			die("XStoreName: Allocation failed");
		}
		XFlush(dpy);
	}
}

int
main(int argc, char *argv[])
{
	struct sigaction act, hup;
	struct timespec start, current, diff, intspec, wait;

	sflag = 0;
	ARGBEGIN {
		case 's':
			sflag = 1;
			break;
		default:
			usage();
	} ARGEND

	if (argc) {
		usage();
	}

	memset(&act, 0, sizeof(act));
	act.sa_handler = terminate;
	sigaction(SIGINT,  &act, NULL);
	sigaction(SIGTERM, &act, NULL);

	memset(&hup, 0, sizeof(hup));
	hup.sa_handler = refresh;
	sigaction(SIGHUP, &hup, NULL);

	if (!sflag && !(dpy = XOpenDisplay(NULL))) {
		die("XOpenDisplay: Failed to open display");
	}

	prepare_bar();

	while (!done) {
		if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) {
			die("clock_gettime:");
		}
		
		update_bar();

		if (!done) {
			if (clock_gettime(CLOCK_MONOTONIC, &current) < 0) {
				die("clock_gettime:");
			}
			difftimespec(&diff, &current, &start);

			intspec.tv_sec = interval / 1000;
			intspec.tv_nsec = (interval % 1000) * 1E6;
			difftimespec(&wait, &intspec, &diff);

			if (wait.tv_sec >= 0) {
				if (nanosleep(&wait, NULL) < 0 &&
				    errno != EINTR) {
					die("nanosleep:");
				}
			}
		}
	}

	if (!sflag) {
		XStoreName(dpy, DefaultRootWindow(dpy), NULL);
		if (XCloseDisplay(dpy) < 0) {
			die("XCloseDisplay: Failed to close display");
		}
	}

	return 0;
}
