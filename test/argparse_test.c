#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../argparse.h"

struct parse_result {
	bool alpha;
	const char *beta;
	const char *positional;
};

static int oh_alpha(void *data, const argparse_option_t *opt, const char *arg) {
	(void)opt;
	struct parse_result *res = data;
	assert(!arg);
	res->alpha = true;
	return 0;
}

static int oh_beta(void *data, const argparse_option_t *opt, const char *arg) {
	(void)opt;
	struct parse_result *res = data;
	assert(arg);
	res->beta = arg;
	return 0;
}

static int oh_gamma(void *data, const argparse_option_t *opt, const char *arg) {
	(void)data;
	(void)opt;
	(void)arg;
	return 0;
}

static int oh_positional(void *data, const argparse_option_t *opt, const char *arg) {
	(void)opt;
	struct parse_result *res = data;
	assert(arg);
	res->positional = arg;
	return 0;
}

static const argparse_option_t options[] = {
	{'a', "alpha", NULL, "option alpha", oh_alpha},
	{'b', "beta", "arg", "option beta", oh_beta},
	{'g', "gamma", "the____________________argument",
		"an option takes argument called 'the____________________argument'", oh_gamma},
	{0, NULL, "xxx", "positional argument", oh_positional},
	{0, NULL, NULL, NULL, NULL},
};

int main(void) {
	{
		const char *argv[] = {"", "-a", "-b", "hello"};
		int argc = sizeof argv / sizeof argv[0];
		struct parse_result res = { false, NULL, NULL };
		const int status = argparse_parse(options, argc, (char **)argv, &res);
		if (!(status == 0 && res.alpha && res.beta && !strcmp(res.beta, "hello")))
			return EXIT_FAILURE;
	}
	{
		const char *argv[] = {"", "--alpha", "--beta", "hello"};
		int argc = sizeof argv / sizeof argv[0];
		struct parse_result res = { false, NULL, NULL };
		const int status = argparse_parse(options, argc, (char **)argv, &res);
		if (!(status == 0 && res.alpha && res.beta && !strcmp(res.beta, "hello")))
			return EXIT_FAILURE;
	}
	{
		const char *argv[] = {"", "-bhello"};
		int argc = sizeof argv / sizeof argv[0];
		struct parse_result res = { false, NULL, NULL };
		const int status = argparse_parse(options, argc, (char **)argv, &res);
		if (!(status == 0 && res.beta && !strcmp(res.beta, "hello")))
			return EXIT_FAILURE;
	}
	{
		char arg0[] = "", arg1[] = "--beta=hello";
		char *argv[] = {arg0, arg1};
		int argc = sizeof argv / sizeof argv[0];
		struct parse_result res = { false, NULL, NULL };
		const int status = argparse_parse(options, argc, argv, &res);
		if (!(status == 0 && res.beta && !strcmp(res.beta, "hello")))
			return EXIT_FAILURE;
	}
	{
		const char *argv[] = {"", "hello"};
		int argc = sizeof argv / sizeof argv[0];
		struct parse_result res = { false, NULL, NULL };
		const int status = argparse_parse(options, argc, (char **)argv, &res);
		if (!(status == 0 && res.positional && !strcmp(res.positional, "hello")))
			return EXIT_FAILURE;
	}

	{
		const char *argv[] = {"", "-c"};
		int argc = sizeof argv / sizeof argv[0];
		struct parse_result res = { false, NULL, NULL };
		const int status = argparse_parse(options, argc, (char **)argv, &res);
		if (!(ARGPARSE_GETERROR(status) == ARGPARSE_ERR_BADOPT
				&& ARGPARSE_GETINDEX(status) == 1))
			return EXIT_FAILURE;
	}
	{
		const char *argv[] = {"", "-ahello"};
		int argc = sizeof argv / sizeof argv[0];
		struct parse_result res = { false, NULL, NULL };
		const int status = argparse_parse(options, argc, (char **)argv, &res);
		if (!(ARGPARSE_GETERROR(status) == ARGPARSE_ERR_NOARG
				&& ARGPARSE_GETINDEX(status) == 1))
			return EXIT_FAILURE;
	}
	{
		const char *argv[] = {"", "-b"};
		int argc = sizeof argv / sizeof argv[0];
		struct parse_result res = { false, NULL, NULL };
		const int status = argparse_parse(options, argc, (char **)argv, &res);
		if (!(ARGPARSE_GETERROR(status) == ARGPARSE_ERR_NEEDARG
				&& ARGPARSE_GETINDEX(status) == 1))
			return EXIT_FAILURE;
	}

	{
		const argparse_program_t prog = {
			.name = "test",
			.usage = "[-a|--alpha] [-b|--beta] positional",
			.help = "argparse test",
			.opts = options,
		};
		argparse_help(&prog);
	}
}
