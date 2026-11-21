#include "jogador.h"
#include "mapa.h"
#include "raylib.h"

Jogador mario;
Texture2D texturaMario;
int texturaCarregada = 0;

void InicializarJogador(float xInicial, float yInicial) {
    mario.x = xInicial;
    mario.y = yInicial;
    mario.velocidadeY = 0;
    mario.vidas = 3;
    mario.pontos = 0;
    mario.noChao = 0;
    
    if (!texturaCarregada) {
        texturaMario = LoadTexture("mario.png");
        texturaCarregada = 1;
    }
}

void AtualizarJogador(void) {
    float velocidadeX = 0.0f;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        velocidadeX = 3.5f;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        velocidadeX = -3.5f;
    }

    // Colisão horizontal e movimento
    mario.x += velocidadeX;

    // Impede o Mário de sair pelas bordas laterais da tela (0 a 1200 pixels)
    if (mario.x < 0) {
        mario.x = 0;
    } else if (mario.x > 1200.0f - 32.0f) {
        mario.x = 1200.0f - 32.0f;
    }

    if (EBloqueio(mario.x, mario.y + 4.0f) || EBloqueio(mario.x + 24.0f, mario.y + 4.0f) ||
        EBloqueio(mario.x, mario.y + 28.0f) || EBloqueio(mario.x + 24.0f, mario.y + 28.0f)) {
        mario.x -= velocidadeX;
    }

    // Gravidade
    mario.velocidadeY += 0.30f; 
    if (mario.velocidadeY > 9.0f) mario.velocidadeY = 9.0f;
    
    mario.y += mario.velocidadeY;
    mario.noChao = 0;

    // Colisão vertical
    if (mario.velocidadeY > 0) { 
        if (EBloqueio(mario.x + 4.0f, mario.y + 32.0f) || EBloqueio(mario.x + 28.0f, mario.y + 32.0f)) {
            int linhaPé = (int)((mario.y + 32.0f - 80.0f) / 25.0f);
            mario.y = (linhaPé * 25.0f) + 80.0f - 32.0f;
            mario.velocidadeY = 0;
            mario.noChao = 1;
        }
    } else if (mario.velocidadeY < 0) { 
        if (EBloqueio(mario.x + 4.0f, mario.y) || EBloqueio(mario.x + 28.0f, mario.y)) {
            mario.velocidadeY = 0;
        }
    }

    // Pulo
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && mario.noChao) {
        mario.velocidadeY = -11.0f;
        mario.noChao = 0;
    }

    // LIMITE INFERIOR: Se cair abaixo da tela (fundo do abismo), perde vida e ressurge no topo
    if (mario.y > 640.0f) {
        mario.vidas--;
        mario.y = 120.0f;           // Altura segura perto do topo da fase
        mario.x = 600.0f;           // Centralizado horizontalmente
        mario.velocidadeY = 0.0f;   // Reseta a inércia da queda
    }
}

void DesenharJogador(void) {
    if (texturaMario.id > 0) {
        Rectangle recSource = { 0.0f, 0.0f, (float)texturaMario.width, (float)texturaMario.height }; 
        Rectangle recDest = { mario.x, mario.y, 32.0f, 32.0f }; 
        Vector2 origem = { 0.0f, 0.0f };
        DrawTexturePro(texturaMario, recSource, recDest, origem, 0.0f, WHITE);
    } else {
        DrawRectangle((int)mario.x, (int)mario.y, 24, 32, RED);
    }
}