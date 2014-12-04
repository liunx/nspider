/*
 * nspider.c
 */
#include <nspider.h>

void (*snpr_handler)(void);
int (*nspr_log_error)(const char *, ...);

/*
 * XXX: the code come from nginx.c:ngx_get_options
 */
#if 0
static int nspr_get_options(int argc, char *const *argv)
{
    unsigned char  *p;
    int             i;

    for (i = 1; i < argc; i++) {

        p = (unsigned char *) argv[i];

        if (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            return NGX_ERROR;
        }

        while (*p) {

            switch (*p++) {

            case '?':
            case 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                break;

            case 'v':
                ngx_show_version = 1;
                break;

            case 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                break;

            case 't':
                ngx_test_config = 1;
                break;

            case 'q':
                ngx_quiet_mode = 1;
                break;

            case 'p':
                if (*p) {
                    ngx_prefix = p;
                    goto next;
                }

                if (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                return NGX_ERROR;

            case 'c':
                if (*p) {
                    ngx_conf_file = p;
                    goto next;
                }

                if (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                return NGX_ERROR;

            case 'g':
                if (*p) {
                    ngx_conf_params = p;
                    goto next;
                }

                if (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                return NGX_ERROR;

            case 's':
                if (*p) {
                    ngx_signal = (char *) p;

                } else if (argv[++i]) {
                    ngx_signal = argv[i];

                } else {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    return NGX_ERROR;
                }

                if (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    goto next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                return NGX_ERROR;

            default:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                return NGX_ERROR;
            }
        }

    next:

        continue;
    }

    return NSPR_OK;
}
#endif

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
    // TODO do log init, we need it to show something not expected
    if (nspr_log_init() != NSPR_OK) {
        return 1;
    }
    nspr_log_error("hello from nspider!\n");
    // TODO get options
    // TODO do initing works
    if (nspr_modules_init() != NSPR_OK) {
        nspr_log_error("modules init failed!\n");
        return 1;
    }
    // TODO do handling works
    if (snpr_handler)
        (void) snpr_handler();
    // TODO do exiting works
    nspr_modules_exit();
    return 0;
}
