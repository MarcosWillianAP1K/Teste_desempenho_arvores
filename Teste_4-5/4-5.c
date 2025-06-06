#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "4-5.h"

// Enum para status da remocao
typedef enum
{
    OK,
    UNDERFLOW
} StatusRemocao;


StatusRemocao tratar_underflow_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai);
StatusRemocao redistribuir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho);
StatusRemocao redistribuir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho);
StatusRemocao fundir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho);
StatusRemocao fundir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho);



int *buscar_45(Arv45 *raiz, int info)
{
    int *retorno = NULL;

    if (raiz != NULL)
    {
        if (info < raiz->info1)
        {
            retorno = buscar_45(raiz->esq, info);
        }
        else if (info == raiz->info1)
        {
            retorno = &(raiz->info1);
        }
        else if (raiz->nInfo >= 2 && info < raiz->info2)
        {
            retorno = buscar_45(raiz->f_cen1, info);
        }
        else if (raiz->nInfo >= 2 && info == raiz->info2)
        {
            retorno = &(raiz->info2);
        }
        else if (raiz->nInfo >= 3 && info < raiz->info3)
        {
            retorno = buscar_45(raiz->f_cen2, info);
        }
        else if (raiz->nInfo >= 3 && info == raiz->info3)
        {
            retorno = &(raiz->info3);
        }
        else if (raiz->nInfo == 4 && info < raiz->info4)
        {
            retorno = buscar_45(raiz->f_cen3, info);
        }
        else if (raiz->nInfo == 4 && info == raiz->info4)
        {
            retorno = &(raiz->info4);
        }
        else
        {
            retorno = buscar_45(raiz->dir, info);
        }
    }

    return retorno;
}


//================ CRIA NO ==================
Arv45 *cria_no_45(int info, Arv45 *F_esq, Arv45 *F_f_cen1)
{
    Arv45 *no = (Arv45 *)malloc(sizeof(Arv45));
    if (no != NULL)
    {
        no->info1 = info;
        no->nInfo = 1;
        no->esq = F_esq;
        no->f_cen1 = F_f_cen1;
        no->f_cen2 = NULL;
        no->f_cen3 = NULL;
        no->dir = NULL;
        // info2, info3, info4 contêm valores indeterminados inicialmente
    }
    return no;
}

//================ VERIFICA FOLHA ==================
int eh_folha_45(Arv45 *no)
{
    int resultado = 0;
    if (no != NULL)
    {
        if (no->esq == NULL && no->f_cen1 == NULL && no->f_cen2 == NULL && no->f_cen3 == NULL && no->dir == NULL)
        {
            resultado = 1;
        }
    }
    return resultado;
}

//================ ADICIONA CHAVE EM NÓ COM ESPAÇO ==================
void adiciona_chave_em_no_com_espaco_45(Arv45 *no_atual, int info_nova, Arv45 *sub_arv_direita_da_info_nova)
{
    // Esta função assume que no_atual != NULL e no_atual->nInfo < 4
    // A checagem de guarda foi removida para aderir ao "single exit" conceitual (erro seria crítico).
    // Em um código de produção, a checagem no início seria importante.

    int j;
    int pos_insercao_chave = 0;

    if (no_atual->nInfo >= 1 && info_nova > no_atual->info1)
        pos_insercao_chave = 1;
    if (no_atual->nInfo >= 2 && info_nova > no_atual->info2)
        pos_insercao_chave = 2;
    if (no_atual->nInfo >= 3 && info_nova > no_atual->info3)
        pos_insercao_chave = 3;

    // Desloca chaves existentes para a direita
    for (j = no_atual->nInfo - 1; j >= pos_insercao_chave; j--)
    {
        if (j == 2)
            no_atual->info4 = no_atual->info3;
        else if (j == 1)
            no_atual->info3 = no_atual->info2;
        else if (j == 0)
            no_atual->info2 = no_atual->info1;
    }

    // Desloca ponteiros de filhos existentes para a direita
    for (j = no_atual->nInfo; j >= pos_insercao_chave + 1; j--)
    {
        if (j == 3)
            no_atual->dir = no_atual->f_cen3;
        else if (j == 2)
            no_atual->f_cen3 = no_atual->f_cen2;
        else if (j == 1)
            no_atual->f_cen2 = no_atual->f_cen1;
    }

    // Insere a nova chave
    if (pos_insercao_chave == 0)
        no_atual->info1 = info_nova;
    else if (pos_insercao_chave == 1)
        no_atual->info2 = info_nova;
    else if (pos_insercao_chave == 2)
        no_atual->info3 = info_nova;
    else if (pos_insercao_chave == 3)
        no_atual->info4 = info_nova;

    // Insere o novo filho (à DIREITA da info_nova)
    if (pos_insercao_chave == 0)
        no_atual->f_cen1 = sub_arv_direita_da_info_nova;
    else if (pos_insercao_chave == 1)
        no_atual->f_cen2 = sub_arv_direita_da_info_nova;
    else if (pos_insercao_chave == 2)
        no_atual->f_cen3 = sub_arv_direita_da_info_nova;
    else if (pos_insercao_chave == 3)
        no_atual->dir = sub_arv_direita_da_info_nova;

    no_atual->nInfo++;
}

