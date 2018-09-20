;
; File: msm\setup.inc 
;
; Descri��o:
;     Faz configura��es de hardware para a arquitetura x86.
;
; Vers�o 1.0, 2015.
;


;=======================================================================
; setup_machine:
;     Configura a arquitetura.  ??obs: @todo: certificar que estamos um 32bit ??
;
setup_machine:
;    nop
;setupSetupMachine:

    ;;
	;; Interrupt enabling: Come�aremos configurando suporte a interrup��es.
	;;

    pushad
	cli

    ;GDT and IDT. 	
    
	lgdt [lgdt_opcode] 
    
	lidt [lidt_opcode]	
	
	;;
	;; @todo: Nessa hora alguns vetores podem ser configurados.
	;;        primeiro as excess�es e depois as ISRs.
	;;

	;;
	;; Selecting the 'Processor Interrup Mode'.
	;; * PIC MODE *
	;;
		
	;;
    ;; Todos os componentes APIC s�o ignorados e o sistema opera
    ;; no modo single-thread usando LINT0.
    ;;	
		
;.setupPICMODE:	

	;PIC.
	cli
	mov al, 00010001b		;begin PIC 1 initialization.
	out 0x20, al
	mov al, 00010001b		;begin PIC 2 initialization.
	out 0xA0, al	
	mov al, 0x20			;IRQ 0-7: interrupts 20h-27h.
	out 0x21, al
	mov al, 0x28			;IRQ 8-15: interrupts 28h-2Fh.
	out 0xA1, al
	mov al, 4
	out 0x21, al
	mov al, 2
	out 0xA1, al
	mov al, 1
	out 0x21, al
	out 0xA1, al
	
	
	;;
	;; Configurando os timers do sistema.
	;; Nossa op��o agora � o PIT.
	;; @todo: Para o RTC podemos fazer uma pequena inicializa��o agora.
	;; pois um m�dulo mais completo est� no kernel base.
	;;
	
	;TIMER.
	mov al, byte 0x36
	mov dx, word 0x43
	out dx, al
	mov eax, dword 11930    ;Timer frequency 100 HZ.
	mov dx, word 0x40
	out dx, al
	mov al, ah
	out dx, al
	
	;;@todo:
	;;Init RTC.

	
	;;step 
    ;;@todo: memory caching control.	
	
	;;step 
	;processor Discovery and initialization
	;(cpuid threads and cores, start inter-processor interrupt sipi, ap wakeu pstate,
	;wakeup vector alignment, caching consdarations,ap idle state)	
	;apenas o b�sico para o boot manager.
	
		
	;;
	;; Fazendo alguma inicializa��o de dispositivos de I/O suportados.
	;;

	;step
	;i/o devices
	;( embedded controller EC, super io SIO, legacy free systems, miscellaneous io devices)
    ;Nesse momento os dispositivos de i/o suportados aqui s�o (keyboard, IDE, graphic card).
	
	;KEYBOARD.
	call keyboardInit
	
	;IDE0.
	call init_ide0_master
	
	;LFB.
	call configura_lfb   ;;?? onde est� essa rotina ??
	
	
	;
	; #BUGBUG
	; @todo:
	;     Aqui estamos usando constantes, porem esses valores precisam ser
	; detectados, pois variam � cada formata��o.
	;
	
	;Detectar os par�metros do sistema de arquivos FAT16.
	;fat lba, root lba ...
	call fat16DetectFSParameters
	
	;Setup current dir.
    mov dword [g_current_fat_lba],        FAT16_FAT_LBA       ;Fat do volume atual
    mov dword [g_current_directory_lba],  FAT16_ROOTDIR_LBA   ;Diret�rio do volume atual.
	mov dword [g_current_dataarea_lba],   FAT16_DATAAREA_LBA  ;?? ;lba do inicio da area de dados do volume atual.
	;Continua...
		
	
    ;;
    ;; Configurando PCI. (O b�sico suportado pelo Boot Manager.)
	;; O boot manager est� carregando o BL utilizando recursos 
	;; do BIOS em modo real. Mas se o BM fosse carregar o BL 
	;; de um dispositivo SATA em 32bit teria que configurar o PCI.
	;; (Se poss�vel deixaremos para o BL o penoso trabalho de lidar
	;; com PCI, j� que o BL � feito em C, isso deve ser mais f�cil para
	;; o BL do que para o BM.)
    ;;
	
	;step
	;;PCI device descovery.	

	
	

	;CURSOR.
	call reset_cursor 
	

	;
	; Continua ...
	;
	
	cli 

    ;Mascara todas as interrup��es.	
	mov  al, 255
	out  0xA1, al
	out  0x21, al
	
	popad
	RET
	
;
; End.
;