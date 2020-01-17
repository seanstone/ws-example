#include <libwebsockets.h>
#include <string.h>
#include <signal.h>
#include "my_protocol.h"

static int interrupted = 0;
static int port = 7681;

static struct lws_protocols protocols[] = {
	LWS_PLUGIN_MY_PROTOCOL,
	{} /* terminator */
};

static const struct lws_protocol_vhost_options pvo = {
	NULL,				/* "next" pvo linked-list */
	NULL,               /* "child" pvo linked-list */
	"lws-my-protocol",	/* protocol name we belong to on this vhost */
	""				    /* ignored */
};

void sigint_handler(int sig)
{
	interrupted = 1;
}

int main(int argc, const char **argv)
{
	signal(SIGINT, sigint_handler);
	lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);
	lwsl_user("LWS server starting\n");

	struct lws_context_creation_info info = {
        .port               = port,
        .protocols          = protocols,
        .pvo                = &pvo,
        .pt_serv_buf_size   = 32 * 1024,
        .options            = 0,
    };

    struct lws_context* context = lws_create_context(&info);
	if (!context) {
		lwsl_err("lws init failed\n");
		return 1;
	}

    int n = 0;
	while (n >= 0 && !interrupted)
		n = lws_service(context, 0);

	lws_context_destroy(context);

	return 0;
}
