#ifndef _CLIENT_H
#define _CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <csim.h>
#include "common.h"
#include "gaussiana_inversa.h"
#include "service.h"


int web_client(double document_size, int variant, int bool_transient, int iter);

void web_session(int cli_id, int variant, int bool_transient, int iter);

int get_doc_class(double doc_size);

#endif
