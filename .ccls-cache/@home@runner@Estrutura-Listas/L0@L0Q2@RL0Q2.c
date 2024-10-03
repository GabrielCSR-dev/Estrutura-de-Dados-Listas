#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_CHAR 2000
#define MAX 1000
#define NOMES 0
#define INTS 1
#define REAIS 2
#define PONTOS 3


typedef struct{
double x;
double y;
double dist;
}Coordenada;

typedef struct{
char (*nomes)[MAX_CHAR];
//obs.: os parênteses clarificam que está sendo declarado um ponteiro para um array de arrays que possuem MAX_CHAR caracteres. Sem os parênteses, seria só um ponteiro pra um array que possui MAX_CHAR caracteres
long int inteiros[MAX];
double reais[MAX];
Coordenada pontos[MAX];
} Categoria;

typedef struct{
int Nnomes;
int Nints;
int Nreais;
int Npontos;
}Quantidades;

Categoria organizanome();
Categoria organizaponto();
Categoria organizareal();
Categoria organizaint();
void Imprimir();

void Imprimir(Categoria elementos, int qtd[], FILE *output){
  int i;
  fprintf(output, "str:");
  if(qtd[NOMES] > 0){
    for(i = 0; i < qtd[NOMES]; i++)
      fprintf(output, "%s ", elementos.nomes[i]);
  } else fprintf(output, " "); //caso não haja nomes ainda deve existir um espaço entre "str:" e "int:"

  fprintf(output, "int:");
  if(qtd[INTS] > 0){
    for(i = 0; i < qtd[INTS]; i++)
      fprintf(output, "%ld ", elementos.inteiros[i]);
  } else fprintf(output, " ");

  fprintf(output, "float:");
  if(qtd[REAIS] > 0){
    for(i = 0; i < qtd[REAIS]; i++)
      fprintf(output, "%.3g ", elementos.reais[i]); 
      //Obs.: "%g" remove os 0s a direita das casas decimais e não inclui o ponto se for um número redondo, ".3" pra imprimir 2 casas decimais
  } else fprintf(output, " ");
    
  fprintf(output, "p:");
  if(qtd[PONTOS] > 0){
    for(i = 0; i < qtd[PONTOS]-1; i++)
      fprintf(output, "(%.3g,%.3g) ", elementos.pontos[i].x, elementos.pontos[i].y); 
    fprintf(output, "(%.3g,%.3g)", elementos.pontos[i].x, elementos.pontos[i].y); //p n imprimir o espaço no fim da linha
  }
  
  fprintf(output, "\n");
}

Categoria organizanome(char token[], Categoria obj, int qtd){
  char aux[MAX];
  strcpy(obj.nomes[qtd-1], token);

  for(int i = 0; i < qtd; i++){  //insertion sort
    for(int j = i + 1; j < qtd; j++){
      if(strcmp(obj.nomes[i], obj.nomes[j]) > 0){
        strcpy(aux, obj.nomes[i]);
        strcpy(obj.nomes[i], obj.nomes[j]);
        strcpy(obj.nomes[j], aux);
      }
    }
  }

  return obj;
}

Categoria organizaponto(char token[], Categoria obj, int qtd){
  int i, j;
  double result;
  Coordenada aux;
  char numberstr[MAX];

  for(i = 1, j = 0; token[i] != ','; i++, j++){    //copia a string da coordenada x para numberstr 
    //obs.: i = 1, pois token[0] = '(', que é um caractere não reconhecido por atof
    numberstr[j] = token[i];        
  }
  numberstr[j] = '\0'; //marca o final da string
  obj.pontos[qtd].x = atof(numberstr); //transforma o x de string para double

  for(i += 1, j = 0; token[i] != ')'; i++, j++){    //copia a string da coordenada y para numberstr
    numberstr[j] = token[i];        
  }
  numberstr[j] = '\0'; //marca o final da string
  obj.pontos[qtd].y = atof(numberstr); //transforma o y de string para double

  result = obj.pontos[qtd].x*obj.pontos[qtd].x + obj.pontos[qtd].y*obj.pontos[qtd].y;
  obj.pontos[qtd].dist = sqrt(result);
  //Calcula a distancia Euc do ponto (x,y) com o ponto (0,0)

  for(i = 0; i < qtd+1; i++){  //insertion sort
    for(j = i + 1; j < qtd+1; j++){
      if(obj.pontos[i].dist > obj.pontos[j].dist){
        aux = obj.pontos[i];
        obj.pontos[i] = obj.pontos[j];
        obj.pontos[j] = aux;
      }
    }
  }

  return obj;
}

