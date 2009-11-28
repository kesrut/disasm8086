#include "disasm.h" 

uchar var[64] ; 
uchar rm_var[64] ; 

char *jump_near(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
	(*j)++ ; 
	uchar low = buffer[*j] ; 
	(*j)++ ;
	uchar high = buffer[*j] ;
	signed short imm = ((high << 8) + low) ; 
	sprintf(var, "0x%x", *j+imm+1) ; 
	return var ;
} 

char *jump_near_indirect(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ;
	char *s = rm(buffer, j, 16) ;
	sprintf(var, "%s", s) ; 
	return var ; 
}

char *jump_far(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ;
	char *s = rm(buffer, j, 16) ;
	sprintf(var, "%s", s) ; 
	return var ; 
}

char *jump_inter_segment(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
	(*j)++ ;
	uchar low = buffer[*j] ; 
	(*j)++ ; 
	uchar high = buffer[*j] ; 
	unsigned short offset = (high << 8) + low ; 
	(*j)++ ;
	low = buffer[*j] ; 
	(*j)++ ; 
	high = buffer[*j] ;  
	unsigned short segment = (high << 8) + low ; 
	sprintf(var,"0x%x:0x%x", segment, offset) ; 
	return var ;   
}

char *call_inter_segment(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
	(*j)++ ;
	uchar low = buffer[*j] ; 
	(*j)++ ; 
	uchar high = buffer[*j] ; 
	unsigned short offset = (high << 8) + low ; 
	(*j)++ ;
	low = buffer[*j] ; 
	(*j)++ ; 
	high = buffer[*j] ;  
	unsigned short segment = (high << 8) + low ; 
	sprintf(var,"0x%x:0x%x", segment, offset) ; 
	return var ;   
}

int main(int argc, char **argv)
{
	uchar *buffer ; 
	long num ; 
	buffer = open_file("/Users/kesrut/Desktop/test.txt", &num) ;
	disasm(buffer, num) ;
	/*
	if (argc > 1)
	{
		if (strlen(argv[1]) < 255)
		{
			buffer = open_file(argv[1], &num) ;
			disasm(buffer, num) ; 
		}     
	}*/
	return 0 ;  
}

uchar* open_file(char *name, long *num)
{
	FILE *fp ; 
	long f_size ; 
	char *buffer ; 
	fp = fopen(name, "rb") ; 
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END) ;
		f_size = ftell(fp) ; 
		*num = f_size ; 
		rewind(fp) ;
		buffer = (char*) malloc(sizeof(char)*f_size) ; 
		if (buffer == NULL) { printf("Memory error!\n"); exit(1) ; }
		long result = fread(buffer, 1, f_size, fp) ; 
		if (result != f_size) { printf("Reading error!\n") ; exit(1) ; } 
		return buffer ; 
	}
	else
	{
		printf("Error reading file!\n") ; 
		exit(1) ; 
	}
}