//================ QUEBRA NO ==================
// Esta função é chamada quando no_original_ptr aponta para um nó com 4 chaves,
// e info_a_ser_inserida (com seu filho_dir_da_info_a_ser_inserida) é a 5ª combinação chave/filho.
// O nó original é modificado para se tornar o nó esquerdo da divisão.
// Retorna o novo nó direito criado.
// chave_promovida_saida conterá a chave que sobe para o pai.
Arv45 *quebra_no_45(Arv45 **no_original_ptr, int N_nova, Arv45 *Nc_novo_filho_dir, int *chave_promovida_saida)
{
    Arv45 *no_orig = *no_original_ptr;
    Arv45 *novo_no_dir = NULL; // Valor de retorno

    // Chaves e filhos originais do nó cheio
    int i1_orig = no_orig->info1, i2_orig = no_orig->info2, i3_orig = no_orig->info3, i4_orig = no_orig->info4;
    Arv45 *c0_orig = no_orig->esq, *c1_orig = no_orig->f_cen1, *c2_orig = no_orig->f_cen2,
          *c3_orig = no_orig->f_cen3, *c4_orig = no_orig->dir;

    // Lógica para determinar a chave promovida e rearranjar as 5 chaves e 6 filhos
    // sem usar arrays temporários. Isso resulta em múltiplos casos.

    if (N_nova < i1_orig)
    { // Caso 1: N i1 i2 i3 i4 -> Promove i2
        *chave_promovida_saida = i2_orig;
        // Nó original (esquerdo): N, i1_orig
        no_orig->info1 = N_nova;
        no_orig->info2 = i1_orig;
        no_orig->esq = c0_orig;
        no_orig->f_cen1 = Nc_novo_filho_dir;
        no_orig->f_cen2 = c1_orig;
        no_orig->nInfo = 2;
        // Novo nó direito: i3_orig, i4_orig
        novo_no_dir = cria_no_45(i3_orig, c2_orig, c3_orig);
        if (novo_no_dir)
        {
            novo_no_dir->info2 = i4_orig;
            novo_no_dir->f_cen2 = c4_orig;
            novo_no_dir->nInfo = 2;
        }
    }
    else if (N_nova < i2_orig)
    { // Caso 2: i1 N i2 i3 i4 -> Promove i2
        *chave_promovida_saida = i2_orig;
        // Nó original (esquerdo): i1_orig, N_nova
        // no_orig->info1 já é i1_orig
        no_orig->info2 = N_nova;
        no_orig->nInfo = 2;
        // no_orig->esq já é c0_orig
        // no_orig->f_cen1 já é c1_orig
        no_orig->f_cen2 = Nc_novo_filho_dir;
        // Novo nó direito: i3_orig, i4_orig
        novo_no_dir = cria_no_45(i3_orig, c2_orig, c3_orig);
        if (novo_no_dir)
        {
            novo_no_dir->info2 = i4_orig;
            novo_no_dir->f_cen2 = c4_orig;
            novo_no_dir->nInfo = 2;
        }
    }
    else if (N_nova < i3_orig)
    { // Caso 3: i1 i2 N i3 i4 -> Promove N_nova
        *chave_promovida_saida = N_nova;
        // Nó original (esquerdo): i1_orig, i2_orig
        // no_orig->info1, info2, esq, f_cen1 já estão corretos
        no_orig->f_cen2 = c2_orig; // Filho à direita de i2_orig (e à esquerda da promovida N_nova)
        no_orig->nInfo = 2;
        // Novo nó direito: i3_orig, i4_orig
        novo_no_dir = cria_no_45(i3_orig, Nc_novo_filho_dir, c3_orig); // Nc_novo_filho_dir é filho esq de i3_orig
        if (novo_no_dir)
        {
            novo_no_dir->info2 = i4_orig;
            novo_no_dir->f_cen2 = c4_orig;
            novo_no_dir->nInfo = 2;
        }
    }
    else if (N_nova < i4_orig)
    { // Caso 4: i1 i2 i3 N i4 -> Promove i3_orig
        *chave_promovida_saida = i3_orig;
        // Nó original (esquerdo): i1_orig, i2_orig
        // no_orig->info1, info2, esq, f_cen1 já estão corretos
        no_orig->f_cen2 = c2_orig;
        no_orig->nInfo = 2;
        // Novo nó direito: N_nova, i4_orig
        novo_no_dir = cria_no_45(N_nova, c3_orig, Nc_novo_filho_dir); // c3_orig é esq de N_nova, Nc_novo_filho_dir é f_cen1 de N_nova
        if (novo_no_dir)
        {
            novo_no_dir->info2 = i4_orig;
            novo_no_dir->f_cen2 = c4_orig;
            novo_no_dir->nInfo = 2;
        }
    }
    else
    { // Caso 5: i1 i2 i3 i4 N -> Promove i3_orig
        *chave_promovida_saida = i3_orig;
        // Nó original (esquerdo): i1_orig, i2_orig
        // no_orig->info1, info2, esq, f_cen1 já estão corretos
        no_orig->f_cen2 = c2_orig;
        no_orig->nInfo = 2;
        // Novo nó direito: i4_orig, N_nova
        novo_no_dir = cria_no_45(i4_orig, c3_orig, c4_orig);
        if (novo_no_dir)
        {
            novo_no_dir->info2 = N_nova;
            novo_no_dir->f_cen2 = Nc_novo_filho_dir;
            novo_no_dir->nInfo = 2;
        }
    }

    // Limpa as chaves e filhos restantes no nó original (que agora é o nó esquerdo e tem 2 chaves)
    no_orig->info3 = 0;
    no_orig->info4 = 0;
    no_orig->f_cen3 = NULL;
    no_orig->dir = NULL;

    // Garante que o novo nó direito também esteja limpo se tiver menos de 2 chaves (não deve acontecer aqui)
    if (novo_no_dir != NULL)
    {
        if (novo_no_dir->nInfo < 2)
        {
            novo_no_dir->info2 = 0;
            novo_no_dir->f_cen2 = NULL;
        }
        novo_no_dir->f_cen3 = NULL;
        novo_no_dir->dir = NULL;
    }

    return novo_no_dir;
}

