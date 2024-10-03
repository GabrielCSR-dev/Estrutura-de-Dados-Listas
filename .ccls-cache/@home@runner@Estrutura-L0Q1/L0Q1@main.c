#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_CHAR 1000
#define MAX_POINTS 50

typedef struct{
long int x;
long int y;
double dist;
}Coordenada;

double shortcutcalculo();
double distancecalculo();
void ordenarpontos();
void lerpontos();

// função com fórmula da hipotenusa pra cálculo da distância euclidiana entre o primeiro e último ponto (x,y) na ordem em que aparecem na entrada
double shortcutcalculo(Coordenada pontos[], int fim){
  double shortcut;
  shortcut = sqrt((pontos[fim].x-pontos[0].x)*(pontos[fim].x-pontos[0].x) + (pontos[fim].y-pontos[0].y)*(pontos[fim].y-pontos[0].y)); 
  return shortcut;
}

//função recursiva com fórmula para calcular as distâncias de um ponto ao próximo na ordem que aparecem na entrada
double distancecalculo(Coordenada pontos[], int i, int qtdpontos, double xf, double xo, double yf, double yo, double totaldis){
  if(i < qtdpontos){ //passa por todos os pontos
    totaldis += sqrt((xf-xo)*(xf-xo) + (yf-yo)*(yf-yo));
    i++;
    distancecalculo(pontos, i, qtdpontos, pontos[i].x, xf, pontos[i].y, yf, totaldis);
  } else{
    return totaldis; //retorna a distância total final
  }
}

void ordenarpontos(FILE *output, Coordenada pontos[], int qtdpontos){
  int i, j;
  double distancia;
  Coordenada copia[MAX_POINTS], aux; 

  for(i = 0; i < qtdpontos; i++)
    copia[i] = pontos[i];   //Fazendo uma copia de pontos[] pra ordená-la sem alterar o OG
  
  for(i = 0; i < qtdpontos; i++){  //insertion sort
    for(j = i + 1; j < qtdpontos; j++){
          if(copia[i].dist > copia[j].dist){
              aux = copia[i];
              copia[i] = copia[j];
              copia[j] = aux;
          }
    }
  } 
  fprintf(output,"points ");
  for(i = 0; i < qtdpontos; i++) //imprime os pontos em ordem crescente
    fprintf(output,"(%ld,%ld) ", copia[i].x, copia[i].y);
}

void lerpontos(char * linha, int *qtd, Coordenada pontos[]){
  char numberstr[MAX_CHAR];
  double result;
  int i, j, p = 0;

  for(i = 0; linha[i] != '('; i++);
  //A string da coordenada x começa
  
    while(linha[i] != '\n' && linha[i] != EOF){         
      
      for(i += 1, j = 0; linha[i] != ','; i++, j++){
        numberstr[j] = linha[i];  //Copia a string da coordenada x para numberstr
      }
      numberstr[j] = '\0';  
      pontos[p].x = atol(numberstr);  //Converte a coordenada x de string para double

      for(i += 1, j = 0; linha[i] != ')'; i++, j++){
        numberstr[j] = linha[i];  //Copia a string da coordenada y para numberstr
      }
      numberstr[j] = '\0';  
      pontos[p].y = atol(numberstr); //Converte a coordenada y de string para double

      for(i += 1; linha[i] != '(' && linha[i] != '\n' && linha[i] != EOF; i++);
      //A string da próxima coordenada x começa, se houver

      result = pontos[p].x*pontos[p].x + pontos[p].y*pontos[p].y;
      pontos[p].dist = sqrt(result);
      //Calcula a distancia Euc do ponto (x,y) com o ponto (0,0)
      
      p++;
  }
  *qtd = p;
}

int main(){
  
  FILE *input, *output;
  char linha[MAX_CHAR];
  char *achou;
  Coordenada pontos[MAX_POINTS];
  int qtdpontos; 
  double distancia, atalho;
  int i = 0;


  input = fopen("L0Q1.in", "r");

  if(input != 0){
    output = fopen("L0Q1.out", "w");
    if(output == 0){
      perror("Erro. Não foi possível abrir o arquivo de saída.\n");
    } else{
      while(!feof(input)){
        if(fgets(linha, MAX_CHAR, input) != NULL){ //evita q a última linha seja repetida e stack smashing
        
          achou = strstr(linha, "points"); //marca inicio da linha dos pontos na entrada
  
          if(achou == NULL){
            printf("Marcador \"points\" não foi encontrado.\n"); 
          } else{
            lerpontos(achou, &qtdpontos, pontos); //'&' permite devolver o valor da função sem usar return
            ordenarpontos(output, pontos, qtdpontos);
            distancia = distancecalculo(pontos, 1, qtdpontos, pontos[1].x, pontos[0].x, pontos[1].y, pontos[0].y, 0);
            fprintf(output,"distance %.2f ", distancia); 
  
            atalho = shortcutcalculo(pontos, qtdpontos-1);
            fprintf(output,"shortcut %.2f\n", atalho);
          }
        }
      }
    }
  } else{
    perror("Erro. Não foi possível abrir o arquivo de entrada.\n");
  }

  fclose(input);
  fclose(output);

  return 0;
}