# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    ports.s                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/01 19:06:45 by majosue           #+#    #+#              #
#    Updated: 2022/11/01 19:06:59 by majosue          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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