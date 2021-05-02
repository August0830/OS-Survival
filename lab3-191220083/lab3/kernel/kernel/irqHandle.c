#include "x86.h"
#include "device.h"


extern int displayRow;
extern int displayCol;

extern TSS tss;
extern ProcessTable pcb[MAX_PCB_NUM]; // pcb
extern int current; // current process

void GProtectFaultHandle(struct StackFrame *sf);

void syscallHandle(struct StackFrame *sf);

void syscallWrite(struct StackFrame *sf);
void syscallPrint(struct StackFrame *sf);

void timerHandle(struct StackFrame *sf);

void syscallFork(struct StackFrame *sf);
void syscallSleep(struct StackFrame *sf);
void syscallExec(struct StackFrame *sf);
void syscallExit(struct StackFrame *sf);

void irqHandle(struct StackFrame *sf) { // pointer sf = esp
	/* Reassign segment register */
	asm volatile("movw %%ax, %%ds"::"a"(KSEL(SEG_KDATA)));
	/*TODO Save esp to stackTop */
	uint32_t tmpStackTop = pcb[current].stackTop;
	pcb[current].prevStackTop = pcb[current].stackTop;
	pcb[current].stackTop = (uint32_t)sf;

	switch(sf->irq) {
		case -1:
			break;
		case 0xd:
			GProtectFaultHandle(sf);
			break;
		case 0x20:
			timerHandle(sf);
			break;
		case 0x80:
			syscallHandle(sf);
			break;
		default:assert(0);
	}
	/*TODO Recover stackTop */
	pcb[current].stackTop = tmpStackTop;
}

void GProtectFaultHandle(struct StackFrame *sf) {
	assert(0);
	return;
}

void timerHandle(struct StackFrame *sf)
{
	uint32_t tmpStackTop;
	//putChar('t');
	//putChar('\n');
	//i==0 is for core process
	//>=1 is for user process
	for(int i=0;i<MAX_PCB_NUM;++i)
	{
		if(pcb[i].state==STATE_BLOCKED)
		{
			pcb[i].sleepTime--;
			if(pcb[i].sleepTime==0)
				pcb[i].state=STATE_RUNNABLE;
		}	
	}
	pcb[current].timeCount++;
	if(pcb[current].timeCount>MAX_TIME_COUNT)
	{
		pcb[current].state=STATE_RUNNABLE;
		pcb[current].timeCount=0;
		int i=0;
		//Round Robin
		for(i=(current+1)%MAX_PCB_NUM;i!=current;i=(i+1)%MAX_PCB_NUM)
		{
			if(pcb[i].state==STATE_RUNNABLE)
				break;
		}
		current=i;
		pcb[current].state=STATE_RUNNING;
	}

	tmpStackTop = pcb[current].stackTop;
	//set tss for user process
	pcb[current].stackTop = pcb[current].prevStackTop;
	tss.esp0 = pcb[current].stackTop;
	tss.ss0=KSEL(SEG_KDATA);
	//switch to kernel
	asm volatile("movl %0, %%esp"::"m"(tmpStackTop)); // switch kernel stack
	asm volatile("popl %gs");
	asm volatile("popl %fs");
	asm volatile("popl %es");
	asm volatile("popl %ds");
	asm volatile("popal");
	asm volatile("addl $8, %esp");
	asm volatile("iret");
	return ;
}

void syscallHandle(struct StackFrame *sf) {
	switch(sf->eax) { // syscall number
		case 0:
			syscallWrite(sf);
			break; // for SYS_WRITE
		/*TODO Add Fork,Sleep... */
		case 1:
			syscallFork(sf);
			break; // SYS_FORK
		case 2:
			syscallExec(sf);
			break;//SYS_EXEC
		case 3: 
			syscallSleep(sf);
			break;//SYS_SLEEP
		case 4:
			syscallExit(sf);
			break;//SYS_EXIT
		default:break;
	}
}

