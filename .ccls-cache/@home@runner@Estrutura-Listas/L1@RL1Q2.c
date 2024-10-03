#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define MAX 255
#define SIZE 100
#define TOTAL 1000

typedef struct Names{
    char name[MAX];
}Names;

typedef struct Mystack{
    Names * S;
    int top;
    int size;
}Mystack;

void init(Mystack * stack){
    stack->size = SIZE;
    stack->S = (Names*)malloc(stack->size * sizeof(Names));
    stack->top = -1;
    return ; 
}

bool isFull(Mystack * stack){
    if(stack->top == stack->size-1){
        return true;
    }
    return false;
}

void push(Mystack * stack, char * name){
    if(!isFull(stack)){
        stack->top = stack->top+1;
        strcpy(stack->S[stack->top].name,name);
    }else{
        printf("Stack is full!\n");
    }
    return ;
}
bool isEmpty(Mystack * stack){
    if(stack->top == -1){
        return true;
    }
    return false;
}
void pop(Mystack * stack){
    if(!isEmpty(stack)){
        stack->top = stack->top-1;
    }
    return ;
}

// void printStack(Mystack * stack){
//     printf("top\n");
//     for(int i = stack->top; i>=0; i--){
//         printf("[%s]\n", stack->S[i].name);
//     }
//     printf("bottom\n");
//     return ; 
// }

void pushAndPop(Mystack * stack, Names * name ,int totalNames, FILE * output){
    char copyname[MAX][TOTAL];
    char aux[MAX];
    int j=0;
    int pops=0;
    int contPop=pops;

    for(int i=0; i<totalNames; i++){
        strcpy(copyname[i],name[i].name);
    }
    while(j<totalNames){
        if(strcmp(copyname[j],stack->S[stack->top].name)<0){
            pop(stack);
            pops+=1;
            strcpy(aux,copyname[j]);
            strcpy(copyname[j],copyname[j-1]);
            strcpy(copyname[j-1],aux);
            j--;
        }else{
            contPop = pops;
            if(pops>0){
                fprintf(output,"%dx-pop ",contPop);
            }
            push(stack,copyname[j]);
            if(stack->top == totalNames-1){
                fprintf(output,"push-%s",stack->S[stack->top].name);
            }else{
                fprintf(output,"push-%s ",stack->S[stack->top].name);
            }
            pops=0;
            j++;
        }
    }
    fprintf(output,"\r");
    return ; 
}

int readNames(char * line, Names * person){
    int i, j, k, contNames;

    for(i=0, contNames=0; line[i]!='\0'; i=j+1, contNames++){
        for(j=i, k=0; line[j]!=' ' && line[j]!='\0' && line[j]!='\r';j++, k++){
            person[contNames].name[k] = line[j];
        }
        person[contNames].name[k]='\0';
    }

    return contNames; 
}

void readWrite(Mystack * stack, Names * person){
    FILE * input;
    FILE * output;
    char line[MAX];
    int totalNames;

    input = fopen("L1Q2.in","r");
    if(input!=0){
        output = fopen("L1Q2.out","w");
        if(output == 0){
            perror("Erro!Nao foi possivel ler o arquivo de saida!\n");
        }else{
            while(!feof(input)){
                if(fgets(line, MAX, input) != NULL){
                    if(strchr(line,'\n')){
                        line[strlen(line)-1]='\0';
                        totalNames = readNames(line, person);
                        pushAndPop(stack, person, totalNames, output);
                        totalNames = 0 ;
                        // printStack(stack);
                        stack->top = -1;
                    }
                }
            }
        }
    }else{
        perror("Erro!Nao foi possivel ler o arquivo de entrada!\n");
    }

    fclose(input);
    fclose(output);

    return ;
}

int main(){

    Mystack * stack = (Mystack*)malloc(sizeof(Mystack));
    Names * person = (Names*) malloc(sizeof(Names));

    init(stack);

    readWrite(stack, person);

    free(stack->S);
    free(stack);
    free(person);

    return EXIT_SUCCESS;
}