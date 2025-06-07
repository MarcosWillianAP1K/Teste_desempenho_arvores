#include <stdio.h>
#include <stdlib.h> // Para free

// --- DEFINES ÚTEIS ---
#define MIN_KEYS 1 // Mínimo de chaves por nó (para nós não-raiz)
#define MAX_KEYS 4 // Máximo de chaves por nó

// Estrutura do No da Arvore 4-5
typedef struct Arv45
{
    int info1, info2, info3, info4;
    int nInfo; // Número de chaves atualmente no nó (de 0 a MAX_KEYS)
    struct Arv45 *esq;
    struct Arv45 *f_cen1;
    struct Arv45 *f_cen2;
    struct Arv45 *f_cen3;
    struct Arv45 *dir;
} Arv45;

// Enum para status da remocao
typedef enum
{
    OK,
    UNDERFLOW // Indica que o nó que foi manipulado (ou um de seus filhos) ficou em underflow
} StatusRemocao;

// --- FUNÇÕES AUXILIARES (Assumindo que já existem ou serão implementadas) ---
// Função para criar um novo nó
Arv45 *criar_no()
{
    Arv45 *novo_no = (Arv45 *)malloc(sizeof(Arv45));
    if (novo_no)
    {
        novo_no->nInfo = 0;
        novo_no->info1 = novo_no->info2 = novo_no->info3 = novo_no->info4 = 0;
        novo_no->esq = novo_no->f_cen1 = novo_no->f_cen2 = novo_no->f_cen3 = novo_no->dir = NULL;
    }
    return novo_no;
}

// Verifica se um nó é folha
int eh_folha_45(Arv45 *no)
{
    return (no->esq == NULL && no->f_cen1 == NULL && no->f_cen2 == NULL && no->f_cen3 == NULL && no->dir == NULL);
}

// Imprime o nó para debug
void imprimir_no(Arv45 *no)
{
    if (no == NULL)
    {
        printf("[NULL] ");
        return;
    }
    printf("[nInfo:%d ", no->nInfo);
    if (no->nInfo >= 1)
        printf("%d", no->info1);
    if (no->nInfo >= 2)
        printf(" %d", no->info2);
    if (no->nInfo >= 3)
        printf(" %d", no->info3);
    if (no->nInfo >= 4)
        printf(" %d", no->info4);
    printf("] ");
}

// ======================= PROTÓTIPOS DAS FUNÇÕES DE TRATAMENTO =======================
// pos_filho_no_pai: índice do filho dentro do pai (0 para esq, 1 para f_cen1, etc.)
StatusRemocao tratar_underflow_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, int pos_filho_no_pai);
StatusRemocao redistribuir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho);
StatusRemocao redistribuir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho);
StatusRemocao fundir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho);
StatusRemocao fundir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho);

// ======================= BUSCAR MENOR =======================
Arv45 *buscar_menor_elemento_45(Arv45 *no)
{
    Arv45 *atual = no;
    if (atual == NULL)
    {
        return NULL;
    }
    while (atual->esq != NULL)
    { // O menor elemento sempre estará na subárvore mais à esquerda
        atual = atual->esq;
    }
    return atual;
}

// ======================= Funções de Manipulação de Chaves e Filhos dentro de um Nó =======================

// Insere uma chave e um filho em uma posição ordenada
void inserir_chave_e_filho_no(Arv45 *no, int valor, Arv45 *filho_esq, Arv45 *filho_dir)
{
    if (no->nInfo == MAX_KEYS)
    {
        fprintf(stderr, "Erro: Tentativa de inserir em nó cheio durante rebalanceamento.\n");
        return;
    }

    int i = no->nInfo;
    while (i > 0)
    {
        int current_key = (i == 1 ? no->info1 : (i == 2 ? no->info2 : (i == 3 ? no->info3 : no->info4)));
        if (valor < current_key)
        {
            if (i == 4)
                no->info4 = no->info3;
            else if (i == 3)
                no->info3 = no->info2;
            else if (i == 2)
                no->info2 = no->info1;
            if (i == 4)
                no->dir = no->f_cen3;
            else if (i == 3)
                no->f_cen3 = no->f_cen2;
            else if (i == 2)
                no->f_cen2 = no->f_cen1;
            else if (i == 1)
                no->f_cen1 = no->esq;
            i--;
        }
        else
        {
            break;
        }
    }
    if (i == 0)
        no->info1 = valor;
    else if (i == 1)
        no->info2 = valor;
    else if (i == 2)
        no->info3 = valor;
    else if (i == 3)
        no->info4 = valor;

    if (i == 0)
    {
        no->esq = filho_esq;
        no->f_cen1 = filho_dir;
    }
    else if (i == 1)
    {
        no->f_cen1 = filho_esq;
        no->f_cen2 = filho_dir;
    }
    else if (i == 2)
    {
        no->f_cen2 = filho_esq;
        no->f_cen3 = filho_dir;
    }
    else if (i == 3)
    {
        no->f_cen3 = filho_esq;
        no->dir = filho_dir;
    }
    no->nInfo++;
}

