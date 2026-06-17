#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structs
typedef struct
{
    char *nome; //Nome do produto
    int preco; // Preço do produto
    int quant; //Quantidade do produto
} produto;


//param:
/*
@*prod = array de produtos do estoque
@*nome = string com nome do produto
@quant = quantidade de produtos que deve ser inserida no estoque
@preco = preço do produto que está sendo inserido
*/
int insert(produto *prod, char *nome, int quant, int preco)
{
    if(prod == NULL || nome == NULL)
    {
        //caso de erro: vetor nulo ou nome nulo
        return -1;
    }
    if(quant < 0 || preco < 0)
    {
        //caso de erro: valores numéricos inválidos
        return -3;
    }

    int tamanho = strlen(nome) + 1;// <- define tamanho do nome do produto

    //função que insere coisas no vetor
    prod->nome = malloc(tamanho * sizeof(char));
    strcpy(prod->nome, nome);

    if(prod->nome == NULL)
    {
        return -4;
    }

    prod->preco = preco;
    prod->quant = quant;
    return 0;
}

void print(produto *prod)
{
    printf("%s\n", prod->nome);
    printf("%d\n", prod->preco);
    printf("%d\n", prod->quant);
}

int main()
{
    produto *estoque = (produto *) malloc(1 * sizeof(produto));

    char *name = malloc(99*sizeof(char));
    int preco = 4;
    int quant = 2;

    strcpy(name, "teste");

    insert(estoque, name, quant, preco);
    print(estoque);
}