void disasm(uchar *buffer, long num)
{
	long j = 0, p  = 0 ;
	uchar t = 0 ; 
	uchar s = 0 ; 
	int prefix = 0 ; 
	while ( j < num)
	{
		s = 0 ; 
		t = 0 ; 
		if (prefix != 1) printf("%.8x  ", (unsigned int)j) ;
		prefix = 0 ; 
		switch (buffer[j])
		{
			case 0x00: printf("add %s\n", rm8_r8(buffer, &j)); break ;
			case 0x01: printf("add %s\n", rm16_r16(buffer, &j)); break ;
			case 0x02: printf("add %s\n", r8_rm8(buffer, &j)) ; break ;
			case 0x03: printf("add %s\n", r16_rm16(buffer, &j)) ; break ;  
			case 0x04: printf("add al, %s\n", imm8(buffer, &j)); break ;
			case 0x05: printf("add ax, %s\n", imm16(buffer, &j)); break ;  
			case 0x06: printf("push es\n") ; break ;
			case 0x07: printf("pop es\n") ; break ;
			case 0x08: printf("or %s\n", rm8_r8(buffer, &j)); break ;
			case 0x09: printf("or %s\n", rm16_r16(buffer, &j)); break ;
			case 0x0a: printf("or %s\n", r8_rm8(buffer, &j)) ; break ;
			case 0x0b: printf("or %s\n", r16_rm16(buffer, &j)) ; break ;
			case 0x0c: printf("or al, %s\n", imm8(buffer, &j)); break ;
			case 0x0d: printf("or ax, %s\n", imm16(buffer, &j)); break ;
			case 0x0e: printf("push cs\n") ; break ; 
			case 0x10: printf("adc %s\n", rm8_r8(buffer, &j)) ; break ;
			case 0x11: printf("adc %s\n", rm16_r16(buffer, &j)); break ;
			case 0x12: printf("adc %s\n", r8_rm8(buffer, &j)) ; break ; 
			case 0x14: printf("adc al,%s\n", imm8(buffer, &j)) ; break ; 
			case 0x13: printf("adc %s\n", r16_rm16(buffer, &j)) ; break ;
			case 0x15: printf("adc ax,%s\n", imm16(buffer, &j)) ; break ;
			case 0x16: printf("push ss\n") ; break ; 
			case 0x17: printf("pop ss\n") ; break ; 
			case 0x18: printf("sbb %s\n", rm8_r8(buffer, &j)); break ;
			case 0x19: printf("sbb %s\n", rm16_r16(buffer, &j)); break ;
			case 0x1a: printf("sbb %s\n", r8_rm8(buffer, &j)) ; break ;
			case 0x1b: printf("sbb %s\n", r16_rm16(buffer, &j)) ; break ;
			case 0x1c: printf("sbb al, %s\n", imm8(buffer, &j)); break ;
			case 0x1d: printf("sbb ax, %s\n", imm16(buffer, &j)); break ; 
			case 0x1e: printf("push ds\n") ; break ; 
			case 0x1f: printf("pop ds\n") ; break ; 
			case 0x21: printf("and %s\n", rm16_r16(buffer, &j)); break ; 
			case 0x22: printf("and %s\n", r8_rm8(buffer, &j)) ; break ;  
			case 0x23: printf("and %s\n", r16_rm16(buffer, &j)) ; break ;
			case 0x24: printf("and al,%s\n", imm8(buffer, &j)) ; break ;  
			case 0x25: printf("and ax, %s\n", imm16(buffer, &j)); break ;  
			case 0x27: printf("daa\n"); break ;
			case 0x28: printf("sub %s\n", rm8_r8(buffer, &j)); break ;
			case 0x29: printf("sub %s\n", rm16_r16(buffer, &j)); break ;
			case 0x2a: printf("sub %s\n", r8_rm8(buffer, &j)) ; break ;
			case 0x2b: printf("sub %s\n", r16_rm16(buffer, &j)) ; break ;
			case 0x2c: printf("sub al, %s\n", imm8(buffer, &j)); break ;
			case 0x2d: printf("sub ax, %s\n", imm16(buffer, &j)); break ;
			case 0x2f: printf("das\n") ; break ; 
			case 0x30: printf("xor %s\n", rm8_r8(buffer, &j)); break ;
			case 0x31: printf("xor %s\n", rm16_r16(buffer, &j)); break ;
			case 0x32: printf("xor %s\n", r8_rm8(buffer, &j)) ; break ;
			case 0x33: printf("xor %s\n", r16_rm16(buffer, &j)) ; break ;
			case 0x34: printf("xor al, %s\n", imm8(buffer, &j)) ; break ;
			case 0x35: printf("xor ax, %s\n", imm16(buffer, &j)); break ;
			case 0x37: printf("aaa\n") ; break ;
			case 0x38: printf("cmp %s\n", rm8_r8(buffer, &j)); break ;
			case 0x39: printf("cmp %s\n", rm16_r16(buffer, &j)); break ;
			case 0x3a: printf("cmp %s\n", r8_rm8(buffer, &j)) ; break ;
			case 0x3b: printf("cmp %s\n", r16_rm16(buffer, &j)) ; break ;
			case 0x3c: printf("cmp al, %s\n", imm8(buffer, &j)); break ;
			case 0x3f: printf("aas\n"); break ;
			case 0x40: printf("inc ax\n") ; break ; 
			case 0x41: printf("inc cx\n"); break  ;
			case 0x42: printf("inc dx\n") ; break ; 
			case 0x43: printf("inc bx\n"); break ; 
			case 0x44: printf("inc sp\n"); break ;
			case 0x45: printf("inc bp\n"); break ; 
			case 0x46: printf("inc si\n"); break ;
			case 0x47: printf("inc di\n") ; break ;
			case 0x48: printf("dec ax\n") ; break ; 
			case 0x49: printf("dec cx\n"); break ;
			case 0x4a: printf("dec dx\n"); break ;
			case 0x4b: printf("dec bx\n"); break ; 
			case 0x4c: printf("dec sp\n"); break ;
			case 0x4d: printf("dec bp\n"); break ;
			case 0x4e: printf("dec si\n") ; break ;
			case 0x4f: printf("dec di\n"); break ; 
			case 0x50: printf("push ax\n"); break ;
			case 0x51: printf("push cx\n") ; break ;
			case 0x52: printf("push dx\n"); break ;
			case 0x53: printf("push bx\n"); break ;
			case 0x54: printf("push sp\n"); break ;
			case 0x55: printf("push bp\n"); break ; 
			case 0x56: printf("push si\n"); break ;
			case 0x57: printf("push di\n"); break ;
			case 0x58: printf("pop ax\n"); break ;
			case 0x59: printf("pop cx\n") ; break ;
			case 0x5a: printf("pop dx\n"); break ;
			case 0x5b: printf("pop bx\n"); break ; 
			case 0x5c: printf("pop sp\n"); break ;
			case 0x5d: printf("pop bp\n") ; break ;
			case 0x5e: printf("pop si\n") ; break ;
			case 0x5f: printf("pop di\n"); break ;
			case 0x60: printf("pusha\n") ; break ;  
			
			case 0x72: printf("jc %s\n", rel8(buffer, &j)) ; break ;  
			case 0x74: printf("jz %s\n", rel8(buffer, &j)) ; break ;
			case 0x7c: printf("jl %s\n", rel8(buffer, &j)) ; break ; 
			case 0x80:
			{ 
				p = j ; s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x00:
					{
						printf("add %s\n", rm8_imm8(buffer, &j)) ; 
					} break ;
					case 0x01:
					{
						printf("or byte %s\n", rm8_imm8(buffer, &j)) ; 
					} break ; 
					case 0x02:
					{
						printf("adc %s\n",  rm8_imm8(buffer, &j)) ; 
					} break ; 
					case 0x04:
					{
						printf("and %s\n",  rm8_imm8(buffer, &j)) ; 
					} break ; 
					default: t=1 ; 
				}  
			if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ; 
			case 0x81:
			{
				p = j ; s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x00:
					{
						printf("add %s\n", rm16_imm16(buffer, &j)) ; 
					} break ; 
					case 0x01:
					{
						printf("or word %s\n", rm16_imm16(buffer, &j)) ; 
					} break ;
					case 0x02:
					{
						printf("adc %s\n",  rm16_imm16(buffer, &j)) ; 
					} break ; 
					case 0x04:
					{
						printf("and %s\n",  rm16_imm16(buffer, &j)) ; 
					} break ; 
					case 0x07:
					{
						printf("cmp %s\n", rm16_imm16(buffer, &j)) ; 
					} break ; 
					default: t=1; 
					
				}
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;  
			case 0x83:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x00:
					{
						printf("add %s\n",  rm16_imm8(buffer, &j)) ; 
					} break ; 
					case 0x01:
					{
						printf("or byte %s\n", rm16_imm8(buffer, &j)) ; 
					} break ;
					case 0x02:
					{
						printf("adc %s\n",  rm16_imm8(buffer, &j)) ; 
					} break ; 
					case 0x04:
					{
						printf("and %s\n",  rm16_imm8(buffer, &j)) ; 
					} break ; 
					case 0x07:
					{
						printf("cmp word %s\n", rm16_imm8(buffer, &j)) ; 
					} break ;
					default: t=1;
				}
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;  
			case 0x84: printf("test %s\n", rm8_r8(buffer, &j)); break ; 
			case 0x85: printf("test %s\n", rm16_r16(buffer, &j)); break ;
			case 0x86: printf("xchg %s\n", rm8_r8(buffer, &j)); break ; 
			case 0x87: printf("xchg %s\n", rm16_r16(buffer, &j)); break ; 
			case 0x88: printf("mov %s\n", rm8_r8(buffer, &j)); break ;
			case 0x89: printf("mov %s\n", rm16_r16(buffer, &j)); break ;
			case 0x8A: printf("mov %s\n", r8_rm8(buffer, &j)) ; break ;
			case 0x8b: printf("mov %s\n", r16_rm16(buffer, &j)) ; break ; 
			case 0x8c: printf("mov %s\n", rm16_sreg(buffer, &j)); break ;
			case 0x8e: printf("mov %s\n", sreg_rm16(buffer, &j)); break ;
			case 0x8f:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x00:
					{
						j++ ; printf("pop %s\n", m16(buffer, &j)) ;
					} break ; 
					default: t=1; 
				}
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;  
			case 0x90: printf("xchg ax,ax\n") ; break ;  		  
			case 0x91: printf("xchg cx,ax\n") ; break ;  		  
			case 0x92: printf("xchg dx,ax\n") ; break ;  		  
			case 0x93: printf("xchg bx,ax\n") ; break ;  		  
			case 0x94: printf("xchg sp,ax\n") ; break ;  		  
			case 0x95: printf("xchg bp,ax\n") ; break ;  		  
			case 0x96: printf("xchg si,ax\n") ; break ;  		  
			case 0x97: printf("xchg di,ax\n") ; break ;  		  
			case 0x98: printf("cbw\n") ; break ; 
			case 0x99: printf("cbd\n") ; break ; 
			case 0x9a: printf("call %s\n", call_inter_segment(buffer, &j)) ; break ;
			case 0x9b: printf("wait\n") ; break ; 
			case 0x9c: printf("pushf\n") ; break ;  
			case 0x9d: printf("popf\n") ; break ;
			case 0x9e: printf("sahf\n"); break ;
			case 0x9f: printf("lahf\n") ; break ;	
			case 0xa0: printf("mov al,%s\n", moffs(buffer, &j)) ; break ; 
			case 0xa1: printf("mov ax,%s\n", moffs(buffer, &j)) ; break ; 
			case 0xa2: printf("mov %s,al\n", moffs(buffer, &j)) ; break ; 
			case 0xa3: printf("mov %s,ax\n", moffs(buffer, &j)) ; break ; 
			case 0xa6: printf("cmpsb\n") ; break ; 
			case 0xa7: printf("cmpsw\n") ; break ;
			case 0xa8: printf("test al,%s\n", imm8(buffer, &j)) ; break ; 
			case 0xa9: printf("test ax,%s\n", imm16(buffer, &j)); break ;
			case 0xac: printf("lodsb\n") ; break ;
			case 0xad: printf("lodsw\n") ; break ; 
			case 0xb0: printf("mov al,%s\n", imm8(buffer, &j)); break ;
			case 0xb1: printf("mov cl,%s\n", imm8(buffer, &j)); break ;
			case 0xb2: printf("mov dl,%s\n", imm8(buffer, &j)); break ;
			case 0xb3: printf("mov bl,%s\n", imm8(buffer, &j)); break ;
			case 0xb4: printf("mov ah,%s\n", imm8(buffer, &j)); break ;
			case 0xb5: printf("mov ch,%s\n", imm8(buffer, &j)); break ;
			case 0xb6: printf("mov dh,%s\n", imm8(buffer, &j)); break ;  
			case 0xb7: printf("mov bh,%s\n", imm8(buffer, &j)); break ;  
			case 0xb8: printf("mov ax,%s\n", imm16(buffer, &j)); break ;
			case 0xb9: printf("mov cx,%s\n", imm16(buffer, &j)); break ;
			case 0xba: printf("mov dx,%s\n", imm16(buffer, &j)); break ;
			case 0xbb: printf("mov bx,%s\n", imm16(buffer, &j)); break ;
			case 0xbc: printf("mov sp,%s\n", imm16(buffer, &j)); break ;
			case 0xbd: printf("mov bp,%s\n", imm16(buffer, &j)); break ;
			case 0xbe: printf("mov si,%s\n", imm16(buffer, &j)); break ;
			case 0xbf: printf("mov di,%s\n", imm16(buffer, &j)); break ;
			case 0xc0:
			{ 
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x04:
					{
						printf("sal %s\n", rm8_imm8(buffer, &j)) ;
					} break ;
					case 0x05:
					{
						printf("shr %s\n", rm8_imm8(buffer, &j)) ;
					} break ;
					case 0x07:
					{
						printf("sar %s\n", rm8_imm8(buffer, &j)) ;
					} break ;
					default: t= 1 ; 
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xc1:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x04:
					{
						printf("sal %s\n", rm16_imm8(buffer, &j)) ;
					} break ;
					case 0x05:
					{
						printf("shr %s\n", rm16_imm8(buffer, &j)) ;
					} break ;
					case 0x07:
					{
						printf("sar %s\n", rm16_imm8(buffer, &j)) ;
					} break ;
					default: t= 1 ; 
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xc2: printf("ret %s\n", imm16(buffer,&j)); break ;
			case 0xc3: printf("ret\n") ; break ; 
			case 0xc6: printf("mov byte %s\n", rm8_imm8(buffer, &j)) ; break ; 
			case 0xc7: printf("mov word %s\n", rm16_imm16(buffer, &j)) ; break ; 
			case 0xca: printf("retf %s\n", imm16(buffer, &j)) ; break ; 
			case 0xcb: printf("retf\n") ; break ; 
			case 0xcc: printf("int3\n") ; break ;
			case 0xcd: printf("int %s\n", imm8(buffer, &j)) ; break ; 
			case 0xce: printf("into\n"); break ; 
			case 0xcf: printf("iret\n") ; break ; 
			case 0xd0:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x04:
					{
						printf("sal %s, 1\n", rm8(buffer, &j)) ;
					} break ;
					case 0x05:
					{
						printf("shr %s, 1\n", rm8(buffer, &j)) ;
					} break ;
					case 0x07:
					{
						printf("sar %s, 1\n", rm8(buffer, &j)) ;	
					} break ;
					default: t= 1 ; 
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xd1:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x04:
					{
						printf("sal %s, 1\n", rm16(buffer, &j)) ;
					} break ;
					case 0x05:
					{
						printf("shr %s, 1\n", rm16(buffer, &j)) ;
					} break ;
					case 0x07:
					{
						printf("sar %s, 1\n", rm16(buffer, &j)) ;
					} break ;
					default: t= 1 ; 
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xd2:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x04:
					{
						printf("sal %s, cl\n", rm8(buffer, &j)) ;
					} break ;
					case 0x05:
					{
						printf("shr %s, cl\n", rm8(buffer, &j)) ;
					} break ;
					case 0x07:
					{
						printf("sar %s, cl\n", rm8(buffer, &j)) ;
					} break ;
					default: t= 1 ; 
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xd3:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x04:
					{
						printf("sal %s, cl\n", rm16(buffer, &j)) ;
					} break ;
					case 0x05:
					{
						printf("shr %s, cl\n", rm16(buffer, &j)) ;
					} break ;	
					case 0x07:
					{
						printf("sar %s, cl\n", rm16(buffer, &j)) ;
					} break ;
					default: t= 1 ; 
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xd4:
			{ 
				
				switch (buffer[++j])
				{
					case 0x0a: printf("aam\n") ; break ; 
					default: j--; t=1;  break ;
				}
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ; 
			case 0xd5:
			{
				switch (buffer[++j])
				{
					case 0x0a: printf("aad\n"); break ;
					default: j-- ; t=1 ;  break ; 
				}
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xe0: printf("loopnz %s\n", rel8(buffer, &j)) ; break ;
			case 0xe1: printf("loopz %s\n", rel8(buffer, &j)) ; break ; 
			case 0xe2: printf("loop %s\n", rel8(buffer, &j)) ; break ; 
			case 0xea: printf("jmp %s\n", jump_inter_segment(buffer, &j)) ; break ; 
			case 0xe8: printf("call %s\n", rel16(buffer, &j)) ; break ;
			case 0xe9: printf("jmp %s\n", jump_near(buffer, &j)) ; break ; 
			case 0xeb: printf("jmp short %s\n", jump_short(buffer, &j)); break ; 
			case 0xf3: printf("repe ") ; prefix = 1 ; break;  // prefix 
			case 0xf4: printf("hlt\n") ; break ; 
			case 0xf5: printf("cmc\n"); break ;
			case 0xf6:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x00:
					{
						printf("test %s\n", rm8_imm8(buffer, &j)) ;
					} break ;
					case 0x04:
					{
						printf("mul byte %s\n", rm8(buffer, &j)) ; break ; 
					} break ;
					case 0x05:
					{
						printf("imul %s\n", rm8(buffer, &j)) ; break ; 
					} break ;
					case 0x06:
					{
					    printf("div byte %s\n", rm8(buffer, &j)) ; break ; 
					} break ;
					default: t= 1 ; 
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xf7:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x00:
					{
						printf("test %s\n", rm16_imm16(buffer, &j)) ;   
					} break ; 
					case 0x04:
					{
						printf("mul word %s\n", rm16(buffer, &j)); break ;
					} break ;
					case 0x05:
					{
						printf("imul %s\n", rm16(buffer, &j)); break ;
					} break ;
					case 0x06:
					{
						printf("div word %s\n", rm16(buffer, &j)) ; break ; 
					} break ; 
					default: t=1 ; 
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ;
			case 0xf8: printf("clc\n") ; break ;
			case 0xf9: printf("stc\n") ; break ;
			case 0xfa: printf("cli\n") ; break ;
			case 0xfb: printf("sti\n") ; break ; 
			case 0xfc: printf("cld\n") ; break ;
			case 0xfd: printf("std\n") ; break ;
			case 0xfe:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x00:
					{
						printf("inc %s\n", rm8(buffer, &j)) ; 
					} break ; 
					case 0x01:
					{
						printf("dec %s\n",  rm8(buffer, &j)) ; 
					} break ;
					default: t= 1 ; 
				} 
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			} tikrinti(t) ; 
			case 0xff:
			{
				p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
				switch (s)
				{
					case 0x00:
					{
					  printf("inc %s\n", rm16(buffer, &j)) ; 
					} break ;
					case 0x01:
					{
						printf("dec %s\n",  rm16(buffer, &j)) ; 
					} break ;
					case 0x04:
					{
						printf("jmp %s\n", jump_near_indirect(buffer, &j)) ; 
					} break ;
					case 0x05:
					{
						printf("jmp far %s\n", jump_far(buffer, &j)) ; 
					} break ; 
					case 0x06:
					{
						j++ ; printf("push %s\n", m16(buffer, &j)) ; 
					} break ;
					default: t= 1 ;    
				}  
				if (t == 1) { printf("0x%x\n", buffer[j]); break; }
			}  tikrinti(t) ;  
			default: printf("0x%x\n", buffer[j]); 
		}
		j++ ; 
	}     
}

