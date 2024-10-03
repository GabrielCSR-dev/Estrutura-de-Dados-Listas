#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 200

typedef struct subNode{
  struct subNode * next;
  struct subNode * prev;
  int key;
}subNode;

typedef struct subLista{
  subNode * head;
  subNode * tail;
}subLista;

typedef struct Node{
  struct Node * next;
  struct Node * prev;
  subLista * sub;
  int key;
  int soma;
}Node;

typedef struct Lista{
  Node * head;
  Node * tail;
}Lista;

Lista * create_list(){
  Lista * l = malloc(sizeof(Lista));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

subLista * create_sublist(){
  subLista * l = malloc(sizeof(Lista));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

Node * create_node(int key){
    Node * node = malloc(sizeof(Node));
    node->sub = create_sublist();
    node->key = key;
    node->soma = 0;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

subNode * create_subnode(int key){
  subNode * node = malloc(sizeof(subNode));
  node->key = key;
  node->next = NULL;
  return node;
}

void bota_na_lista(Node * node, Lista * l){
    Node * i;

    if(l->head == NULL){
        l->head = node;
        l->tail = node;
    } else{
        for(i = l->head; i != NULL && node->key > i->key; i = i->next);
        if(i == l->head){
            node->next = l->head;
            l->head->prev = node;
            l->head = node;
        } else if(i == NULL){
            l->tail->next = node;
            node->prev = l->tail;
            l->tail = node;
        }else{
            i->prev->next = node;
            node->prev = i->prev;
            i->prev = node;
            node->next = i;
        }
    }
    return;
}

void bota_na_sublista(subNode * Node, subLista * l){
    subNode * i;

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

void processalinha(char * linha, Lista * l){
  char * achou = NULL;
  int startFlag = 0, intFlag = 0;
  int key = 1, numero = 0, soma = 0;
  Node * node = NULL;
  subNode * subnode = NULL;
  char digits[] = "0123456789";
  
  achou = strstr(linha, "start ");
  linha = achou + strlen("start ");
  
  
  
  char * token = strtok(linha, " ");

  printf("%s\n", token);

  while(token != NULL){
    startFlag = 0;
    if(strcmp(token, "start") == 0){
      startFlag = 1;
    } else if(strpbrk(token, digits) != NULL){
      intFlag = 1;
    }

    if(startFlag){
      startFlag = 0;
      if(node != NULL)
        
      node = create_node(key);
      key++;
    } else if(intFlag){
      intFlag = 0;
      numero = atoi(token);
      if(strcmp(token, "0") != 0 && numero != 0){
        subnode = create_subnode(numero);
        bota_na_sublista(subnode, node->sub);
        soma = soma + numero;
      }
    }
      
    token = strtok(NULL, " ");
  }
  
  return;
}

int main(void){
  FILE * input, * output;
  char * linha = malloc(MAX_CHAR * sizeof(char));
  Lista * l = create_list();

  input = fopen("L1Q1.in", "r");
  
  if(input){
    // while(!feof(input))
    if(fgets(linha, MAX_CHAR, input) != NULL){
      processalinha(linha, l);
    }
  }else{
    perror("Não foi possível abrir o arquivo de entrada.\n");
  }

  fclose(input);

  return 0;
}