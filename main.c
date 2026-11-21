#include "raylib.h"
#include "mapa.h"
#include "jogador.h"
#include "inimigo.h"
#include <stdio.h>
#include <string.h>

typedef enum {
    ESTADO_MENU = 0,
    ESTADO_JOGANDO,
    ESTADO_RANKING,
    ESTADO_DIGITANDO_NOME,
    ESTADO_GAMEOVER, 
    ESTADO_VITORIA,  
    ESTADO_SAIR
} GameState;

typedef struct {
    char nome[30];
    int pontos;
} HighScore;

extern int modoVulneravelGlobal;

int VerificarNovoHighScore(int novaPontuacao) {
    if (novaPontuacao <= 0) return 0;
    
    HighScore scores[5] = {{"---", 0}, {"---", 0}, {"---", 0}, {"---", 0}, {"---", 0}};
    FILE *f = fopen("highscores.bin", "rb");
    if (f) {
        fread(scores, sizeof(HighScore), 5, f);
        fclose(f);
    }

    return (novaPontuacao > scores[4].pontos);
}

void SalvarRankingComNome(int novaPontuacao, const char *nomeJogador) {
    HighScore scores[5] = {{"---", 0}, {"---", 0}, {"---", 0}, {"---", 0}, {"---", 0}};
    FILE *f = fopen("highscores.bin", "rb");
    if (f) {
        fread(scores, sizeof(HighScore), 5, f);
        fclose(f);
    }

    for (int i = 0; i < 5; i++) {
        if (novaPontuacao > scores[i].pontos) {
            for (int j = 4; j > i; j--) {
                scores[j] = scores[j - 1];
            }
            scores[i].pontos = novaPontuacao;
            snprintf(scores[i].nome, sizeof(scores[i].nome), "%s", nomeJogador);
            break;
        }
    }

    f = fopen("highscores.bin", "wb");
    if (f) {
        fwrite(scores, sizeof(HighScore), 5, f);
        fclose(f);
    }
}

