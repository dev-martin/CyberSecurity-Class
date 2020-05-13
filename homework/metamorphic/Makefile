all: base morph1 morph2 morph3

base: base.s
	gcc $< -o $@

morph1: morph1.s
	gcc $< -o $@

morph2: morph2.s
	gcc $< -o $@

morph3: morph3.s
	gcc $< -o $@

base.s: base.c
	gcc -S $< -o $@

