#ifndef MY_PROTOCOL_H
#define MY_PROTOCOL_H

#include <libwebsockets.h>

typedef struct vhd_t {
	struct lws_context* context;
	struct lws_vhost*   vhost;
} vhd_t;

/* per session data */
typedef struct my_pss_t {
	uint32_t msglen;
} my_pss_t;

int my_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
int client_connect(vhd_t* vhd);

#define LWS_PLUGIN_MY_PROTOCOL \
	{ \
		"lws-my-protocol", \
		my_callback, \
		sizeof(my_pss_t), \
		1024, \
		0, NULL, 0 \
	}

#endif