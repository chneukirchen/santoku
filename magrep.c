#include <sys/stat.h>
#include <sys/types.h>

#include <ctype.h>
#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "blaze822.h"
#include "xpledge.h"

static int aflag;
static int cflag;
static int dflag;
static int hflag;
static int iflag;
static int lflag;
static int oflag;
static int pflag;
static int qflag;
static int vflag;
static long mflag;
static long matches;

static regex_t pattern;
static char *header;
static char *curfile;

int
match(char *file, char *hdr, char *s)
{
	if (oflag && !cflag && !qflag && !vflag && !lflag) {
		regmatch_t pmatch = {0};
		int len, matched;
		matched = 0;
		while (s && *s && regexec(&pattern, s, 1, &pmatch, 0) == 0) {
			s += pmatch.rm_so;
			if (!(len = pmatch.rm_eo-pmatch.rm_so)) {
				s += 1;
				continue;
			}
			if (pflag)
				printf("%s: %s: ", file, hdr);
			else if (hflag)
				printf("%s: ", hdr);
			printf("%.*s\n", len, s);
			s += len;
			matched++;
		}
		return (matched && matches++);
	} else if (vflag ^ (s && regexec(&pattern, s, 0, 0, 0) == 0)) {
		if (qflag)
			exit(0);
		matches++;
		if (!cflag) {
			if (vflag || !hflag)
				printf("%s", file);
			if (pflag && !vflag)
				printf(": %s: %s", hdr, s);
			else if (hflag && !vflag)
				printf("%s: %s", hdr, s);
			putchar('\n');
		}
		if (mflag && matches >= mflag)
			exit(0);
		return 1;
	}

	return 0;
}

blaze822_mime_action
match_part(int depth, struct message *msg, char *body, size_t bodylen)
{
	(void)depth;

	char *ct = blaze822_hdr(msg, "content-type");

	blaze822_mime_action r = MIME_CONTINUE;

	if (!ct || strncmp(ct, "text/plain", 10) == 0) {
		char *charset = 0, *cs, *cse;
		if (blaze822_mime_parameter(ct, "charset", &cs, &cse))
			charset = strndup(cs, cse-cs);
		if (!charset ||
		    strcasecmp(charset, "utf-8") == 0 ||
		    strcasecmp(charset, "utf8") == 0 ||
		    strcasecmp(charset, "us-ascii") == 0) {
			if ((hflag || pflag) && !cflag && !oflag && !vflag) {
				char *s, *p;
				for (p = s = body; p < body+bodylen+1; p++) {
					if (*p == '\r' || *p == '\n') {
						*p = 0;
						if (p-s > 1)
							match(curfile, "/", s);
						s = p+1;
					}
				}
			} else if (match(curfile, "/", body)) {
				r = MIME_STOP;
			}
		} else {
			/* XXX decode here */
		}
		free(charset);
	}

	return r;
}

void
match_body(char *file)
{
	char *filename;
	filename = curfile = file;
	while (*filename == ' ' || *filename == '\t')
		filename++;

	struct message *msg = blaze822_file(filename);
	if (!msg)
		return;

	blaze822_walk_mime(msg, 0, match_part);
	blaze822_free(msg);
}

int
match_value(char *file, char *h, char *v)
{
	if (dflag) {
		char d[4096];
		blaze822_decode_rfc2047(d, v, sizeof d, "UTF-8");
		return match(file, h, d);
	} else if (aflag) {
		char *disp, *addr;
		while ((v = blaze822_addr(v, &disp, &addr))) {
			if (addr && match(file, h, addr))
				return 1;
		}
	} else {
		return match(file, h, v);
	}
	return 0;
}

void
magrep(char *file)
{
	if (!*header) {
		char *flags = strstr(file, ":2,");
		if (flags)
			match(file, "flags", flags+3);
		return;
	} else if (strcmp(header, "/") == 0) {
		match_body(file);
		return;
	}

	char *filename = file;
	while (*filename == ' ' || *filename == '\t')
		filename++;

	struct message *msg = blaze822(filename);
	if (!msg)
		return;

	if (strcmp(header, "*") == 0) {
		char *hdr = 0;
		while ((hdr = blaze822_next_header(msg, hdr))) {
			char *v = strchr(hdr, ':');
			if (v) {
				*v = 0;
				if (match_value(file, hdr, v + 1 + (v[1] == ' ')) && lflag)
					break;
				*v = ':';
			}
		}
	} else {
		char *v = blaze822_chdr(msg, header);
		if (v)
			(void)match_value(file, header, v);
		else
			(void)match(file, header, 0);
	}

	blaze822_free(msg);
}

int
main(int argc, char *argv[])
{
	int c;
	while ((c = getopt(argc, argv, "acdhilm:opqv")) != -1)
		switch (c) {
		case 'a': aflag = 1; break;
		case 'c': cflag = 1; break;
		case 'd': dflag = 1; break;
		case 'h': hflag = 1; break;
		case 'i': iflag = REG_ICASE; break;
		case 'l': lflag = 1; break;
		case 'm': mflag = atol(optarg); break;
		case 'o': oflag = 1; break;
		case 'p': pflag = 1; break;
		case 'q': qflag = 1; break;
		case 'v': vflag = 1; break;
		default:
usage:
			fprintf(stderr,
"Usage: magrep [-c|-h|-o|-p|-q|-m max] [-v] [-i] [-l] [-a|-d] header:regex [msgs...]\n");
			exit(2);
		}

	if (argc == optind)
		goto usage;
	header = argv[optind++];

	char *rx = strchr(header, ':');
	if (!rx)
		goto usage;

	xpledge("stdio rpath", "");

	*rx++ = 0;
	int r = regcomp(&pattern, rx, REG_EXTENDED | iflag);
	if (r != 0) {
		char buf[256];
		regerror(r, &pattern, buf, sizeof buf);
		fprintf(stderr, "magrep: regex error: %s\n", buf);
		exit(2);
	}

	if (argc == optind && isatty(0))
		blaze822_loop1(":", magrep);
	else
		blaze822_loop(argc-optind, argv+optind, magrep);

	if (cflag && !qflag && !mflag)
		printf("%ld\n", matches);

	return !matches;
}
