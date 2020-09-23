/*
 * File: main.c
 *
 *     Simple text editor for Gramado Window Server.
 * 
 * History:
 *     2020 - Created by Fred Nora.
 */


// Connecting via AF_INET.

// tutorial example taken from. 
// https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
 
/*
    To make a process a TCP server, you need to follow the steps given below −

    Create a socket with the socket() system call.

    Bind the socket to an address using the bind() system call. 
    For a server socket on the Internet, an address consists of a 
    port number on the host machine.

    Listen for connections with the listen() system call.

    Accept a connection with the accept() system call. 
    This call typically blocks until a client connects with the server.

    Send and receive data using the read() and write() system calls.
*/ 

// See:
// https://wiki.osdev.org/Message_Passing_Tutorial
// https://wiki.osdev.org/Synchronization_Primitives
// ...
 
 
#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <packet.h>


// The client-side library.
#include <gws.h>

#include <fileman.h>



//
// == ports ====================================
//

#define PORTS_WS 4040
#define PORTS_NS 4041
#define PORTS_FS 4042
// ...



// #test
// Tentando deixar o buffer aqui e aproveitar em mais funções.
char __buffer[512];

#define IP(a, b, c, d) (a << 24 | b << 16 | c << 8 | d)

// tipos de pacotes.
//#define SERVER_PACKET_TYPE_REQUEST    1000 
//#define SERVER_PACKET_TYPE_REPLY      1001 
//#define SERVER_PACKET_TYPE_EVENT      1002
//#define SERVER_PACKET_TYPE_ERROR      1003



// Hello!
// Podemos isso na lib.
int _hello_request(int fd);
int _hello_response(int fd);
void _hello(int fd);

//message support
int _loop(int fd);
int _getmessage_request(int fd);
int _getmessage_response(int fd);




int fileman_init_globals(void)
{
    gws_debug_print("fileman_init_globals:\n");
    
    gScreenWidth  = gws_get_system_metrics(1);
    gScreenHeight = gws_get_system_metrics(2);

    
    //...
    
    return 0;
}


int fileman_init_windows(void)
{
    int i=0;

    gws_debug_print("fileman_init_windows:\n");
    
    for (i=0; i<WINDOW_COUNT_MAX; i++){
        windowList[i] = 0;
    };
    
    return 0;
}




int _getmessage_request(int fd)
{
    // Isso permite ler a mensagem na forma de longs.
    unsigned long *message_buffer = (unsigned long *) &__buffer[0];   

    int n_writes = 0;   // For sending requests.

    //char *name = "Window name 1";

   
    //
    // Send request.
    //


    // #debug
    //gws_debug_print ("gwm: Writing ...\n");      

    // Enviamos um request para o servidor.
    // ?? Precisamos mesmo de um loop para isso. ??
    // msg = 369 (get input event)

    while (1)
    {
        // Create window    
        message_buffer[0] = 0;       // window. 
        message_buffer[1] = 369;    //get message request  //1001;    // msg. Create window.
        message_buffer[2] = 0;
        message_buffer[3] = 0;
 
        //message_buffer[4] = left; //120;   //x
        //message_buffer[5] = top; //120;   //y
        //message_buffer[6] = width; //480;   //w
        //message_buffer[7] = height; //320;   //h
        
        //message_buffer[8] = bg_color; //xCOLOR_GRAY2; 

         
        //...

        // Write!
        // Se foi possível enviar, então saimos do loop.  

        // n_writes = write (fd, __buffer, sizeof(__buffer));
        n_writes = send (fd, __buffer, sizeof(__buffer), 0);
       
        if (n_writes>0){ break; }
    }

    return 0; 
}


