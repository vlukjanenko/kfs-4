#!/bin/bash
qemu-system-i386 -s -S -m 2048 -drive file=kernel.img,format=raw & gdb majosue-kernel