void remover_chave_e_filho_de_no(Arv45 *no, int idx)
{
    if (idx < 0 || idx >= no->nInfo)
        return;

    for (int i = idx; i < no->nInfo - 1; i++)
    {
        if (i == 0)
            no->info1 = no->info2;
        else if (i == 1)
            no->info2 = no->info3;
        else if (i == 2)
            no->info3 = no->info4;

        if (i == 0)
            no->f_cen1 = no->f_cen2;
        else if (i == 1)
            no->f_cen2 = no->f_cen3;
        else if (i == 2)
            no->f_cen3 = no->dir;
    }
    if (no->nInfo - 1 == 0)
    {
        no->info1 = 0;
        no->f_cen1 = NULL;
    }
    else if (no->nInfo - 1 == 1)
    {
        no->info2 = 0;
        no->f_cen2 = NULL;
    }
    else if (no->nInfo - 1 == 2)
    {
        no->info3 = 0;
        no->f_cen3 = NULL;
    }
    else if (no->nInfo - 1 == 3)
    {
        no->info4 = 0;
        no->dir = NULL;
    }

    no->nInfo--;
}

void remover_chave_folha(Arv45 *no, int idx)
{
    if (idx < 0 || idx >= no->nInfo)
        return;

    for (int i = idx; i < no->nInfo - 1; i++)
    {
        if (i == 0)
            no->info1 = no->info2;
        else if (i == 1)
            no->info2 = no->info3;
        else if (i == 2)
            no->info3 = no->info4;
    }
    if (no->nInfo - 1 == 0)
        no->info1 = 0;
    else if (no->nInfo - 1 == 1)
        no->info2 = 0;
    else if (no->nInfo - 1 == 2)
        no->info3 = 0;
    else if (no->nInfo - 1 == 3)
        no->info4 = 0;
    no->nInfo--;
}

int get_key(Arv45 *no, int idx)
{
    if (idx == 0)
        return no->info1;
    if (idx == 1)
        return no->info2;
    if (idx == 2)
        return no->info3;
    if (idx == 3)
        return no->info4;
    return -1;
}

void set_key(Arv45 *no, int idx, int val)
{
    if (idx == 0)
        no->info1 = val;
    else if (idx == 1)
        no->info2 = val;
    else if (idx == 2)
        no->info3 = val;
    else if (idx == 3)
        no->info4 = val;
}

Arv45 *get_child(Arv45 *no, int idx)
{
    if (idx == 0)
        return no->esq;
    if (idx == 1)
        return no->f_cen1;
    if (idx == 2)
        return no->f_cen2;
    if (idx == 3)
        return no->f_cen3;
    if (idx == 4)
        return no->dir;
    return NULL;
}

void set_child(Arv45 *no, int idx, Arv45 *child_node)
{
    if (idx == 0)
        no->esq = child_node;
    else if (idx == 1)
        no->f_cen1 = child_node;
    else if (idx == 2)
        no->f_cen2 = child_node;
    else if (idx == 3)
        no->f_cen3 = child_node;
    else if (idx == 4)
        no->dir = child_node;
}

