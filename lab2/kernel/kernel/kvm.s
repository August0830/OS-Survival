
kvm.o：     文件格式 elf32-i386


Disassembly of section .text:

00000000 <initSeg>:
   0:	55                   	push   %ebp
   1:	89 e5                	mov    %esp,%ebp
   3:	ba 00 00 00 00       	mov    $0x0,%edx
   8:	66 c7 05 08 00 00 00 	movw   $0xffff,0x8
   f:	ff ff 
  11:	66 c7 05 0a 00 00 00 	movw   $0x0,0xa
  18:	00 00 
  1a:	c6 05 0c 00 00 00 00 	movb   $0x0,0xc
  21:	c6 05 0d 00 00 00 9a 	movb   $0x9a,0xd
  28:	c6 05 0e 00 00 00 cf 	movb   $0xcf,0xe
  2f:	c6 05 0f 00 00 00 00 	movb   $0x0,0xf
  36:	66 c7 05 10 00 00 00 	movw   $0xffff,0x10
  3d:	ff ff 
  3f:	66 c7 05 12 00 00 00 	movw   $0x0,0x12
  46:	00 00 
  48:	c6 05 14 00 00 00 00 	movb   $0x0,0x14
  4f:	c6 05 15 00 00 00 92 	movb   $0x92,0x15
  56:	c6 05 16 00 00 00 cf 	movb   $0xcf,0x16
  5d:	c6 05 17 00 00 00 00 	movb   $0x0,0x17
  64:	66 c7 05 18 00 00 00 	movw   $0xff,0x18
  6b:	ff 00 
  6d:	66 c7 05 1a 00 00 00 	movw   $0x0,0x1a
  74:	00 00 
  76:	c6 05 1c 00 00 00 20 	movb   $0x20,0x1c
  7d:	c6 05 1d 00 00 00 fa 	movb   $0xfa,0x1d
  84:	c6 05 1e 00 00 00 c0 	movb   $0xc0,0x1e
  8b:	c6 05 1f 00 00 00 00 	movb   $0x0,0x1f
  92:	66 c7 05 20 00 00 00 	movw   $0xff,0x20
  99:	ff 00 
  9b:	66 c7 05 22 00 00 00 	movw   $0x0,0x22
  a2:	00 00 
  a4:	c6 05 24 00 00 00 20 	movb   $0x20,0x24
  ab:	c6 05 25 00 00 00 f2 	movb   $0xf2,0x25
  b2:	c6 05 26 00 00 00 c0 	movb   $0xc0,0x26
  b9:	c6 05 27 00 00 00 00 	movb   $0x0,0x27
  c0:	66 c7 05 28 00 00 00 	movw   $0x63,0x28
  c7:	63 00 
  c9:	b8 00 00 00 00       	mov    $0x0,%eax
  ce:	66 a3 2a 00 00 00    	mov    %ax,0x2a
  d4:	89 c1                	mov    %eax,%ecx
  d6:	c1 e9 10             	shr    $0x10,%ecx
  d9:	88 0d 2c 00 00 00    	mov    %cl,0x2c
  df:	c6 05 2d 00 00 00 89 	movb   $0x89,0x2d
  e6:	c6 05 2e 00 00 00 40 	movb   $0x40,0x2e
  ed:	c1 e8 18             	shr    $0x18,%eax
  f0:	a2 2f 00 00 00       	mov    %al,0x2f
  f5:	66 c7 05 00 00 00 00 	movw   $0x37,0x0
  fc:	37 00 
  fe:	66 89 15 02 00 00 00 	mov    %dx,0x2
 105:	c1 ea 10             	shr    $0x10,%edx
 108:	66 89 15 04 00 00 00 	mov    %dx,0x4
 10f:	b8 00 00 00 00       	mov    $0x0,%eax
 114:	0f 01 10             	lgdtl  (%eax)
 117:	c7 05 04 00 00 00 ff 	movl   $0x1fffff,0x4
 11e:	ff 1f 00 
 121:	c7 05 08 00 00 00 10 	movl   $0x10,0x8
 128:	00 00 00 
 12b:	b8 28 00 00 00       	mov    $0x28,%eax
 130:	0f 00 d8             	ltr    %ax
 133:	b8 10 00 00 00       	mov    $0x10,%eax
 138:	8e d8                	mov    %eax,%ds
 13a:	8e d0                	mov    %eax,%ss
 13c:	31 c0                	xor    %eax,%eax
 13e:	0f 00 d0             	lldt   %ax
 141:	5d                   	pop    %ebp
 142:	c3                   	ret    
 143:	90                   	nop