Categoria organizareal(char token[], Categoria obj, int qtd){
  double aux;
  obj.reais[qtd-1] = atof(token); //atof converte token de string pra double

  for(int i = 0; i < qtd; i++){  //insertion sort
    for(int j = i + 1; j < qtd; j++){
      if(obj.reais[i] > obj.reais[j]){
        aux = obj.reais[i];
        obj.reais[i] = obj.reais[j];
        obj.reais[j] = aux;
      }
    }
  }

  return obj;
}

Categoria organizaint(char token[], Categoria obj, int qtd){
  long int aux;
  obj.inteiros[qtd-1] = atol(token); //atol converte token de string pra long int

  for(int i = 0; i < qtd; i++){  //insertion sort
    for(int j = i + 1; j < qtd; j++){
      if(obj.inteiros[i] > obj.inteiros[j]){
        aux = obj.inteiros[i];
        obj.inteiros[i] = obj.inteiros[j];
        obj.inteiros[j] = aux;
      }
    }
  }

  return obj;
}

void tipotoken(char linha[], FILE *output){
  char n[] = "0123456789";
  char *token;
  Categoria elemento = { .inteiros = {0}, .reais = {0}, .pontos = {0}};
  int qtd[4] = {0,0,0,0};
  elemento.nomes = malloc(sizeof(char[MAX_CHAR][MAX_CHAR])); 
  //aloca memória pra matriz de 2000x2000 char
  
  token = strtok(linha, " "); 
  //"corta" a linha[] no primeiro endereço em que houver o/os caracteres informados, " ", no caso, substituindo-o por "\0"
  //OBS.: strtok NÃO é threadsafe

  while(token != NULL){ //token retornará NULL quando não houver mais ' ' na linha[]
    if(strpbrk(token, n) != NULL){        //função para verificar se tem algum dos caracteres do vetor n[] dentro de token, números no caso
      if(strchr(token, ',') != NULL){    //procura o caractere ',', se houver, é um ponto com base na formatação
        elemento = organizaponto(token, elemento, qtd[PONTOS]);
        qtd[PONTOS]++;
      }else if(strchr(token, '.') != NULL){  //procura o caractere '.', se houver, é um float com base na formatação
        qtd[REAIS]++;
        elemento = organizareal(token, elemento, qtd[REAIS]);
      }else{  
        qtd[INTS]++;
        elemento = organizaint(token, elemento, qtd[INTS]);  //se token tiver números e não tiver ',' ou '.', é um inteiro com base na formatação
      }
    } else{
      qtd[NOMES]++;
      elemento = organizanome(token, elemento, qtd[NOMES]); //se token não tiver números, é um nome com base na formatação
    }

    token = strtok(NULL, " "); //pega o próx token da linha[]
  }

  Imprimir(elemento, qtd, output);
  free(elemento.nomes); //libera a memória alocada pela matriz
}

int main(){
  FILE *input, *output;
  char linha[MAX];

  input = fopen("L0Q2.in", "r");
  if(!input){
    perror("Erro ao brir o arquivo de entrada.\n");
  } else{
    output = fopen("L0Q2.out", "w");
    if(!output){
      perror("Erro ao brir o arquivo de saída.\n");
    } else{
      while(!feof(input)){
        if(fgets(linha, MAX, input) != NULL){
          *strchr(linha, '\n') = '\0'; //remove o '\n' da linha
          tipotoken(linha, output);
        }
      }
    }
  }

  fclose(input);
  fclose(output);

  return 0;
}