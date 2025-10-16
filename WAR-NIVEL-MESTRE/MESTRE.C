#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da estrutura Territorio
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/**
 * Função para cadastrar os territórios.
 * Recebe um ponteiro para o vetor de territórios e o número total.
 */
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n=== Cadastro do Território ID %d ===\n", i);
        printf("Digite o nome do território: ");
        scanf(" %[^\n]", mapa[i].nome);

        printf("Digite a cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

/**
 * Função para exibir todos os territórios do mapa.
 * Parâmetros: ponteiro para o vetor e quantidade de territórios.
 */
void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n===== ESTADO ATUAL DOS TERRITÓRIOS =====\n");
    for (int i = 0; i < n; i++) {
        printf("ID [%d] | Nome: %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/**
 * Função que simula um ataque entre dois territórios.
 * Regras:
 * - Atacante e defensor não podem ter a mesma cor.
 * - Atacante precisa ter pelo menos 2 tropas para atacar.
 * - Dado do atacante e do defensor são gerados aleatoriamente (1 a 6).
 * - Se atacante vence, toma o território, transfere cor e metade das tropas.
 * - Se defensor vence, atacante perde 1 tropa.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\nErro: Você não pode atacar um território da mesma cor!\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("\nErro: Tropas insuficientes para atacar (mínimo 2 tropas).\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n--- Simulação de Batalha ---\n");
    printf("Atacante (%s) rolou: %d\n", atacante->nome, dadoAtacante);
    printf("Defensor (%s) rolou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: Atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas < 1) tropasTransferidas = 1;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
    } else {
        printf("Resultado: Defensor venceu!\n");
        atacante->tropas -= 1;
    }
}

/**
 * Função que atribui uma missão ao jogador.
 * Recebe o ponteiro destino para armazenar a missão,
 * o vetor de strings missoes[], e o total de missões.
 * Sorteia uma missão aleatoriamente e copia para destino.
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

/**
 * Exibe a missão para o jogador.
 * Recebe a missão por valor (const char*).
 */
void exibirMissao(const char* missao) {
    printf("\n>>> Sua missão: %s\n", missao);
}

/**
 * Verifica se a missão foi cumprida.
 * Recebe a missão por referência (char*), o vetor de territórios e seu tamanho.
 * Retorna 1 se cumprida, 0 caso contrário.
 * Lógica inicial simples para cada missão pré-definida.
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        int consecutivos = 0;
        for (int i = 0; i < tamanho - 1; i++) {
            if (strcmp(mapa[i].cor, mapa[i + 1].cor) == 0 && mapa[i].tropas > 0) {
                consecutivos++;
                if (consecutivos >= 2) return 1; // 3 territórios seguidos
            } else {
                consecutivos = 0;
            }
        }
    } else if (strcmp(missao, "Eliminar todas as tropas da cor vermelha") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0 && mapa[i].tropas > 0) {
                return 0; // ainda existem tropas vermelhas
            }
        }
        return 1; // nenhuma tropa vermelha restante
    } else if (strcmp(missao, "Controlar 2 territorios com mais de 10 tropas") == 0) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 10) {
                contador++;
                if (contador >= 2) return 1;
            }
        }
    } else if (strcmp(missao, "Controlar o territorio chamado Fortaleza") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].nome, "Fortaleza") == 0 && mapa[i].tropas > 0) {
                return 1;
            }
        }
    } else if (strcmp(missao, "Ter tropas em todos os territorios") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas <= 0) {
                return 0;
            }
        }
        return 1;
    }

    return 0;
}

/**
 * Libera a memória alocada dinamicamente.
 * Recebe ponteiros para os territórios e para as missões.
 */
void liberarMemoria(Territorio* mapa, char* missao1, char* missao2) {
    free(mapa);
    free(missao1);
    free(missao2);
}

int main() {
    srand(time(NULL));

    const int numTerritorios = 5;

    // Aloca memória para o vetor de territórios
    Territorio* mapa = (Territorio*) calloc(numTerritorios, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória para territórios.\n");
        return 1;
    }

    // Missões pré-definidas (mínimo 5)
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar 2 territorios com mais de 10 tropas",
        "Controlar o territorio chamado Fortaleza",
        "Ter tropas em todos os territorios"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Aloca memória para as missões dos jogadores
    char* missaoJogador1 = (char*) malloc(100 * sizeof(char));
    char* missaoJogador2 = (char*) malloc(100 * sizeof(char));
    if (!missaoJogador1 || !missaoJogador2) {
        printf("Erro ao alocar memória para missões.\n");
        liberarMemoria(mapa, missaoJogador1, missaoJogador2);
        return 1;
    }

    // Sorteia as missões dos jogadores
    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);

    // Cadastro dos territórios
    cadastrarTerritorios(mapa, numTerritorios);

    // Exibe a missão dos jogadores apenas uma vez no início
    printf("\n=== Missão do Jogador 1 ===\n");
    exibirMissao(missaoJogador1);
    printf("\n=== Missão do Jogador 2 ===\n");
    exibirMissao(missaoJogador2);

    int turno = 1;
    int opcao;

    do {
        printf("\n======= TURNO %d =======\n", turno);
        exibirTerritorios(mapa, numTerritorios);

        printf("\n--- MENU ---\n");
        printf("1 - Realizar ataque\n");
        printf("0 - Sair do jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            int idAtacante, idDefensor;

            printf("Informe o ID do território atacante: ");
            scanf("%d", &idAtacante);
            printf("Informe o ID do território defensor: ");
            scanf("%d", &idDefensor);

            // Validação de índices
            if (idAtacante < 0 || idAtacante >= numTerritorios ||
                idDefensor < 0 || idDefensor >= numTerritorios) {
                printf("IDs inválidos!\n");
                continue;
            }

            atacar(&mapa[idAtacante], &mapa[idDefensor]);

            // Verifica silenciosamente se algum jogador venceu após o ataque
            if (verificarMissao(missaoJogador1, mapa, numTerritorios)) {
                printf("\n*** Jogador 1 VENCEU! Missão cumprida! ***\n");
                break;
            }
            if (verificarMissao(missaoJogador2, mapa, numTerritorios)) {
                printf("\n*** Jogador 2 VENCEU! Missão cumprida! ***\n");
                break;
            }

            turno++;
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missaoJogador1, missaoJogador2);

    printf("\nMemória liberada. Programa encerrado.\n");

    return 0;
}