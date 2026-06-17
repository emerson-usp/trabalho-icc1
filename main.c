#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structs
typedef struct
{
    char *nome; //Nome do produto
    double preco; // Preço do produto
    int quant; //Quantidade do produto
} produto;


//param:
/*
@*prod = array de produtos do estoque
@*nome = string com nome do produto
@quant = quantidade de produtos que deve ser inserida no estoque
@preco = preço do produto que está sendo inserido
*/
int insert(produto *prod, char *nome, int quant, double preco)
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
    
    if(prod->nome == NULL)
    {
        //caso de erro: não é possível alocar o nome do produto na memória
        return -4;
    }

    strcpy(prod->nome, nome);

    prod->preco = preco;
    prod->quant = quant;
    return 0;
}

//param:
/*
@*arquivo = recebe o arquivo criado para o backup
@*prod = recebe os dados da struct produto
*/
int salvarBackup(FILE *arquivo, produto *prod)
{
    fprintf(arquivo, "%s\n", prod->nome);
    fprintf(arquivo, "%lf\n", prod->preco);
    fprintf(arquivo, "%d\n", prod->quant);
    return 0;
}

//param:
/*
@ *prod = recebe os dados do struct produto
@ quantidade = quantidade de registros que vão ser inseridos no arquivo de texto
*/
int geraBackup(produto *prod, int quantidade)
{
    FILE *estoque = fopen("estoque.txt", "w");

    if(estoque == NULL)
    {
        //caso de erro: arquivo não gerado ou corrompido
        return 1;
    }
    else
    {
        for(int i = 0; i<quantidade; i++)
        {
            salvarBackup(estoque, &prod[i]);
        }
        fclose(estoque);
        return 0;
    }
}

//funções debug

void verArquivo(FILE *arq)
{
    char linha[100];
    while(fgets(linha, sizeof(linha), arq) != NULL)
    {
        printf("%s", linha);
    }
    fclose(arq);
    printf("-------------------------------------------------");
}

void print(produto *prod)
{
    printf("%s\n", prod->nome);
    printf("%lf\n", prod->preco);
    printf("%d\n", prod->quant);
}

int main()
{
    produto *estoque = (produto *) malloc(2 * sizeof(produto));

    char *name = malloc(99*sizeof(char));
    double preco = 4.0;
    int quant = 2;

    strcpy(name, "teste");

    insert(&estoque[0], name, quant, preco);

    quant = 1;

    insert(&estoque[1], name, quant, preco);
    //print(&estoque[0]);

    if(geraBackup(estoque, 2)) return 1;

    FILE *arq_leitura = fopen("estoque.txt", "r");

    verArquivo(arq_leitura);

    free(name);
    
    for(int i = 0; i<2; i++)
    {
        free(estoque[i].nome);
    }

    free(estoque);

    return 0;
}
