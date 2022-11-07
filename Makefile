# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: majosue <majosue@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/12 21:25:37 by majosue           #+#    #+#              #
#    Updated: 2022/11/07 12:23:53 by majosue          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = majosue-kernel
CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -Wall -Wextra -Werror -fno-builtin \
-fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -MMD
SRC = kernel/kernel.c boot/boot.s lib/baselib.c kernel/screen.c \
lib/ports.s kernel/keyboard.c lib/printf.c
OBJECTS := $(SRC:%.c=%.o)
OBJECTS := $(OBJECTS:%.s=%.o)
DPS := $(OBJECTS:%.o=%.d)
NO_OF_FILES := $(words $(SRC))

define idx
$(shell
index=1;
for word in $(OBJECTS); do 
	if [ "$$word" = "$(1)" ]; then 
		echo $$index/$(NO_OF_FILES); 
		break; 
	fi; 
	index=$$((index+1)); 
	done)
endef

all: $(NAME)

$(NAME): $(OBJECTS) linker.ld
	@echo "Linking kernel to $(NAME)..."
	@$(CC) -T linker.ld $(OBJECTS) -o $(NAME) $(CFLAGS) -lgcc
	@echo "Compilation done for $(NAME)..."

%.o: %.s
	@echo "[$(call idx,$@)] Compiling (ASM)	$@..."
	@nasm -f elf32 $< -o $@

%.o: %.c
	@echo "[$(call idx,$@)] Compiling (C)	$@..."
	@$(CC) -c $< -o $@ $(CFLAGS) -I includes

-include $(DPS)

install: $(NAME)
	@./install.sh
	@./qemu.sh

clean:
	@echo "Cleaning objects..."
	@rm -f $(OBJECTS)
	@rm -f $(DPS)

fclean: clean
	@echo "Cleaning Kernel..."
	@rm -f kernel.img
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re install