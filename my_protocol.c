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
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            lwsl_user("LWS_CALLBACK_SERVER_WRITEABLE\n");
            // flags = lws_write_ws_flags(LWS_WRITE_BINARY, pmsg->first, pmsg->final);
            // /* notice we allowed for LWS_PRE in the payload already */
            // int m = lws_write(wsi, ((unsigned char *)pmsg->payload) + LWS_PRE, pmsg->len, flags);
            // if (m < (int)pmsg->len) {
            //     lwsl_err("ERROR %d writing to ws socket\n", m);
            //     return -1;
            // }
            // lws_callback_on_writable(wsi);
            break;

        case LWS_CALLBACK_RECEIVE:
            // lwsl_user("LWS_CALLBACK_RECEIVE: %4d (rpp %5d, first %d, "
            //     "last %d, bin %d, msglen %d (+ %d = %d))\n",
            //     (int)len, (int)lws_remaining_packet_payload(wsi),
            //     lws_is_first_fragment(wsi),
            //     lws_is_final_fragment(wsi),
            //     lws_frame_is_binary(wsi), pss->msglen, (int)len,
            //     (int)pss->msglen + (int)len);
            // amsg.first = lws_is_first_fragment(wsi);
            // amsg.final = lws_is_final_fragment(wsi);
            // amsg.binary = lws_frame_is_binary(wsi);
            // memcpy((char *)amsg.payload + LWS_PRE, in, len);
            // lws_callback_on_writable(wsi);
            break;

        case LWS_CALLBACK_CLOSED:
            lwsl_user("LWS_CALLBACK_CLOSED\n");
            break;

        default:
            break;
	}

	return 0;
}