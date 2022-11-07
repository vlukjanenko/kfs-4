#!/bin/bash
if [ ! -f "majosue-kernel" ]; then
    echo "majosue-kernel does not file exist, run make first"
    exit
fi
if [ ! -f "clean.img" ]; then
    echo "clean.img does not file exist, create clean.img first"
    exit
fi
if [  -e "tmp" ]; then
    sudo umount tmp;
    rm -rf tmp;
fi
cp clean.img kernel.img
mkdir tmp
loop=`sudo losetup --show -f -P kernel.img`
sudo mount "$loop"p1 tmp
sudo cp majosue-kernel tmp/boot 
echo "menuentry \"majosue-kernel\" { multiboot /boot/majosue-kernel }" \
| sudo tee tmp/boot/grub/grub.cfg > /dev/null 
sync
sudo umount tmp
sudo rm -rf tmp 
sudo losetup -d $loop