; ??ðý?-¢-ÿ¯ ô R? ÿ¿¿ÿ ÿ? £òð ¨ ð¤¢ÿ ÷ ôÿ¿¯¢¬ ýð¡ô<?¯
text segment 'code'
     assume CS:text,DS:text
     org 100h

.186
; *************** ?¡-R÷-ÿ¯ ô R¦?ý£ ÿ *****************
main proc far
 call Init	; ÷««÷ÿ?¿ ð-ð¦ðÿ<ðÿ¦ð®
 include TSR.inc	; ÷ò<®§ÿ?¿  ?ðý?-¢-«? ôRýô R? ÿ¿¿« ð ýÿ--«?
 iret
main endp

TSRSize = $-main ;  ÿ¿?   ?ðý?-¢-Rc §ÿ¡¢ð

include Init.inc	; ÷ò<®§ÿ?¿ ð-ð¦ðÿ<ðÿ¦ðR--«? ôRýô R? ÿ¿¿« ð ýÿ--«?

text ends
end main