int main(void) {
    InitWindow(1200, 700, "Mario-INF - UFRGS");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    GameState estadoAtual = ESTADO_MENU;
    Texture2D texturaTitulo = LoadTexture("titulo.png");

    char nomeDigitado[4] = "\0";
    int letraCount = 0;

    while (!WindowShouldClose() && estadoAtual != ESTADO_SAIR) {
        
        // Menu Principal (clicável com o rato e teclado)
        if (estadoAtual == ESTADO_MENU) {
            Rectangle btnNovoJogo = { 500, 380, 200, 30 };
            Rectangle btnCarregar = { 500, 420, 200, 30 };
            Rectangle btnTop5     = { 500, 460, 200, 30 };
            Rectangle btnSair     = { 500, 500, 200, 30 };

            Vector2 mousePos = GetMousePosition();
            int clicou = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            if ((clicou && CheckCollisionPointRec(mousePos, btnNovoJogo)) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ONE)) {
                mario.vidas = 3;
                mario.pontos = 0;
                faseAtual = 1;
                CarregarFase("fase1.txt");
                estadoAtual = ESTADO_JOGANDO;
            }
            if ((clicou && CheckCollisionPointRec(mousePos, btnCarregar)) || IsKeyPressed(KEY_TWO)) {
                mario.vidas = 3;
                mario.pontos = 0;
                faseAtual = 1;
                CarregarFase("fase1.txt");
                estadoAtual = ESTADO_JOGANDO;
            }
            if ((clicou && CheckCollisionPointRec(mousePos, btnTop5)) || IsKeyPressed(KEY_THREE)) {
                estadoAtual = ESTADO_RANKING;
            }
            if ((clicou && CheckCollisionPointRec(mousePos, btnSair)) || IsKeyPressed(KEY_FOUR)) {
                estadoAtual = ESTADO_SAIR;
            }
        } 
        else if (estadoAtual == ESTADO_RANKING) {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                estadoAtual = ESTADO_MENU;
            }
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            AtualizarJogador();
            AtualizarInimigos();

            int inimigosRestantes = 0;
            for (int i = 0; i < totalInimigos; i++) {
                if (inimigos[i].ativo) inimigosRestantes++;
            }

            // Avanço de fases
            if (inimigosRestantes == 0 && totalInimigos > 0) {
                faseAtual++;
                if (faseAtual == 2) {
                    CarregarFase("fase2.txt");
                } else if (faseAtual == 3) {
                    CarregarFase("fase3.txt");
                } else {
                    estadoAtual = ESTADO_VITORIA;
                    faseAtual = 1;
                }
            }

            // Game Over por falta de vidas
            if (mario.vidas <= 0) {
                estadoAtual = ESTADO_GAMEOVER;
                faseAtual = 1;
            }

            // Atalho ESC para desistir
            if (IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = ESTADO_GAMEOVER;
                faseAtual = 1;
            }
        }
        // Tratamento da tela de Game Over
        else if (estadoAtual == ESTADO_GAMEOVER) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (VerificarNovoHighScore(mario.pontos)) {
                    estadoAtual = ESTADO_DIGITANDO_NOME;
                    letraCount = 0;
                    nomeDigitado[0] = '\0';
                } else {
                    estadoAtual = ESTADO_MENU;
                }
            }
        }
        // Tratamento da tela de Vitória
        else if (estadoAtual == ESTADO_VITORIA) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (VerificarNovoHighScore(mario.pontos)) {
                    estadoAtual = ESTADO_DIGITANDO_NOME;
                    letraCount = 0;
                    nomeDigitado[0] = '\0';
                } else {
                    estadoAtual = ESTADO_MENU;
                }
            }
        }
        // Digitação das iniciais para o Top 5
        else if (estadoAtual == ESTADO_DIGITANDO_NOME) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letraCount < 3)) {
                    nomeDigitado[letraCount] = (char)key;
                    nomeDigitado[letraCount + 1] = '\0';
                    letraCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                letraCount--;
                if (letraCount < 0) letraCount = 0;
                nomeDigitado[letraCount] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) && letraCount > 0) {
                SalvarRankingComNome(mario.pontos, nomeDigitado);
                estadoAtual = ESTADO_RANKING;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);

            if (estadoAtual == ESTADO_MENU) {
                if (texturaTitulo.id > 0) {
                    float posX = (1200 - (texturaTitulo.width * 2)) / 2.0f;
                    DrawTextureEx(texturaTitulo, (Vector2){ posX, 100.0f }, 0.0f, 2.0f, WHITE);
                } else {
                    DrawText("MARIO-INF", 450, 150, 40, RED);
                }

                DrawText("NOVO JOGO", 500, 380, 20, RAYWHITE);
                DrawText("CARREGAR MAPA", 500, 420, 20, RAYWHITE);
                DrawText("TOP 5", 500, 460, 20, RAYWHITE);
                DrawText("SAIR", 500, 500, 20, RAYWHITE);
            } 
            else if (estadoAtual == ESTADO_JOGANDO) {
                char bufferPontos[32];
                char bufferVidas[32];
                char bufferFase[32];
                sprintf(bufferPontos, "PONTOS: %d", mario.pontos);
                sprintf(bufferVidas, "VIDAS: %d", mario.vidas);
                sprintf(bufferFase, "FASE: %d", faseAtual);

                DrawText(bufferPontos, 100, 30, 20, RAYWHITE);
                DrawText(bufferVidas, 400, 30, 20, RAYWHITE);
                DrawText(bufferFase, 700, 30, 20, RAYWHITE);

                if (modoVulneravelGlobal) {
                    DrawText("ATENCAO: INIMIGOS VULNERAVEIS!", 420, 65, 20, GOLD);
                }
                
                // Desenha o mapa (com as plataformas, chão ilustrativo e canos)
                DesenharMapa();
                DesenharInimigos();
                DesenharJogador();

                DrawText("Pressione ESC para sair", 100, 670, 14, GRAY);
            }
            else if (estadoAtual == ESTADO_GAMEOVER) {
                DrawText("GAME OVER", 450, 230, 50, RED);
                
                char pontosFinal[64];
                sprintf(pontosFinal, "Pontuação Final: %d", mario.pontos);
                DrawText(pontosFinal, 470, 320, 20, RAYWHITE);

                DrawText("Pressione ENTER ou Clique para Continuar", 400, 400, 16, GRAY);
            }
            else if (estadoAtual == ESTADO_VITORIA) {
                DrawText("VOCE VENCEU O JOGO!", 390, 230, 40, GOLD);
                
                char pontosFinal[64];
                sprintf(pontosFinal, "Pontuação Total: %d", mario.pontos);
                DrawText(pontosFinal, 470, 320, 20, RAYWHITE);

                DrawText("Pressione ENTER ou Clique para Continuar", 400, 400, 16, GRAY);
            }
            else if (estadoAtual == ESTADO_DIGITANDO_NOME) {
                DrawText("NOVO RECORDE NO TOP 5!", 420, 180, 24, GOLD);
                DrawText("Digite suas iniciais (3 letras) e pressione ENTER:", 340, 230, 18, RAYWHITE);
                
                Rectangle recInput = { 520, 280, 160, 50 };
                DrawRectangleRec(recInput, DARKGRAY);
                DrawRectangleLines((int)recInput.x, (int)recInput.y, (int)recInput.width, (int)recInput.height, RAYWHITE);
                DrawText(nomeDigitado, 555, 292, 30, GREEN);
            }
            else if (estadoAtual == ESTADO_RANKING) {
                DrawText("TOP 5 - HIGHSCORES", 450, 120, 30, GOLD);
                
                HighScore scores[5] = {0};
                FILE *f = fopen("highscores.bin", "rb");
                if (f) {
                    fread(scores, sizeof(HighScore), 5, f);
                    fclose(f);
                }
                
                for(int i = 0; i < 5; i++) {
                    char linhaRank[64];
                    sprintf(linhaRank, "%d. %s  ---  %d pts", i + 1, scores[i].nome, scores[i].pontos);
                    DrawText(linhaRank, 420, 200 + (i * 45), 20, RAYWHITE);
                }

                DrawText("Pressione ESC para voltar ao Menu", 440, 550, 16, GRAY);
            }

        EndDrawing();
    }

    if (texturaTitulo.id > 0) UnloadTexture(texturaTitulo);
    CloseWindow();
    return 0;
}