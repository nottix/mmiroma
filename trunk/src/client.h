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

double mu_session = 3.86;
double lambda_session = 9.46;
double alfa_tt = 1.4;
double alfa_obj = 1.33;
double mu_html = 7.63;
double sigma_html = 1.001;
double alfa_html = 1;
double mu_emb = 8.215;
double sigma_emb = 1.46;

int web_client(double document_size);

void web_session(int cli_id, int variant);