uchar *imm8(uchar *buffer, long *j)
{
    (*j)++ ;
    sprintf(var, "0x%x", buffer[*j]) ;     
    return var;  
}

uchar *imm16(uchar *buffer, long *j)
{
	(*j)++ ; 
	uchar low = buffer[*j] ; 
	(*j)++ ; 
	uchar high = buffer[*j] ; 
	uint16 imm = (high << 8) + low ;
	sprintf(var, "0x%x", imm) ; 
	return var ;  
}

char *rm(uchar *buffer, long *j, uchar bit)
{
    memset(rm_var, '\0', 64) ; 
    uchar rm_byte = buffer[++(*j)] ; 
    uchar mod = (rm_byte) >> 6 ; 
    uchar rm = rm_byte &  7 ; 
    uchar dip = 0 ; 
    signed short int disp = 0  ;
    char sign = '+' ;  
    switch (mod)
    {
        case 0x00:
		{
			dip = 1  ;   
		} break ; 
        case 0x1:
		{
			uchar low = buffer[++(*j)] ; 
			disp  = (signed char) low ; 
		} break ;
        case 0x2:
		{
			(*j)++ ; 
			uchar low =  buffer[*j] ; 
			(*j)++ ; 
			uchar high = buffer[*j] ; 
			disp =  (((high << 8) + low)  );      
		} break ; 
        case 0x03:
		{
			if (bit == 8) return reg8[rm] ; 
			else return reg16[rm] ; 
		} break ;                                        
    }
    if (disp < 0)  { sign = '-' ; disp = -disp ; }
    switch (rm)
    {
        case 0x00: if (dip != 1)  sprintf(rm_var,"[bx+si%c%x]", sign,  disp) ; else sprintf(rm_var,"[bx+si]") ; break ; 
        case 0x01: if (dip != 1)  sprintf(rm_var,"[bx+di%c%x]", sign,  disp) ; else sprintf(rm_var,"[bx+di]") ;  break ; 
        case 0x02: if (dip != 1)  sprintf(rm_var,"[bp+si%c%x]", sign,  disp) ; else sprintf(rm_var,"[bp+si]") ;  break ;
        case 0x03: if (dip != 1)  sprintf(rm_var,"[bp+di%c%x]", sign,  disp) ; else sprintf(rm_var,"[bp+di]") ;  break ;
        case 0x04: if (dip != 1)  sprintf(rm_var,"[si%c%x]", sign,  disp) ; else sprintf(rm_var,"[si]") ; break ;
        case 0x05: if (dip != 1)  sprintf(rm_var,"[di%c%x]", sign,  disp) ; else  sprintf(rm_var,"[di]"); break ;
        case 0x06:
		{
			if (dip == 1)
			{
				(*j)++ ; 
				uchar low = buffer[*j] ; 
				(*j)++ ; 
				uchar high = buffer[*j] ; 
				disp = ((high << 8) + low) ;
				sprintf(rm_var,"[0x%hx]", disp) ; 
			}
			else  {  sprintf(rm_var,"[bp%hx]",  disp) ; } break ; 
		} break ; 
        case 0x07: if (dip != 1 ) sprintf(rm_var,"[bx%c%x]", sign, disp) ; else  sprintf(rm_var,"[bx]") ; break ; 
    }
	return rm_var ; 
}

