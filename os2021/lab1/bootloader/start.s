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
	inb $0x92,%al
	orb $0x02,%al
	outb %al,$0x92 #A20
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
/*	pushl $13
	pushl $message
	calll displayStr
loop:
	jmp loop

message:
	.string "Hello, iWorld!\n\0"

displayStr:
	movl 4(%esp), %ebx
	movl 8(%esp), %ecx
	movl $((80*5+0)*2), %edi
	movb $0x0c, %ah
nextChar:
	movb (%ebx), %al
	movw %ax, %gs:(%edi)
	addl $2, %edi
	incl %ebx
	loopnz nextChar # loopnz decrease ecx by 1
	ret*/
.p2align 2
gdt: 
	.word 0x0000
	.word 0x0000
	.word 0x0000
	.word 0x0000
	 #empty
	
	.word 0xffff #15~0
	.word 0x0000 #32~16
	.byte 0x00 #39~32
	.byte 0x9a #47~40
	.word 0x00cf #63~48
	 #cs
	
	.word 0xffff #15~0 
	.word 0x0000 #32~16
	.byte 0x00 #39~32
	.byte 0x92 #47~40
	.word 0x00cf #63~48
	#ds

	.word 0xffff #15~0
	.word 0x8000 #32~16
	.byte 0x0b #39~32
	.byte 0x92 #47~40
	.word 0x00cf #63~48
	#tss
		
	#GDT definition here

gdtDesc: 
	.word (gdtDesc-gdt-1)
	.long gdt	
	#gdtDesc definition here
