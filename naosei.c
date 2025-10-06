#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define PATH_LEN 250
#define FILE_NAME_LEN 100
#define MSG_LEN 1000
void trataPath(char *path, int tamMax, char* arg){
int argLen = strlen(arg);
assert(argLen<tamMax);
if(arg[argLen-1]='/'){
arg[argLen-1]='\0';
}
strcpy(path,arg);
}

void trataNomeArquivo(char *path, int tamMax, char* arg){
int argLen = strlen(arg);
assert((argLen+4)<tamMax);
sprintf(path,"%s.txt",arg);
}

void main(int argc, char *argv[]){
char dir[PATH_LEN], arq[FILE_NAME_LEN], msg[MSG_LEN];
char *fullNameArq;
FILE *f;
/* MOSTRA OS PARAMETROS */
for(int i= 0; i< argc; i++){
printf("argv[%d] = %s\n",i,argv[i]);
}
/* TRATA PARAMETROS */

int i = 1;
strcpy(msg,"");
while (i < argc){
if (strcmp(argv[i],"-d")==0){
i++;
/* se i >= argc: ERRO-falta parametro */
trataPath(dir,PATH_LEN, argv[i]);
}
else if (strcmp(argv[i],"-f") == 0){
i++;
/* se i >= argc: ERRO-falta parametro */
trataNomeArquivo(arq,FILE_NAME_LEN,argv[i]);
}
else{
strcat(msg,argv[i]);
strcat(msg," ");
}
i++;
} //while
/* GRAVA MENSAGEM NO ARQUIVO (DE ACORDO COM OS PARAMETROS) */
int pLen = strlen(dir);
int fLen = strlen(arq);
fullNameArq = (char *)malloc((pLen+fLen+2)*sizeof(char));
sprintf(fullNameArq, "%s/%s",dir,arq);
f = (FILE *)fopen(fullNameArq,"w");
fprintf(f,"%s\n",msg);
fclose(f);
free(fullNameArq);
}