char *rm8_imm8(uchar *buffer, long *j )
{
	memset(var, '\0', 64) ; 
	char *s = rm(buffer, j, 8) ;
	(*j)++ ;
	sprintf(var, "%s,0x%x",s, buffer[*j]) ;     
	return var; 
}

char *rm16_imm16(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ;
	char *s = rm(buffer, j, 16) ; 
	(*j)++ ; 
	uchar low = buffer[*j] ; 
	(*j)++ ; 
	uchar high = buffer[*j] ; 
	uint16 imm = (high << 8) + low ;
	sprintf(var, "%s, 0x%x",s, imm) ;     
	return var ; 
}

/* moffs - need to implement segment override */

char *moffs(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ;
	(*j)++ ; 
	uchar low = buffer[*j] ; 
	(*j)++ ; 
	uchar high = buffer[*j] ; 
	uint16 moffs = (high << 8) + low ; 
	sprintf(var, "[0x%x]", moffs) ; 
	return var ; 
}

char *rm16_imm8(uchar *buffer, long *j)
{
    memset(var, '\0', 64) ; 
    char *s = rm(buffer, j, 16) ;
	(*j)++ ;
	signed char  c  = (signed char)  buffer[*j] ;
	if (abs(c) == c) sprintf(var, "%s,byte +0x%x",s, c) ;     
	else {
		c = -c ; 
		sprintf(var, "%s,byte -0x%x",s, c) ;     
	}
	return var; 
}

