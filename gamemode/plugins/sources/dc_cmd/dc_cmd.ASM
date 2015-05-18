define WINDOWS 1
;define LINUX 1

if defined WINDOWS
    format pe gui 4.0 dll as 'dll'
    include 'win32a.inc'
    include 'macro/import32.inc'
end if
if defined LINUX
    format ELF as 'o'
end if

include 'macro/proc32.inc'
include 'include/macros.inc' ;for optimization of some instructions
include 'include/AMX_Functions.inc'
include 'include/AMX_Constants.inc'

; all-in-one section
if defined WINDOWS
    section '' writable readable executable discardable
end if
if defined LINUX
    section '' executable writeable
end if

proc Supports
    mov     eax, SUPPORTS_VERSION+SUPPORTS_AMX_NATIVES
    retn
endp

;align 10h

proc _Load
.AMX = 4
    mov     edx, [esp+.AMX]
    mov     eax, [edx+40h]
    mov     ecx, [edx]
    mov     [pAMXFunctions],eax;get pointers to abstract machine functions
    mov     [logprintf], ecx   ;and pointer to logprintf()
    ; print loading message
    push    msg_load1
    call    [logprintf]
    mov     [esp], dword msg_load2
    call    [logprintf]
    add     esp, 4
    mov     eax, 1
if defined LINUX
    retn
else
    retn    4
end if
endp

;align 10h

proc Unload
    push    msg_unload
    call    [logprintf]
    add     esp, 4
    retn
endp

;align 10h

proc AmxLoad
.AMX = 4
    mov     ecx, [esp+.AMX]
    push    -1
    mov     eax, [pAMXFunctions]
    push    AMX_Natives
    mov     eax, [eax+amx_Register]
    push    ecx
    call    eax
    add     esp, 12
    movntq  eax, AMX_ERR_NONE ; will be translated into "xor eax, eax"
if defined LINUX
    retn
else
    retn    4
end if
endp

;align 10h

proc AmxUnload
.AMX = 4
    movntq  eax, AMX_ERR_NONE ; xor eax, eax
if defined LINUX
    retn
else
    retn    4
end if
endp


;align 10h

proc amx_DC_CMD
.AMX		 = 8
.params 	 = 12
.addr_cmdtext	 = -4
.len		 = -8
.params_pos	 = -12
.pubidx 	 = -16
.addr		 = -4 ; reusing space from .addr_cmdtext
.retval 	 = -8 ; reusing space from .len
    ; standard prologue code
    push    ebp
    mov     ebp, esp
    sub     esp, 20

    ; get the cmdtext[] address
    lea     eax, [ebp+.addr_cmdtext]
    mov     edx, [ebp+.params]
    push    eax
    mov     edx, [edx+8]
    mov     ecx, [ebp+.AMX]
    push    edx
    mov     eax, [pAMXFunctions]
    push    ecx
    mov     eax, [eax+amx_GetAddr]
    call    eax
    add     esp, 12

    ; get length of cmdtext[]
    lea     ecx, [ebp+.len]
    mov     edx, [ebp+.addr_cmdtext]
    push    ecx
    mov     eax, [pAMXFunctions]
    push    edx
    mov     eax, [eax+amx_StrLen]
    call    eax
    add     esp, 8

    ; debug output
    ;mov     eax, [ebp+.len]
    ;push    eax
    ;mov     edx, [ebp+.addr_cmdtext]
    ;push    edx
    ;push    str_dccmd1
    ;call    [logprintf]
    ;add     esp, 12

    ; retrieve cmdtext length in eax
    mov     eax, [ebp+.len]
    ; if length is 0 - exit (already fixed in 0.3x)
    ; comment by eider: better leave it here in case of problems with kye's fix!
    or	    eax, eax
    jz	    .return1

    ; increase length for adding '\0' (+1 byte)
    ; and align value to 4 before reserving stack space
    add     eax, 1+4
    and     eax, 1111111111111100b
    ; reserve stack space for string
    sub     esp, eax

    ; cmdname = "cmd"
    ; get string from abstract machine
    mov     ecx, esp
    sub     esp, 16
    mov     eax, [pAMXFunctions]
    mov     edx, [ebp+.addr_cmdtext]
    mov     [esp], ecx
    mov     eax, [eax+amx_GetString]
    mov     ecx, [ebp+.len]
    mov     [esp+4], edx
    inc     ecx
    mov     [esp+8], dword 0
    mov     [esp+12], ecx
    call    eax
    add     esp, 16

    ; debug output
    ;push    esp
    ;push    str_dccmd2
    ;call    [logprintf]
    ;add     esp, 8

    ; cmdname[3] = '_'
    ; comment by eider: you can change it to anything you want, just remember it will change public's name! (eg. from _help to cmd_help)
    mov     [esp], byte '_'

    mov     ecx, esp
    xor     eax, eax
.loop1:
    inc     ecx
    mov     al, byte [ecx]
    or	    eax, eax
    jz	    .noparams1
    cmp     eax, ' '
    jle     .loop1_exit_has_params
    cmp     eax, 'A'
    jl	    .loop1
    cmp     eax, 'Z'
    jg	    .loop1
    add     byte [ecx], 'a'-'A'
    jmp     .loop1
