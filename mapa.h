#ifndef MAPA_H
#define MAPA_H

#define COLUNAS 120
#define LINHAS 29

extern char mapa[LINHAS][COLUNAS];
extern int faseAtual;
extern int totalInimigosFase;
extern int tempoEsperaInimigos;

void CarregarFase(const char *nomeArquivo);
void DesenharMapa(void);
int EBloqueio(float x, float y);

#endif