//================ INSERCAO (Recursiva) ==================
int insere_45_recursivo(Arv45 **raiz_ptr, int valor, int *chave_promovida_acima, Arv45 **novo_irmao_a_direita)
{
    int status_operacao = 0;
    Arv45 *no_atual = (raiz_ptr != NULL) ? *raiz_ptr : NULL;

    *chave_promovida_acima = 0;
    *novo_irmao_a_direita = NULL;

    if (no_atual == NULL)
    {
        *chave_promovida_acima = valor;
        status_operacao = 1;
    }
    else
    {
        int duplicado = 0;
        if (no_atual->nInfo >= 1 && valor == no_atual->info1)
            duplicado = 1;
        else if (no_atual->nInfo >= 2 && valor == no_atual->info2)
            duplicado = 1;
        else if (no_atual->nInfo >= 3 && valor == no_atual->info3)
            duplicado = 1;
        else if (no_atual->nInfo >= 4 && valor == no_atual->info4)
            duplicado = 1;

        if (duplicado)
        {
            // status_operacao já é 0
        }
        else
        {
            if (eh_folha_45(no_atual))
            {
                if (no_atual->nInfo < 4)
                { // 4 é MAX_KEYS
                    adiciona_chave_em_no_com_espaco_45(no_atual, valor, NULL);
                    status_operacao = 1;
                }
                else
                { // Nó folha cheio (nInfo == 4), precisa quebrar
                    *novo_irmao_a_direita = quebra_no_45(raiz_ptr, valor, NULL, chave_promovida_acima);
                    status_operacao = 1;
                }
            }
            else
            { // Nó interno
                Arv45 **proximo_filho_ptr = NULL;
                if (valor < no_atual->info1)
                {
                    proximo_filho_ptr = &(no_atual->esq);
                }
                else if (no_atual->nInfo == 1 || (no_atual->nInfo >= 2 && valor < no_atual->info2))
                {
                    proximo_filho_ptr = &(no_atual->f_cen1);
                }
                else if (no_atual->nInfo == 2 || (no_atual->nInfo >= 3 && valor < no_atual->info3))
                {
                    proximo_filho_ptr = &(no_atual->f_cen2);
                }
                else if (no_atual->nInfo == 3 || (no_atual->nInfo >= 4 && valor < no_atual->info4))
                {
                    proximo_filho_ptr = &(no_atual->f_cen3);
                }
                else
                { // valor > última chave ou nInfo é 4 e valor > info4
                    proximo_filho_ptr = &(no_atual->dir);
                }

                int chave_vinda_de_baixo = 0;
                Arv45 *filho_direito_vindo_de_baixo = NULL;

                if (insere_45_recursivo(proximo_filho_ptr, valor, &chave_vinda_de_baixo, &filho_direito_vindo_de_baixo))
                {
                    if (chave_vinda_de_baixo != 0)
                    { // Algo subiu da chamada recursiva
                        if (no_atual->nInfo < 4)
                        { // Nó atual tem espaço
                            adiciona_chave_em_no_com_espaco_45(no_atual, chave_vinda_de_baixo, filho_direito_vindo_de_baixo);
                            status_operacao = 1;
                        }
                        else
                        { // Nó atual está cheio, precisa quebrar
                            *novo_irmao_a_direita = quebra_no_45(raiz_ptr, chave_vinda_de_baixo, filho_direito_vindo_de_baixo, chave_promovida_acima);
                            status_operacao = 1;
                        }
                    }
                    else
                    { // Inserção abaixo resolvida sem promoção para este nó.
                        status_operacao = 1;
                    }
                } // else status_operacao já é 0 (falha na subárvore)
            }
        }
    }
    return status_operacao;
}

//================ INSERCAO (Principal) ==================
int insere_45(Arv45 **raiz, int valor)
{
    int sucesso = 0;
    int chave_promovida_da_raiz = 0;
    Arv45 *novo_no_irmao_da_raiz = NULL;

    if (raiz != NULL)
    {
        if (*raiz == NULL)
        {
            *raiz = cria_no_45(valor, NULL, NULL);
            if (*raiz != NULL)
            {
                sucesso = 1;
            }
        }
        else
        {
            int duplicado_na_raiz = 0;
            if ((*raiz)->nInfo >= 1 && valor == (*raiz)->info1)
                duplicado_na_raiz = 1;
            else if ((*raiz)->nInfo >= 2 && valor == (*raiz)->info2)
                duplicado_na_raiz = 1;
            else if ((*raiz)->nInfo >= 3 && valor == (*raiz)->info3)
                duplicado_na_raiz = 1;
            else if ((*raiz)->nInfo >= 4 && valor == (*raiz)->info4)
                duplicado_na_raiz = 1;

            if (duplicado_na_raiz)
            {
                printf("Valor %d ja existe na arvore (raiz)!\n", valor);
                // sucesso permanece 0
            }
            else if (insere_45_recursivo(raiz, valor, &chave_promovida_da_raiz, &novo_no_irmao_da_raiz))
            {
                if (chave_promovida_da_raiz != 0)
                { // Raiz original quebrou
                    Arv45 *antiga_raiz = *raiz;
                    *raiz = cria_no_45(chave_promovida_da_raiz, antiga_raiz, novo_no_irmao_da_raiz);
                    if (*raiz != NULL)
                    {
                        sucesso = 1;
                    }
                    else
                    {
                        fprintf(stderr, "Erro ao criar nova raiz!\n");
                        if (novo_no_irmao_da_raiz)
                            free(novo_no_irmao_da_raiz); // Tenta liberar se falhou
                        // sucesso permanece 0
                    }
                }
                else
                { // Inserção bem-sucedida sem nova raiz
                    sucesso = 1;
                }
            } // else: falha na inserção recursiva, sucesso permanece 0
        }
    }
    return sucesso;
}

//================ BUSCAR MENOR ==================
Arv45 *buscar_menor_elemento_45(Arv45 *no)
{
    Arv45 *resultado = NULL;
    Arv45 *atual = no;

    if (atual != NULL)
    {
        while (atual->esq != NULL)
        {
            atual = atual->esq;
        }
        resultado = atual;
    }
    return resultado;
}

