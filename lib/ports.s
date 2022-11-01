; read byte from port
global inb:function
inb:
	mov dx, [esp+4]
	in al, dx
	ret
.end:

; write byte to port
global outb:function
outb:
	mov dx, [esp+4]
	mov al, [esp+8]
	out dx, al
	ret
.end:	