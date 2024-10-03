#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 100

typedef struct fnode{
    float key;
    struct fnode * next;
}fnode;

typedef struct fLista{
    fnode * head;
    fnode * tail;
}fLista;

typedef struct inode{
    int key;
    struct inode * prev;
    struct inode * next;
    fLista * sat;
}inode;

typedef struct iLista{
    inode * head;
    inode * tail;
}iLista;

iLista * create_list(){
  iLista * l = malloc(sizeof(iLista));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

fLista * create_Flist(){
  fLista * l = malloc(sizeof(fLista));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

inode * create_node(int key){
    inode * node = malloc(sizeof(inode));
    node->sat = create_Flist();
    node->key = key;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

fnode * create_Fnode(double key){
  fnode * node = malloc(sizeof(fnode));
  // printf("\nno1 %f\n", key);
  node->key = key;
  node->next = NULL;
  // printf("\nno %f\n", node->key);

  return node;
}

void bota_na_lista(inode * Node, iLista * l){
    inode * i;

    if(l->head == NULL){
        l->head = Node;
        l->tail = Node;
    } else{
        for(i = l->head; i != NULL && Node->key > i->key; i = i->next);
        if(i == l->head){
            Node->next = l->head;
            l->head->prev = Node;
            l->head = Node;
        } else if(i == NULL){
            l->tail->next = Node;
            Node->prev = l->tail;
            l->tail = Node;
        }else{
            i->prev->next = Node;
            Node->prev = i->prev;
            i->prev = Node;
            Node->next = i;
        }
    }
    return;
}



void bota_na_Flista(fLista * l, fnode * Node){
    fnode * i, * prev;
    int ciclo = 0;

    if(l->head == NULL){
      l->head = Node;
      l->tail = Node;
    } else{
      for(i = l->head; ciclo < 1 && Node->key > i->key; i = i->next){
        prev = i;
        if(i == l->tail) ciclo++;  
      }
      if(ciclo > 0){
        l->tail->next = Node;
        l->tail = Node;
      }else if(i == l->head){
        Node->next = l->head;
        l->head = Node;
      } else{
        prev->next = Node;
        Node->next = i;
      }
      // l->tail->next = Node; //Inserir no final
      // l->tail = Node;
    }
    l->tail->next = l->head;
    return;
}

inode * verifica_real(double key, iLista * l){
    double dif;
    inode * i;
  
    for(i = l->head; i != NULL; i = i->next){
      dif = i->key - key;
      if(dif < 0)
        dif = -dif;
      if(dif < 1.0)
        break;
    }

    return i;
}

void processalinha(char * linha, iLista * l){
    int tipo = 0;
    int numero;
    double numerof;
    int icont = 0, fcont = 0;
    int * inteirosNew;
    double * reaisNew;
    inode * node, * pai;
    fnode * filho;

    // char * copia = malloc(sizeof(linha));

    // strcpy(copia, linha);

    char * token = strtok(linha, " ");

    while(token != NULL){
        if(strcmp(token, "LE") == 0){
            tipo = 1;
        } else if(strcmp(token, "LI") == 0){
            tipo = 2;
        } 
        if(tipo == 1){
            numero = atoi(token);
            if(numero != 0 || strcmp(token, "0") == 0){
                node = create_node(numero);
                // printf("\nnumero = %d %p\n", numero, node);
                bota_na_lista(node, l);
            }
        } else if(tipo == 2){
            numerof = atof(token);
            if(numerof != 0 || strcmp(token, "0") == 0){
                pai = verifica_real(numerof, l);
                if(pai != NULL){
                  filho = create_Fnode(numerof);
                  // printf("\nnumero = %.3g %p\n", numerof, filho);
                  bota_na_Flista(pai->sat, filho);
                }
            }
        }
        token = strtok(NULL, " ");
    }
  // printf("\nbatata\n");
    // free(copia);
    return;
}

void imprime_lista(iLista * l, FILE * output){
    inode * i; 
    fnode * j;
    fprintf(output, "[");
    if(l->head != NULL){
        for(i = l->head; i != l->tail; i = i->next){
            fprintf(output, "%d(", i->key);
            if(i->sat->head != NULL){
                for(j = i->sat->head; j != i->sat->tail; j = j->next){
                    fprintf(output, "%.3g->", j->key);
                }
                fprintf(output, "%.3g", j->key);
            }
            fprintf(output, ")->");
        }
        fprintf(output, "%d(", i->key);
        if(i->sat->head != NULL){
            for(j = i->sat->head; j != i->sat->tail; j = j->next){
                fprintf(output, "%.3g->", j->key);
            }
            fprintf(output, "%.3g", j->key);
        }
        fprintf(output, ")");
    }   
    fprintf(output, "]\n");
    
    return;
}

void apaga_sublista(fLista * l){
    fnode * i, * j;
    i = l->head;
    l->tail->next = NULL;
    while(i != l->tail){
        l->head = i->next;
        j = i;
        i = i->next;
        free(j);
    }
    l->tail = NULL;
    l->head = NULL;
    free(i);
    return;
}

void apaga_lista(iLista * l){
    inode * i, * j;
    i = l->head;
    while(i != l->tail){
        i->next->prev = NULL;
        l->head = i->next;
        if(i->sat->head != NULL)
            apaga_sublista(i->sat);
        j = i;
        i = i->next;
        free(j);
    }
    l->tail = NULL;
    l->head = NULL;
    free(i);
    return;
}

int main(){
    FILE * input, * output;
    char * linha = malloc(MAX_CHAR * sizeof(char));
    // int * inteiros = malloc(NI*sizeof(int));
    // double * reais = malloc(NF*sizeof(double));
    iLista * listadupla = create_list();
    int loop = 0;
    inode * i;
    fnode * j;

    // initlista(listadupla);

    input = fopen("L1Q3.in", "r");

    if(input){
        output = fopen("L1Q3.out", "w");
        if(output){
            while(!feof(input)){
                if(fgets(linha, MAX_CHAR, input) != NULL){
                    processalinha(linha, listadupla); 
                    imprime_lista(listadupla, output);
                    if(listadupla->head != NULL)  
                        apaga_lista(listadupla);
                }
            }
        } else{
            perror("Não foi possível abrir o arquivo de saída.\n");
        }
        
    }else{
        perror("Não foi possível abrir o arquivo de entrada.\n");
    }

    free(linha);
    fclose(output);
    fclose(input);

    return 0;
}