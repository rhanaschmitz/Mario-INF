#include "mapa.h"
#include "jogador.h"
#include "inimigo.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

char mapa[LINHAS][COLUNAS];
int faseAtual = 1;
int totalInimigosFase = 0;
int tempoEsperaInimigos = 0;

// Textura apenas para o chão de tijolinhos
static Texture2D texturaChao;
static int recursosCarregados = 0;

void CarregarFase(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        TraceLog(LOG_ERROR, "FALHA AO ABRIR O ARQUIVO DE MAPA: %s", nomeArquivo);
        return;
    }

    InicializarInimigos();

    if (!recursosCarregados) {
        texturaChao = LoadTexture("floor.png");
        recursosCarregados = 1;
    }

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            mapa[i][j] = ' ';
        }
    }

    char linhaBuffer[COLUNAS + 20];
    int linhaAtual = 0;

    while (linhaAtual < LINHAS && fgets(linhaBuffer, sizeof(linhaBuffer), file) != NULL) {
        
        if (linhaAtual == 28) {
            int qtd = 0, tempo = 0;
            if (sscanf(linhaBuffer, "%d %d", &qtd, &tempo) == 2) {
                totalInimigosFase = qtd;
                tempoEsperaInimigos = tempo;
            } else {
                totalInimigosFase = 3;
                tempoEsperaInimigos = 5;
            }
            break;
        }

        for (int j = 0; j < COLUNAS; j++) {
            char c = linhaBuffer[j];
            if (c == '\n' || c == '\r' || c == '\0') {
                break;
            }

            mapa[linhaAtual][j] = c;

            // Posição inicial do Mário
            if (c == 'm' || c == 'M') {
                InicializarJogador((float)(j * 10), (float)(linhaAtual * 25 + 80));
                mapa[linhaAtual][j] = ' '; 
            }
            // Inimigos ('e', 'E', 's', 'S')
            else if (c == 'e' || c == 'E' || c == 's' || c == 'S') {
                AdicionarInimigo((float)(j * 10), (float)(linhaAtual * 25 + 80));
            }
        }
        linhaAtual++;
    }

    fclose(file);
}

void DesenharMapa(void) {
    // 1. Desenha as plataformas azuis do cenário
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            char c = mapa[i][j];
            if (c == 'p' || c == 'P' || c == '#') {
                DrawRectangle(j * 10, i * 25 + 80, 10, 25, BLUE);
            }
        }
    }

    // 2. Desenha o chão de tijolinhos esticado na base da tela
    if (texturaChao.id > 0) {
        Rectangle origemRec = { 0.0f, 0.0f, (float)texturaChao.width, (float)texturaChao.height };
        Rectangle destinoRec = { 0.0f, 615.0f, 1200.0f, 35.0f }; 
        Vector2 origemVetor = { 0.0f, 0.0f };
        DrawTexturePro(texturaChao, origemRec, destinoRec, origemVetor, 0.0f, WHITE);
    } else {
        DrawRectangle(0, 615, 1200, 35, BROWN);
    }
}

int EBloqueio(float x, float y) {
    int coluna = (int)(x / 10.0f);
    int linha = (int)((y - 80.0f) / 25.0f);

    if (linha < 0 || linha >= LINHAS || coluna < 0 || coluna >= COLUNAS) {
        return 0;
    }

    if (mapa[linha][coluna] == 'p' || mapa[linha][coluna] == 'P' || mapa[linha][coluna] == '#') {
        return 1;
    }

    return 0;
}