//================ TRATAR UNDERFLOW ==================
StatusRemocao tratar_underflow_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai)
{
    StatusRemocao status_final = OK;
    Arv45 *filho_com_underflow = (ponteiro_filho_no_pai != NULL) ? *ponteiro_filho_no_pai : NULL;

    // Nó está em underflow se tem 0 chaves (1 é MIN_KEYS_ALLOWED)
    if (filho_com_underflow == NULL || (filho_com_underflow->nInfo >= 1))
    {
        // status_final permanece OK;
    }
    else if (filho_com_underflow->nInfo == 0)
    { // Underflow confirmado
        if (pai == NULL)
        {                             // Underflow na raiz
            status_final = UNDERFLOW; // Será tratada por remover_45
        }
        else
        {
            Arv45 *irmao_esq = NULL;
            Arv45 *irmao_dir = NULL;
            int pos_filho = -1;
            int operacao_realizada = 0;

            if (pai->esq == filho_com_underflow)
            {
                pos_filho = 0;
                irmao_dir = pai->f_cen1;
            }
            else if (pai->f_cen1 == filho_com_underflow)
            {
                pos_filho = 1;
                irmao_esq = pai->esq;
                irmao_dir = pai->f_cen2;
            }
            else if (pai->f_cen2 == filho_com_underflow)
            {
                pos_filho = 2;
                irmao_esq = pai->f_cen1;
                irmao_dir = pai->f_cen3;
            }
            else if (pai->f_cen3 == filho_com_underflow)
            {
                pos_filho = 3;
                irmao_esq = pai->f_cen2;
                irmao_dir = pai->dir;
            }
            else if (pai->dir == filho_com_underflow && pai->nInfo == 4)
            { // 4 é MAX_KEYS
                pos_filho = 4;
                irmao_esq = pai->f_cen3;
            }

            if (pos_filho != -1)
            {
                if (irmao_dir != NULL && irmao_dir->nInfo > 1)
                { // > MIN_KEYS_ALLOWED
                    status_final = redistribuir_com_irmao_direito_45(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
                    operacao_realizada = 1;
                }
                else if (irmao_esq != NULL && irmao_esq->nInfo > 1)
                { // > MIN_KEYS_ALLOWED
                    status_final = redistribuir_com_irmao_esquerdo_45(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
                    operacao_realizada = 1;
                }
                else if (irmao_dir != NULL && irmao_dir->nInfo == 1)
                { // == MIN_KEYS_ALLOWED
                    status_final = fundir_com_irmao_direito_45(ponteiro_filho_no_pai, pai, irmao_dir, pos_filho);
                    operacao_realizada = 1;
                }
                else if (irmao_esq != NULL && irmao_esq->nInfo == 1)
                { // == MIN_KEYS_ALLOWED
                    status_final = fundir_com_irmao_esquerdo_45(ponteiro_filho_no_pai, pai, irmao_esq, pos_filho);
                    operacao_realizada = 1;
                }

                if (!operacao_realizada)
                {
                    status_final = UNDERFLOW; // Não conseguiu resolver, propaga
                }
            }
            else if (filho_com_underflow != pai)
            { // Se não é a raiz e não foi encontrado como filho
                fprintf(stderr, "Erro critico: Filho em underflow nao eh filho do pai.\n");
                status_final = UNDERFLOW;
            }
        }
    }
    return status_final;
}

//================ REDISTRIBUIR COM IRMAO ESQUERDO ==================
StatusRemocao redistribuir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho)
{
    StatusRemocao status_retorno = OK;
    Arv45 *filho = *ponteiro_filho_no_pai; // Nó em underflow (nInfo == 0)

    if (pai == NULL || irmao_esq == NULL || filho == NULL || irmao_esq->nInfo <= 1)
    { // 1 é MIN_KEYS_ALLOWED
        status_retorno = UNDERFLOW;
    }
    else
    {
        int chave_pai_desce = 0;
        Arv45 *filho_transferido_do_irmao = NULL;
        int chave_irmao_sobe = 0;

        // Determina chave do pai que desce
        if (pos_filho == 1)
            chave_pai_desce = pai->info1;
        else if (pos_filho == 2)
            chave_pai_desce = pai->info2;
        else if (pos_filho == 3)
            chave_pai_desce = pai->info3;
        else if (pos_filho == 4)
            chave_pai_desce = pai->info4; // Se filho é dir do pai
        else
        {
            status_retorno = UNDERFLOW;
        } // Posição inválida

        if (status_retorno == OK)
        {
            // Determina chave e filho que vêm do irmão esquerdo
            if (irmao_esq->nInfo == 2)
            {
                chave_irmao_sobe = irmao_esq->info2;
                filho_transferido_do_irmao = irmao_esq->f_cen2;
            }
            else if (irmao_esq->nInfo == 3)
            {
                chave_irmao_sobe = irmao_esq->info3;
                filho_transferido_do_irmao = irmao_esq->f_cen3;
            }
            else if (irmao_esq->nInfo == 4)
            {
                chave_irmao_sobe = irmao_esq->info4;
                filho_transferido_do_irmao = irmao_esq->dir;
            }
            else
            {
                status_retorno = UNDERFLOW;
            } // nInfo do irmão não permite doar

            if (status_retorno == OK)
            {
                // Atualiza pai
                if (pos_filho == 1)
                    pai->info1 = chave_irmao_sobe;
                else if (pos_filho == 2)
                    pai->info2 = chave_irmao_sobe;
                else if (pos_filho == 3)
                    pai->info3 = chave_irmao_sobe;
                else if (pos_filho == 4)
                    pai->info4 = chave_irmao_sobe;

                // Atualiza filho (nó em underflow)
                Arv45 *esq_original_filho = filho->esq; // Salva o filho esquerdo original do nó em underflow
                filho->info1 = chave_pai_desce;
                filho->nInfo = 1;
                filho->esq = filho_transferido_do_irmao;
                filho->f_cen1 = esq_original_filho;
                filho->f_cen2 = NULL;
                filho->f_cen3 = NULL;
                filho->dir = NULL;

                // Atualiza irmão esquerdo
                if (irmao_esq->nInfo == 2)
                {
                    irmao_esq->info2 = 0;
                    irmao_esq->f_cen2 = NULL;
                }
                else if (irmao_esq->nInfo == 3)
                {
                    irmao_esq->info3 = 0;
                    irmao_esq->f_cen3 = NULL;
                }
                else if (irmao_esq->nInfo == 4)
                {
                    irmao_esq->info4 = 0;
                    irmao_esq->dir = NULL;
                }
                irmao_esq->nInfo--;
            }
        }
    }
    return status_retorno;
}

//================ REDISTRIBUIR COM IRMAO DIREITO ==================
StatusRemocao redistribuir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho)
{
    StatusRemocao status_retorno = OK;
    Arv45 *filho = *ponteiro_filho_no_pai; // Nó em underflow (nInfo == 0)

    if (pai == NULL || irmao_dir == NULL || filho == NULL || irmao_dir->nInfo <= 1)
    { // 1 é MIN_KEYS_ALLOWED
        status_retorno = UNDERFLOW;
    }
    else
    {
        int chave_pai_desce = 0;
        Arv45 *filho_transferido_do_irmao = NULL;
        int chave_irmao_sobe = 0;

        if (pos_filho == 0)
            chave_pai_desce = pai->info1;
        else if (pos_filho == 1)
            chave_pai_desce = pai->info2;
        else if (pos_filho == 2)
            chave_pai_desce = pai->info3;
        else if (pos_filho == 3 && pai->nInfo == 4)
            chave_pai_desce = pai->info4; // 4 é MAX_KEYS
        else
        {
            status_retorno = UNDERFLOW;
        }

        if (status_retorno == OK)
        {
            chave_irmao_sobe = irmao_dir->info1;         // Primeira chave do irmão direito
            filho_transferido_do_irmao = irmao_dir->esq; // Filho esquerdo do irmão direito

            // Atualiza pai
            if (pos_filho == 0)
                pai->info1 = chave_irmao_sobe;
            else if (pos_filho == 1)
                pai->info2 = chave_irmao_sobe;
            else if (pos_filho == 2)
                pai->info3 = chave_irmao_sobe;
            else if (pos_filho == 3 && pai->nInfo == 4)
                pai->info4 = chave_irmao_sobe;

            // Atualiza filho (nó em underflow)
            // O filho esquerdo original do filho permanece.
            filho->info1 = chave_pai_desce;
            filho->nInfo = 1;
            filho->f_cen1 = filho_transferido_do_irmao;
            filho->f_cen2 = NULL;
            filho->f_cen3 = NULL;
            filho->dir = NULL;

            // Atualiza irmão direito (desloca chaves e filhos para a esquerda)
            irmao_dir->info1 = irmao_dir->info2;
            irmao_dir->info2 = irmao_dir->info3;
            irmao_dir->info3 = irmao_dir->info4;
            irmao_dir->info4 = 0;
            irmao_dir->esq = irmao_dir->f_cen1;
            irmao_dir->f_cen1 = irmao_dir->f_cen2;
            irmao_dir->f_cen2 = irmao_dir->f_cen3;
            irmao_dir->f_cen3 = irmao_dir->dir;
            irmao_dir->dir = NULL;
            irmao_dir->nInfo--;
        }
    }
    return status_retorno;
}

