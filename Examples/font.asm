; ??��?-�-�� ��R?����� �?���� ���� � ������ ���<?�
text segment 'code'
     assume CS:text,DS:text
     org 100h

.186
; *************** ?�-R�-�� ��R�?���� *****************
main proc far
 call Init	; �����?� �-���<����
 include TSR.inc	; ��<���?� �?��?-�-�? �R���R?����� � ��--�?
 iret
main endp

TSRSize = $-main ; ���?� �?��?-�-Rc �����

include Init.inc	; ��<���?� �-���<����R--�? �R���R?����� � ��--�?

text ends
end main