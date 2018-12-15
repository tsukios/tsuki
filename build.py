import os, glob, sys
from pathlib import Path

def run(command):
	print(command)
	code = os.system(command)
	if code != 0:
		print()
		print()
		print(f" !!! COMMAND EXITED WITH CODE {code}, ABORTING !!! ")
		print()
		print()
		sys.exit(1)

linker_script = "linker.ld"

assembler = "yasm"
assembler_flags = "-felf32"

compiler = "clang"
compiler_flags = "--target=i686-elf -march=i686"

compile_flags = "-std=gnu99 -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-builtin"
link_flags = f"-T {linker_script} -ffreestanding -O2 -nostdlib -lgcc"

obj_dir = "obj/"
kernel_dir = "kernel/"
libc_dir = "libc/"
asm_dir = f"{kernel_dir}asm/"

obj_files = glob.iglob(obj_dir + "**/*.o", recursive=True)
kernel_files = glob.iglob(kernel_dir + "**/*.c", recursive=True)
libc_files = glob.iglob(libc_dir + "**/*.c", recursive=True)
asm_files = glob.iglob(asm_dir + "**/*.asm", recursive=True)

iso_dir = "iso/"
build_dir = "build/"

bin_file = f"{build_dir}tsukios.bin"
iso_file = f"{build_dir}tsukios.iso"

def assemble():
	for file in asm_files:
		run(f"{assembler} {assembler_flags} {file} -o {obj_dir}{Path(file).stem + '.o'}")

def compile_libc():
	for file in libc_files:
		run(f"{compiler} {compile_flags} -c {file} -I{libc_dir}include -o {obj_dir}{Path(file).stem + '.o'} {compiler_flags}")

def compile_kernel():
	for file in kernel_files:
		run(f"{compiler} {compile_flags} -c {file} -I{libc_dir}include -I{kernel_dir}include -o {obj_dir}{Path(file).stem + '.o'} {compiler_flags}")

def link():
	run(f"{compiler} {link_flags} {' '.join(list(obj_files))} -o {bin_file} {compiler_flags}")

def make_iso():
	run(f"cp {bin_file} {iso_dir}boot/")
	run(f"grub-mkrescue -o {iso_file} {iso_dir}")

def all():
	assemble()
	compile_libc()
	compile_kernel()
	link()
	make_iso()

def clean():
	run(f"rm {bin_file}")
	run(f"rm {iso_file}")
	run(f"rm {' '.join(list(obj_files))}")

def qemu():
	run(f"qemu-system-i386 -cdrom {iso_file}")

if __name__ == "__main__":
	print()
	print(" *** TsukiOS Builder ***")
	print()


	if len(sys.argv) != 2:
		print(f" Usage: {sys.argv[0]} <target>")
		print()
		print(" Available Targets:")
		print("   assemble, kernel, link, iso, all, clean, qemu")
		print()
		sys.exit(1)

	target = sys.argv[1].strip().lower()

	if target == "assemble":
		assemble()
	elif target == "libc":
		compile_libc()
	elif target == "kernel":
		compile_kernel()
	elif target == "link":
		link()
	elif target == "iso":
		make_iso()
	elif target == "all":
		all()
	elif target == "clean":
		clean()
	elif target == "qemu":
		all()
		qemu()
	else:
		print("Unknown target, exiting")
		print()
		sys.exit(1)

	print()