void syscallFork(struct StackFrame *sf)
{
	int i,j;
	putChar('I');putChar('n');putChar('F');putChar('o');
	for(i=0;i<MAX_PCB_NUM;++i)
	{
		//putChar('0'+pcb[i].state);
		//putChar('\n');
		if(pcb[i].state==STATE_DEAD)
			break;
	}
	if(i!=MAX_PCB_NUM)//has resource for fork
	{
		enableInterrupt();
		for(j=0;j<0x100000;++j)
		{
			*(uint8_t*)(j+(i+1)*0x100000)=*(uint8_t*)(j+(current+1)* 0x100000);
			if(j%0x1000==0)
				asm volatile("int $0x20"); //XXX Testing irqTimer during syscall
		}//copy the data and code
		disableInterrupt();

		for(j=0;j<sizeof(ProcessTable);++j)
			*((uint8_t*)(&pcb[i])+j)=*((uint8_t *)(&pcb[current])+j);
		//copy pcb info
		pcb[i].stackTop = (uint32_t)&(pcb[i].regs);
		pcb[i].prevStackTop = (uint32_t)&(pcb[i].stackTop);
		pcb[i].state = STATE_RUNNABLE;
		pcb[i].timeCount = 0;
		pcb[i].sleepTime = 0;
		pcb[i].pid = i;
		//set regs
		pcb[i].regs.ss = USEL(2+2*i);
		pcb[i].regs.cs = USEL(1+2*i);
		pcb[i].regs.ds = USEL(2+2*i);
		pcb[i].regs.es = USEL(2+2*i);
		pcb[i].regs.fs = USEL(2+2*i);
		pcb[i].regs.gs = USEL(2+2*i);
		//set return value
		pcb[i].regs.eax=0;
		pcb[current].regs.eax=i;
		putChar('F');putChar('o');putChar('r');putChar('k');
		putChar('0' + pcb[i].pid);putChar('\n');
	}
	else	
		pcb[current].regs.eax=-1;
	
	return ;//return to user space
}

void syscallExec(struct StackFrame *sf)
{
	return ;
}


void syscallSleep(struct StackFrame *sf)
{
	int i=0;
	pcb[current].state=STATE_BLOCKED;
	pcb[current].sleepTime=sf->ecx;
	for(i=(current+1)%MAX_PCB_NUM;i!=current;i=(i+1)%MAX_PCB_NUM)
		if(pcb[i].state==STATE_RUNNABLE)
			break;

	current = i;
	pcb[current].state = STATE_RUNNABLE;

	//recover stackTop of selected process
	uint32_t tmpStackTop = pcb[current].stackTop;
	pcb[current].stackTop=pcb[current].prevStackTop;
	//set tss for user process
	tss.esp0=pcb[current].stackTop;
	tss.ss0=KSEL(SEG_KDATA);

	//switch to kernel stack
	asm volatile("movl %0, %%esp" : :"m"(tmpStackTop));
	asm volatile("popl %gs");
	asm volatile("popl %fs");
	asm volatile("popl %es");
	asm volatile("popl %ds");
	asm volatile("popal");
	asm volatile("addl $8, %esp");
	asm volatile("iret");
	return;
}

void syscallExit(struct StackFrame*sf)
{
	int i=0;
	pcb[current].state=STATE_DEAD;
	for(i=(current+1)%MAX_PCB_NUM;i!=current;i=(i+1)%MAX_PCB_NUM)
		if(pcb[i].state==STATE_RUNNABLE)
			break;
	current = i;
	pcb[current].state=STATE_RUNNING;
	uint32_t tmpStackTop = pcb[current].stackTop;
	pcb[current].stackTop=pcb[current].prevStackTop;
	//set tss for user process
	tss.esp0=pcb[current].stackTop;
	tss.ss0=KSEL(SEG_KDATA);

	//switch to kernel stack
	asm volatile("movl %0, %%esp" : :"m"(tmpStackTop));
	asm volatile("popl %gs");
	asm volatile("popl %fs");
	asm volatile("popl %es");
	asm volatile("popl %ds");
	asm volatile("popal");
	asm volatile("addl $8, %esp");
	asm volatile("iret");
	return;
}

void syscallWrite(struct StackFrame *sf) {
	switch(sf->ecx) { // file descriptor
		case 0:
			syscallPrint(sf);
			break; // for STD_OUT
		default:break;
	}
}

void syscallPrint(struct StackFrame *sf) {
	int sel = sf->ds; //TODO segment selector for user data, need further modification
	char *str = (char*)sf->edx;
	int size = sf->ebx;
	int i = 0;
	int pos = 0;
	char character = 0;
	uint16_t data = 0;
	asm volatile("movw %0, %%es"::"m"(sel));
	for (i = 0; i < size; i++) {
		asm volatile("movb %%es:(%1), %0":"=r"(character):"r"(str+i));
		if(character == '\n') {
			displayRow++;
			displayCol=0;
			if(displayRow==25){
				displayRow=24;
				displayCol=0;
				scrollScreen();
			}
		}
		else {
			data = character | (0x0c << 8);
			pos = (80*displayRow+displayCol)*2;
			asm volatile("movw %0, (%1)"::"r"(data),"r"(pos+0xb8000));
			displayCol++;
			if(displayCol==80){
				displayRow++;
				displayCol=0;
				if(displayRow==25){
					displayRow=24;
					displayCol=0;
					scrollScreen();
				}
			}
		}
		//asm volatile("int $0x20"); //XXX Testing irqTimer during syscall
		//asm volatile("int $0x20":::"memory"); //XXX Testing irqTimer during syscall
	}
	
	updateCursor(displayRow, displayCol);
	//TODO take care of return value
	return;
}