//================ FUNDIR COM IRMAO ESQUERDO ==================
StatusRemocao fundir_com_irmao_esquerdo_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_esq, int pos_filho)
{
    StatusRemocao status_pai_final = UNDERFLOW;
    Arv45 *filho_underflow = (ponteiro_filho_no_pai != NULL) ? *ponteiro_filho_no_pai : NULL;
    int pode_prosseguir = 1; // Flag para controlar o fluxo

    // Verificações iniciais
    if (pai == NULL || irmao_esq == NULL || filho_underflow == NULL || irmao_esq->nInfo != 1)
    {
        // 1 é MIN_KEYS_ALLOWED
        // fprintf(stderr, "Erro: Condicoes invalidas para fusao esquerda (irmao_esq->nInfo=%d).\n", irmao_esq ? irmao_esq->nInfo : -1);
        pode_prosseguir = 0; // Não continua a execução
    }

    if (pode_prosseguir)
    {
        int chave_pai_desce = 0;
        Arv45 *fu_esq_original = filho_underflow->esq;
        Arv45 *fu_fcen1_original = filho_underflow->f_cen1;

        // Verificação da posição do filho
        if (pos_filho == 1)
        {
            chave_pai_desce = pai->info1;
            pai->info1 = pai->info2;
            pai->info2 = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen1 = pai->f_cen2;
            pai->f_cen2 = pai->f_cen3;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 2)
        {
            chave_pai_desce = pai->info2;
            pai->info2 = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen2 = pai->f_cen3;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 3)
        {
            chave_pai_desce = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 4)
        {
            chave_pai_desce = pai->info4;
            pai->info4 = 0;
            pai->dir = NULL;
        }
        else
        {
            /*fprintf(stderr, "Pos_filho %d invalida em fusao esquerda\n", pos_filho);*/
            pode_prosseguir = 0; // Substitui o early return
        }

        if (pode_prosseguir) // Continua apenas se pos_filho for válido
        {
            pai->nInfo--;

            // irmao_esq (1 chave: info1) + chave_pai_desce. Resulta em 2 chaves.
            irmao_esq->info2 = chave_pai_desce; // info1 do irmao_esq já existe
            irmao_esq->nInfo = 2;

            // Filhos de irmao_esq (agora com 2 chaves: info1, info2): esq, f_cen1, f_cen2
            // irmao_esq->esq (original do irmao_esq)
            // irmao_esq->f_cen1 (original do irmao_esq)
            // irmao_esq->f_cen2 (novo) = fu_esq_original (filho esquerdo do nó que estava em underflow)
            irmao_esq->f_cen2 = fu_esq_original;

            // Tratamento especial para o caso onde f_cen1 original era NULL
            if (irmao_esq->f_cen1 == NULL && fu_esq_original != NULL)
            {
                irmao_esq->f_cen1 = fu_esq_original;   // fu_esq se torna f_cen1
                irmao_esq->f_cen2 = fu_fcen1_original; // fu_fcen1 se torna f_cen2
            }
            else if (fu_fcen1_original != NULL)
            {
                // fprintf(stderr, "Alerta: Fusao esquerda, fu_fcen1_original nao acomodado diretamente nos 3 filhos.\n");
            }

            irmao_esq->f_cen3 = NULL;
            irmao_esq->dir = NULL; // Limpa para consistência de 2 chaves

            free(filho_underflow);
            if (ponteiro_filho_no_pai != NULL)
                *ponteiro_filho_no_pai = NULL;

            // Determina o status final
            if (pai->nInfo == 0 && pai != NULL)
            {
                status_pai_final = UNDERFLOW;
            }
            else if (pai->nInfo > 0 && pai->nInfo < 1)
            {
                status_pai_final = UNDERFLOW;
            } // 1 é MIN_KEYS_ALLOWED
            else
            {
                status_pai_final = OK;
            }
        }
    }

    return status_pai_final; // Único ponto de retorno da função
}

