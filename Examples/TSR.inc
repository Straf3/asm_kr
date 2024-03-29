; ------- ��� ��� १����⭮� ��� ------
; ********** ���� ��ࠡ��稪 ���⨯���᭮�� ���뢠��� 2Fh **********
; *           �ᯮ������ ��� �ࠢ����� �ணࠬ��� �����. 	*
; * �室: ��=0CDh - ���墠�뢠���� �㭪��. 			*
; *       AL: ����訢����� ����⢨�				*
; *  AL=00: �஢�ઠ �� ��⠭����������. ��室: AL=01: ����㦥�� 	*
; *  AL=01: ���㧨�� �ணࠬ��. ��室: AL=00: �ᯥ�, AL=0E: ��㤠�	*
; *********************************************************************
 New_2Fh proc near
  cmp AH, 0CDh  		; ��� �㭪��?
  jne CallStandart 		; ���-��뢠�� �⠭����� ��ࠡ��稪
  cmp AL, 00h		; �஢�ઠ �� ��⠭����������
  je Loaded
  cmp AL, 01h		; ���㧨�� �ணࠬ��
  je UnLoad
  iret		 	; ###
 CallStandart:
 ; �ᯮ��㥬 jmp, �⮡� �� ��뢠���� � �⥪��. � �⮬ ��砥 ��᫥
 ; ��ࠡ�⪨ ��⥬���� ��ࠡ��稪� � ��� �ࠢ����� �� �����頥���.
  jmp CS:Old_2Fh		; ��뢠�� �⠭����� ��ࠡ��稪
 Loaded:
  mov AL, 01h		; ��, �ண� 㦥 ��⠭������
  iret
 UnLoad:			; ���㧨�� �ணࠬ��
  push DS ES BX CX DX	; ��࠭�� �ᯮ��㥬� ॣ�����
  mov AL, 0Eh		; ��襬 ��� �訡�� �� ���㧪�
  call CheckIfLast		; �஢��塞, ����� �� ���㦠��
  cmp AL, 0Eh		; �����-�� ��室
  je Finish_New_2Fh		
 ; ����⠭�������� ���墠祭�� ������
 ; ����⠭�������� 09h
  mov AX, 2509h		; 25-��⠭���� �����, 09-�����
  lds DX, CS:Old_09h	; DS:=HIBYTE(Old_09h), DX:=LOBYTE(Old_09h)
  int 21h
 ; ����⠭�������� 2Fh
  mov AX, 252Fh		; 25-��⠭���� �����, 2F-�����
  lds DX, CS:Old_2Fh	; DS:=HIBYTE(Old_2Fh), DX:=LOBYTE(Old_2Fh)
  int 21h
 ; ����稬 �� PSP ���� ᮡ�⢥����� ���㦥��� � ���㧨� ���
  mov ES,CS:2Ch
  mov AH, 49h		; �㭪�� �᢮�������� �����
  int 21h
 ; � ⥯��� ���㧨� ᠬ� �ணࠬ��
  push CS			
  pop ES			; ES:=CS-㪠�뢠�� �� PSP
  mov AH, 49h		; �㭪�� �᢮�������� �����
  int 21h			
 Finish_New_2Fh:  
  pop DX CX BX ES DS 	; ����⠭��������  �ᯮ��㥬� ॣ�����
  iret
 New_2Fh endp

; *********** �஢�ઠ, ���� �� ��� ����� 09h ��᫥���� ***********
; *         ���쪮 � �⮬ ��砥 �������� ���४⭠� ���㧪�     	 *
; *  �.�. ��� �訡�� 㦥 ����ᠭ � AL, �� ������������ ���㧪� ���� *
; *  �ᥣ� ���� ���� �� ��室. � ��⨢��� ��砥-������� � AL ����  *
; **********************************************************************
CheckIfLast proc
; ����稬 � ES:BX ���� ��᫥����� �����
 push AX			; ��࠭塞
 mov AH, 35h
 mov AL, 09h
 int 21h
 pop AX
 cmp BX, offset New_09h
 jne FinCheck
 mov CX,ES
 mov BX,CS
 cmp CX,BX
 jne FinCheck
 mov AL,00
