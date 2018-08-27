#pragma once

#include "mongoose.h"

static const char *url = "http://www.google.com";
static int exit_flag = 0;

static void ev_handler(struct mg_connection *c, int ev, void *p);
