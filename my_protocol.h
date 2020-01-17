#ifndef MY_PROTOCOL_H
#define MY_PROTOCOL_H

#include <libwebsockets.h>

int my_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

/* per session data */
typedef struct my_pss_t {
	uint32_t msglen;
} my_pss_t;

#define LWS_PLUGIN_MY_PROTOCOL \
	{ \
		"lws-my-protocol", \
		my_callback, \
		sizeof(my_pss_t), \
		1024, \
		0, NULL, 0 \
	}

#endif