// ======================= TRATAR UNDERFLOW =======================
StatusRemocao tratar_underflow_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, int pos_filho)
{
    Arv45 *filho_com_underflow = *ponteiro_filho_no_pai;
    StatusRemocao status_final = OK;

    if (filho_com_underflow == NULL || filho_com_underflow->nInfo >= MIN_KEYS)
    {
        return OK;
    }

    if (pai == NULL)
    {
        return UNDERFLOW;
    }

    Arv45 *irmao_esq = NULL;
    Arv45 *irmao_dir = NULL;

    if (pos_filho == 0)
    {
        irmao_dir = get_child(pai, pos_filho + 1);
    }
    else if (pos_filho == pai->nInfo)
    {
        irmao_esq = get_child(pai, pos_filho - 1);
    }
    else
    {
        irmao_esq = get_child(pai, pos_filho - 1);
        irmao_dir = get_child(pai, pos_filho + 1);
    }

    if (irmao_dir != NULL && irmao_dir->nInfo > MIN_KEYS)
    {
        status_final = redistribuir_com_irmao_direito_45(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
    }
    else if (irmao_esq != NULL && irmao_esq->nInfo > MIN_KEYS)
    {
        status_final = redistribuir_com_irmao_esquerdo_45(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
    }
    else if (irmao_dir != NULL)
    {
        status_final = fundir_com_irmao_direito_45(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
    }
    else if (irmao_esq != NULL)
    {
        status_final = fundir_com_irmao_esquerdo_45(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
    }
    else
    {
        fprintf(stderr, "Erro critico: No com underflow sem irmaos para rebalancear.\n");
        status_final = UNDERFLOW;
    }

    return status_final;
}

// ======================= REDISTRIBUIR COM IRMAO ESQUERDO =======================
StatusRemocao redistribuir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho)
{
    Arv45 *filho_underflow = *ponteiro_filho_no_pai;

    if (pai == NULL || irmao_esq == NULL || filho_underflow == NULL || irmao_esq->nInfo <= MIN_KEYS)
    {
        return UNDERFLOW;
    }

    int chave_pai_desce_idx = pos_filho - 1;
    if (chave_pai_desce_idx < 0)
        return UNDERFLOW;

    int chave_pai_desce = get_key(pai, chave_pai_desce_idx);

    filho_underflow->info1 = chave_pai_desce;
    filho_underflow->nInfo++;

    Arv45 *filho_transferido_do_irmao = get_child(irmao_esq, irmao_esq->nInfo);
    filho_underflow->esq = filho_transferido_do_irmao;

    int chave_irmao_sobe = get_key(irmao_esq, irmao_esq->nInfo - 1);
    set_key(pai, chave_pai_desce_idx, chave_irmao_sobe);

    set_key(irmao_esq, irmao_esq->nInfo - 1, 0);
    set_child(irmao_esq, irmao_esq->nInfo, NULL);
    irmao_esq->nInfo--;

    return OK;
}

// ======================= REDISTRIBUIR COM IRMAO DIREITO =======================
StatusRemocao redistribuir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho)
{
    Arv45 *filho_underflow = *ponteiro_filho_no_pai;

    if (pai == NULL || irmao_dir == NULL || filho_underflow == NULL || irmao_dir->nInfo <= MIN_KEYS)
    {
        return UNDERFLOW;
    }

    int chave_pai_desce_idx = pos_filho;
    if (chave_pai_desce_idx >= pai->nInfo)
        return UNDERFLOW;

    int chave_pai_desce = get_key(pai, chave_pai_desce_idx);

    filho_underflow->info1 = chave_pai_desce;
    filho_underflow->nInfo++;

    Arv45 *filho_transferido_do_irmao = irmao_dir->esq;
    filho_underflow->f_cen1 = filho_transferido_do_irmao;

    int chave_irmao_sobe = irmao_dir->info1;
    set_key(pai, chave_pai_desce_idx, chave_irmao_sobe);

    for (int i = 0; i < irmao_dir->nInfo - 1; i++)
    {
        set_key(irmao_dir, i, get_key(irmao_dir, i + 1));
        set_child(irmao_dir, i, get_child(irmao_dir, i + 1));
    }
    set_child(irmao_dir, irmao_dir->nInfo - 1, get_child(irmao_dir, irmao_dir->nInfo));

    set_key(irmao_dir, irmao_dir->nInfo - 1, 0);
    set_child(irmao_dir, irmao_dir->nInfo, NULL);
    irmao_dir->nInfo--;

    return OK;
}

// ======================= FUNDIR COM IRMAO ESQUERDO =======================
StatusRemocao fundir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho)
{
    Arv45 *filho_underflow = *ponteiro_filho_no_pai;
    if (pai == NULL || irmao_esq == NULL || filho_underflow == NULL || irmao_esq->nInfo == 0)
    {
        return UNDERFLOW;
    }

    int chave_pai_desce_idx = pos_filho - 1;
    if (chave_pai_desce_idx < 0)
        return UNDERFLOW;

    int chave_pai_desce = get_key(pai, chave_pai_desce_idx);

    set_key(irmao_esq, irmao_esq->nInfo, chave_pai_desce);
    irmao_esq->nInfo++;

    free(filho_underflow);
    *ponteiro_filho_no_pai = NULL;

    for (int i = chave_pai_desce_idx; i < pai->nInfo - 1; i++)
    {
        set_key(pai, i, get_key(pai, i + 1));
        set_child(pai, i + 1, get_child(pai, i + 2));
    }
    set_child(pai, pai->nInfo, NULL);
    set_key(pai, pai->nInfo - 1, 0);
    pai->nInfo--;

    if (pai->nInfo < MIN_KEYS && pai->nInfo > 0)
    {
        return UNDERFLOW;
    }
    else if (pai->nInfo == 0)
    {
        return UNDERFLOW;
    }
    return OK;
}

// ======================= FUNDIR COM IRMAO DIREITO =======================
StatusRemocao fundir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho)
{
    Arv45 *filho_underflow = *ponteiro_filho_no_pai;
    if (pai == NULL || irmao_dir == NULL || filho_underflow == NULL || irmao_dir->nInfo == 0)
    {
        return UNDERFLOW;
    }

    int chave_pai_desce_idx = pos_filho;
    if (chave_pai_desce_idx >= pai->nInfo)
        return UNDERFLOW;

    int chave_pai_desce = get_key(pai, chave_pai_desce_idx);

    set_key(filho_underflow, 0, chave_pai_desce);
    filho_underflow->nInfo++;

    for (int i = 0; i < irmao_dir->nInfo; i++)
    {
        set_key(filho_underflow, filho_underflow->nInfo, get_key(irmao_dir, i));
        filho_underflow->nInfo++;
    }

    set_child(filho_underflow, 1, irmao_dir->esq);
    set_child(filho_underflow, 2, irmao_dir->f_cen1);
    set_child(filho_underflow, 3, irmao_dir->f_cen2);
    set_child(filho_underflow, 4, irmao_dir->f_cen3);
    // Nota: O quinto filho (dir) do irmao_dir só é movido se irmao_dir.nInfo == 4.
    // O set_child genérico (set_child(no, idx, child)) pode ser mais robusto se as posições forem calculadas.

    for (int i = chave_pai_desce_idx; i < pai->nInfo - 1; i++)
    {
        set_key(pai, i, get_key(pai, i + 1));
        set_child(pai, i + 1, get_child(pai, i + 2));
    }
    set_child(pai, pai->nInfo, NULL);
    set_key(pai, pai->nInfo - 1, 0);
    pai->nInfo--;

    free(irmao_dir);

    if (pai->nInfo < MIN_KEYS && pai->nInfo > 0)
    {
        return UNDERFLOW;
    }
    else if (pai->nInfo == 0)
    {
        return UNDERFLOW;
    }
    return OK;
}

