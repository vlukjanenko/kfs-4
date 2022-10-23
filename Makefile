# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: majosue <majosue@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/12 21:25:37 by majosue           #+#    #+#              #
#    Updated: 2022/10/23 22:34:11 by majosue          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = majosue-kernel
CC = i686-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -Wall -Wextra -Werror -fno-builtin \
-fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -MMD
LINKER = i686-elf-ld
SRC = kernel/kernel.c boot/boot.s lib/baselib.c
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

$(NAME): $(OBJECTS)
	@echo "Linking kernel to $(NAME)..."
	@$(LINKER) -T linker.ld $(OBJECTS) -o $(NAME)
	@echo "Compilation done for $(NAME)..."

%.o: %.s
	@echo "[$(call idx,$@)] Compiling (ASM)	$@..."
	@nasm -f elf32 $< -o $@

%.o: %.c
	@echo "[$(call idx,$@)] Compiling (C)	$@..."
	@$(CC) -c $< -o $@ $(CFLAGS) -I includes

-include $(DPS)

install: kernel.img $(NAME)
	$(shell loop=`sudo losetup --show -f -P kernel.img`; \
	sudo mount "$$loop"p1 lib; \
	sudo cp $(NAME) lib/boot; \
	sudo touch /lib/boot/grub/grub.cfg; \
	echo "menuentry \"majosue-kernel\" { multiboot /boot/majosue-kernel }" \
	| sudo tee lib/boot/grub/grub.cfg > /dev/null; \
	sudo umount lib; \
	sudo losetup -d $$loop)
	./qemu.sh

kernel.img:
	@cp clean.img kernel.img

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