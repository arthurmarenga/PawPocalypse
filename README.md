# PawPocalypse 🐾 — Cães vs. Gatos Zumbis

Em um mundo pós-apocalíptico onde os gatos foram infectados por um vírus misterioso e se transformaram em hordas famintas de Gatos Zumbis, a última esperança da humanidade está nas patas dos seus companheiros mais leais: os Cachorros. Organizados por raças, cada uma com habilidades únicas e papéis estratégicos, os cães se uniram para defender os últimos territórios e deter a invasão felina.

Jogo de estratégia e defesa de pistas em pixel-art, inspirado no clássico estilo *Plants vs. Zombies*, desenvolvido em **C puro (padrão C99)** com a biblioteca gráfica e de áudio **[raylib](https://www.raylib.com/)** como projeto prático para a disciplina de Programação Imperativa e Funcional (PIF).

---

## Sumário

1. [Como compilar e jogar](#1-como-compilar-e-jogar)
2. [Como jogar](#2-como-jogar)
3. [Conteúdo do jogo](#3-conteúdo-do-jogo)
4. [Onde está cada requisito do trabalho](#4-onde-está-cada-requisito-do-trabalho)
5. [Mapa do código](#5-mapa-do-código)
6. [Teclas de atalho e teste](#6-teclas-de-atalho-e-teste)
7. 7. [Vídeo demonstrativo](#7-video-de-demonstração-do-jogo)

---

## 1. Como compilar e jogar

Para compilar e executar o projeto, é necessário ter instalado um compilador C (como `gcc` ou `clang`) com suporte a C99, o utilitário `make` e a biblioteca **raylib** configurada no ambiente.

### Instalação da raylib por sistema operacional

| Sistema Operacional | Método de Instalação Recomendado |
|---|---|
| **Windows** | Instale o MinGW-w64 e a raylib (geralmente em `C:/raylib/raylib/src`). O `Makefile` já inclui os links para `-lraylib -lopengl32 -lgdi32 -lwinmm`. Caso sua raylib esteja em outro diretório, ajuste a variável `RAYLIB_PATH` no `Makefile`. |
| **Linux (Ubuntu / Debian)** | Requer a **raylib 6.0**, GCC e Make. Caso a raylib não esteja disponível nos repositórios da distribuição, compile a versão 6.0 a partir do [código-fonte oficial](https://github.com/raysan5/raylib). A compilação e a execução foram testadas no Ubuntu 26.04.1 com WSL 2. |
| **macOS** | Utilize o Homebrew: <br>`brew install raylib` <br>*(o `Makefile` detecta automaticamente o prefixo do Homebrew em `/opt/homebrew` ou `/usr/local`)*. |

### Comandos de compilação (na pasta do projeto)

Abra o terminal ou prompt de comando na raiz do projeto onde está localizado o `Makefile`:

```bash
make          # Compila todos os arquivos-fonte em build/ e gera o executável (pawpocalypse / pawpocalypse.exe)
make run      # Compila (caso haja alterações) e executa o jogo imediatamente
make clean    # Remove a pasta build/ e o binário gerado, limpando a compilação anterior
```
### Comandos de compilação para linux
Para instalar as dependências, compilar a raylib 6.0 e executar o PawPocalypse, utilize o comando abaixo no terminal Linux.

**Instalação em uma única linha:**

```bash
sudo apt update && sudo apt install -y git build-essential cmake pkg-config libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev && git clone --depth 1 --branch 6.0 https://github.com/raysan5/raylib.git raylib6 && cmake -S raylib6 -B raylib6/build -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF && cmake --build raylib6/build -j 4 && sudo cmake --install raylib6/build && sudo ldconfig && git clone https://github.com/arthurmarenga/PawPocalypse.git && cd PawPocalypse && make run
```
caso tenha a raylib 6.0 e as ferramentas de compilação execute 

```bash
git clone https://github.com/arthurmarenga/PawPocalypse.git && cd PawPocalypse && make run
```

> **Resolução e Escala:** O jogo renderiza toda a cena em uma tela virtual interna em pixel-art de **640x360**, que é redimensionada em escala inteira (com bordas pretas de compensação e textura com filtro sem suavização) para a janela de **1280x720**. A tecla **F11** alterna para o modo janela sem bordas / tela cheia.

---

## 2. Como jogar

Os Gatos Zumbis invadem a área pela borda direita do mapa, linha por linha, avançando em direção à sua casa na extrema esquerda. Você deve posicionar estrategicamente seus cachorros no campo de batalha (**4 linhas por 8 colunas**) para derrotá-los antes que atinjam o quintal. **Se qualquer gato conseguir alcançar a casa na esquerda, a partida é imediatamente perdida.**

### Controles e Comandos

| Ação | Como fazer |
|---|---|
| **Navegar e Interagir** | Botão esquerdo do mouse para clicar em botões, selecionar cartas, escolher posições e coletar petiscos |
| **Seleção de Deck** | Antes de iniciar uma fase, selecione 5 das 10 raças disponíveis (o Pote de Ração é fixo e sempre incluído) |
| **Comprar / Posicionar Cachorro** | Clique na carta do cachorro na barra superior (ou use as teclas **1** a **6**) e depois clique na célula desejada no campo |
| **Coletar Recursos (Petiscos)** | Clique com o mouse sobre os petiscos (ossos) que caem periodicamente do céu ou que são produzidos pelo Pote de Ração (cada um concede **+25 petiscos**) |
| **Remover Cachorro (Pá)** | Clique no ícone da **Pá** no HUD superior ou pressione a tecla **S**, depois clique sobre o cachorro que deseja retirar do campo |
| **Cancelar Ação / Seleção** | Clique com o **botão direito do mouse** para desmarcar cartas ou a pá |
| **Pausar o Jogo** | Pressione **ESC** ou a tecla **P** durante uma partida |
| **Música de Fundo** | Pressione a tecla **M** a qualquer momento para ligar ou desligar a música |
| **Alternar Tela Cheia** | Pressione **F11** |

### Dinâmica e Economia

- **Petiscos:** Cada raça de cachorro consome uma quantidade específica de petiscos para entrar em campo e possui um tempo de recarga (*cooldown*). O jogador inicia cada fase padrão com **200 petiscos**.
- **Fase 5 (Batalha contra o Chefão):** Na batalha decisiva contra o Chefão, não há produção ou coleta de petiscos. Os cachorros chegam **gratuitamente através de uma esteira rolante vertical** na lateral esquerda. Basta clicar no cachorro disponível na esteira e colocá-lo no tabuleiro.
- **Telas de Resultado e Pontuação:** Ao término de cada partida (seja vitória ou derrota), é exibida uma tela com o tempo transcorrido, número de gatos derrotados e a pontuação obtida. Ao vencer, o save é atualizado e o progresso é salvo em disco.

---

## 3. Conteúdo do jogo

### Fases e Modos

| Fase / Modo | Descrição e Estrutura |
|---|---|
| **Tutorial** | Modo guiado interativo que ensina passo a passo: coletar petiscos do céu, posicionar o Pote de Ração, colocar um Beagle atirador, usar a pá para limpar uma casa e combater o primeiro zumbi invasor. |
| **Fases 1 a 4** | Quatro fases sequenciais de sobrevivência por ondas, cada uma composta por **4 hordas** (1 inicial mais leve, 2 intermediárias e 1 horda final com grande concentração de inimigos). Antes de cada horda surgir, um alerta visual na tela avisa a chegada dos zumbis. |
| **Fase 5 (Chefão)** | Batalha contra o formidável **Gato Chefão** (5000 de vida e dano massivo de 150). O chefão troca de linha, invoca outros gatos zumbis e realiza ataques periódicos à distância, causando dano aos cachorros posicionados na linha em que está.|

### Sistema de Saves

O jogo conta com **3 slots independentes de gravação** (`assets/data/saves.txt`). Cada save armazena:
- Estado de uso do slot;
- Maior fase liberada;
- Conclusão individual de cada uma das 5 fases;
- Pontuação total acumulada;
- Tempo total jogado (em segundos).

### As 10 Raças Caninas (e o Pote de Ração)

| Raça / Unidade | Papel Tático | Custo | Recarga | Vida | Dano / Cura | Cadência | Comportamento Especial |
|---|---|:---:|:---:|:---:|:---:|:---:|---|
| **Pote de Ração** | Gerador | 50 | 7,5 s | 300 | — | 15,0 s | Produz petiscos periodicamente para financiar o exército canino. |
| **Beagle** | Atirador | 100 | 7,5 s | 300 | 20 | 1,5 s | Dispara projéteis de baba em linha reta no primeiro gato da pista. |
| **Bulldog** | Muralha | 50 | 30,0 s | 4000 | — | — | Altíssima resistência para conter o avanço inimigo. Bloqueia o pulo de gatos acrobatas. |
| **Dálmata** | Atirador Duplo | 200 | 7,5 s | 300 | 20 (x2) | 1,5 s | Dispara duas bolhas seguidas de projétil por ciclo de ataque na mesma linha. |
| **Husky** | Latido Gelado | 175 | 7,5 s | 300 | 20 | 1,5 s | Dispara projéteis congelantes que aplicam lentidão de movimento aos gatos atingidos. |
| **Pastor Alemão** | Mordida Forte | 150 | 7,5 s | 300 | 40 | 1,0 s | Defensor de corpo a corpo rápido que morde gatos na área frontal. |
| **Westie** | Toca-Armadilha | 25 | 30,0 s | 300 | 600 | Único | Fica enterrado preparando-se por 10 s; explode em contato direto causando 600 de dano. |
| **São Bernardo** | Suporte de Cura | 125 | 15,0 s | 300 | +50 vida | 5,0 s | Cura periodicamente aliados posicionados nas 8 células vizinhas ao seu redor. |
| **Dachshund** | Artilharia 3 Linhas | 300 | 7,5 s | 300 | 20 | 1,5 s | Dispara jatos de baba simultaneamente em sua linha e nas duas linhas adjacentes. |
| **Rottweiler** | Explosão 3x3 | 150 | 35,0 s | 300 | 1800 | 1,0 s | Ativa-se em 1 segundo e se detona, erradicando inimigos em uma área de 3x3 células. |
| **Shiba Inu** | Grito Desestabilizador | 125 | 15,0 s | 300 | — | 6,0 s | Emite um latido aterrorizante que empurra os gatos da linha de volta para trás. |

### Os Inimigos (Gatos Zumbis)

| Tipo de Gato | Vida | Velocidade | Dano | Pontos | Peculiaridade |
|---|:---:|:---:|:---:|:---:|---|
| **Gato Zumbi Padrão** | 200 | 8 px/s | 100 | 10 pts | Inimigo básico da horda, avança em velocidade constante. |
| **Gato da Caixa** | 560 | 8 px/s | 100 | 15 pts | Protegido por caixa de papelão, absorve mais dano antes de expor a vida básica. |
| **Gato da Lata** | 1300 | 8 px/s | 100 | 30 pts | Armadura metálica de alta resistência, exigindo dano concentrado. |
| **Gato Siamês** | 180 | 16 px/s | 100 | 25 pts | Muito veloz (dobro da velocidade comum), surpreende defesas desprevenidas. |
| **Gato Persa** | 450 | 7 px/s | 100 | 30 pts | Imune ao efeito de lentidão provocado pelo gelo do Husky, mas ainda recebe o dano dos projéteis. |
| **Gato de Telhado** | 250 | 9 px/s | 100 | 25 pts | Salta acrobaticamente por cima do 1º cachorro que encontrar (exceto se for o Bulldog). |
| **Maine Coon** | 3000 | 5 px/s | 300 | 50 pts | Tanque colossal, absorve imensa quantidade de disparos e causa 300 de dano por mordida. |
| **Gato Chefão** | 5000 | — | 150 | 200 pts | O líder supremo felino da Fase 5: troca de faixas, invoca reforços e ataca com bolas de pelo. |



## 4. Onde está cada requisito do trabalho

O projeto foi desenvolvido em C99, utilizando os conceitos obrigatórios de programação exigidos pela disciplina, além de funcionalidades complementares.

### Requisitos obrigatórios de programação

| Requisito | Implementação e Localização no Código |
|---|---|
| **Structs (Registros)** | Utilizadas para organizar os dados do jogo: `Jogo` (`include/game.h`), `Partida`, `Carta`, `Efeito` e `Esteira` (`include/partida.h`), `Mapa` (`include/map.h`), `Cachorro` e `InfoCachorro` (`include/tower.h`), `Gato` e `InfoGato` (`include/enemy.h`), `Projetil` (`include/projetil.h`), `Petisco` (`include/petisco.h`), `Horda` (`include/horda.h`), `Save` (`include/save.h`), `Audio` (`include/audio.h`) e `Botao` (`include/ui.h`). |
| **Ponteiros** | A grade do campo de batalha utiliza uma matriz bidimensional de ponteiros: `Cachorro *grade[LINHAS_MAPA][COLUNAS_MAPA]`, definida em `include/map.h`. Os módulos `enemy.c`, `projetil.c` e `petisco.c` também utilizam ponteiros duplos para manipular listas encadeadas. |
| **Alocação Dinâmica (`malloc` e `free`)** | Utilizada na criação e liberação de estruturas do jogo, incluindo `jogo_criar` / `jogo_destruir`, `partida_criar` / `partida_destruir`, `cachorro_criar` / `cachorro_destruir`, além dos nós das listas de gatos, projéteis e petiscos. O módulo `audio.c` também utiliza memória alocada dinamicamente para gerar amostras de áudio. |
| **Listas Encadeadas** | Três sistemas utilizam listas simplesmente encadeadas com nós alocados dinamicamente: gatos em campo (`src/enemy.c`), projéteis disparados (`src/projetil.c`) e petiscos disponíveis para coleta (`src/petisco.c`). |
| **Matrizes** | O mapa utiliza uma matriz 4x8 de ponteiros (`Mapa.grade` em `include/map.h`). A composição das hordas utiliza uma matriz tridimensional `composicao[TOTAL_FASES][HORDAS_POR_FASE][TOTAL_TIPOS_GATO]` em `src/horda.c`. O módulo `src/audio.c` contém uma matriz de acordes musicais, enquanto `src/petisco.c` utiliza arranjos de strings para representar figuras em pixel-art. |
| **Escrita e Leitura em Arquivos (`FILE*`)** | O progresso é armazenado em `assets/data/saves.txt`, utilizando funções da biblioteca padrão de C como `fopen`, `fgets`, `sscanf`, `fprintf` e `fclose`, implementadas no módulo `src/save.c`. |

### Recursos adicionais e outros aspectos técnicos

| Recurso | Implementação e Localização no Código |
|---|---|
| **Ponteiros de Função** | A tabela `habilidades[TOTAL_TIPOS_CACHORRO]`, em `src/habilidade.c`, associa cada tipo de cachorro à função responsável pela sua habilidade. |
| **Compilação Automatizada com Makefile** | O `Makefile` possui configurações para Windows, Linux e macOS, com os comandos `make`, `make run` e `make clean`. Também organiza os arquivos compilados em `build/` e gera dependências automáticas de cabeçalhos (`.d`). A compilação e execução foram testadas em Windows e Linux. |
| **Interface Gráfica e Menus** | Máquina de estados em `src/game.c`, responsável pelo Menu Principal, Seleção de Save, Mapa de Fases, Escolha de Deck, Instruções, Partida, Pausa e Tela de Resultado. Os componentes de interface são implementados em `src/ui.c`. |
| **Movimentação, Animações e Colisões** | Os módulos `src/enemy.c`, `src/projetil.c`, `src/petisco.c` e `src/partida.c` controlam movimentação, atualizações e interações entre elementos. Os sprites animados são gerenciados em `src/sprite.c`. |
| **Fases, Progressão e Chefão** | O jogo possui cinco fases e um tutorial interativo. A quinta fase apresenta um chefão com movimentação entre linhas, invocação de inimigos, ataques especiais e uma esteira para disponibilizar cachorros durante a batalha. |
| **Controle de Versão (Git)** | O desenvolvimento está registrado no GitHub, com histórico de commits incrementais e branches utilizadas nas diferentes etapas do projeto. |


## 5. Mapa do código

O código é estruturado de forma modular e desacoplada, separando definições de dados e contratos (`include/*.h`) de suas implementações concretas (`src/*.c`):

```text
main.c
  └─ game.c ────────── Máquina de estados: Menu, Saves, Fases, Deck, Instruções, Pausa, Fim de Partida
       ├─ save.c ───────── Persistência dos 3 slots de gravação em arquivo (assets/data/saves.txt)
       ├─ audio.c ──────── Síntese procedural de áudio chiptune e controle de reprodução
       └─ partida.c ────── Loop de jogo da fase ativa: HUD, custos, cartas, pá, esteira e colisões
            ├─ map.c ────────── Campo de pistas 4x8 (matriz de ponteiros para Cachorro)
            ├─ tower.c ──────── Atributos das raças e renderização dos defensores
            ├─ habilidade.c ─── Lógica tática de cada raça via tabela de ponteiros de função
            ├─ enemy.c ──────── Lista encadeada de Gatos Zumbis: avanço, mordida, saltos e dano
            ├─ projetil.c ───── Lista encadeada de projéteis (babas, gelo e jatos)
            ├─ petisco.c ────── Lista encadeada de petiscos: queda vertical, produção e coleta
            └─ horda.c ──────── Temporizador e controle de ondas/hordas de inimigos por fase

Módulos utilitários compartilhados:
  ├─ ui.c ──────── Botões, textos centralizados, barras de vida, caixas de diálogo e painéis
  ├─ sprite.c ──── Carregamento e animação por tempo das spritesheets em pixel-art
  ├─ cenario.c ─── Desenho do céu com gradiente procedural, prédios e silhuetas de fundo
  └─ config.h ──── Constantes globais (dimensões da tela virtual, cores do tema, fontes)
```

### Arquitetura dos Módulos

| Módulo | Arquivos | Papel e Responsabilidade |
|---|---|---|
| **Main** | `src/main.c` | Ponto de entrada do executável, inicialização da janela gráfica e laço principal de atualização e renderização. |
| **Game** | `include/game.h`<br>`src/game.c` | Coordena a máquina de estados das telas, orquestra a criação/destruição de partidas e intermedia saves. |
| **Partida** | `include/partida.h`<br>`src/partida.c` | Controla o estado de uma partida em curso: atualização de tempo, processamento de cliques/atalhos, fluxo do tutorial, esteira da Fase 5 e verificação de vitória/derrota. |
| **Mapa** | `include/map.h`<br>`src/map.c` | Gerencia a grade 4x8 do gramado, conversão entre coordenadas de tela e índices de célula, e alocação dos cachorros. |
| **Torres (Cachorros)** | `include/tower.h`<br>`src/tower.c` | Tabela de atributos imutáveis das raças caninas e rotinas de desenho de spritesheets/formas reservas. |
| **Habilidades** | `include/habilidade.h`<br>`src/habilidade.c` | Contém a implementação atômica da habilidade de cada raça e despacha ações através de um vetor de ponteiros de função. |
| **Inimigos (Gatos)** | `include/enemy.h`<br>`src/enemy.c` | Gestão da lista encadeada de zumbis: movimentação horizontal, verificação de linha, cálculo de mordidas, congelamento e chefão. |
| **Projéteis** | `include/projetil.h`<br>`src/projetil.c` | Lista encadeada de disparos em deslocamento pela tela, detecção de colisão com inimigos e aplicação de dano/efeitos. |
| **Petiscos** | `include/petisco.h`<br>`src/petisco.c` | Lista encadeada de ossinhos: geração pelo pote ou queda do céu, física simplificada de descida e coleta por clique. |
| **Hordas** | `include/horda.h`<br>`src/horda.c` | Tabela de composição de inimigos por fase e temporizador de liberação de ondas sucessivas com anúncio na tela. |
| **Saves** | `include/save.h`<br>`src/save.c` | Carregamento e escrita estruturada do arquivo de progresso em texto puro. |
| **Áudio** | `include/audio.h`<br>`src/audio.c` | Geração dinâmica de ondas sonoras chiptune via software e controle de volume/mudo. |
| **Sprites** | `include/sprite.h`<br>`src/sprite.c` | Carregamento seguro de texturas, cálculo de quadros de animação por tempo decorrido e fallback procedural caso falte imagem. |
| **Cenário** | `include/cenario.h`<br>`src/cenario.c` | Renderização do plano de fundo dinâmico: transição de céu ao entardecer e skyline urbano. |
| **Interface (UI)** | `include/ui.h`<br>`src/ui.c` | Tratamento de botões retangulares, estados de hover/clique e renderização de textos e ícones em pixel-art. |
| **Configurações** | `include/config.h` | Definição de constantes macro do jogo: dimensões virtuais (640x360), paleta de cores e tamanhos de fonte. |



## 6. Teclas de atalho e teste

### Controles Gerais
- **1 a 6:** Seleciona rapidamente as cartas do deck correspondentes na barra superior.
- **S:** Seleciona / desseleciona a **Pá** para remover uma unidade do gramado.
- **Botão Direito do Mouse:** Cancela qualquer seleção atual (carta ou pá).
- **ESC / P:** Pausa ou despausa a partida ativa.
- **M:** Liga / desliga o áudio musical procedural.
- **F11:** Alterna entre modo janela e tela cheia sem bordas.

### Teclas de Desenvolvimento e Teste
Para facilitar a demonstração pedagógica e testes rápidos das fases, foram incluídos atalhos de depuração:
- **F9 (durante a partida):** Conclui e vence instantaneamente a fase atual (não se aplica ao tutorial), liberando a fase seguinte e computando os bônus correspondentes no save em uso.

## 7. Video de demonstração do jogo
[Gameplay](https://youtu.be/4E5DysHAkSI)
