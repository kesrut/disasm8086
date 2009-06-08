#include "disasm.h"

uchar var[64] ; 
uchar rm_var[64] ; 

int main(int argc, char **argv)
{
  uchar *buffer ; 
  long num ; 
  if (argc > 1)
  {
     if (strlen(argv[1]) < 255)
     {
       buffer = open_file(argv[1], &num) ;
       disasm(buffer, num) ; 
     }     
  }
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
   while ( j < num)
   {
     s = 0 ; 
     t = 0 ; 
      switch (buffer[j])
      {
        case 0x00: printf("add %s\n", rm8_r8(buffer, &j)); break ;
        case 0x01: printf("add %s\n", rm16_r16(buffer, &j)); break ;
        case 0x02: printf("add %s\n", r8_rm8(buffer, &j)) ; break ;
        case 0x03: printf("add %s\n", r16_rm16(buffer, &j)) ; break ;  
        case 0x04: printf("add al, %s\n", imm8(buffer, &j)); break ;
        case 0x05: printf("add ax, %s\n", imm16(buffer, &j)); break ;  
        case 0x10: printf("adc %s\n", rm8_r8(buffer, &j)) ; break ;
        case 0x11: printf("adc %s\n", rm16_r16(buffer, &j)); break ;
        case 0x12: printf("adc %s\n", r8_rm8(buffer, &j)) ; break ; 
        case 0x14: printf("adc al,%s\n", imm8(buffer, &j)) ; break ; 
        case 0x13: printf("adc %s\n", r16_rm16(buffer, &j)) ; break ;
        case 0x15: printf("adc ax,%s\n", imm16(buffer, &j)) ; break ;
        case 0x20: printf("and %s\n", rm8_r8(buffer, &j)); break ;
        case 0x21: printf("and %s\n", rm16_r16(buffer, &j)); break ; 
        case 0x22: printf("and %s\n", r8_rm8(buffer, &j)) ; break ;  
        case 0x23: printf("and %s\n", r16_rm16(buffer, &j)) ; break ;
        case 0x24: printf("and al,%s\n", imm8(buffer, &j)) ; break ;  
        case 0x25: printf("and ax, %s\n", imm16(buffer, &j)); break ;  
        case 0x27: printf("daa\n"); break ;
        case 0x2f: printf("das\n") ; break ; 
        case 0x37: printf("aaa\n") ; break ;
        case 0x3c: printf("cmp al,%s\n", imm8(buffer, &j)) ; break ;  
        case 0x3d: printf("cmp ax, %s\n", imm16(buffer, &j)); break ;   
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
        case 0x80:
        {
            p = j ; s = ((buffer[++p] & 0x38) >> 3) ;
            switch (s)
            {
                case 0x00:
                {
                  printf("add %s\n", rm8_imm8(buffer, &j)) ; 
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
            }  } tikrinti(t) ; 
        case 0x81:
        {
           p = j ; s = ((buffer[++p] & 0x38) >> 3) ;
           switch (s)
           {
              case 0x00:
              {
                printf("add %s\n", rm16_imm16(buffer, &j)) ; 
              } break ; 
              case 0x02:
              {
                printf("adc %s\n",  rm16_imm16(buffer, &j)) ; 
              } break ; 
              case 0x04:
              {
                 printf("and %s\n",  rm16_imm16(buffer, &j)) ; 
              } break ; 
              default: t=1; 
           }
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
              case 0x02:
              {
                printf("adc %s\n",  rm16_imm8(buffer, &j)) ; 
              } break ; 
              case 0x04:
              {
                 printf("and %s\n",  rm16_imm8(buffer, &j)) ; 
              } break ; 
              default: t=1; 
           }
        } tikrinti(t) ;  
        case 0x88: printf("mov %s\n", rm8_r8(buffer, &j)); break ;
	case 0x89: printf("mov %s\n", rm16_r16(buffer, &j)); break ;
        case 0x8A: printf("mov %s\n", r8_rm8(buffer, &j)) ; break ;
        case 0x8b: printf("mov %s\n", r16_rm16(buffer, &j)) ; break ; 
	case 0x8c: printf("mov %s\n", rm16_sreg(buffer, &j)); break ;
	case 0x8e: printf("mov %s\n", sreg_rm16(buffer, &j)); break ;
	case 0x9c: printf("pushf\n") ; break ;  
        case 0xa0: printf("mov al, %s\n", moffs(buffer, &j)) ; break ; 
	case 0xa1: printf("mov ax, %s\n", moffs(buffer, &j)) ; break ; 
	case 0xa2: printf("mov %s, al\n", moffs(buffer, &j)) ; break ; 
	case 0xa3: printf("mov %s, ax\n", moffs(buffer, &j)) ; break ; 
	case 0xb0: printf("mov al, %s\n", imm8(buffer, &j)); break ;
	case 0xb1: printf("mov cl, %s\n", imm8(buffer, &j)); break ;
	case 0xb2: printf("mov dl, %s\n", imm8(buffer, &j)); break ;
	case 0xb3: printf("mov bl, %s\n", imm8(buffer, &j)); break ;
	case 0xb4: printf("mov bh, %s\n", imm8(buffer, &j)); break ;
	case 0xb5: printf("mov ch, %s\n", imm8(buffer, &j)); break ;
        case 0xb6: printf("mov dh, %s\n", imm8(buffer, &j)); break ;	
 	case 0xb7: printf("mov bh, %s\n", imm8(buffer, &j)); break ;	
	case 0xb8: printf("mov ax, %s\n", imm16(buffer, &j)); break ;
	case 0xb9: printf("mov cx, %s\n", imm16(buffer, &j)); break ;
	case 0xba: printf("mov dx, %s\n", imm16(buffer, &j)); break ;
	case 0xbb: printf("mov bx, %s\n", imm16(buffer, &j)); break ;
	case 0xbc: printf("mov sp, %s\n", imm16(buffer, &j)); break ;
	case 0xbd: printf("mov bp, %s\n", imm16(buffer, &j)); break ;
	case 0xbe: printf("mov si, %s\n", imm16(buffer, &j)); break ;
	case 0xbf: printf("mov di, %s\n", imm16(buffer, &j)); break ;
	case 0xc6: printf("mov %s\n", rm8_imm8(buffer, &j)) ; break ; 
	case 0xc7: printf("mov %s\n", rm16_imm16(buffer, &j)) ; break ; 
	case 0xd4:
        { 
            switch (buffer[++j])
            {
                case 0x0a: printf("aam\n") ; break ; 
                default: j--; t=1;  break ;
            }
        } tikrinti(t) ; 
        case 0xd5:
        {
            
            switch (buffer[++j])
            {
                case 0x0a: printf("aad\n"); break ;
                default: j-- ; t=1 ;  break ; 
            }
        } tikrinti(t) ;
        case 0xf4: printf("hlt\n") ; break ; 
        case 0xf5: printf("cmc\n"); break ;
        case 0xf6: printf("mul al,%s\n", imm8(buffer, &j)) ; break ;   
        case 0xf7: printf("mul ax, %s\n", imm16(buffer, &j)); break ;  
        case 0xf8: printf("clc\n"); break ;
        case 0xfa: printf("cli\n") ; break ;
        case 0xfc: printf("cld\n") ; break ; 
        case 0xfe:
        {
           p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
           switch (s)
           {
              case 0x01:
              {
                 printf("dec %s\n",  rm8(buffer, &j)) ; 
              } break ;
              default: t= 1 ; 
          } 
        } tikrinti(t) ; 
        case 0xff:
        {
           p = j ;  s = ((buffer[++p] & 0x38) >> 3) ;
           switch (s)
           {
              case 0x01:
              {
                 printf("dec %s\n",  rm16(buffer, &j)) ; 
              } break ;
              case 0x06:
              {
                j++ ; printf("push %s\n", m16(buffer, &j)) ; 
              } break ;
              default: t= 1 ;    
          }  
        }  tikrinti(t) ;  
        default: printf("0x%x\n", buffer[j]) ;  
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
        case 0x00: sprintf(rm_var,"[bx+si%c%x]", sign,  disp) ; break ; 
        case 0x01: sprintf(rm_var,"[bx+di%c%x]", sign,  disp) ; break ; 
        case 0x02: sprintf(rm_var,"[bp+si%c%x]", sign,  disp) ; break ;
        case 0x03: sprintf(rm_var,"[bp+di%c%x]", sign,  disp) ; break ;
        case 0x04: sprintf(rm_var,"[si%c%x]", sign,  disp) ; break ;
        case 0x05: sprintf(rm_var,"[di%c%x]", sign,  disp) ; break ;
        case 0x06:
                 {
                    if (dip == 1)
                    {
                       (*j)++ ; 
                       uchar low = buffer[*j] ; 
                       (*j)++ ; 
                       uchar high = buffer[*j] ; 
                       disp = ((high << 8) + low) ;
                       sprintf(rm_var,"[%hx]", disp) ; 
                    }
                    else  {  sprintf(rm_var,"[bp%hx]",  disp) ; } break ; 
                 } break ; 
        case 0x07: {  sprintf(rm_var,"[bx%c%hx]", sign, disp) ; } break ; 
    }
  return rm_var ; 
}

char *rm8_imm8(uchar *buffer, long *j )
{
   memset(var, '\0', 64) ; 
   char *s = rm(buffer, j, 8) ;
   (*j)++ ;
   sprintf(var, "%s, 0x%x",s, buffer[*j]) ;     
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
   if (abs(c) == c) sprintf(var, "%s, byte +0x%x",s, c) ;     
   else {
   c = -c ; 
   sprintf(var, "%s, byte -0x%x",s, c) ;     
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