// ======================= REMOCAO (Recursiva) =======================
StatusRemocao remover_45_recursivo(Arv45 **ponteiro_no_atual_ptr, int valor)
{
    Arv45 *no_atual = *ponteiro_no_atual_ptr;
    StatusRemocao status_recursao = OK;

    if (no_atual == NULL)
    {
        return OK;
    }

    int i = 0;
    while (i < no_atual->nInfo && valor > get_key(no_atual, i))
    {
        i++;
    }

    if (i < no_atual->nInfo && valor == get_key(no_atual, i))
    {
        if (eh_folha_45(no_atual))
        {
            remover_chave_folha(no_atual, i);
            if (no_atual->nInfo < MIN_KEYS)
            {
                status_recursao = UNDERFLOW;
            }
        }
        else
        {
            Arv45 **ponteiro_para_raiz_subarvore_sucessor = NULL;
            if (i == 0)
                ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen1);
            else if (i == 1)
                ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen2);
            else if (i == 2)
                ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen3);
            else if (i == 3)
                ponteiro_para_raiz_subarvore_sucessor = &(no_atual->dir);
            else
                return UNDERFLOW;

            Arv45 *sucessor_node = buscar_menor_elemento_45(*ponteiro_para_raiz_subarvore_sucessor);
            if (sucessor_node == NULL)
            {
                fprintf(stderr, "Erro critico: Sucessor nao encontrado para %d (subarvore do sucessor pode ser invalida)!\n", valor);
                return UNDERFLOW;
            }
            int valor_sucessor = sucessor_node->info1;

            set_key(no_atual, i, valor_sucessor);

            StatusRemocao status_rem_suc = remover_45_recursivo(ponteiro_para_raiz_subarvore_sucessor, valor_sucessor);
            if (status_rem_suc == UNDERFLOW)
            {
                status_recursao = tratar_underflow_45(ponteiro_para_raiz_subarvore_sucessor, no_atual, i + 1);
            }
        }
    }
    else
    {
        Arv45 **proximo_ponteiro_filho = NULL;
        if (i == 0)
            proximo_ponteiro_filho = &(no_atual->esq);
        else if (i == 1)
            proximo_ponteiro_filho = &(no_atual->f_cen1);
        else if (i == 2)
            proximo_ponteiro_filho = &(no_atual->f_cen2);
        else if (i == 3)
            proximo_ponteiro_filho = &(no_atual->f_cen3);
        else if (i == 4)
            proximo_ponteiro_filho = &(no_atual->dir);

        if (proximo_ponteiro_filho != NULL && *proximo_ponteiro_filho != NULL)
        {
            status_recursao = remover_45_recursivo(proximo_ponteiro_filho, valor);
            if (status_recursao == UNDERFLOW)
            {
                status_recursao = tratar_underflow_45(proximo_ponteiro_filho, no_atual, i);
            }
        }
        else
        {
            status_recursao = OK;
        }
    }

    return status_recursao;
}

