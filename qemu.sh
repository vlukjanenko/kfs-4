#!/bin/bash
qemu-system-i386 -m 2048 -drive file=kernel.img,format=raw -curses