//================ FUNDIR COM IRMAO DIREITO ==================
StatusRemocao fundir_com_irmao_direito_45(Arv45 **ponteiro_filho_no_pai, Arv45 *pai, Arv45 *irmao_dir, int pos_filho)
{
    StatusRemocao status_pai_final = UNDERFLOW;
    Arv45 *filho_underflow = (ponteiro_filho_no_pai != NULL) ? *ponteiro_filho_no_pai : NULL;
    int pode_prosseguir = 1; // Flag para controlar o fluxo

    // Verificações iniciais
    if (pai == NULL || irmao_dir == NULL || filho_underflow == NULL || irmao_dir->nInfo != 1)
    { // 1 é MIN_KEYS_ALLOWED
        // fprintf(stderr, "Erro: Condicoes invalidas para fusao direita (irmao_dir->nInfo=%d).\n", irmao_dir ? irmao_dir->nInfo : -1);
        pode_prosseguir = 0; // Não continua a execução
    }

    if (pode_prosseguir)
    {
        int chave_pai_desce = 0;
        Arv45 *fu_esq_original = filho_underflow->esq;
        Arv45 *fu_fcen1_original = filho_underflow->f_cen1;
        Arv45 *id_esq_original = irmao_dir->esq;
        Arv45 *id_fcen1_original = irmao_dir->f_cen1;
        int id_info1_original = irmao_dir->info1;

        // Ajusta o pai e determina a chave que desce
        if (pos_filho == 0)
        {
            chave_pai_desce = pai->info1;
            pai->info1 = pai->info2;
            pai->info2 = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->esq = pai->f_cen1; // irmao_dir (modificado) se torna o novo pai->esq
            pai->f_cen1 = pai->f_cen2;
            pai->f_cen2 = pai->f_cen3;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 1)
        {
            chave_pai_desce = pai->info2;
            pai->info2 = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen1 = pai->f_cen2; // irmao_dir (modificado) se torna o novo pai->f_cen1
            pai->f_cen2 = pai->f_cen3;
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else if (pos_filho == 2)
        {
            chave_pai_desce = pai->info3;
            pai->info3 = pai->info4;
            pai->info4 = 0;
            pai->f_cen2 = pai->f_cen3; // irmao_dir (modificado) se torna o novo pai->f_cen2
            pai->f_cen3 = pai->dir;
            pai->dir = NULL;
        }
        else
        {
            /*fprintf(stderr, "Pos_filho %d invalida em fusao direita\n", pos_filho);*/
            pode_prosseguir = 0; // Substitui o early return
        }

        if (pode_prosseguir) // Continua apenas se pos_filho for válido
        {
            pai->nInfo--;

            // Realiza a fusão no irmao_dir.
            // Novo irmao_dir terá 2 chaves: chave_pai_desce, chave_original_irmao_dir (id_info1_original)
            // Mantendo a ordem: chave_pai_desce < id_info1_original (assumindo que filho_underflow estava à esquerda de irmao_dir)
            irmao_dir->info1 = chave_pai_desce;
            irmao_dir->info2 = id_info1_original;
            irmao_dir->nInfo = 2;

            // Filhos do novo irmao_dir (2 chaves, 3 filhos: esq, f_cen1, f_cen2)
            // Ordem dos filhos originais: fu_esq, fu_fcen1 (do filho_underflow)
            //                               id_esq, id_fcen1 (do irmao_dir)
            irmao_dir->esq = fu_esq_original;      // Filho à esquerda da nova info1 (chave_pai_desce)
            irmao_dir->f_cen1 = fu_fcen1_original; // Filho entre nova info1 e nova info2 (id_info1_original)
            irmao_dir->f_cen2 = id_esq_original;   // Filho à direita da nova info2 (que era esquerdo de id_info1_original)

            // O id_fcen1_original (filho direito original de id_info1_original) seria o 4º.
            // Se fu_fcen1_original era NULL, podemos tentar usar id_esq_original e id_fcen1_original.
            if (fu_fcen1_original == NULL)
            {
                irmao_dir->f_cen1 = id_esq_original;
                irmao_dir->f_cen2 = id_fcen1_original;
            }
            else if (id_fcen1_original != NULL)
            { // Se fu_fcen1_original existia, e id_esq_original foi para f_cen2
              // fprintf(stderr, "Alerta: Fusao direita, id_fcen1_original nao acomodado diretamente.\n");
            }

            // Garante que os ponteiros não utilizados sejam NULL
            irmao_dir->f_cen3 = NULL;
            irmao_dir->dir = NULL;

            free(filho_underflow);
            // O ponteiro do pai (e.g., pai->esq) já aponta para irmao_dir.

            if (pai->nInfo == 0 && pai != NULL)
            {
                status_pai_final = UNDERFLOW;
            }
            else if (pai->nInfo > 0 && pai->nInfo < 1)
            {
                status_pai_final = UNDERFLOW;
            } // 1 é MIN_KEYS_ALLOWED
            else
            {
                status_pai_final = OK;
            }
        }
    }

    return status_pai_final; // Único ponto de retorno da função
}

//================ REMOCAO (Recursiva) ==================
StatusRemocao remover_45_recursivo(Arv45 **ponteiro_no_atual_ptr, int valor)
{
    StatusRemocao status_final = OK;
    Arv45 *no_atual = (ponteiro_no_atual_ptr != NULL) ? *ponteiro_no_atual_ptr : NULL;

    if (no_atual == NULL)
    {
        // status_final permanece OK
    }
    else
    {
        int valor_encontrado_neste_no = 0;
        int indice_valor_removido = -1;
        Arv45 **proximo_ponteiro_recursao = NULL;
        int pode_prosseguir_logica = 1;

        if (no_atual->nInfo >= 1 && valor == no_atual->info1)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 0;
        }
        else if (no_atual->nInfo >= 2 && valor == no_atual->info2)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 1;
        }
        else if (no_atual->nInfo >= 3 && valor == no_atual->info3)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 2;
        }
        else if (no_atual->nInfo >= 4 && valor == no_atual->info4)
        {
            valor_encontrado_neste_no = 1;
            indice_valor_removido = 3;
        }
        else if (no_atual->nInfo >= 1 && valor < no_atual->info1)
        {
            proximo_ponteiro_recursao = &(no_atual->esq);
        }
        else if (no_atual->nInfo == 1 || (no_atual->nInfo >= 2 && valor < no_atual->info2))
        {
            proximo_ponteiro_recursao = &(no_atual->f_cen1);
        }
        else if (no_atual->nInfo == 2 || (no_atual->nInfo >= 3 && valor < no_atual->info3))
        {
            proximo_ponteiro_recursao = &(no_atual->f_cen2);
        }
        else if (no_atual->nInfo == 3 || (no_atual->nInfo == 4 && valor < no_atual->info4))
        {
            proximo_ponteiro_recursao = &(no_atual->f_cen3);
        }
        else if (no_atual->nInfo >= 1)
        { // valor > última chave
            proximo_ponteiro_recursao = &(no_atual->dir);
        }
        else
        { // nInfo = 0 ou valor não se encaixa na lógica de descida
            pode_prosseguir_logica = 0;
        }

        if (pode_prosseguir_logica && valor_encontrado_neste_no)
        {
            if (eh_folha_45(no_atual))
            {
                // Desloca chaves para a esquerda para cobrir a removida
                if (indice_valor_removido == 0)
                {
                    no_atual->info1 = no_atual->info2;
                    no_atual->info2 = no_atual->info3;
                    no_atual->info3 = no_atual->info4;
                    no_atual->info4 = 0;
                }
                else if (indice_valor_removido == 1)
                {
                    no_atual->info2 = no_atual->info3;
                    no_atual->info3 = no_atual->info4;
                    no_atual->info4 = 0;
                }
                else if (indice_valor_removido == 2)
                {
                    no_atual->info3 = no_atual->info4;
                    no_atual->info4 = 0;
                }
                else if (indice_valor_removido == 3)
                {
                    no_atual->info4 = 0;
                } // Apenas zera a última
                if (no_atual->nInfo > 0)
                    no_atual->nInfo--;

                if (no_atual->nInfo < 1)
                { // 1 é MIN_KEYS_ALLOWED
                    status_final = UNDERFLOW;
                }
            }
            else
            { // Nó interno
                Arv45 *subarvore_para_sucessor = NULL;
                Arv45 **ponteiro_para_raiz_subarvore_sucessor = NULL;

                // Determina a subárvore que contém o sucessor
                if (indice_valor_removido == 0)
                {
                    subarvore_para_sucessor = no_atual->f_cen1;
                    ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen1);
                }
                else if (indice_valor_removido == 1)
                {
                    subarvore_para_sucessor = no_atual->f_cen2;
                    ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen2);
                }
                else if (indice_valor_removido == 2)
                {
                    subarvore_para_sucessor = no_atual->f_cen3;
                    ponteiro_para_raiz_subarvore_sucessor = &(no_atual->f_cen3);
                }
                else if (indice_valor_removido == 3)
                {
                    subarvore_para_sucessor = no_atual->dir;
                    ponteiro_para_raiz_subarvore_sucessor = &(no_atual->dir);
                }
                else
                {
                    pode_prosseguir_logica = 0;
                    status_final = UNDERFLOW; /*Índice inválido para nó interno*/
                }

                Arv45 *sucessor_node = NULL;
                if (pode_prosseguir_logica && subarvore_para_sucessor != NULL)
                {
                    sucessor_node = buscar_menor_elemento_45(subarvore_para_sucessor);
                }
                else if (pode_prosseguir_logica && subarvore_para_sucessor == NULL)
                {
                    // Se a subárvore que deveria conter o sucessor é NULL, algo está errado.
                    // Um nó interno com uma chave deve ter um filho à direita dessa chave se não for a última chave.
                    // Se for a última chave, seu filho direito (dir) é a subárvore.
                    fprintf(stderr, "Erro critico: Subarvore do sucessor eh NULL ao remover %d de no interno!\n", valor);
                    status_final = UNDERFLOW; // Trata como erro grave
                    pode_prosseguir_logica = 0;
                }

                if (pode_prosseguir_logica && (sucessor_node == NULL))
                { // Se subarvore_para_sucessor não era NULL, mas buscar_menor_elemento falhou
                    fprintf(stderr, "Erro critico: Sucessor nao encontrado para %d (subarvore do sucessor pode ser invalida)!\n", valor);
                    status_final = UNDERFLOW; // Trata como erro grave
                    pode_prosseguir_logica = 0;
                }

                if (pode_prosseguir_logica)
                {
                    int valor_sucessor = sucessor_node->info1; // Menor elemento de uma subárvore é sempre info1
                    // Substitui a chave a ser removida pelo sucessor
                    if (indice_valor_removido == 0)
                        no_atual->info1 = valor_sucessor;
                    else if (indice_valor_removido == 1)
                        no_atual->info2 = valor_sucessor;
                    else if (indice_valor_removido == 2)
                        no_atual->info3 = valor_sucessor;
                    else if (indice_valor_removido == 3)
                        no_atual->info4 = valor_sucessor;

                    // Remove recursivamente o sucessor de sua posição original
                    StatusRemocao status_rem_suc = remover_45_recursivo(ponteiro_para_raiz_subarvore_sucessor, valor_sucessor);
                    if (status_rem_suc == UNDERFLOW)
                    {
                        status_final = tratar_underflow_45(ponteiro_para_raiz_subarvore_sucessor, no_atual);
                    }
                    else
                    {
                        status_final = status_rem_suc;
                    }
                }
            }
        }
        else if (pode_prosseguir_logica)
        { // Descer para encontrar o valor
            if (proximo_ponteiro_recursao != NULL)
            { // Verifica se o ponteiro para o ponteiro do filho é válido
                StatusRemocao status_recursao = remover_45_recursivo(proximo_ponteiro_recursao, valor);
                if (status_recursao == UNDERFLOW)
                {
                    status_final = tratar_underflow_45(proximo_ponteiro_recursao, no_atual);
                }
                else
                {
                    status_final = status_recursao;
                }
            } // else: valor não encontrado e sem caminho válido, status_final permanece OK
        }
        // Se pode_prosseguir_logica se tornou falso, status_final já foi definido (provavelmente UNDERFLOW)
    }
    return status_final;
}

