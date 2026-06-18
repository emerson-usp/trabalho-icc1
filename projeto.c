#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structs
typedef struct
{
  int codigo; //Código do produto
  char *nome; //Nome do produto
  double preco; // Preço do produto
  int quant; //Quantidade do produto
} produto;

/*
Função de inserção de um novo produto no estoque.
Parâmetros:
@*prod = Produto a ser inserido no estoque.
@codigo = código de identificação do produto
@*nome = string com nome do produto
@quant = quantidade de produtos que deve ser inserida no estoque
@preco = preço do produto que está sendo inserido
*/
int insert(produto *prod, int cod, char *nome, int quant, double preco)
{
  if(prod == NULL || nome == NULL)
  {
    //Caso de erro: vetor nulo ou nome nulo.
    return -1;
  }
  if(quant < 0 || preco < 0)
  {
    //Caso de erro: valores numéricos inválidos.
    return -3;
  }

  int tamanho = strlen(nome) + 1;// <- Define tamanho do nome do produto.

  //Alocação dinâmica do nome.
  prod->nome = malloc(tamanho * sizeof(char));
    
  if(prod->nome == NULL)
  {
    //Caso de erro: não é possível alocar o nome do produto na memória.
    return -4;
  }

  strcpy(prod->nome, nome);

  prod->codigo = cod;
  prod->preco = preco;
  prod->quant = quant;

  return 0;
}

/*
Função de consulta de estoque.
Parâmetros:
@estoque[] - Vetor de todos os produtos.
@quantProd - Quantidade de todos os produtos. (Tentei achar alguma maneira de buscar pelo tamanho do vetor, mas os vetores criados por alocação
dinâmica tem tamanho somente do ponteiro inicial.)
*/
void consultarEstoque(produto estoque[], int quantProd)
{
  for (int i = 0; i < quantidade; i++)
  {
    printf("%d %s %d\n", estoque[i].codigo, estoque[i].nome, estoque[i].quant);
    
  }
  printf("--------------------------------------------------");
}

/*
Função de consulta de saldo.
Parâmetros:
@saldo - Saldo total atual.
*/
void consultarSaldo(double saldo)
{
  printf("Saldo: %.2lf\n", saldo);
  printf("--------------------------------------------------");
}

int main()
{
  //Declaração de variáveis
  produto *estoque;
  double saldo;
  int tamEstoque;
  char comando[3] = "\0";

  //Variável para verificar se é a primeira execução.
  int existia = 0;

  //Variável de controle do código dos produtos.
  int codigoAtual = 0;

  //Leitura e verificação do arquivo de backup.
  FILE *arqEstoque = fopen("backup.txt", "r");

  if (arqEstoque == NULL)
  {
    //Leitura do tamanho de estoque inicial (Veja o caso de teste do pdf do projeto e o último tópico abaixo das funções necessárias).
    scanf(" %d", &tamEstoque);
    
    saldo = 100.00;
    estoque = (produto *)malloc(tamEstoque * sizeof(produto));
  }
  else
  {
    //Leitura dos dados no arquivo de backup caso ele exista.
    existia = 1;
    fscanf(arqEstoque, "%d", &tamEstoque);
    fscanf(arqEstoque, "%lf", &saldo);
    
    codigoAtual = tamEstoque;
    estoque = (produto *)malloc(tamEstoque * sizeof(produto));

    for (int i = 0; i < tamEstoque; i++)
    {
      //String temporária para armazenar o nome antes de alocá-lo dinâmicamente.
      char nomeTemp[100];

      fscanf(arqEstoque, "%d", &estoque[i].codigo);
      fscanf(arqEstoque, "%s", nomeTemp);
      fscanf(arqEstoque, "%lf", &estoque[i].preco);
      fscanf(arqEstoque, "%d", &estoque[i].quant);
      
      //Aloca o nome dinâmicamente e insere o valor da string temporária.
      estoque[i].nome = malloc((strlen(nomeTemp) + 1) * sizeof(char));
      strcpy(estoque[i].nome, nomeTemp);
    }
    fclose(arqEstoque);
  }

  //Leitura dos comandos.
  while(strcmp(comando, "FE") != 0)
  {
    scanf(" %s", comando);

    if(strcmp(comando, "IP") == 0)
    {
    
      char nomeTemp[100];
      int quantTemp;
      double precoTemp;

      //Verifica se o estoque existia antes da execução e realoca o tamanho do estoque.
      if(existia == 1)
      {
        estoque = realloc(estoque, tamEstoque * sizeof(produto));

        scanf(" %s %lf %d", nomeTemp, &precoTemp, &quantTemp);

        insert(&estoque[codigoAtual], codigoAtual, nomeTemp, quantTemp, precoTemp);

        codigoAtual++;
      }
      else if(existia == 0)
      {
        scanf(" %s %lf %d", nomeTemp, &precoTemp, &quantTemp);

        insert(&estoque[codigo], codigoAtual, nomeTemp[100], precoTemp, quantTemp);

        codigoAtual++;
      }
    }
  }

  return 0;
}
