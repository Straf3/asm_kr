; *************** ���樠������. �믮������ ��ࢮ� *****************
Init proc
; ���砫� �஢��塞, ���� �� ��ࠬ����
 jmp CheckParams		; ####
NoParams: 
 ; �஢�ઠ: ����饭� �� 㦥 �ணࠬ��
 mov AX, 0CD00h		; ���墠祭��� �㭪��
 int 2Fh	
 cmp AL,1			; AL=1, �᫨ �ண� ����饭�
 jne Work			; ��� �����-���室��, �������, � ࠡ��
 jmp Try2Kill	; ��� ���� ����� => ���� �������� �ண�

Exit:
 mov AX, 4C00h
 int 21h
 ret 
Init endp

; ************************ ��楤��� ��� Init ************************

; ************************ �஢�ઠ ��ࠬ��஢ ***********************
CheckParams proc
 mov AH, ES:[80h]		; ����� �࠭���� ����� ���. ��ப�
 cmp AH, 0
 je NoParams
; �᫨ ��ࠬ��� ���㧪�, �஡㥬 ���㧨��
 mov AH, ES:[83h]
 cmp AH, cParamLow
 je Try2Kill
 cmp AH, cParamUp
 je Try2Kill
; ��ࠬ��� �ࠢ��
 cmp AH, cParamHelp
 je Help
; ����-�뤠�� ᮮ�饭�� � �� ��室
 mov DX, offset sWrongParam
 call PrintStr
 jmp Exit
CheckParams endp

; ************************ �뢮� ����� ***********************
Help proc
 mov DX, offset sHelp
 call PrintStr
 jmp Exit
Help endp

; *************** ���㧪� ��ࢮ� ����� �ணࠬ�� *************
Try2Kill proc
 ; �஢�ઠ: ����饭� �� 㦥 �ணࠬ��
 mov AX, 0CD00h		; ���墠祭��� �㭪��
 int 2Fh	
 cmp AL,1			; AL=1, �᫨ �ண� ����饭�
 je Kill			; ����㦥�� => ���㦠��
 mov DX, offset sNotLoaded
 call PrintStr
 jmp Exit
Kill:
 ; ��� ���㧪� ��ࢮ� ����� ���� ����᪠�� ���뢠��� � �㭪樥� 1
 mov AH, 0CDh
 mov AL, 01h
 int 2Fh
 cmp AL,0Eh		; �ਧ���: ���������� ���㧨��
 jne OK
 mov DX, offset sCantKill
 call PrintStr
 jmp Exit
OK:
 mov DX, offset sKilling
 call PrintStr
 jmp Exit
Try2Kill endp

; ************* �᭮���� ࠡ��: ��⠭���� ����஢ � �.�. ************
Work proc 
 ; ���࠭塞 ���� ����� 2Fh
 mov AH, 35h
 mov AL, 2Fh
 int 21h
 mov word ptr Old_2Fh,BX
 mov word ptr Old_2Fh+2,ES
 ; ��⠭���� ���� ����� 2Fh
 mov AH, 25h
 mov AL, 2Fh
 mov DX, offset New_2Fh
 int 21h
 ; ���࠭塞 ���� ����� 09h
 mov AH, 35h
 mov AL, 09h
 int 21h
 mov word ptr Old_09h,BX	; ���࠭塞 IP
 mov word ptr Old_09h+2,ES	; ���࠭塞 CS
 ; ��⠭���� ���� ����� 09h0
 mov AH, 25h
 mov AL, 09h
 mov DX, offset New_09h
 int 21h
 
 ; �����訬 �ண�, ��⠢�� ���� १����⭮�
 mov DX, offset sLoaded
 call PrintStr
 mov DX, (TSRSize+10Fh)/16	; ������ १����⭮� ��� � ��ࠣ���
 mov AX, 3100h
 int 21h
Work endp

;  ***** ��楤�� �뢮�� ��ப� *****
PrintStr proc near
; �室: DX-ᬥ饭�� ��ப�  ��室: ���������
 mov AH, 09h		; �㭪�� �뢮�� ��ப�
 int 21h
 ret		
PrintStr endp

; ++++++++++ ����� ��� ���樠����樨 ++++++++++
sKilling db '���㦠�...���㧨�!$'
sCantKill db '�� ���� ���㧨����! ��-� ᥫ �� ��� ����� ���뢠��� !$'
sLoaded db 'Font Changer ����㦥�.',10,13,'$'
sNotLoaded db 'Font Changer �� ����㦥�-��祣� ���㦠��!$'
sAlreadyHere db 'Font Changer 㦥 ����㦥�-����ୠ� ����㧪� ����������.$'
sWrongParam db '������ ��ࠬ���.$'
sHelp db	'�맮� �ணࠬ��: Font.com [/u|U|?]',10,13,'��ࠬ���� u � U '
      db	'�ᯮ������� ��� ���㧪� 㦥 ����饭��� �����.',10,13
      db  '��ࠬ��� /? �ᯮ������ ��� ����祭�� �⮩ �ࠢ��.$'	
cParamHelp = '?'
cParamLow  = 'u'
cParamUp   = 'U'