//================ REMOCAO (Principal) ==================
int remover_45(Arv45 **raiz_ptr, int valor)
{
    int sucesso = 0;
    if (raiz_ptr == NULL || *raiz_ptr == NULL)
    {
        // sucesso permanece 0
    }
    else
    {
        StatusRemocao status_geral = remover_45_recursivo(raiz_ptr, valor);
        Arv45 *raiz_atual = *raiz_ptr; // Para legibilidade e evitar desreferenciamento múltiplo

        if (raiz_atual != NULL && raiz_atual->nInfo == 0)
        { // Raiz ficou completamente vazia
            Arv45 *raiz_antiga = raiz_atual;
            // A nova raiz é o único filho que sobrou (normalmente o esq após fusões)
            if (raiz_antiga->esq != NULL)
                *raiz_ptr = raiz_antiga->esq;
            else if (raiz_antiga->f_cen1 != NULL)
                *raiz_ptr = raiz_antiga->f_cen1; // Menos comum, mas para cobrir
            else
                *raiz_ptr = NULL; // Árvore ficou vazia
            free(raiz_antiga);
            status_geral = OK; // O underflow da raiz (ficando vazia) é resolvido aqui
        }

        if (status_geral == OK)
        {
            sucesso = 1;
        } // else: falha (UNDERFLOW propagado ou erro crítico), sucesso permanece 0
    }
    return sucesso;
}

