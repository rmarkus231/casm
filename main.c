#include "bfi.h"
#include <stdio.h>
#include <string.h>


/*
int main(int argc, char* argv[]){
	if(argc != 2){
		printf("needs 2 arguments");
		return 1;
	}else if(*argv[2] == '1'){
		interpret(argv[1]);
	}else if(*argv[2] == '2'){
		interpret2(argv[1]);
	}else if(!strcmp(argv[2],"asm")){
		get_asm(argv[1]);
	}
	return 0;
}
*/
int main(int argc, char* argv[]){
	if(argc != 3){
		printf("needs 2 arguments, you have gived %d\n",argc);
		return 1;
	}else if(!strcmp(argv[2],"1")){
		interpret("++[>++[>---<-]<-]#");
	}else if(!strcmp(argv[2],"2")){
		interpret2("++[>++[>---<-]<-]#");
	}else if(!strcmp(argv[2],"asm")){
		get_asm("++[>++[>---<-]<-]#");
	}
	return 0;
}
