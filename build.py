import os, glob, sys
from pathlib import Path

def run(command, can_fail=False):
	print(command)
	code = os.system(command)
	if code != 0 and not can_fail:
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

compile_flags = "-std=c99 -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-builtin"
link_flags = f"-T {linker_script} -ffreestanding -O2 -nostdlib"

user_compile_flags = "-std=c99 -O2 -Wall -Wextra -nostdlib -nostdinc -fno-builtin \
	-fno-stack-protector -nodefaultlibs"
user_link_flags = "-T linker.ld -O2 -nostdlib"

obj_dir = "obj/"
kernel_dir = "kernel/"
libc_dir = "libc/"
asm_dir = f"{kernel_dir}asm/"

initrd_dir = "initrd/"
init_dir = f"{initrd_dir}test_src/"
iso_dir = "iso/"
build_dir = "bin/"

bin_file = f"{build_dir}tsukios.bin"
iso_file = f"{build_dir}tsukios.iso"

qemu_flags = "-s"

def assemble():
	for file in glob.glob(asm_dir + "**/*.asm", recursive=True):
		run(f"{assembler} {assembler_flags} {file} -o {obj_dir}{Path(file).stem + '_asm.o'}")

def compile_libc():
	for file in glob.glob(libc_dir + "**/*.c", recursive=True):
		run(f"{compiler} {compile_flags} -c {file} -I{libc_dir}include -o {obj_dir}{Path(file).stem + '_libc.o'} {compiler_flags}")

def compile_kernel():
	for file in glob.glob(kernel_dir + "**/*.c", recursive=True):
		run(f"{compiler} {compile_flags} -c {file} -I{libc_dir}include -I{kernel_dir}include -o {obj_dir}{Path(file).stem + '.o'} {compiler_flags}")

def link():
	obj_files = glob.glob(obj_dir + "**/*.o", recursive=True)
	run(f"{compiler} {link_flags} {' '.join(list(obj_files))} -o {bin_file} {compiler_flags}")

def initrd():
	run(f"cd {init_dir}; {assembler} {assembler_flags} start.asm -o start.o")
	run(f"cd {init_dir}; {compiler} {user_compile_flags} -c main.c -I../../{libc_dir}include -o main.o {compiler_flags}")
	run(f"cd {init_dir}; {compiler} {user_link_flags} *.o ../../{obj_dir}*_libc.o -o test {compiler_flags}")
	run(f"tar -cvf {iso_dir}boot/tsukios.initrd {initrd_dir}*")

def make_iso():
	run(f"cp {bin_file} {iso_dir}boot/")
	run(f"grub-mkrescue -o {iso_file} {iso_dir}")

def all():
	assemble()
	compile_libc()
	compile_kernel()
	link()
	initrd()
	make_iso()

def clean():
	run(f"rm {bin_file}", can_fail=True)
	run(f"rm {iso_file}", can_fail=True)
	obj_files = glob.glob(obj_dir + "**/*.o", recursive=True)
	run(f"rm {' '.join(list(obj_files))}", can_fail=True)
	run(f"rm {init_dir}*.o", can_fail=True)

def qemu():
	run(f"qemu-system-i386 -cdrom {iso_file} {qemu_flags}")

target_map = {
	"assemble": assemble,
	"libc": compile_libc,
	"kernel": compile_kernel,
	"link": link,
	"initrd": initrd,
	"iso": make_iso,
	"all": all,
	"clean": clean,
	"qemu": qemu,
}

if __name__ == "__main__":
	print()
	print(" *** TsukiOS Builder ***")
	print()


	if len(sys.argv) == 1:
		print(f"  Usage: {sys.argv[0]} <target> [<target>...]")
		print()
		print("  Available Targets:")
		print(f"    {', '.join(list(target_map.keys()))}")
		print()
		sys.exit(1)

	targets = [target.strip().lower() for target in sys.argv[1:]]

	print("  Checking targets...")
	print()

	for target in targets:
		keys = [*target_map]
		if not target in keys:
			print(f"  Unknown target '{target}', exiting")			
			print()
			sys.exit(1)

	for target in targets:
		target_map[target]()

	print()
