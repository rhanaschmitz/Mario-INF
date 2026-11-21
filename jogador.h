#ifndef JOGADOR_H
#define JOGADOR_H

typedef struct {
    float x, y;       
    float velocidadeY; 
    int vidas;        
    int pontos;       
    int noChao;       
} Jogador;

extern Jogador mario;

void InicializarJogador(float xInicial, float yInicial);
void AtualizarJogador(void);
void DesenharJogador(void);

#endif