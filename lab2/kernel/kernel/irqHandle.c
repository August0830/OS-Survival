#include "x86.h"
#include "device.h"

extern int displayRow;
extern int displayCol;

extern uint32_t keyBuffer[MAX_KEYBUFFER_SIZE];
extern int bufferHead;
extern int bufferTail;


void GProtectFaultHandle(struct TrapFrame *tf);

void KeyboardHandle(struct TrapFrame *tf);

void syscallHandle(struct TrapFrame *tf);
void syscallWrite(struct TrapFrame *tf);
void syscallPrint(struct TrapFrame *tf);
void syscallRead(struct TrapFrame *tf);
void syscallGetChar(struct TrapFrame *tf);
void syscallGetStr(struct TrapFrame *tf);


void irqHandle(struct TrapFrame *tf) { // pointer tf = esp
	/*
	 * 中断处理程序
	 */
	/* Reassign segment register */
	asm volatile("movw %%ax, %%ds"::"a"(KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax, %%es"::"a"(KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax, %%fs"::"a"(KSEL(SEG_KDATA)));
	//asm volatile("movw %%ax, %%gs"::"a"(KSEL(SEG_KDATA)));
	uint32_t addr=0;
	switch(tf->irq) {
		//填好中断处理程序的调用
		case -1:break;
		case 0xd: GProtectFaultHandle(tf);break;
		case 0x80: syscallHandle(tf);break;
		default:assert(0);
	}
	
}

void GProtectFaultHandle(struct TrapFrame *tf){
	assert(0);
	return;
}

void KeyboardHandle(struct TrapFrame *tf){
	uint32_t code = getKeyCode();
	if(code == 0xe){ // 退格符
		//要求只能退格用户键盘输入的字符串，且最多退到当行行首
		if(displayCol!=0)
		{
			displayCol--;
			bufferTail--;
			if(bufferTail<bufferHead)
				bufferTail=bufferHead;
		}
	}else if(code == 0x1c){ // 回车符
		//处理回车情况
		displayRow++;
		displayCol=0;
		if(displayRow>=25)
		{
			displayRow=24;
			scrollScreen();
		}
		if(bufferTail<MAX_KEYBUFFER_SIZE)
		{
			bufferTail++;
			keyBuffer[bufferTail-1]='\n';		
		}	
	}else if(code < 0x81){ // 正常字符
		//注意输入的大小写的实现、不可打印字符的处理
		if(bufferTail<MAX_KEYBUFFER_SIZE)
		{
			bufferTail++;
			keyBuffer[bufferTail-1]=getChar(code);
			displayCol++;
			if(displayCol>=80)
			{
				displayCol=0;
				displayRow++;
				if(displayRow>=25)
				{
					displayRow=24;
					displaycol=0;
					scrollScreen();
				}
			}
		}
	}
	syscall(SYS_WRITE, STD_OUT, (uint32_t)keyBuffer, (uint32_t)MAX_KEYBUFFER_SIZE, (uint32_t)bufferTail, 0);
	updateCursor(displayRow, displayCol);
}

void syscallHandle(struct TrapFrame *tf) {
	switch(tf->eax) { // syscall number
		case 0:
			syscallWrite(tf);
			break; // for SYS_WRITE
		case 1:
			syscallRead(tf);
			break; // for SYS_READ
		default:break;
	}
}

void syscallWrite(struct TrapFrame *tf) {
	switch(tf->ecx) { // file descriptor
		case 0:
			syscallPrint(tf);
			break; // for STD_OUT
		default:break;
	}
}

void syscallPrint(struct TrapFrame *tf) {
	int sel = SEG_UDATA; // segment selector for user data, need further modification
	char *str = (char*)tf->edx;
	int size = tf->ebx;
	int i = 0;
	int pos = 0;
	char character = 0;
	uint16_t data = 0;
	asm volatile("movw %0, %%es"::"m"(sel));
	for (i = 0; i < size; i++) {
		asm volatile("movb %%es:(%1), %0":"=r"(character):"r"(str+i));
		if(chracter!='\n')
		{
			data = character | (0x0c << 8);
			pos = (80*displayRow+displayCol)*2;
			asm volatile("movw %0, (%1)"::"r"(data),"r"(pos+0xb8000));//print a character
			displayCol++;
			if(displayCol>=80)
			{
				displayCol=0;
				displayRow++;
			}
			if(displayRow>=25)
			{
				displayRow=24;
				displayCol=0;
				scrollScreen();
			}
		}
		else
		{
			displayCol=0;
			displayRow++;
			if(displayRow>=25)
			{
				displayRow=24;
				displayCol=0;
				scrollScreen();
			}
		}				
		//完成光标的维护和打印到显存
	}
	
	updateCursor(displayRow, displayCol);
}

void syscallRead(struct TrapFrame *tf){
	switch(tf->ecx){ //file descriptor
		case 0:
			syscallGetChar(tf);
			break; // for STD_IN
		case 1:
			syscallGetStr(tf);
			break; // for STD_STR
		default:break;
	}
}

void syscallGetChar(struct TrapFrame *tf){
	// TODO: 自由实现
	int sel = SEG_UDATA; // segment selector for user data, need further modification
	char ch = (char)tf->edx;
	int size = tf->ebx;
}

void syscallGetStr(struct TrapFrame *tf){
	// TODO: 自由实现
}
