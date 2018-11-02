

//ps2kbd.c 

#include <kernel.h>

int dummyps2kbdkkk;


/*
 * init_keyboard:
 *     ??
 *     Inicializa o driver de teclado.
 *
 *  @todo: enviar para o driver de teclado o que for de lá.
 *         criar a variável keyboard_type ;;; ABNT2 
 */
// void keyboardInit()
void ps2_keyboard_initialize (){
	
	//user.h
	ioControl_keyboard = (struct ioControl_d *) malloc( sizeof(struct ioControl_d) );
	
	if ( (void *) ioControl_keyboard == NULL )
	{
		printf("ldsic-init_keyboard: ioControl_keyboard fail");
		die();
	}else{
	    
	    ioControl_keyboard->id = 0;
	    ioControl_keyboard->used = 1;
	    ioControl_keyboard->magic = 1234;
	    
		ioControl_keyboard->tid = 0;  //qual threa está usando o dispositivo.
	    //ioControl_keyboard->
	};
	
    //int Type = 0;

    //
    // @todo: 
	//     Checar se o teclado é do tipo abnt2.   
	//     É necessário sondar parâmetros de hardware,
	//     como fabricante, modelo para configirar estruturas 
	//     e variáveis.
	//


/*
    switch(Type)
	{
	    //NULL
		case 0:	
		    break;
			
	    //Americano.
		case 1:	
		    break;

		//pt-ABNT2	
	    case 2:	
		    break;
			
		//...
		
		//Modelo americano
		default:	
		    break;
	}
	
*/
	//
	// Set abnt2.
	//

	abnt2 = (int) 1;

    //Checar quem está tentando inicializar o módulo.    

	//model.
	
	//handler.
	
	//...

    //Key status.
	key_status = 0;
    escape_status = 0;
    tab_status = 0;
    winkey_status = 0;
    ctrl_status = 0;
    alt_status = 0;
    shift_status = 0;
	capslock_status = 0;
	scrolllock_status = 0;
	numlock_status = 0;
	//...

	//AE    Enable Keyboard Interface: clears Bit 4 of command register
	//      enabling keyboard interface.
	kbdc_wait(1);
	outportb(0x64,0xAE);   // Activar a primeira porta PS/2
	
	//reset
	kbdc_wait(1);
	outportb(0x60,0xFF);


	//Leds.
	//LED_SCROLLLOCK 
	//LED_NUMLOCK 
	//LED_CAPSLOCK  	
	keyboard_set_leds(LED_NUMLOCK);
	
	//...
	
	// Ativar a primeira porta PS/2.
	//kbdc_wait(1);
	//outportb(0x64,0xAE);   
	
	//Debug support.
	scStatus = 0;

    g_driver_keyboard_initialized = (int) 1;
};

//Pega o status das teclas de modificação.

unsigned long keyboardGetKeyState ( unsigned char key ){
	
	unsigned long State = 0;
	
	switch (key){
		
		case VK_LSHIFT: 
		    State = shift_status; 
			break;

	    case VK_LCONTROL:
		    State = ctrl_status;
		    break;

	    case VK_LWIN:
		    State = winkey_status;
		    break;

	    case VK_LMENU:
		    State = alt_status;
		    break;

	    case VK_RWIN:
		    State = winkey_status;
		    break;

	    case VK_RCONTROL:
		    State = ctrl_status;
		    break;
			
	    case VK_RSHIFT:
		    State = shift_status;
		    break;

	    case VK_CAPITAL:
		    State = capslock_status;
		    break;

	    case VK_NUMLOCK:
		    State = numlock_status;
		    break;
			
		case VK_SCROLL:
            State = scrolllock_status;
            break;			
			
		//...
	};

	//Nothing.

    return (unsigned long) State;		
};

// Inicializa o status das teclas de modificação.
// são usadas em comjunto com outras teclas para criar atalhos.
// modificam temporariamente a função de outra tecla.
void ldisc_init_modifier_keys (){
	
	// Modifier keys.
	
	// Shift.
	shift_status = 0;
	
	// Control.
	ctrl_status = 0;
	
	// Win key.
	winkey_status = 0;
    
	// Alternate.
    alt_status = 0;

	// Alternate Graphic.
    //altgr_status = 0; //@todo
    
	// Function.
	//fn_status = 0;  //@todo
	
	//...
};



// modificam permanentemente a função de outra tecla.
//ativa as teclas extendidas.
void ldisc_init_lock_keys (){
	
    // Capital Lock.	
	capslock_status = 0;
	
	// Scrolling Lock.
	scrolllock_status = 0;
	
	// Number Lock.
	numlock_status = 0;	
};




/*
 * keyboardEnable:
 *     Enable keyboard.
 */
void keyboardEnable (){
	
	//Wait for bit 1 of status reg to be zero.
    
	while ( (inportb(0x64) & 2) != 0 )
	{
		//Nothing.
	};
	
	//Send code for setting Enable command.
    outportb(0x60,0xF4);
    //sleep(100);
};


/*
 * keyboardDisable:
 *     Disable keyboard.
 */
void keyboardDisable (){
	
	//Wait for bit 1 of status reg to be zero.
    
	while ( (inportb(0x64) & 2) != 0 )
	{
		//Nothing.
	};
	
	//Send code for setting disable command.
    outportb(0x60,0xF5);
    //sleep(100);
};


/*
 * keyboard_set_leds:
 *     Set keyboard flags.
 *     ED = Set led.
 */
