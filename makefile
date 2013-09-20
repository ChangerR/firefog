all:firefog.img

firefog.img:bootsect.bin	setup.bin	kernel.bin
	MakeImg	bootsect.bin	setup.bin	kernel.bin	firefog.img
	
bootsect.bin:bootsect.asm
	nasm	-o bootsect.bin	-fbin	bootsect.asm

setup.bin:setup.asm
	nasm	-o	setup.bin	-fbin	setup.asm
	
kernel.bin:
	cd kernel&&nmake /NOLOGO	-f	kernel.mk
	cd kernel&&copy	kernel.bin ..
clean:
	del	*.bin
	del	firefog.img
	(cd	kernel&&nmake	/NOLOGO	-f	 kernel.mk clean)