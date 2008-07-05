#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <csim.h>
#include "common.h"
#include "gaussiana_inversa.h"
#include "service.h"

FACILITY cpuWS[NUM_SERVER];
FACILITY diskWS[NUM_DISK];
//FACILITY WS[NUM_SERVER];
FACILITY L2;
FACILITY CPU_web_switch;
FACILITY inLink;
FACILITY outLink;
TABLE wsrtime;
TABLE rtime;

int web_client(double document_size);

void web_session(int client_id, int variant);
