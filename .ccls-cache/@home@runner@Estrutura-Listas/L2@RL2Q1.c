#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255

typedef struct Node{
  struct Node * p;
  struct Node * left;
  struct Node * right;
  int altura;
  int key;
}Node;

typedef struct Tree{
  Node * root;
}Tree;

Node * create_node(int key){
  Node * no = malloc(sizeof(Node));
  no->key = key;
  no->p = NULL;
  no->right = NULL;
  no->left = NULL;
  no->altura = -1;
  return no;
}

Tree * create_tree(){
  Tree * t = malloc(sizeof(Tree));
  t->root = NULL;
  return t;
}

void bota_na_arvore(Node * n, Tree * t, FILE * out){
  Node * i, * prev = NULL;
  
  if(t->root == NULL){
    t->root = n;
    n->altura = 0;
  } else{
    i = t->root;
    while(i != NULL){
      n->altura += 1;
      prev = i;
      if(n->key < i->key){
        i = i->left;
      } else{
        i = i->right;
      }
    }
    n->altura += 1;
    if(n->key < prev->key){
      prev->left = n;
    } else{
      prev->right = n;
    }
  }
  fprintf(out, "%d ", n->altura);
  return;
}

void busca_maior_no(Tree * t, Node ** maior, Node ** pai){
  Node * i = NULL;  
  if(t->root != NULL){
    i = t->root;
    while(i != NULL){ 
      *pai = *maior;
      *maior = i;
      i = i->right;
    }
  }
  return;
}

// void imprime_alturas(Node * no, Tree * t){ //Imprime por camda da árvore
//   if(no != NULL){
//     if(no == t->root)
//       printf("%d ", no->altura);
//     if(no->left != NULL)
//       printf("%d ", no->left->altura);
//     if(no->right != NULL)
//       printf("%d ", no->right->altura);
//     imprime_alturas(no->left, t);
//     imprime_alturas(no->right, t);
//   }
//   return;
// }

void imprime_maior(Node * maior, Node * predecessor, FILE * out){
  if(maior != NULL){
    fprintf(out, "max %d alt %d pred ", maior->key, maior->altura);
    if(predecessor != NULL){
      fprintf(out, "%d\n", predecessor->key);
    }else{
      fprintf(out, "NaN\n");
    }
  }
  return;
}

void queima_arvore(Node * n, Tree * t){
  if(n != NULL){
    if(n == t->root)
      t->root = NULL;
    queima_arvore(n->left, t);
    queima_arvore(n->right, t);
    free(n);
  }
  return;
}

void processa_linha(char * linha, Tree * t, FILE * out){
  char * token = strtok(linha, " ");
  int key;
  Node * no;
  while(token != NULL){
    key = atoi(token);
    no = create_node(key);
    bota_na_arvore(no, t, out);
    token = strtok(NULL, " ");
  }
  return;
}

int main(void){
  FILE * input, * output;
  Tree * tree = create_tree();
  Node * predecessor = NULL, * maior = NULL;
  char linha[MAX];

  input = fopen("L2Q1.in", "r");
  if(input){
    output = fopen("L2Q1.out", "w");
    if(output){
      while(!feof(input)){
        if(fgets(linha, MAX, input) != NULL){
          processa_linha(linha, tree, output);
          busca_maior_no(tree, &maior, &predecessor);
          imprime_maior(maior, predecessor, output);
          queima_arvore(tree->root, tree);
        }
      }
    }else{
      perror("O arquivo de saída não foi aberto.\n");
    }
  }else{
    perror("O arquivo de entrada não foi aberto.\n");
  }

  free(tree);
  fclose(output);
  fclose(input);
  return 0;
}