FinCheck:
 ret
CheckIfLast endp

; *********** ���� ��ࠡ��稪 ���뢠��� �� ���������� 09h ***********
; *             �ᯮ������ ��� ��⨢�樨 ������ ����. 	 	*
; *********************************************************************
 New_09h proc near
  cli
  pushf
  ; � ����� �ᯮ��㥬 call, ⠪ ��� ᭠砫� ��뢠�� ��⥬��
  ; ��ࠡ��稪, � ��⮬ ���� ��३� � ��� 
  call CS:Old_09h
  push AX BX CX ES
  ; ����稬 ᨬ��� �� ����
  mov AX, 40h
  mov ES,AX		; �� ����� 40h ��室���� ����� ����
  mov BX, ES:1ah		; ���� ��।���� ᨬ����
  mov AX, ES:[BX]		; � �� - ᪠�-��� ����⮩ ������
  cmp AX, F5			; �뫠 ����� F5?
  je TwasF5				; ���⠢�� 䫠� � ���� � ���
  cmp AX, F6			; ��� ?
  je SwapSection
  jne Finish_New_09h	; ���-�� ��室
  
TwasF5:
  cmp flagf5, 0
  jne FlagOne
  mov flagf5, 1
  jmp Finish_New_09h
	FlagOne:
		mov flagf5, 0
		jmp Finish_New_09h
  
SwapSection:
  cmp flagf5, 1
  jne Finish_New_09h
  call SwapChars		; ��-�����塞 ����
  
Finish_New_09h:
  pop ES CX BX AX
  sti
  iret
 New_09h endp

; ******* ��楤�� ������ ���� � ����ᨬ��� �� ���祭�� flag *******
SwapChars proc near
 pusha
 push ES			; ��࠭塞 ॣ�����
 
 ;mov AX,0B800h	; ���� ⥪�⮢�� ����������
 ;mov ES, AX
 ;mov Ah, flag
 ;mov Al, 0fh
 ;mov ES:1,ax	; 0F- ���� �� ��୮� 䮭�, 07-ASCII ��� ᨬ����

 cmp flag,1
 je LoadNew
 cmp flag,2
 je LoadOld
 ; 1-� ࠧ - ��࠭塞 ���� ����, ��⮬ ���室�� �� LoadNew
 push DS
 mov AX,1130h
 mov BH, 01h 		; ����砥� ���� ��ண� ����
 int 10h
 ; ES:BP - ���� ��ண� ����
 ; ��� movsb ����: DS:SI-���� ���筨��
 push DS ES		; \ ���塞 ᮤ�ন���
 pop DS ES		; / DS � ES ���⠬�.
 mov SI,BP
 add SI,'�'*16		; ��砫�� ᨬ���
 mov DI,offset buffchars
 ; ES:DI-���� ��񬭨��
 cld			; DF:=0 (��������� �����)
 mov CX,16*5		; ������⢮ ����७��
 rep movsb		; oldchar:=���� ����
 pop DS			; ����⠭�������� DS
LoadNew: 
 ; ��㥬 ᨬ��� � ���孥� ����� 㣫� ��࠭�
 mov AX,0B800h	; ���� ⥪�⮢�� ����������
 mov ES, AX
 mov ES:0,0F07h	; 0F- ���� �� ��୮� 䮭�, 07-ASCII ��� ᨬ����

 mov AX, CS		; ES:=CS
 mov ES, AX
 
 mov AX, 1110h		; ����㧪� � ��⨢��� ����
 mov BH,16		; ���� ᨬ����
 mov BL,0			; ���� ������������
 mov CX,5			; ������⢮ ᨬ�����
 mov DX,'�'		; ASCII ��� ᨬ���� �
 mov BP, offset newchar	; ES:BP - ���� ������ ����
 int 10h
 mov flag,2	; ������騩 ࠧ ���� �㤥� ����㦠�� ���� ����
 jmp Fin
 
