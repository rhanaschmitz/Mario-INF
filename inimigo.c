#include "inimigo.h"
#include "mapa.h"
#include "jogador.h"
#include "raylib.h"

Inimigo inimigos[MAX_INIMIGOS];
int totalInimigos = 0;
Texture2D texturaInimigo;
int texturaInimigoCarregada = 0;

static float tempoAcumulado = 0.0f;
int modoVulneravelGlobal = 0; // Deixamos global para exibir o aviso na main
static float tempoVulneravelRestante = 0.0f;

void InicializarInimigos(void) {
    totalInimigos = 0;
    tempoAcumulado = 0.0f;
    modoVulneravelGlobal = 0;
    tempoVulneravelRestante = 0.0f;
    
    if (!texturaInimigoCarregada) {
        texturaInimigo = LoadTexture("inimigo.png");
        texturaInimigoCarregada = 1;
    }
}

void AdicionarInimigo(float x, float y) {
    if (totalInimigos < MAX_INIMIGOS) {
        inimigos[totalInimigos].x = x;
        inimigos[totalInimigos].y = y;
        inimigos[totalInimigos].velocidadeX = -1.5f;
        inimigos[totalInimigos].ativo = 1;
        inimigos[totalInimigos].vulneravel = 0;
        totalInimigos++;
    }
}

void AtualizarInimigos(void) {
    float deltaTime = GetFrameTime();
    tempoAcumulado += deltaTime;

    // Usa o tempo de espera lido da 29ª linha do mapa atual (variável externa do mapa.h)
    float tempoAlvo = (tempoEsperaInimigos > 0) ? (float)tempoEsperaInimigos : 15.0f;

    if (!modoVulneravelGlobal && tempoAcumulado >= tempoAlvo) {
        modoVulneravelGlobal = 1;
        tempoVulneravelRestante = 4.0f; // Mantém 4 segundos de vulnerabilidade
        tempoAcumulado = 0.0f;
    }

    if (modoVulneravelGlobal) {
        tempoVulneravelRestante -= deltaTime;
        if (tempoVulneravelRestante <= 0) {
            modoVulneravelGlobal = 0;
        }
    }

    for (int i = 0; i < totalInimigos; i++) {
        if (!inimigos[i].ativo) continue;

        inimigos[i].vulneravel = modoVulneravelGlobal;

        // Movimento horizontal
        inimigos[i].x += inimigos[i].velocidadeX;

        // Evita que saiam das bordas laterais da tela (0 a 1200 pixels)
        if (inimigos[i].x <= 0) {
            inimigos[i].x = 0;
            inimigos[i].velocidadeX *= -1.0f;
        } else if (inimigos[i].x >= 1200.0f - 32.0f) {
            inimigos[i].x = 1200.0f - 32.0f;
            inimigos[i].velocidadeX *= -1.0f;
        }

        // Se bater em uma parede interna do mapa, inverte o sentido
        float checkX = (inimigos[i].velocidadeX > 0) ? inimigos[i].x + 32.0f : inimigos[i].x;
        if (EBloqueio(checkX, inimigos[i].y + 16.0f)) {
            inimigos[i].velocidadeX *= -1.0f;
        }

        // Gravidade para o inimigo se manter nas plataformas
        inimigos[i].y += 2.5f;
        if (EBloqueio(inimigos[i].x + 4.0f, inimigos[i].y + 32.0f) || EBloqueio(inimigos[i].x + 28.0f, inimigos[i].y + 32.0f)) {
            int linhaPé = (int)((inimigos[i].y + 32.0f - 80.0f) / 25.0f);
            inimigos[i].y = (linhaPé * 25.0f) + 80.0f - 32.0f;
        }

        // Segurança caso caia abaixo da tela
        if (inimigos[i].y > 680.0f) {
            inimigos[i].y = 100.0f; // Retorna para cima
        }

        // Colisão com o Mário
        Rectangle recMario = { mario.x, mario.y, 32.0f, 32.0f };
        Rectangle recInimigo = { inimigos[i].x, inimigos[i].y, 32.0f, 32.0f };

        if (CheckCollisionRecs(recMario, recInimigo)) {
            if (inimigos[i].vulneravel) {
                inimigos[i].ativo = 0;
                mario.pontos += 800; // Soma os 800 pontos exigidos[cite: 3, 4]
                mario.velocidadeY = -6.0f;
            } else {
                mario.vidas--; // Desconta vida[cite: 3, 4]
                mario.x = 100.0f;
                mario.y = 100.0f;
                mario.velocidadeY = 0;
            }
        }
    }
}

void DesenharInimigos(void) {
    for (int i = 0; i < totalInimigos; i++) {
        if (inimigos[i].ativo) {
            // Fica azul quando vulnerável para o jogador saber que pode atingir
            Color corInimigo = inimigos[i].vulneravel ? BLUE : WHITE;
            if (texturaInimigo.id > 0) {
                Rectangle recSource = { 0.0f, 0.0f, (float)texturaInimigo.width, (float)texturaInimigo.height };
                Rectangle recDest = { inimigos[i].x, inimigos[i].y, 32.0f, 32.0f };
                Vector2 origem = { 0.0f, 0.0f };
                DrawTexturePro(texturaInimigo, recSource, recDest, origem, 0.0f, corInimigo);
            } else {
                DrawRectangle((int)inimigos[i].x, (int)inimigos[i].y, 32, 32, GREEN);
            }
        }
    }
}