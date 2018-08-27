#include "stdafx.h"
#include "HttpHelp.h"




static void ev_handler(struct mg_connection *c, int ev, void *p) {
	if (ev == MG_EV_HTTP_REPLY) {
		c->flags |= MG_F_CLOSE_IMMEDIATELY;
		struct http_message *hm = (struct http_message *) p;
		fwrite(hm->message.p, 1, hm->message.len, stdout);
		putchar('\n');
		exit_flag = 1;
	}
	else if (ev == MG_EV_CLOSE) {
		exit_flag = 1;
	};
}