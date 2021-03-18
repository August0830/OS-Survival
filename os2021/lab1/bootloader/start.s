/* Real Mode Hello World */
#.code16
#
#.global start
#start:
#	movw %cs, %ax
#	movw %ax, %ds
#	movw %ax, %es
#	movw %ax, %ss
#	movw $0x7d00, %ax
#	movw %ax, %sp # setting stack pointer to 0x7d00

#loop:
#	jmp loop


/* Protected Mode Hello World */
#.code16
#
#.global start
#start:
#	movw %cs, %ax
#	movw %ax, %ds
#	movw %ax, %es
#	movw %ax, %ss
#
#.code32
#start32:
#	movw $0x10, %ax # setting data segment selector
#	movw %ax, %ds
#	movw %ax, %es
#	movw %ax, %fs
#	movw %ax, %ss
#loop32:
#	jmp loop32
#
#
#.p2align 2


/* Protected Mode Loading Hello World APP */
.code16

.global start
start:
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	cli
	in $0x92,%al
	or $0x02,%al
	out %al,$0x92 #A20
	lgdt gdtDesc #GDTR TODO: Protected Mode Here
	movl %cr0,%eax
	orl  $0x1,%eax
	movl %eax,%cr0 #Protected Mode
	ljmp $0x08,$start32

.code32
start32:
	movw $0x10, %ax # setting data segment selector
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %ss
	movw $0x18, %ax # setting graphics data segment selector
	movw %ax, %gs
	
	movl $0x8000, %eax # setting esp
	movl %eax, %esp
	jmp bootMain # jump to bootMain in boot.c

.p2align 2
gdt: 
	.word $0x0000
	.word $0x0000
	.word $0x0000
	.word $0x0000
	.byte $0x00  #empty
	
	.word $0x0000
	.word $0x0000 
	.word $0x03ff
	.word $0xffff
	.byte $0x9a #cs
	
	.word $0x0000
	.word $0x0000 
	.word $0x03ff
	.word $0xffff
	.byte $0x92 #ds

	.word $0x000b
	.word $0x8000
	.word $0x03ff
	.word $0xffff
	.byte $0x89 #tss
		
	#GDT definition here

gdtDesc: 
	.word (gdtDesc-gdt-1)
	.long gdt	
	#gdtDesc definition here
