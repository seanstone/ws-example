#define LWS_PLUGIN_STATIC

#include <libwebsockets.h>
#include <string.h>
#include "my_protocol.h"

struct msg {
    size_t len;
	void *payload;
};

typedef struct vhd_t {
	struct lws_context* context;
	struct lws_vhost*   vhost;
} vhd_t;

void my_write(struct lws *wsi)
{
    char buffer[LWS_PRE + 10];
    char* msg = "hello";
    snprintf(&buffer[LWS_PRE], sizeof(msg), msg);
    lws_write(wsi, &buffer[LWS_PRE], sizeof(msg), lws_write_ws_flags(LWS_WRITE_TEXT, 1, 1));
}

int my_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	struct my_pss_t* pss = (my_pss_t*) user;
	struct vhd_t* vhd = (vhd_t*) lws_protocol_vh_priv_get(lws_get_vhost(wsi), lws_get_protocol(wsi));

	switch (reason)
    {
        case LWS_CALLBACK_PROTOCOL_INIT:
            lwsl_warn("LWS_CALLBACK_PROTOCOL_INIT\n");
            vhd = lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi), lws_get_protocol(wsi), sizeof(vhd_t));
            if (!vhd) return -1;
            vhd->context = lws_get_context(wsi);
            vhd->vhost   = lws_get_vhost(wsi);
            break;

        case LWS_CALLBACK_ESTABLISHED:
            lwsl_warn("LWS_CALLBACK_ESTABLISHED\n");
            my_write(wsi);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            lwsl_user("LWS_CALLBACK_SERVER_WRITEABLE\n");
            my_write(wsi);
            break;

        case LWS_CALLBACK_RECEIVE:
            lwsl_user("LWS_CALLBACK_RECEIVE: len %d (rpp %d, first %d, last %d, bin %d)\n",
                (int)len, (int)lws_remaining_packet_payload(wsi),
                lws_is_first_fragment(wsi),
                lws_is_final_fragment(wsi),
                lws_frame_is_binary(wsi));
            lws_callback_on_writable(wsi);
            // memcpy((char *)amsg.payload + LWS_PRE, in, len);
            break;

        case LWS_CALLBACK_CLOSED:
            lwsl_user("LWS_CALLBACK_CLOSED\n");
            break;

        default:
            break;
	}

	return 0;
}