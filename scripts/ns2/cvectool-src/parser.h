#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "cvector.h"

SeqVector *parse_seq(const char *header, FILE *fp, int &line);
ConcVector *parse_conc(const char *header, FILE *fp, int &line);

#endif
