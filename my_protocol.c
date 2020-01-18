#define LWS_PLUGIN_STATIC

#include <libwebsockets.h>
#include <string.h>
#include "my_protocol.h"

void my_write(struct lws *wsi, char* msg, size_t len)
{
    char buffer[LWS_PRE + len];
    memcpy(&buffer[LWS_PRE], msg, len);
    lws_write(wsi, &buffer[LWS_PRE], len, lws_write_ws_flags(LWS_WRITE_BINARY, 1, 1));
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
            client_connect(vhd);
            break;

        case LWS_CALLBACK_ESTABLISHED: {
            lwsl_warn("LWS_CALLBACK_ESTABLISHED\n");
            char* msg = "hello";
            my_write(wsi, msg, strlen(msg));
        } break;

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            lwsl_user("LWS_CALLBACK_SERVER_WRITEABLE\n");
            char* msg = "hello";
            my_write(wsi, msg, strlen(msg));
        } break;

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

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            lwsl_user("LWS_CALLBACK_CLIENT_ESTABLISHED\n");
            break;

        case LWS_CALLBACK_CLIENT_WRITEABLE:
            lwsl_user("LWS_CALLBACK_CLIENT_WRITEABLE\n");
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            lwsl_user("LWS_CALLBACK_CLIENT_RECEIVE: %4d (rpp %5d, first %d, last %d, bin %d)\n",
                (int)len, (int)lws_remaining_packet_payload(wsi),
                lws_is_first_fragment(wsi),
                lws_is_final_fragment(wsi),
                lws_frame_is_binary(wsi));
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            lwsl_err("CLIENT_CONNECTION_ERROR\n");
            break;

        case LWS_CALLBACK_CLIENT_CLOSED:
            lwsl_user("LWS_CALLBACK_CLIENT_CLOSED\n");
            break;

        default:
            break;
	}

	return 0;
}

int client_connect(vhd_t* vhd)
{
	struct lws_client_connect_info i = {
        .context    = vhd->context,
        .vhost      = vhd->vhost,
        .address    = "localhost",
        .port       = 7681,
        .path       = "",
        .host       = "",
        .origin     = "",
    };
	return !lws_client_connect_via_info(&i);
}