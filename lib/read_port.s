global read_port:function
read_port:
	mov dx, [esp+4]
	in al, dx
	ret
.end:

global write_port:function
write_port:
	mov dx, [esp+4]
	mov al, [esp+8]
	out dx, al
	ret
.end:	