char *rm8_r8(uchar *buffer, long *j)
{
    memset(var, '\0', 64) ; 
    uchar reg = ((buffer[++(*j)] & 0x38) >> 3 ); 
    (*j)-- ; 
	char *s = rm(buffer, j, 8) ;
	char *c_reg = reg8[reg] ; 
	strcpy(var, s)  ;
	strcat(var, ",")  ;
	strcat(var, c_reg) ; 
	strcat(var, "\0") ; 
	return var ; 
}

char *rm16_r16(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
	uchar reg = ((buffer[++(*j)] & 0x38) >> 3) ;
	(*j)-- ; 
	char *s = rm(buffer, j, 16)  ;
	char *c_reg = reg16[reg]  ;
	strcpy(var, s)  ;
	strcat(var, ",")  ;
	strcat(var, c_reg) ; 
	strcat(var, "\0") ; 
	return var ; 
}

char *rm16_sreg(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
	uchar reg = ((buffer[++(*j)] & 0x38) >> 3) ;
	(*j)-- ; 
	char *s = rm(buffer, j, 16)  ;
	char *c_reg = sreg[reg]  ;  
	strcpy(var, s)  ;
	strcat(var, ",")  ;
	strcat(var, c_reg) ; 
	strcat(var, "\0") ; 
	return var ; 
}