int _getmessage_response(int fd)
{
    unsigned long *message_buffer = (unsigned long *) &__buffer[0];   
    int n_reads = 0;    // For receiving responses.
    int y=0;

    //
    // Waiting for response. ==================
    //

    // Espera para ler a resposta. 
    // Esperando com yield como teste.
    // Isso demora, pois a resposta só será enviada depois de
    // prestado o servido.
    // obs: Nesse momento deveríamos estar dormindo.

    // #debug
    //gws_debug_print ("gwm: Waiting ...\n");      

    for(y=0; y<15; y++)
        gws_yield();


    // #todo
    // Podemos checar antes se o fd 
    // representa um objeto que permite leitura.
    // Pode nem ser possível.
    // Mas como sabemos que é um soquete,
    // então sabemos que é possível ler.


    //
    // read
    //

    // #debug
    //gws_debug_print ("gwm: reading ...\n");      


    // #caution
    // Waiting for response.
    // We can stay here for ever.

response_loop:

    //n_reads = read ( fd, __buffer, sizeof(__buffer) );
    n_reads = recv ( fd, __buffer, sizeof(__buffer), 0 );
    
    //if (n_reads<=0){
    //     gws_yield(); 
    //    goto response_loop;
    //}
    
    // Se retornou 0, podemos tentar novamente.
    if (n_reads == 0){
        gws_yield(); 
        goto response_loop;
    }

    // Se retornou -1 é porque algo está errado com o arquivo.
    if (n_reads < 0){
        gws_debug_print ("editor: recv fail\n");
        printf          ("editor: recv fail\n");
        printf ("Something is wrong with the socket.\n");
        exit(1);
    }


    //
    // The msg index.
    //
    
    // Get the message sended by the server.

    int window          = (int) message_buffer[0];
    int msg             = (int) message_buffer[1];
    unsigned long long1 = (unsigned long) message_buffer[2];
    unsigned long long2 = (unsigned long) message_buffer[3];
    
    //#debug
    //if(msg!=0)
        //printf ("%c",long1); //printf ("{%d%c} ",msg,long1);


    switch (msg){

        case MSG_KEYDOWN:
            // ...
            switch (long1)
            {
                //case 0:
                    //relax cpu
                    //break; 
                    
                //case VK_RETURN:
                    //goto process_event;
                    //break;
                  
                //case VK_TAB:
                //case VK_BACK:
                
                //...

                default:
                    //gws_draw_text ( (int) fd, (int) 0, 40, 40, COLOR_BLUE, "x");
                    //terminal_write_char(long1) #todo
                    printf ("%c",long1); 
                    fflush(stdout);
                    goto process_event;
                    break;
            };
            break;


          //case MSG_KEYUP:
          case 21:  
            //gws_debug_print ("MSG_KEYUP\n");
            goto process_event;
            break;

            
        case MSG_SYSKEYDOWN:
            switch (long1)
            {
                case VK_F1:
                    //printf ("editor: VK_F1\n");
                    // fd, window id, left, top, color, string.
                    gws_draw_text ( (int) fd, (int) 0, 40, 40, COLOR_BLUE, "[F1]");
                    break;

                case VK_F2:
                    //printf ("editor: VK_F2\n");
                    break;
                    
                case VK_F3:
                    //printf ("editor: VK_F3\n");
                    break;
                    
                case VK_F4:
                    //printf ("editor: VK_F4 reboot\n");
                    gws_draw_text ( (int) fd, (int) 0, 80, 80, COLOR_BLUE, "[F4] Exiting...");
                    gws_debug_print("editor: Closing socket and exiting ...\n");
                    close (fd);
                    exit(0);
                    break;
                    
                default:
                    goto process_event;
                    break;
            };
            break;
            

        case MSG_SYSKEYUP:
            switch (long1)
            {
                //case VK_F1:
                default: 
                    goto process_event;
                    break;
            };
            break;

        // Commands.
        case MSG_COMMAND:
            switch (long1)
            {
                //case CMD_ABOUT:
                    //printf ("terminal: CMD_ABOUT\n");
                    //goto process_event;
                    //break;
                    
                default:
                    goto process_event;
                    break;
            };

 
        case SERVER_PACKET_TYPE_REQUEST:
            gws_yield ();
            goto response_loop;
            break;
            
        // Reply!
        case SERVER_PACKET_TYPE_REPLY:
            goto process_reply;
            break;
            
        case SERVER_PACKET_TYPE_EVENT:
            goto process_event;
            //goto response_loop;
            break;
            
        case SERVER_PACKET_TYPE_ERROR:
            gws_debug_print ("editor: SERVER_PACKET_TYPE_ERROR\n");
            goto response_loop;
            //exit (-1);
            break;
        
        default:
            gws_debug_print ("@");
            goto process_event;
            //goto response_loop;
            break; 
    };

//
// Process reply.
//

// A resposta tras o window id no início do buffer.
    
process_reply:

    gws_debug_print ("editor: process_reply:\n"); 

    // #test
    //gws_debug_print ("editor: Testing close() ...\n"); 
    //close (fd);

    //gws_debug_print ("gwst: bye\n"); 
    //printf ("editor: Window ID %d \n", message_buffer[0] );
    //printf ("editor: Bye\n");
    
    // #todo
    // Podemos usar a biblioteca e testarmos
    // vários serviços da biblioteca nesse momento.

    return 0;

//
// Process an event.
//

process_event:
    gws_debug_print ("editor: We got an event\n"); 
    return 0;
}


// loop
// Loop de requests para o gws.
int _loop(int fd)
{

    if (fd<0){
        gws_debug_print ("_loop: fd fail\n");
        printf          ("_loop: fd fail\n");
        return -1;
    }


    while (1){
        _getmessage_request(fd);
        _getmessage_response(fd);
    };

    return 0; 
}


//...




