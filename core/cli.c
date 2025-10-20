#include <core/cli.h>
#include <core/log.h>
#include <core/utils.h>

#include <core/cstd.h>
#include <string.h>

/* static function declaration start */
static struct cli_opt load_long_opt(const char *arg, const struct cli_opt opts[], usz count);
static struct cli_opt load_short_opt(const char *arg, const struct cli_opt opts[], usz count);
static b32 is_last_opt(const struct cli_opt opt);
/* static function declaration end */

struct cli_opt_result cli_getopt(const struct cli_opt opt_arr[], usz n_opts, usz *p_counter, struct cli_args args)
{
    struct cli_opt_result res   = INIT_OPT_RESULT_INVALID;
    struct cli_opt opt_entry     = {0};
    const char *arg             = NULL;
    usz counter                 = *p_counter;

    ASSERT_RT(is_last_opt(opt_arr[n_opts - 1]),
              "corrupt data! last entry of 'struct cli_opt' array needs to be {0}");

    res.err_code = OPT_NO_ERR;
    n_opts -= CLI_OPT_NULL_ENTRY; /* we will always ignore the last entry */

    if (counter == 0) /* skip argv[0] */
        counter = 1;

    ASSERT_RT(args.c >= counter,
              "index not inside argument range! (%d)", counter);

    res.opt_ind = counter;
    arg = args.v[counter];

    if (arg[0] != '-') {
        res.err_code = OPT_ERR_INVALID_OPT;
        return res;
    }

    /* consume current arg */
    counter++;

    /* is long argument? (double dash) */
    if (arg[1] == '-') {
        opt_entry = load_long_opt(arg, opt_arr, n_opts);
    } else {
        opt_entry = load_short_opt(arg, opt_arr, n_opts);
    }

    if (opt_entry.id == -1)
        res.err_code = OPT_ERR_INVALID_OPT;

    res.id = opt_entry.id;

    if (opt_entry.has_arg) {
        if (args.c >= counter + 1) {
            res.arg = args.v[counter];
            counter++; /* skip option */
        } else {
            res.err_code = OPT_ERR_EXPECTED_SUBOPT;
        }
    }

    *p_counter = counter;

    return res;
}

static const char *g_error_messages[] = {
    [OPT_NO_ERR]                = "No Error",
    [OPT_ERR_INVALID_OPT]       = "Unknown Argument!",
    [OPT_ERR_EXPECTED_SUBOPT]   = "Option requires additional argument!",
    [OPT_ERR_UNKNOWN]           = "Unknown Error!",
};

const char *cli_getopt_string_error(usz err_code)
{
    if (err_code > OPT_ERR_UNKNOWN)
        err_code = OPT_ERR_UNKNOWN;

    return g_error_messages[err_code];
}
/* constants for skipping the dashes */
enum arg_offsets {
    SHORT_ARG_OFFSET = 1,
    LONG_ARG_OFFSET = 2,
};

static struct cli_opt load_long_opt(const char *arg, const struct cli_opt opts[], usz count)
{
    struct cli_opt ret = INIT_OPT_INVALID;

    arg += LONG_ARG_OFFSET;

    for (usz i = 0; i < count; ++i) {
        if (0 == strcmp(opts[i].long_opt, arg)) {
            ret = opts[i];
            break;
        }
    }

    return ret;
}

static struct cli_opt load_short_opt(const char *arg, const struct cli_opt opts[], usz count)
{
    struct cli_opt ret = INIT_OPT_INVALID;
    char short_arg;

    arg += SHORT_ARG_OFFSET;
    short_arg = arg[0];

    for (usz i = 0; i < count; ++i) {
        if (short_arg == opts[i].short_opt) {
            ret = opts[i];
            break;
        }
    }
    return ret;
}

static b32 is_last_opt(const struct cli_opt opt)
{
    return (opt.short_opt == 0
            && opt.long_opt == NULL
            && opt.has_arg == 0
            && opt.desc == NULL);
}