//=============== IMPRIMIR ==================
void imprime_45_em_ordem(Arv45 *raiz)
{
    if (raiz != NULL)
    {
        imprime_45_em_ordem(raiz->esq);
        if (raiz->nInfo >= 1)
            printf("%d ", raiz->info1);
        imprime_45_em_ordem(raiz->f_cen1);
        if (raiz->nInfo >= 2)
            printf("%d ", raiz->info2);
        imprime_45_em_ordem(raiz->f_cen2);
        if (raiz->nInfo >= 3)
            printf("%d ", raiz->info3);
        imprime_45_em_ordem(raiz->f_cen3);
        if (raiz->nInfo == 4)
            printf("%d ", raiz->info4);
        imprime_45_em_ordem(raiz->dir);
    }
}

void imprime_arvore_visual_45(Arv45 *raiz, char *prefixo, int eh_ultimo, int eh_raiz)
{
    if (raiz != NULL)
    {
        printf("%s", prefixo);
        if (!eh_raiz)
        {
            printf("%s", eh_ultimo ? "`------ " : "+------ ");
        }
        else
        {
            printf(" Raiz--> ");
        }

        printf("[");
        if (raiz->nInfo >= 1)
            printf("%d", raiz->info1);
        if (raiz->nInfo >= 2)
            printf(", %d", raiz->info2);
        if (raiz->nInfo >= 3)
            printf(", %d", raiz->info3);
        if (raiz->nInfo == 4)
            printf(", %d", raiz->info4);
        printf("]\n");

        char novo_prefixo[1024];
        sprintf(novo_prefixo, "%s%s", prefixo, eh_raiz ? "         " : (eh_ultimo ? "         " : "|        "));

        int num_filhos_reais = 0;
        if (raiz->esq)
            num_filhos_reais++;
        if (raiz->f_cen1)
            num_filhos_reais++;
        if (raiz->f_cen2)
            num_filhos_reais++;
        if (raiz->f_cen3)
            num_filhos_reais++;
        if (raiz->dir)
            num_filhos_reais++;

        int filhos_impressos = 0;
        if (raiz->esq != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->esq, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
        if (raiz->f_cen1 != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->f_cen1, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
        if (raiz->f_cen2 != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->f_cen2, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
        if (raiz->f_cen3 != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->f_cen3, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
        if (raiz->dir != NULL)
        {
            filhos_impressos++;
            imprime_arvore_visual_45(raiz->dir, novo_prefixo, filhos_impressos == num_filhos_reais, 0);
        }
    }
}

//================ LIBERAR MEMORIA ==================
void libera_arvore_45(Arv45 **raiz)
{
    if (raiz != NULL && *raiz != NULL)
    {
        libera_arvore_45(&((*raiz)->esq));
        libera_arvore_45(&((*raiz)->f_cen1));
        libera_arvore_45(&((*raiz)->f_cen2));
        libera_arvore_45(&((*raiz)->f_cen3));
        libera_arvore_45(&((*raiz)->dir));
        free(*raiz);
        *raiz = NULL;
    }
}

