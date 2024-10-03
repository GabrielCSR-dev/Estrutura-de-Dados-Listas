#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 1000

typedef struct Node{
  struct Node * p;
  struct Node * right;
  struct Node * left;
  int key; 
  int altura;
}Node;

typedef struct Tree{
  Node * root;
}Tree;

Node * createNode(int key){
  Node * n = malloc(sizeof(Node));
  n->key = key;
  n->altura = -1;
  n->p = NULL;
  n->right = NULL;
  n->left = NULL;
  return n;
}

Tree * createTree(){
  Tree * t = malloc(sizeof(Tree));
  t->root = NULL;
  return t;
}

void insertTree(Node * n, Tree * t){
  Node * i, * anterior;

  if(t->root == NULL){
    t->root = n;
    n->altura += 1;
  } else{
    i = t->root;
    while(i != NULL){
      anterior = i;
      if(n->key < i->key) i = i->left;
      else i = i->right;
      n->altura += 1;
    }
    n->altura += 1;
    if(n->key < anterior->key) anterior->left = n;
    else anterior->right = n;
    n->p = anterior;
  }
}

Node * buscaNode(Tree * t, int key){
  Node * i;
  i = t->root;
  while(i != NULL && i->key != key){
    if(key < i->key) i = i->left;
    else i = i->right;
  }
  return i;
}

void DiminuiAlturaSubArvore(Node * n){
    if(n != NULL){
        n->altura -= 1;
        DiminuiAlturaSubArvore(n->left);
        DiminuiAlturaSubArvore(n->right);
    }
    return;
}

Node * menor_sucessor(Node * n){
  Node * candidato_sucessor;
  Node * anterior;
  Node * pai;
  candidato_sucessor = n->right;
  if(candidato_sucessor != NULL){
      if(candidato_sucessor->left == NULL){
          return candidato_sucessor;
      } else{
          while(candidato_sucessor != NULL){
              anterior = candidato_sucessor;
              candidato_sucessor = candidato_sucessor->left;
          }
          return anterior;
      }
  } else{
      pai = n->p;
      while(pai != NULL && pai->right == n){
          n = pai;
          pai = pai->p;
      }
      return pai;
  }
}

void remove_node(Tree * t, Node * n){
  if(n->right == NULL && n->left == NULL){
      if(n == t->root) t->root = NULL;
      else if(n->p->right == n) n->p->right = NULL;
      else n->p->left = NULL;
      free(n);
  }else if((n->right == NULL) ^ (n->left == NULL)){
      Node * connect;
      if(n->right != NULL) connect = n->right;
      else connect = n->left;

      if(n == t->root) t->root = connect;
      else if(n->p->right == n) n->p->right = connect;
      else n->p->left = connect;
      connect->p = n->p; 
      DiminuiAlturaSubArvore(connect);
      free(n);
  } else{
      Node * sucessor = menor_sucessor(n);
      n->key = sucessor->key; //Copiar a chave e --todos os satélites-- do sucessor para o nó
      remove_node(t, sucessor);
  }
}

void ProcessaLinha(char linha[], Tree * tree){
  char * token = strtok(linha, " ");
  int key;
  int flagComando = 0;
  Node * no;

  while(token != NULL){
    if(flagComando == 1){
      key = atoi(token);
      if(key != 0 || strcmp(token,"0") == 0){
        no = createNode(key);
        insertTree(no, tree);
      }
    }else if(flagComando == 2){
      key = atoi(token);
      if(key != 0 || strcmp(token,"0") == 0){
        no = buscaNode(tree, key);
        if(no == NULL){
            no = createNode(key);
            insertTree(no, tree); 
        }else{
            remove_node(tree, no);  
        }
      }
    }

    if(strcmp(token,"a") == 0){
      flagComando = 1;
    }else if(strcmp(token,"r") == 0){
      flagComando = 2;
    }
    token = strtok(NULL, " ");
  }

}

void ImprimeEmOrdem(Tree * t, FILE * out){
  Node * no, * menor;
  for(menor = t->root; menor != NULL; menor = menor->left)
    no = menor;
  while(menor_sucessor(no) != NULL){
    fprintf(out, "%d (%d) ", no->key, no->altura);
    no = menor_sucessor(no);
  }
  fprintf(out, "%d (%d)\n", no->key, no->altura);
  return;
}

void QueimaArvore(Tree * t, Node * n){
    if(n != NULL){
        if(n == t->root) t->root = NULL;
        QueimaArvore(t, n->left);
        QueimaArvore(t, n->right);
        free(n);
    }
    return;
}

int main(void){
  FILE * input, * output;
  char linha[TAM];
  Tree * tree = createTree();
  input = fopen("L2Q3.in", "r");

  if(input){
    output = fopen("L2Q3.out", "w");
    if(output){
        while(!feof(input)){
            if(fgets(linha, TAM, input) != NULL){
                *strchr(linha, '\n') = '\0';
                ProcessaLinha(linha, tree);
                ImprimeEmOrdem(tree, output);
                QueimaArvore(tree, tree->root);
            }
        }
    }else{
        perror("Erro ao abrir o arquivo de saída.\n");
    }
  }else{
    perror("Erro ao abrir o arquivo de entrada.\n");
  }

  fclose(output);
  fclose(input);
  return 0;
}