.loop1_exit_has_params:
    mov     [ecx], byte 0	   ; replace first ' ' character by '\0'
    mov     [ebp+.params_pos], ecx ; save position of first ' ' character
.loop1_exit:

    ; get index of command's public function
    lea     eax, [ebp+.pubidx]
    mov     edx, esp
    push    eax
    mov     ecx, [ebp+.AMX]
    push    edx
    mov     eax, [pAMXFunctions]
    push    ecx
    mov     eax, [eax+amx_FindPublic]
    call    eax
    add     esp, 12

    ; if public isn't found
    mov     eax, [ebp+.pubidx]
    cmp     eax, 07FFFFFFFh
    je	    .return0

    ; debug output
    ;mov     eax, [ebp+.pubidx]
    ;push    eax
    ;push    str_dccmd4
    ;call    [logprintf]
    ;add     esp, 8

    mov     ecx, [ebp+.params_pos]
    cmp     ecx, str_params_empty ; if params string is empty -
    je	    .invoke_cmd 	  ; skip processing
    inc     ecx 		  ; else - search for parameters
.loop2:
    mov     al, byte [ecx]
    inc     ecx
    or	    eax, eax
    jz	    .loop2_exit
    cmp     eax, 32
    jle     .loop2
.loop2_exit:
    dec     ecx
    mov     [ebp+.params_pos], ecx

.invoke_cmd:
    ; debug output
    ;mov     edx, esp
    ;mov     ecx, [ebp+.params_pos]
    ;push    ecx
    ;push    edx
    ;push    str_dccmd3
    ;call    [logprintf]
    ;add     esp, 12

    ; pushing params string
    mov     edx, [ebp+.params_pos]
    sub     esp, 24
    mov     eax, [pAMXFunctions]
    mov     [esp+20], dword 0
    lea     ecx, [ebp+.addr]
    mov     [esp+16], dword 0
    mov     eax, [eax+amx_PushString]
    mov     [esp+12], edx
    mov     [esp+8], dword 0
    mov     edx, [ebp+.AMX]
    mov     [esp+4], ecx
    mov     [esp], edx
    call    eax
    add     esp, 24

    ; pushing playerid
    mov     ecx, [ebp+.params]
    mov     eax, [pAMXFunctions]
    mov     ecx, [ecx+4]
    mov     edx, [ebp+.AMX]
    push    ecx
    mov     eax, [eax+amx_Push]
    push    edx
    call    eax
    add     esp, 8

    ; calling public function
    mov     ecx, [ebp+.pubidx]
    lea     edx, [ebp+.retval]
    push    ecx
    mov     eax, [pAMXFunctions]
    push    edx
    mov     ecx, [ebp+.AMX]
    mov     eax, [eax+amx_Exec]
    push    ecx
    call    eax
    add     esp, 12

    ; releasing memory allocated after pushing params string
    mov     edx, [ebp+.addr]
    mov     eax, [pAMXFunctions]
    push    edx
    mov     ecx, [ebp+.AMX]
    mov     eax, [eax+amx_Release]
    push    ecx
    call    eax
    add     esp, 8

    ; return code from public function
    mov eax, [ebp+.retval]
    cmp eax, 0
    jz .return0

.return1:
    mov     eax, 1
.return:
    mov     esp, ebp
    pop     ebp
    retn
.return0:
    xor     eax, eax
    jmp     .return
.noparams1:
    mov     [ebp+.params_pos], dword str_params_empty
    jmp     .loop1_exit
endp

;section '.data' data readable writable

; strings for debug output
;str_dccmd1	 db	 'cmdtextaddr = 0x%X, len = %d', 0
;str_dccmd2	 db	 'In: cmdtext = "%s"', 0
;str_dccmd3	 db	 'Out: cmd = "%s", params = "%s"', 0
;str_dccmd4	 db	 'pubidx = %d', 0

logprintf	    dd	0
pAMXFunctions	    dd	0

msg_load1	    db	'  Daniel''s CMD plugin v1.1 modified by FullServer.eu', 0
msg_unload	    db	'  Daniel''s CMD plugin unloaded', 0
str_params_empty    db	0


name_DC_CMD	    db "DC_CMD", 0

AMX_Natives:
    dd	name_DC_CMD,	amx_DC_CMD
    dd	0, 0


;align 10h

if defined WINDOWS
    ;section '.edata' export data readable
    data export
	export '',\
		_Load, 'Load',\
		Unload, 'Unload',\
		AmxLoad, 'AmxLoad',\
		AmxUnload, 'AmxUnload',\
		Supports, 'Supports'
    end data
    ;section '.reloc' fixups data discardable
    data fixups
    end data
end if
if defined LINUX
    public Supports
    public _Load as 'Load'
    public Unload
    public AmxLoad
    public AmxUnload
end if

db 13, 10, 13, 10
db 'Feel the power of ASSembler', 0 ;I have no idea what it does here, lol

db 13, 10
msg_load2 db '  (c) 2012-2013, Daniel_Cortez, (c) 2014-2015, FullServer.eu', 0
db 13, 10, 13, 10