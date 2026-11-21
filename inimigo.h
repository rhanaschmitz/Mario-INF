#ifndef INIMIGO_H
#define INIMIGO_H

#include "raylib.h"

typedef struct {
    float x, y;
    float velocidadeX;
    int ativo;       // 1 se vivo, 0 se morto
    int vulneravel;  // 1 se vulnerável (modo vulnerável periódico)
} Inimigo;

#define MAX_INIMIGOS 10

extern Inimigo inimigos[MAX_INIMIGOS];
extern int totalInimigos;

void InicializarInimigos(void);
void AdicionarInimigo(float x, float y);
void AtualizarInimigos(void);
void DesenharInimigos(void);

#endif