char *r8_rm8(uchar *buffer, long *j)
{
    memset(var, '\0', 64) ; 
    uchar reg = ((buffer[++(*j)] & 0x38) >> 3 ); 
    (*j)-- ; 
    char *s = rm(buffer, j, 8) ;
    char *c_reg = reg8[reg] ; 
    strcpy(var, c_reg)  ;
    strcat(var, ",")  ;
    strcat(var, s) ; 
    strcat(var, "\0") ; 
    return var ; 
}


char *r16_rm16(uchar *buffer, long *j)
{
	uchar reg = ((buffer[++(*j)] & 0x38) >> 3) ;
	(*j)-- ;
	memset(var, '\0', 64) ;  
	char *s = rm(buffer, j, 16)  ;
	char *c_reg = reg16[reg]  ;
	strcpy(var, c_reg)  ;
	strcat(var, ",")  ;
	strcat(var, s) ; 
	strcat(var, "\0") ; 
	return var ; 
}

char *sreg_rm16(uchar *buffer, long *j)
{
	uchar reg = ((buffer[++(*j)] & 0x38) >> 3) ;
	(*j)-- ;
	memset(var, '\0', 64) ;  
	char *s = rm(buffer, j, 16)  ;
	char *c_reg = sreg[reg]  ;
	strcpy(var, c_reg)  ;
	strcat(var, ",")  ;
	strcat(var, s) ; 
	strcat(var, "\0") ; 
	return var ; 
}


