#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct do produto
typedef struct
{
  int codigo;   //Código do produto
  char *nome;   //Nome do produto
  double preco; //Preço do produto
  int quant;    //Quantidade do produto em estoque
} produto;

/* ---------------------------------------------------------- */
/* Funções auxiliares                                          */
/* ---------------------------------------------------------- */

/*
Imprime uma linha de exatamente 50 hífens, exigida no fim de toda saída.
*/
void imprimirLinha(void)
{
  for (int i = 0; i < 50; i++) printf("-");
  printf("\n");
}

/*
Preenche os dados de um produto já existente no vetor de estoque.
Faz a alocação dinâmica do nome do produto.
Parâmetros:
@*prod  = posição do vetor de estoque que vai receber o produto.
@cod    = código de identificação do produto.
@*nome  = string com o nome do produto.
@quant  = quantidade do produto que está sendo inserida.
@preco  = preço do produto.
Retorna 0 em sucesso e um código negativo em caso de erro.
*/
int insert(produto *prod, int cod, char *nome, int quant, double preco)
{
  if (prod == NULL || nome == NULL)
  {
    //Caso de erro: vetor nulo ou nome nulo.
    return -1;
  }
  if (quant < 0 || preco < 0)
  {
    //Caso de erro: valores numéricos inválidos.
    return -3;
  }

  int tamanho = strlen(nome) + 1; //Tamanho do nome do produto.

  prod->nome = malloc(tamanho * sizeof(char));
  if (prod->nome == NULL)
  {
    //Caso de erro: não foi possível alocar o nome do produto.
    return -4;
  }

  strcpy(prod->nome, nome);
  prod->codigo = cod;
  prod->preco = preco;
  prod->quant = quant;

  return 0;
}

/*
Lê um "token" (sequência de caracteres sem espaço) do teclado,
alocando e realocando memória dinamicamente conforme necessário.
Não existe limite de tamanho para o nome lido.
Retorna um ponteiro para a string alocada (deve ser liberada depois).
*/
char *lerNomeDinamico(void)
{
  int capacidade = 8;
  int tam = 0;
  char *nome = malloc(capacidade * sizeof(char));
  int c;

  //Ignora espaços/quebras de linha antes do início do nome.
  c = getchar();
  while (c == ' ' || c == '\n' || c == '\t') c = getchar();

  while (c != EOF && c != ' ' && c != '\n' && c != '\t')
  {
    if (tam >= capacidade - 1)
    {
      capacidade *= 2;
      nome = realloc(nome, capacidade * sizeof(char));
    }
    nome[tam++] = (char)c;
    c = getchar();
  }
  nome[tam] = '\0';

  return nome;
}

/*
Mesma ideia de lerNomeDinamico, mas lendo de um arquivo (usada para
recarregar o backup do dia anterior). Também sem tamanho fixo.
*/
char *lerNomeDinamicoArquivo(FILE *arq)
{
  int capacidade = 8;
  int tam = 0;
  char *nome = malloc(capacidade * sizeof(char));
  int c;

  c = fgetc(arq);
  while (c == ' ' || c == '\n' || c == '\t') c = fgetc(arq);

  while (c != EOF && c != ' ' && c != '\n' && c != '\t')
  {
    if (tam >= capacidade - 1)
    {
      capacidade *= 2;
      nome = realloc(nome, capacidade * sizeof(char));
    }
    nome[tam++] = (char)c;
    c = fgetc(arq);
  }
  nome[tam] = '\0';

  return nome;
}

/*
Garante que o vetor de estoque tem espaço para mais um produto.
Caso o vetor esteja cheio, dobra a capacidade (realloc).
Parâmetros:
@**estoque    = endereço do ponteiro do vetor de estoque.
@numProdutos  = quantidade de produtos atualmente cadastrados.
@*capacidade  = endereço da capacidade atual do vetor (será atualizada).
*/
void garantirCapacidade(produto **estoque, int numProdutos, int *capacidade)
{
  if (numProdutos >= *capacidade)
  {
    *capacidade = (*capacidade == 0) ? 1 : (*capacidade * 2);
    *estoque = realloc(*estoque, (*capacidade) * sizeof(produto));
  }
}

/* ---------------------------------------------------------- */
/* Funcionalidades do sistema                                  */
/* ---------------------------------------------------------- */

/*
1. Inserir produto no estoque (comando IP).
Não gera custo para o caixa.
Parâmetros:
@**estoque     = endereço do ponteiro do vetor de estoque.
@*numProdutos  = endereço da quantidade de produtos cadastrados (será incrementada).
@*capacidade   = endereço da capacidade atual do vetor.
@*nome, quant, preco = dados do novo produto.
*/
void inserirProduto(produto **estoque, int *numProdutos, int *capacidade,
                     char *nome, int quant, double preco)
{
  garantirCapacidade(estoque, *numProdutos, capacidade);
  insert(&(*estoque)[*numProdutos], *numProdutos, nome, quant, preco);
  (*numProdutos)++;
}

/*
2. Aumentar o estoque de um produto (comando AE).
Gera custo para o caixa: quantidade * preço atual do produto.
Parâmetros:
@estoque[]   = vetor de produtos.
@codigo      = código do produto a ser reabastecido.
@quantidade  = quantidade a ser adicionada ao estoque.
@*saldo      = endereço do saldo do caixa (será decrementado).
*/
void aumentarEstoque(produto estoque[], int codigo, int quantidade, double *saldo)
{
  *saldo -= estoque[codigo].preco * quantidade;
  estoque[codigo].quant += quantidade;
}

