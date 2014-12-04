/*
 * nspider.c
 */
#include <nspider.h>

void (*nspr_handler)(void);
int (*nspr_log_error)(const char *, ...);

/*
 * we use xxx_null function to avoid null pointer
 * of extern variables we export
 */
int nspr_log_error_null(const char *format, ...)
{
    // do nothing
    return NSPR_OK;
}

void nspr_handler_null(void)
{
    // do nothing
}

static int nspr_option_show_help = 0;
static int nspr_option_show_version = 0;
/*
 * XXX: the code come from [nginx.c:ngx_get_options]
 */
static int nspr_get_options(int argc, char *const *argv)
{
    unsigned char  *p;
    int             i;

    for (i = 1; i < argc; i++) {

        p = (unsigned char *) argv[i];

        if (*p++ != '-') {
            nspr_log_error("invalid option: \"%s\"", argv[i]);
            return NSPR_ERROR;
        }

        while (*p) {
            switch (*p++) {
            case '?':
            case 'h':
                nspr_option_show_help = 1;
                break;
            case 'V':
            case 'v':
                nspr_option_show_version = 1;
                break;
            default:
                nspr_log_error("invalid option: \"%c\"\n", *(p - 1));
                return NSPR_ERROR;
            }
        }

        continue;
    }

    return NSPR_OK;
}

static int nspr_log_init(void)
{
    int i;
    int ret = NSPR_ERROR;
    for (i = 0; nspr_nspider_modules[i]; i++) {
        if (nspr_nspider_modules[i]->id == NSPR_ID_LOG) {
            ret = nspr_nspider_modules[i]->init();
            break;
        }
    }
    return ret;
}

static int nspr_modules_init(void)
{
    int i;
    int ret = NSPR_ERROR;
    for (i = 0; nspr_nspider_modules[i]; i++) {
        if (nspr_nspider_modules[i]->init) {
            ret = nspr_nspider_modules[i]->init();
            if (ret != NSPR_OK)
                break;
        }
    }
    return ret;
}

static void nspr_modules_exit(void)
{
    int i;
    for (i = 0; nspr_nspider_modules[i]; i++) {
        if (nspr_nspider_modules[i]->exit) {
            nspr_nspider_modules[i]->exit();
        }
    }
}

int main(int argc, char *const *argv)
{
    if (nspr_log_error == NULL)
        nspr_log_error = nspr_log_error_null;
    if (nspr_handler == NULL)
        nspr_handler = nspr_handler_null;

    // do log init, we need it to show something not expected
    if (nspr_log_init() != NSPR_OK) {
        return 1;
    }

    if (nspr_get_options(argc, argv) != NSPR_OK)
        return 1;

    if (nspr_option_show_help == 1) {
        nspr_log_error(
                "nspider version: nspider/0.1.0\n"
                "Usage: nspider [-?hvV]\n\n"
                "Options:\n"
                "-?,-h          : this help\n"
                "-v             : show version and exit\n"
                "-V             : show version and exit\n"
                );
        return 1;
    }

    if (nspr_option_show_version == 1) {
        nspr_log_error(
                "nspider version: nspider/0.1.0\n"
                );
        return 1;
    }

    // TODO do initing works
    if (nspr_modules_init() != NSPR_OK) {
        nspr_log_error("modules init failed!\n");
        return 1;
    }
    // TODO do handling works
        (void) nspr_handler();
    // TODO do exiting works
    nspr_modules_exit();
    return 0;
}