void keyboard_set_leds (char flag){
	
	//@todo: filtro.

	//Wait for bit 1 of status reg to be zero.
    while( (inportb(0x64) & 2) != 0 ){
		//Nothing.
	};
	//Send code for setting the flag.
    outportb(0x60,0xED);            
    sleep(100);

	//Wait for bit 1 of status reg to be zero.
	while( (inportb(0x64) & 2) != 0 ){
	    //Nothing.	
	};
    //Send flag. 
	outportb(0x60,flag);
	sleep(100);
	
	//@todo mudar o status.
    //switch(flag)
    //{
		
	//}	
};





/*
 * KdGetWindowPointer:
 *     Retorna o ponteiro da estrutura de janela que pertence a thread.
 *     Dado o id de uma thread, retorna o ponteiro de estrutura da janela 
 * à qual a thread pertence.
 */
void *KdGetWindowPointer (int tid){
	
	struct thread_d *t;

	//@todo: filtrar argumento. 
	
	if ( tid < 0 )
        return NULL;
        
		
	// Structure.
	t = (void *) threadList[tid];

	if ( (void *) t == NULL )
	{
        return NULL;        
	};
	
	return (void *) t->window;
};


/*
 * KbGetMessage:
 *     Pega a mensagem na fila de mensagens na estrutura da thread
 * com foco de entrada.
 *
 * Na estrutura da thread com foco de entrada tem uma fila de mensagens.
 * Pegar a mensagem.
 * 
 * Para falha, retorna -1.
 *
 * @todo: bugbug: A mensagem deve estar na fila do processo, na
 *                estrutura do proceso. (Talvez não na thread e nem na janela.)
 */
int KbGetMessage (int tid){
	
	int ret_val;
	struct thread_d *t;
	
	// Structure.
	t = (void*) threadList[tid];

	if ( (void *) t != NULL )
	{
        ret_val = (int) t->msg;
	}else{
	    ret_val = (int) -1;    //Fail.
	};

// Done.
//done:

	WindowProcedure->msgStatus = 0;    //Muda o status.
	return (int) ret_val;              //Retorna a mensagem.
};


/*
 * KbGetLongParam1:
 *    Pega o parametro "long1" do procedimento de janela de uma thread.
 */
unsigned long KbGetLongParam1 (int tid){
   	
	struct thread_d *t;
	
	// Structure.
	t = (void *) threadList[tid];

	if ( (void *) t == NULL)
	{
        return (unsigned long) 0;    //@todo: fail;
	};

// Done.
//done:
    return (unsigned long) t->long1;
};

/*
 * KbGetLongParam2:
 *     Pega o parametro "long2" do procedimento de janela de uma thread.
 */
unsigned long KbGetLongParam2 (int tid){
	
	struct thread_d *t;
	
	// Structure.
	t = (void *) threadList[tid];

	if ( (void *) t == NULL)
	{
        return (unsigned long) 0;    //@todo: fail;
	};

// Done.
//done:
    return (unsigned long) t->long2;
};



/*
 * reboot: 
 *     @todo: essa rotina poderá ter seu próprio arquivo.
 *     Reboot system via keyboard port.
 *     ?? #bugbug Por que o reboot está aqui ??
 *
 * *IMPORTANTE: a interface fechou o que tinha qe fechar,
 * hal chamou essa hotina para efetuar a parte de hardware reboot apenas.
 * @todo: Atribuições.
 *
 * Atribuições: 
 *     + Desabilitar as interrupções.
 *     + Salvar registros.
 *     + Salvar programas abertos e não salvos.
 *     + Fechar todas tarefas antes.
 *     + Efetuar o tipo de reboot especificado.
 *    + Outras ...
 */
void reboot (){
    
    //@todo: 
	// +criar uma variavel global que especifique o tipo de reboot.
    // +criar um switch para efetuar os tipos de reboot.
	// +criar rota de fuga para reboot abortado.
	// +Identificar o uso da gui antes de apagar a tela.
	//  modo grafico ou modo texto.
	//
	
	//
	// Video.
	//
	
	/*
	sleep(2000);
	//kclear(9);
    set_up_cursor(0,0);	
    set_up_text_color(0x0f, 0x09);
	printf("\n\n REBOOTING ...\n\n");


	//
	// Scheduler stuffs.
	//
	
	sleep(1000);
	printf("locking scheduler ...\n");
	scheduler_lock();
	
	//
	// Tasks.
	//
	
	//@todo: fazer função com while. semelhante ao dead task collector.
	
	sleep(1000);
	printf("killing tasks ...\n");
	//kill_thread(current_task); 
	
	//
	// Final message.
	//
	
	sleep(1000);
	printf("turning off ...\n");
    
	
	refresh_screen();
	
	//
	// Interruoções.
	//
	
	sleep(7000);
	asm("cli");
	
	*/
	
	
	// @todo: disable();
	
//
// Done.
//

//done:

    hal_reboot ();
	die ();
};



//Get alt Status.
int get_alt_status (){
	
    return (int) alt_status;
};


//Get control status.
int get_ctrl_status (){
	
    return (int) ctrl_status;
};

 
int get_shift_status (){
	
    return (int) shift_status;	
};
 

/*
 * kbdc_wait:
 *     Espera por flag de autorização para ler ou escrever.
 *     (Nelson Cole) 
 */
void kbdc_wait (unsigned char type){
	
	if (type==0)
	{
		//#bugbug rever
        while ( !inportb(0x64) & 1 )
		{
			outanyb (0x80);
		};
		
    }else{
		
        while ( inportb(0x64) & 2 )
		{
			outanyb (0x80);
		};
	};
};



/*
 * Constructor.
int keyboardKeyboard(){
	;
};
*/


/*
 obs: definido acima.
int keyboardInit(){
	;
};
*/

/*
void keyboard();
void keyboard()
{
	//@todo: Create global.
	if(gKeyboardType == 1){
		abnt2_keyboard_handler();
	}
	//...
	return;
}
*/