char *rm8(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ;  
	char *s = rm(buffer, j, 8)  ;
	strcpy(var, s) ;
	return var ; 
}

char *rm16(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
	char *s = rm(buffer, j, 16) ;
	strcpy(var, s) ; 
	return var ; 
}

char *jump_short(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ;
	(*j)++ ; 
	signed char imm = buffer[*j] ; 
	sprintf(var, "0x%x",(unsigned char) *j+imm+1) ; 
	return var ; 
}

char *jmp_short_byte(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
	(*j)++ ; 
	signed char imm = buffer[*j]  ;
	unsigned short result = ( *j+imm+1 ) ; 
	sprintf(var, "0x%x", result) ; 
	return var ; 
}
char *m16(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
    (*j)++ ; 
	uchar low = buffer[*j] ; 
	(*j)++ ; 
	uchar high = buffer[*j] ; 
	uint16 imm = (high << 8) + low ;
	sprintf(var, "word [0x%x]", imm) ; 
	return var ;  
}

char *rel16(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
    (*j)++ ; 
	uchar low = buffer[*j] ; 
	(*j)++ ; 
	uchar high = buffer[*j] ; 
	signed short imm = (high << 8) + low ; 
	uint16 result = *j + imm + 1 ; 
	sprintf(var, "0x%x", result) ; 
	return var ;  
}

char *rel8(uchar *buffer, long *j)
{
	memset(var, '\0', 64) ; 
    (*j)++ ; 
	signed char imm = buffer[*j] ; 
    unsigned short result = *j + imm + 1 ; 
	sprintf(var, "0x%x", result) ; 
	return var ;  
}




