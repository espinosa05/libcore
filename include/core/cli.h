#ifndef __CORE_CLI_H__
#define __CORE_CLI_H__

#include <core/types.h>
#include <core/error_report.h>

enum opt_status_codes {
    OPT_NO_ERR = 0,
    OPT_ERR_INVALID_OPT,
    OPT_ERR_EXPECTED_SUBOPT,
    OPT_ERR_UNKNOWN,
};

#define REQUIRED_ARGUMENT 1
#define NO_ARGUMENT 0

#define INVALID_OPT_VAL -1
struct cli_args {
    usz     c;
    char    **v;
};

/* definition for option value */
struct cli_opt {
    s32 id;
    /* short option value (E.g.: '-o') */
    char short_opt;
    /* long option value (E.g.: '--output' */
    char *long_opt;
    /* select for sub option */
    b32 has_arg;
    /* flag description (E.g.: 'sets a custom path for the output file') */
    char *desc;
};

/* definition for 'cli_getopt' result */
struct cli_opt_result {
    /* unique enum value */
    s32 id;
    /* error code */
    sz err_code;
    /* the argument of the option, if supported */
    char *arg;
    /* last cli parameter index */
    sz opt_ind;
};

/* sentinel initializer for 'Opt_Result' */
#define INIT_OPT_RESULT_INVALID {       \
    .id = -1,                           \
    .err_code = -1,                     \
    .arg = NULL,                        \
    .opt_ind = -1                       \
}

/* sentinel initializer for 'Opt' */
#define INIT_OPT_INVALID {          \
    .id = -1,                       \
    .short_opt = INVALID_OPT_VAL,   \
    .long_opt = NULL,               \
    .has_arg = 0,                   \
    .desc = NULL,                   \
}

#define CLI_GETOPT_SUCCESS(opt) (opt.err_code == OPT_NO_ERR)

#define CLI_OPT_NULL_ENTRY 1
struct cli_opt_result cli_getopt(const struct cli_opt opt_arr[],
                                 usz n_ots, usz *p_counter, struct cli_args args);

const char *cli_getopt_string_error(usz err_code);

#endif /* __CORE_CLI_H__ */
