; *************** Инициализация. Выполняется первой *****************
Init proc
; Сначала проверяем, есть ли параметры
 jmp CheckParams		; ####
NoParams: 
 ; Проверка: запущена ли уже программа
 mov AX, 0CD00h		; перехваченная функция
 int 2Fh	
 cmp AL,1			; AL=1, если прога запущена
 jne Work			; Нет копии-переходим, наконец, к работе
 jmp Try2Kill	; Уже есть копия => просто завершить прогу

Exit:
 mov AX, 4C00h
 int 21h
 ret 
Init endp

; ************************ Процедуры для Init ************************

; ************************ Проверка параметров ***********************
CheckParams proc
 mov AH, ES:[80h]		; Здесь хранится длина ком. строки
 cmp AH, 0
 je NoParams
; Если параметр выгрузки, пробуем выгрузить
 mov AH, ES:[83h]
 cmp AH, cParamLow
 je Try2Kill
 cmp AH, cParamUp
 je Try2Kill
; Параметр справки
 cmp AH, cParamHelp
 je Help
; Иначе-выдать сообщение и на выход
 mov DX, offset sWrongParam
 call PrintStr
 jmp Exit
CheckParams endp

; ************************ Вывод помощи ***********************
Help proc
 mov DX, offset sHelp
 call PrintStr
 jmp Exit
Help endp

; *************** Выгрузка первой копии программы *************
Try2Kill proc
 ; Проверка: запущена ли уже программа
 mov AX, 0CD00h		; перехваченная функция
 int 2Fh	
 cmp AL,1			; AL=1, если прога запущена
 je Kill			; загружена => выгружаем
 mov DX, offset sNotLoaded
 call PrintStr
 jmp Exit
Kill:
 ; Для выгрузки первой копии просто запускаем прерывание с функцией 1
 mov AH, 0CDh
 mov AL, 01h
 int 2Fh
 cmp AL,0Eh		; признак: невозможно выгрузить
 jne OK
 mov DX, offset sCantKill
 call PrintStr
 jmp Exit
OK:
 mov DX, offset sKilling
 call PrintStr
 jmp Exit
Try2Kill endp

; ************* Основная работа: установка векторов и т.д. ************
Work proc 
 ; Сохраняем старый вектор 2Fh
 mov AH, 35h
 mov AL, 2Fh
 int 21h
 mov word ptr Old_2Fh,BX
 mov word ptr Old_2Fh+2,ES
 ; Установим новый вектор 2Fh
 mov AH, 25h
 mov AL, 2Fh
 mov DX, offset New_2Fh
 int 21h
 ; Сохраняем старый вектор 09h
 mov AH, 35h
 mov AL, 09h
 int 21h
 mov word ptr Old_09h,BX	; Сохраняем IP
 mov word ptr Old_09h+2,ES	; Сохраняем CS
 ; Установим новый вектор 09h0
 mov AH, 25h
 mov AL, 09h
 mov DX, offset New_09h
 int 21h
 
 ; Завершим прогу, оставив часть резидентной
 mov DX, offset sLoaded
 call PrintStr
 mov DX, (TSRSize+10Fh)/16	; Размер резидентной части в параграфах
 mov AX, 3100h
 int 21h
Work endp

;  ***** Процедура вывода строки *****
PrintStr proc near
; Вход: DX-смещение строки  Выход: отсутствует
 mov AH, 09h		; функция вывода строки
 int 21h
 ret		
PrintStr endp

; ++++++++++ Данные для инициализации ++++++++++
sKilling db 'Выгружаю...Выгрузил!$'
sCantKill db 'Не могу выгрузиться! Кто-то сел на мой вектор прерывания !$'
sLoaded db 'Font Changer загружен.',10,13,'$'
sNotLoaded db 'Font Changer не загружен-нечего выгружать!$'
sAlreadyHere db 'Font Changer уже загружен-повторная загрузка невозможна.$'
sWrongParam db 'Неверный параметр.$'
sHelp db	'Вызов программы: Font.com [/u|U|?]',10,13,'Параметры u и U '
      db	'используются для выгрузки уже запущенной копии.',10,13
      db  'Параметр /? используется для получения этой справки.$'	
cParamHelp = '?'
cParamLow  = 'u'
cParamUp   = 'U'