//char *hello = "Hello there!\n";
/*
#define IP(a, b, c, d) (a << 24 | b << 16 | c << 8 | d)
struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port   = 7548, 
    .sin_addr   = IP(192, 168, 1, 79),
};
*/




int _hello_response(int fd){

    unsigned long *message_buffer = (unsigned long *) &__buffer[0];   
    int n_reads = 0;    // For receiving responses.


    //
    // waiting
    //

    // Espera para ler a resposta. 
    // Esperando com yield como teste.
    // Isso demora, pois a resposta só será enviada depois de
    // prestado o servido.
    // obs: Nesse momento deveríamos estar dormindo.

    // #debug
    debug_print ("editor: Waiting ...\n");      


    int y;
    for(y=0; y<15; y++)
        gws_yield();



    //
    // read
    //

    // #debug
    debug_print ("editor: Reading ...\n");      


       //#caution
       //we can stay here for ever.
       //it's a test yet.
__again:
    n_reads = read ( fd, __buffer, sizeof(__buffer) );
    
    // Não vamos insistir num arquivo vazio.
    //if (n_reads<=0){
    //     gws_yield();        
    //    goto __again;
    //}

    if (n_reads == 0){
        gws_yield(); 
        goto __again;
    }
    
    if (n_reads < 0){
        printf ("editor: recv fail.\n");
        printf ("Something is wrong with the socket.\n");
        exit (1);
    }
    

    
    // Get the message sended by the server.
    int msg = (int) message_buffer[1];
    
    switch (msg)
    {
        case SERVER_PACKET_TYPE_REQUEST:
            gws_yield ();
            goto __again;
            break;
            
        case SERVER_PACKET_TYPE_REPLY:
            debug_print ("editor: SERVER_PACKET_TYPE_REPLY received\n"); 
            goto process_reply;
            break;
            
        case SERVER_PACKET_TYPE_EVENT:
            //todo: call procedure.
            goto __again;
            break;
            
        case SERVER_PACKET_TYPE_ERROR:
            debug_print ("editor: SERVER_PACKET_TYPE_ERROR\n");
            goto __again;
            //exit (-1);
            break;
        
        default:
            goto __again;
            break; 
    };


process_reply:

    //
    // done:
    //

    //printf("%d bytes readed\n",n_reads);
    printf("RESPONSE: {%s} \n",__buffer+16);

    return 0;
}


int _hello_request(int fd){

    // Isso permite ler a mensagem na forma de longs.
    unsigned long *message_buffer = (unsigned long *) &__buffer[0];   

    int n_writes = 0;   // For sending requests.

     //
     // Loop for new message.
     //

    unsigned long ____color = 0x00FF00;

// loop:
new_message:

    
    //
    // Write
    //

    // #debug
    debug_print ("editor: Writing ...\n");  

    // Enviamos um request para o servidor.
    // ?? Precisamos mesmo de um loop para isso. ??


    // Write!
    // Se foi possível enviar, então saimos do loop.  
    // obs: podemos usar send();

    while (1){

        // Solicita um hello na posição x,y. 
        message_buffer[0] = 0;       // window. 
        message_buffer[1] = 1000;    // msg=hello.
        message_buffer[2] = 10;       // x
        message_buffer[3] = 40;       // y (Depois da topbar) 
        // ...

        n_writes = write (fd, __buffer, sizeof(__buffer));
        
        if (n_writes>0){ break; }
    };

    return 0;
}


void _hello(int fd)
{
    if (fd<0)
        return;


    //while(1){
        _hello_request(fd);
        _hello_response(fd);
    //}
}


/*
 **********************************************
 * main:
 *     Main function for editor.bin.
 */