00000144 <enterUserSpace>:
 144:	55                   	push   %ebp
 145:	89 e5                	mov    %esp,%ebp
 147:	b8 23 00 00 00       	mov    $0x23,%eax
 14c:	50                   	push   %eax
 14d:	b8 ff ff 2f 00       	mov    $0x2fffff,%eax
 152:	50                   	push   %eax
 153:	9c                   	pushf  
 154:	58                   	pop    %eax
 155:	80 cc 02             	or     $0x2,%ah
 158:	50                   	push   %eax
 159:	b8 1b 00 00 00       	mov    $0x1b,%eax
 15e:	50                   	push   %eax
 15f:	8b 45 08             	mov    0x8(%ebp),%eax
 162:	50                   	push   %eax
 163:	cf                   	iret   
 164:	5d                   	pop    %ebp
 165:	c3                   	ret    
 166:	66 90                	xchg   %ax,%ax

00000168 <loadUMain>:
 168:	55                   	push   %ebp
 169:	89 e5                	mov    %esp,%ebp
 16b:	56                   	push   %esi
 16c:	53                   	push   %ebx
 16d:	bb c9 00 00 00       	mov    $0xc9,%ebx
 172:	be 00 00 20 00       	mov    $0x200000,%esi
 177:	90                   	nop
 178:	83 ec 08             	sub    $0x8,%esp
 17b:	53                   	push   %ebx
 17c:	56                   	push   %esi
 17d:	e8 fc ff ff ff       	call   17e <loadUMain+0x16>
 182:	81 c6 00 02 00 00    	add    $0x200,%esi
 188:	43                   	inc    %ebx
 189:	83 c4 10             	add    $0x10,%esp
 18c:	81 fb 91 01 00 00    	cmp    $0x191,%ebx
 192:	75 e4                	jne    178 <loadUMain+0x10>
 194:	8b 0d 18 00 20 00    	mov    0x200018,%ecx
 19a:	a1 1c 00 20 00       	mov    0x20001c,%eax
 19f:	8b 90 04 00 20 00    	mov    0x200004(%eax),%edx
 1a5:	31 c0                	xor    %eax,%eax
 1a7:	90                   	nop
 1a8:	8a 9c 10 00 00 20 00 	mov    0x200000(%eax,%edx,1),%bl
 1af:	88 98 00 00 20 00    	mov    %bl,0x200000(%eax)
 1b5:	40                   	inc    %eax
 1b6:	3d 00 90 01 00       	cmp    $0x19000,%eax
 1bb:	75 eb                	jne    1a8 <loadUMain+0x40>
 1bd:	b8 23 00 00 00       	mov    $0x23,%eax
 1c2:	50                   	push   %eax
 1c3:	b8 ff ff 2f 00       	mov    $0x2fffff,%eax
 1c8:	50                   	push   %eax
 1c9:	9c                   	pushf  
 1ca:	58                   	pop    %eax
 1cb:	80 cc 02             	or     $0x2,%ah
 1ce:	50                   	push   %eax
 1cf:	b8 1b 00 00 00       	mov    $0x1b,%eax
 1d4:	50                   	push   %eax
 1d5:	51                   	push   %ecx
 1d6:	cf                   	iret   
 1d7:	8d 65 f8             	lea    -0x8(%ebp),%esp
 1da:	5b                   	pop    %ebx
 1db:	5e                   	pop    %esi
 1dc:	5d                   	pop    %ebp
 1dd:	c3                   	ret    
