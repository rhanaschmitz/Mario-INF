# 🍄 Mario-INF - UFRGS

Projeto de jogo estilo arcade desenvolvido em C utilizando a biblioteca [Raylib](https://www.raylib.com/), inspirado no clássico Mario Bros.

## Funcionalidades
- **Sistema de Fases:** Carregamento dinâmico de mapas através de arquivos de texto (`fase1.txt`, `fase2.txt`, etc.).
- **Mecânicas de Jogo:** Movimentação fluida, colisão com plataformas, gravidade e sistema de vidas.
- **Inimigos:** Inimigos com lógica de IA básica e contagem por fase.
- **Ranking (HighScores):** Sistema de pontuação persistente salvo em arquivo binário (`highscores.bin`) com tela de inserção de iniciais para o Top 5.
- **Menus Interativos:** Menu principal completo, telas de Game Over e Vitória.

## Tecnologias Utilizadas
- Linguagem **C**
- Biblioteca **Raylib**

Certifique-se de ter o GCC e a Raylib instalados, e execute os seguintes comandos no terminal na pasta do projeto:

```powershell
gcc main.c mapa.c jogador.c inimigo.c -o mario_inf.exe -lraylib -lgdi32 -lwinmm
.\mario_inf.exe