/*
3. Modificar o preço de um produto (comando MP).
Parâmetros:
@estoque[]   = vetor de produtos.
@codigo      = código do produto.
@novoPreco   = novo preço a ser atribuído.
*/
void modificarPreco(produto estoque[], int codigo, double novoPreco)
{
  estoque[codigo].preco = novoPreco;
}

/*
4. Realizar uma venda (comando VE).
Lê códigos de produtos até encontrar -1, decrementa 1 unidade do
estoque de cada produto vendido, imprime nome e preço de cada item
e o total da venda; soma o total ao saldo do caixa.
Parâmetros:
@estoque[]  = vetor de produtos.
@*saldo     = endereço do saldo do caixa (será incrementado).
*/
void realizarVenda(produto estoque[], double *saldo)
{
  int codigo;
  double total = 0.0;

  scanf(" %d", &codigo);
  while (codigo != -1)
  {
    printf("%s %.2lf\n", estoque[codigo].nome, estoque[codigo].preco);
    total += estoque[codigo].preco;
    estoque[codigo].quant--;

    scanf(" %d", &codigo);
  }

  printf("Total: %.2lf\n", total);
  imprimirLinha();

  *saldo += total;
}

/*
5. Consultar o estoque dos produtos (comando CE).
Lista código, nome e quantidade de todos os produtos.
*/
void consultarEstoque(produto estoque[], int numProdutos)
{
  for (int i = 0; i < numProdutos; i++)
  {
    printf("%d %s %d\n", estoque[i].codigo, estoque[i].nome, estoque[i].quant);
  }
  imprimirLinha();
}

/*
6. Consultar o saldo do caixa (comando CS).
*/
void consultarSaldo(double saldo)
{
  printf("Saldo: %.2lf\n", saldo);
  imprimirLinha();
}

/*
7. Finalizar o dia (comando FE).
Grava em "backup.txt" todas as informações necessárias para o
programa ser carregado no dia seguinte: quantidade de produtos,
saldo do caixa e os dados de cada produto.
*/
void finalizarDia(produto estoque[], int numProdutos, double saldo)
{
  FILE *arq = fopen("backup.txt", "w");
  if (arq == NULL)
  {
    //Caso de erro: não foi possível criar o arquivo de backup.
    return;
  }

  fprintf(arq, "%d\n", numProdutos);
  fprintf(arq, "%lf\n", saldo);

  for (int i = 0; i < numProdutos; i++)
  {
    fprintf(arq, "%d\n", estoque[i].codigo);
    fprintf(arq, "%s\n", estoque[i].nome);
    fprintf(arq, "%lf\n", estoque[i].preco);
    fprintf(arq, "%d\n", estoque[i].quant);
  }

  fclose(arq);
}


void liberarEstoque(produto estoque[], int numProdutos)
{
  for (int i = 0; i < numProdutos; i++)
  {
    free(estoque[i].nome);
  }
  free(estoque);
}



int main()
{
  produto *estoque = NULL;
  double saldo;
  int numProdutos = 0;
  int capacidade = 0;
  char comando[3];

  //Leitura e verificação do arquivo de backup do dia anterior.
  FILE *arqBackup = fopen("backup.txt", "r");

  if (arqBackup == NULL)
  {
    //Não existe backup: pergunta o espaço inicial do estoque.
    int capacidadeInicial;
    scanf(" %d", &capacidadeInicial);

    saldo = 100.00;
    capacidade = capacidadeInicial;
    estoque = malloc(capacidade * sizeof(produto));
  }
  else
  {
    //Existe backup: carrega os dados do dia anterior.
    fscanf(arqBackup, "%d", &numProdutos);
    fscanf(arqBackup, "%lf", &saldo);

    capacidade = numProdutos;
    estoque = malloc(capacidade * sizeof(produto));

    for (int i = 0; i < numProdutos; i++)
    {
      fscanf(arqBackup, "%d", &estoque[i].codigo);
      estoque[i].nome = lerNomeDinamicoArquivo(arqBackup); //já vem alocado dinamicamente
      fscanf(arqBackup, "%lf", &estoque[i].preco);
      fscanf(arqBackup, "%d", &estoque[i].quant);
    }
    fclose(arqBackup);
  }

  //Leitura e execução dos comandos até "FE".
  while (1)
  {
    scanf(" %s", comando);

    if (strcmp(comando, "IP") == 0)
    {
      int quantTemp;
      double precoTemp;

      char *nomeTemp = lerNomeDinamico();
      scanf(" %d %lf", &quantTemp, &precoTemp);
      inserirProduto(&estoque, &numProdutos, &capacidade, nomeTemp, quantTemp, precoTemp);
      free(nomeTemp); //insert() já copiou o nome para dentro do produto, então este temporário pode ser liberado
    }
    else if (strcmp(comando, "AE") == 0)
    {
      int codigo, quantidade;
      scanf(" %d %d", &codigo, &quantidade);
      aumentarEstoque(estoque, codigo, quantidade, &saldo);
    }
    else if (strcmp(comando, "MP") == 0)
    {
      int codigo;
      double novoPreco;
      scanf(" %d %lf", &codigo, &novoPreco);
      modificarPreco(estoque, codigo, novoPreco);
    }
    else if (strcmp(comando, "VE") == 0)
    {
      realizarVenda(estoque, &saldo);
    }
    else if (strcmp(comando, "CE") == 0)
    {
      consultarEstoque(estoque, numProdutos);
    }
    else if (strcmp(comando, "CS") == 0)
    {
      consultarSaldo(saldo);
    }
    else if (strcmp(comando, "FE") == 0)
    {
      finalizarDia(estoque, numProdutos, saldo);
      break;
    }
  }

  liberarEstoque(estoque, numProdutos);

  return 0;
}