int remover_45(Arv45 **raiz_ptr, int valor)
{
    if (raiz_ptr == NULL || *raiz_ptr == NULL)
    {
        return 0; // Árvore vazia, falha na remoção (o valor não está lá)
    }

    StatusRemocao status_geral = remover_45_recursivo(raiz_ptr, valor);

    // Tratamento final se a raiz ficou em underflow (nInfo == 0)
    // Isso acontece se a raiz se fundiu com um filho e o filho "assumiu" o lugar,
    // ou se o último elemento da árvore foi removido.
    Arv45 *raiz_atual = *raiz_ptr;
    if (raiz_atual != NULL && raiz_atual->nInfo == 0)
    { // Raiz está vazia
        Arv45 *nova_raiz = NULL;
        // Se a raiz tem um filho, este filho se torna a nova raiz (altura diminui).
        if (raiz_atual->esq != NULL)
        {
            nova_raiz = raiz_atual->esq;
        }
        else if (raiz_atual->f_cen1 != NULL) // Em cenários muito específicos, f_cen1 pode ser o único filho.
        {
            nova_raiz = raiz_atual->f_cen1;
        }
        // Se raiz_atual->nInfo == 0 e todos os filhos são NULL, a árvore está vazia.
        // nova_raiz permanecerá NULL, o que está correto para uma árvore vazia.

        free(raiz_atual);      // Libera o nó da raiz antiga (vazia)
        *raiz_ptr = nova_raiz; // Atualiza o ponteiro da raiz da árvore

        return 1; // Remoção bem-sucedida, com rebalanceamento da raiz
    }
    // Se a raiz não está vazia (nInfo >= 1 para a raiz, que é o mínimo global)
    // E o status geral da recursão é OK (significa que todo o underflow foi resolvido)
    else if (raiz_atual != NULL && status_geral == OK)
    {
        return 1; // Remoção bem-sucedida, sem necessidade de mudar a raiz ou ela já foi tratada
    }
    // Se a raiz ainda existe, mas o status_geral indica UNDERFLOW,
    // significa que algo deu errado e o underflow não pôde ser completamente resolvido.
    // Ou o valor simplesmente não foi encontrado na árvore, e o status_geral seria OK.
    else
    {
        // Este caso pode cobrir:
        // 1. Valor não encontrado na árvore (status_geral == OK, mas nenhuma remoção ocorreu de fato).
        // 2. Um erro inesperado ou underflow não resolvível que não levou à raiz ficar vazia.
        // Para fins de um retorno simples (0/1), se o valor não foi encontrado, é uma falha na remoção.
        // Se ele foi encontrado e o status_geral não é OK (i.e. UNDERFLOW), é uma falha no balanceamento.
        // Podemos simplificar: se o status_geral é OK, a remoção foi bem-sucedida (o valor existia).
        // Caso contrário, algo deu errado (o valor não foi encontrado ou underflow não resolvido).

        // Uma forma mais precisa de saber se o valor foi *realmente* removido seria:
        // 1. Procurar o valor antes da remoção. Se achou: tentar remover.
        // 2. Depois da remoção, procurar novamente. Se não achou: sucesso.
        // No entanto, para o retorno de 0/1, se o processo recursivo terminou com OK, assumimos sucesso.
        // Se terminou com UNDERFLOW (e a raiz não ficou vazia), é falha.
        return (status_geral == OK) ? 1 : 0;
    }
}