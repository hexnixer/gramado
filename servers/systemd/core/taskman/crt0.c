
// crt0.c


//usado para inicializar a rt na libc99


#include "tm.h"


extern int main ( int argc, char *argv[] ); 

static char *argv[] = { "-init",NULL };
static char *envp[] = { "ROOT=root:/volume0", NULL };

//
// # Entry point #
//


void crt0 (){
	
    int ExitCode;	

//initRT:	

    //Inicializando o suporte a alocação dinâmica de memória.
	libcInitRT();

	//Inicializando o suporte ao fluxo padrão.
    stdioInitialize();	
	
	// #debug
	// Testing output message.
	// printf("crt0: TASKMAN.BIN is alive \n");
    // refresh_screen();
	
	ExitCode = (int) main ( 1, argv ); 
	
	// Chama kill ou exit de acordo com o problema ocorrido em main.
	// o erro pode vir no retorno ou implementa-se uma forma de pegar a execessão 
	// ocorrida durante a execussão de main.
	
	exit ( ExitCode );
	
	while (1){
		asm ("pause");
	};
};