LoadOld: 
 ; ��㥬 ᨬ��� � ���孥� ����� 㣫� ��࠭�
 mov AX,0B800h	; ���� ⥪�⮢�� ����������
 mov ES, AX
 mov ES:0,0F09h	; 0F- ���� �� ��୮� 䮭�, 09-ASCII ��� ᨬ����

 mov AX, CS		; ES:=CS
 mov ES, AX
 
 mov AX, 1110h		; ����㧪� � ��⨢��� ����
 mov BH,16		; ���� ᨬ����
 mov BL,0			; ���� ������������
 mov CX,5			; ������⢮ ᨬ�����
 mov DX,'�'		; ASCII ��� ᨬ���� �
 mov BP, offset oldchars	; ES:BP - ���� ������ ����
 int 10h
 mov flag,1	; ������騩 ࠧ ���� �㤥� ����㦠�� ���� ����
 
Fin:
 pop ES
 popa			; ����⠭�������� ॣ�����
 ret			; ������
SwapChars endp

; +++++++ ����� १����⭮� ��� +++++++
 Old_2Fh dd 0
 Old_09h dd 0
 F6	equ 4000h		; ������ ������
 F5	equ 3F00h
 flagf5 db 0	; 1/0=>F5 �뫠/�� �뫠 �����
 flag db 0		; 0=>1-� �����; 1=>��㧨�� ���� ����; 2=>����                      
 buffchars db 80 dup(0)      	; ���� ��� ��ண� ����
oldchars db 00000000b
        db 00000000b
        db 00000000b
        db 00010000b
        db 00111000b
        db 01101100b
        db 11000110b
        db 11000110b
        db 11000110b
        db 11111110b
        db 11000110b
        db 11000110b
        db 11000110b
        db 00000000b
        db 00000000b
        db 00000000b
	; ?
        db 00000000b    
        db 00000000b
        db 00000000b
        db 11111110b
        db 01100110b
        db 01100010b
        db 01111100b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 11111100b
        db 00000000b
        db 00000000b
        db 00000000b
	; �
        db 00000000b    
        db 00000000b
        db 00000000b
        db 11111100b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01111100b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 11111100b
        db 00000000b
        db 00000000b
        db 00000000b
	; ?
        db 00000000b    
        db 00000000b
        db 00000000b
        db 11111110b
        db 01100110b
        db 01100010b
        db 01100000b
        db 01100000b
        db 01100000b
        db 01100000b
        db 01100000b
        db 01100000b
        db 11110000b
        db 00000000b
        db 00000000b
        db 00000000b
	; �
        db 00000000b    
        db 00000000b
        db 00000000b
        db 00011111b
        db 00110110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 11111111b
        db 11000011b
        db 00000000b
        db 00000000b
	; �
newchar db 00000000b	; ���� ����⠭�� �㪢 �����
        db 00000000b
        db 00000000b
        db 00010000b
        db 00111000b
        db 01101100b
        db 11000110b
        db 11000110b
        db 11000110b
        db 11111110b
        db 11000110b
        db 11000110b
        db 11000110b
        db 00000000b
        db 11111111b
        db 00000000b
	; ?
        db 00000000b    
        db 00000000b
        db 00000000b
        db 11111110b
        db 01100110b
        db 01100010b
        db 01111100b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 11111100b
        db 00000000b
        db 11111111b
        db 00000000b
	; �
        db 00000000b    
        db 00000000b
        db 00000000b
        db 11111100b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01111100b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 11111100b
        db 00000000b
        db 11111111b
        db 00000000b
	; ?
        db 00000000b    
        db 00000000b
        db 00000000b
        db 11111110b
        db 01100110b
        db 01100010b
        db 01100000b
        db 01100000b
        db 01100000b
        db 01100000b
        db 01100000b
        db 01100000b
        db 11110000b
        db 00000000b
        db 11111111b
        db 00000000b
	; �
        db 00000000b    
        db 00000000b
        db 00000000b
        db 00011111b
        db 00110110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 01100110b
        db 11111111b
        db 11000011b
        db 11111111b
        db 00000000b