int main ( int argc, char *argv[] ){

    int client_fd = -1;

    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;

    // Connecting to the window server in this machine.
    addr_in.sin_port   = PORTS_WS;   
    addr_in.sin_addr.s_addr = IP(127,0,0,1); 

    // Metrics.
    unsigned long w = gws_get_system_metrics(1);
    unsigned long h = gws_get_system_metrics(2);


    debug_print ("------------------------\n"); 
    debug_print ("editor: Initializing ...\n");


    //
    // Socket
    //

    // #debug
    gws_debug_print ("editor: Creating socket\n");
    //printf          ("editor: Creating socket\n");


    // cria o soquete.
    // AF_GRAMADO
    //client_fd = socket ( 8000, SOCK_STREAM, 0 );
    client_fd = socket ( AF_INET, SOCK_STREAM, 0 );
    
    if ( client_fd < 0 ){
       printf ("editor: Couldn't create socket\n");
       exit(1);
    }


    //
    // connect
    // 

    //nessa hora colocamos no accept um fd.
    //então o servidor escreverá em nosso arquivo.

    while (1){

        // #debug
        gws_debug_print ("editor: Connecting to ws via inet ...\n");
        //printf          ("editor: Connecting to ws via inet ...\n");

        if (connect (client_fd, (void *) &addr_in, sizeof(addr_in)) < 0)
        { 
            gws_debug_print("editor: Connection Failed \n");
            printf         ("editor: Connection Failed \n"); 
        
        }else{ break; }; 
    };

    //
    // messages
    //
   
    // #test
    // Testing loop; ok.
    // #todo
    // Podemos checar antes se o fd 
    // representa um objeto que permite leitura.
    // Pode nem ser possível.
    // Mas como sabemos que é um soquete,
    // então sabemos que é possível ler.

    _hello(client_fd);


    /*
    // libgws
    // Create window using the client-side gui.

    // main window.
    gws_create_window_using_socket (client_fd,
        WT_SIMPLE,1,1,"File Manager",
        40, 40, 640, 320,
        0, 0, COLOR_WHITE, COLOR_WHITE );
   */



    //#todo: salvar em global
    //por enquanto aqui
    int main_window=0;
    int addressbar_window=0; 
    int client_window=0;
    int button=0;


    //
    // == main window ============================================
    //

    main_window = gws_create_window (client_fd,
        WT_OVERLAPPED,1,1,"Editor", //WT_SIMPLE,1,1,"Editor",
        40,40,640,480, //0,0,w,h,
        0,0,COLOR_GRAY, COLOR_GRAY);

    if ( main_window < 0 )             
        debug_print("Editor: main_window fail\n"); 


     // Text.
     gws_draw_text (
        (int) client_fd,       // fd,
        (int) main_window,     // window id,
        (unsigned long)  8, 
        (unsigned long) 32 + (40/3), 
        (unsigned long) COLOR_BLACK,
        " Name: ");

    //
    // Address bar (edit box)
    //

    addressbar_window = gws_create_window (client_fd,
        WT_EDITBOX,1,1,"address-bar",
        ((640/8)*1), 32 +4,     //(40/3) + (8*8), 4,    //((w-(w/2))/2), 4, 
        ((640/8)*3), 32,    //(w/2), 32,
        main_window,0,COLOR_WHITE, COLOR_WHITE);

    if ( addressbar_window < 0 )             
        debug_print("Editor: addressbar_window fail\n"); 
        
     
     gws_draw_text (
        (int) client_fd,             // fd,
        (int) addressbar_window,     // window id,
        (unsigned long) 8,           // left,
        (unsigned long) 32 + (40/3),      //8,    // top,
        (unsigned long) COLOR_BLACK,
        "text.txt");

    //
    // [Save F?] button
    //

    button = gws_create_window (client_fd,
        WT_BUTTON,1,1,"Save",
        ((640/8)*6), 32 +4,     //(w-100-4), 4, //(640-100), 4, 
        ((640/8)*1), 32,
        main_window, 0, COLOR_GRAY, COLOR_GRAY);

    if ( button < 0 ) 
        debug_print("Editor: button fail\n"); 

    //
    // == Client window =======================
    //

    // client window (White)
    client_window = gws_create_window (client_fd,
        WT_EDITBOX,1,1,"client",
        4, 32 + 40, 
        (640-8), (480 -32 - 40 - 4),
        main_window,0,COLOR_WHITE, COLOR_WHITE);

    if ( client_window < 0 ) 
        debug_print("Editor: client_window fail\n"); 

    int t=0;
    
    for (t=0; t<32; t++){
    gws_draw_text (
        (int) client_fd,       // fd,
        (int) client_window,   // window id,
        (unsigned long) 4,    //40,    // left,
        (unsigned long) t*8,  //40,    // top,
        (unsigned long) COLOR_BLACK,
        "Hello, this is a text editor!");
    }
    
    
    // Show main window.
    gws_refresh_window(client_fd,main_window);

    //
    // Loop
    //

    // Loop de requests para o gws.
    _loop (client_fd);


    // #importante
    // Se não usarmos o loop acima, então podemos pegar
    // as mensagens do sistema....
    // O ws pode mandar mensagens de sistema para o
    // wm registrado.


    /*
    struct gws_event_d *Event;
     
    for(;;){
        
        Event = (struct gws_event_d *) gws_next_event();
        
        if (Event.type == 0){
           gws_debug_print("editor: event 0\n");
        
        }else if (Event.type == 1){
           gws_debug_print("editor: event 1\n");
        
        }else if (Event.type == 2){
           gws_debug_print("editor: event 2\n");
        
        }else{
           gws_debug_print("editor: Not valid event!\n");
        };
    };
    */


// exit
    //close (client_fd);
    debug_print ("editor: bye\n"); 
    printf      ("editor: bye\n");

    return 0;
}


//
// End.
//







