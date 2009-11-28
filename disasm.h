#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tikrinti(t) if ( t==0 ) break;

char* reg8[8] = {"al", "cl", "dl","bl","ah","ch","dh","bh"} ; 
char* reg16[8] = {"ax","cx","dx","bx","sp","bp","si", "di"} ;  
char* sreg[4] = {"es", "cs", "ss", "ds"} ; 

typedef unsigned char uchar ;
typedef unsigned short int uint16;  
uchar * open_file(char *name, long *num) ;
void disasm(uchar *buffer, long num) ;
uchar *imm8(uchar *buffer, long *j) ;   
uchar *imm16(uchar *buffer, long *j) ;
char *rm(uchar *buffer, long *j, uchar bit) ;
char *rm8_imm8(uchar *buffer, long *j ) ;
char *rm16_imm16(uchar *buffer, long *j) ;
char *rm16_imm8(uchar *buffer, long *j) ;
char *rm8_r8(uchar *buffer, long *j) ;
char *rm16_r16(uchar *buffer, long *j) ; 
char *r8_rm8(uchar *buffer, long *j) ; 
char *r16_rm16(uchar *buffer, long *j) ;
char *rm8(uchar *buffer, long *j) ;
char *rm16(uchar *buffer, long *j) ;  
char *m16(uchar *buffer, long *j) ; 
char *rm16_sreg(uchar *buffer, long *j) ;
char *sreg_rm16(uchar *buffer, long *j) ;
char *moffs(uchar *buffer, long *j) ; 
char *jump_short(uchar *buffer, long *j) ;
char *jump_near(uchar *buffer, long *j) ;
char *jump_near_indirect(uchar *buffer, long *j) ; 
char *jump_inter_segment(uchar *buffer, long *j) ; 
char *jump_far(uchar *buffer, long *j) ;  
char *call_inter_segment(uchar *buffer, long *j) ; 
char *jmp_short_byte(uchar *buffer, long *j) ; 
char *rel16(uchar *buffer, long *j) ;
char *rel8(uchar *buffer, long *j) ; 