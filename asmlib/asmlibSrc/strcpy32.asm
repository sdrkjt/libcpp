;*************************  strcpy32.asm  ************************************
; Author:           Agner Fog
; Date created:     2008-07-19
; Last modified:    2008-07-19
; Description:
; Faster version of the standard strcpy function:
; char * A_strcpy(char * dest, const char * src);
; Copies zero-terminated string from src to dest, including terminating zero.
;
; Overriding standard function memcpy:
; The alias ?OVR_strcpy is changed to _strcpy in the object file if
; it is desired to override the standard library function strcpy.
;
; Optimization:
; Uses optimized functions A_strlen and A_memcpy.
;
; Copyright (c) 2008 GNU General Public License www.gnu.org/licenses/gpl.html
;******************************************************************************
.686
.xmm
.model flat

public _A_strcpy                       ; Function _A_strcpy
public ?OVR_strcpy                     ; ?OVR removed if standard function memcpy overridden

; Imported from strlen32.asm
extern _A_strlen:near

; Imported from memcpy32.asm
extern _A_memcpy:near


.code

; extern "C" char * A_strcpy(char * dest, const char * src) {
;    return memcpy(dest, src, strlen(src)+1);
; }

; Function entry:
_A_strcpy PROC    NEAR
?OVR_strcpy LABEL NEAR

        mov     eax, [esp+8]           ; src
        push    eax
        call    _A_strlen              ; length of src
        pop     ecx                    ; ecx = src. Assume unchanged by _A_strlen
        inc     eax                    ; include terminating zero in length
        mov     edx, [esp+4]           ; dest
        push    eax                    ; length+1
        push    ecx                    ; src
        push    edx                    ; dest
        call    _A_memcpy              ; copy
        add     esp, 12                ; clean up stack
        ret

_A_strcpy ENDP

END
