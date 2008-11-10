#ifndef _CLIENT_H
#define _CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <csim.h>
#include "common.h"
#include "workload.h"
#include "service.h"

// Ritorna l'indice del server meno utilizzato (serve per la least loaded)
int get_least_loaded();

//Gestisce l'intero flusso richiesta-risposta
int web_client(double document_size, int variant, int bool_transient, int iter);

//Generazione di una sessione e delle relative richieste 
void web_session(int cli_id, int variant, int bool_transient, int iter);

//assegna una classe ad un documento, sulla base delle distanze da i centroidi
int get_doc_class(double doc_size);

#endif
