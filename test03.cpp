#include "stdafx.h"
#include "myhead.h"


unsigned char testfunction(void)
{
	
	
		__asm
		{
			
			mov ax,0x04567
		}
	

	
		return 0;
	
}

char *logicinstrarray[8] ={
	"add",
	"or" ,
	"adc",
	"sbb",
	"and",
	"sub",
	"xor",
	"cmp",
};
char *dainstrarray[8] ={
	"daa",
	"das" ,
	"aaa",
	"aas",
};
char *jccinstrarray[16] ={
	"jo",
	"jno" ,
	"jb",
	"jnb",
	"je",
	"jnz",
	"jbe",
	"ja",
	"js",
	"jns",
	"jpe",
	"jpo",
	"jl",
	"jle",
	"jg",
};
char *setccinstrarray[16] ={
	"seto",
	"setno",
	"setb",
	"setae",
	"sete",
	"setne",
	"setbe",
	"seta",
	"sets",
	"setns",
	"setpe",
	"setpo",
	"setl",
	"setge",
	"setle",
	"setg",
};
char *flaginstrarray[8] ={
	"cwde",
	"cdq" ,
	"",
	"wait",
	"pushfd",
	"popfd",
	"sahf",
	"lahf",
};
char *stringinstrarray[4] ={
	"",
	"stos",
	"lods",
	"scas",
};

char *shiftinstrarray[8] ={
	"rol",
	"ror",
	"rcl",
	"rcr",
	"shl",
	"shr",
	"sal",
	"sar",
};

char *loopinstrarray[4] ={
	"loopdne",
	"loopde",
	"loopd",
	"jecxz",
};

char *jmpinstrarray[4] ={
	"call",
	"jmp",
	"jmp far",
	"jmp short",
};

char *unaryinstrarray[8] ={
	"test",
	"test",
	"not",
	"neg",
	"mul",
	"imul",
	"div",
	"idiv",
};

char *flagbitinstrarray[8] ={
	"clc",
	"stc",
	"cli",
	"sti",
	"cld",
	"std",
};

char *feffgrpinstrarray[8] ={
	"inc",
	"dec",
	"call",
	"call far",
	"jmp",
	"jmp far",
	"push",
	"",
};
char *descriptorinstrarray[8] ={
	"sldt",
	"str",
	"lldt",
	"ltr",
	"verr",
	"verw",
	"",
	"",
};
char *descriptor0f01meminstrarray[8]={
	"sgdt",
	"sidt",
	"lgdt",
	"lidt",
	"smsw",
	"",
	"lmsw",
	"invlpg",
};

unsigned char prefixarray[11] = {
	0x26,
	0x2e,
	0x36,
	0x3e,
	0x64,
	0x65,
	0x66,
	0x67,
	0xf0,
	0xf2,
	0xf3,
};
//------------------------------------------------

char *register32bitname[8] = {
	"eax",
	"ecx",
	"edx",
	"ebx",
	"esp",
	"ebp",
	"esi",
	"edi",
};
char *register16bitname[8] = {
	"ax",
	"cx",
	"dx",
	"bx",
	"sp",
	"bp",
	"si",
	"di",
};
char *register8bitname[8] = {
	"al",
	"cl",
	"dl",
	"bl",
	"ah",
	"ch",
	"dh",
	"bh",
};
char *segmentregister[8] = {
	"es",
	"cs",
	"ss",
	"ds",
	"fs",
	"gs",
	"seg6:",
	"seg7:",
};
char *addressmethod16bit[8] = {
	"bx+si",
	"bx+di",
	"bp+si",
	"bp+di",
	"si",
	"di",
	"bp",
	"bx",
};

char *xmmregister[8] = {
	"xmm0",
	"xmm1",
	"xmm2",
	"xmm3",
	"xmm4",
	"xmm5",
	"xmm6",
	"xmm7",
};

char *mmregister[8] = {
	"mm0",
	"mm1",
	"mm2",
	"mm3",
	"mm4",
	"mm5",
	"mm6",
	"mm7",
};

char *crregister[8] = {
	"cr0",
	"cr1",
	"cr2",
	"cr3",
	"cr4",
	"cr5",
	"cr6",
	"cr7",
};

char *drregister[8] = {
	"dr0",
	"dr1",
	"dr2",
	"dr3",
	"dr4",
	"dr5",
	"dr6",
	"dr7",
};
//------------------------------------------------


char base[10] = "";
char displacement[10] = "";
char registername[10] = "";
char index[10] = "";
char scale[10] = "";
char immediate[10] = "";
char segment[10] = "";
char ptr[20] = "";
char instruction[30] = "";

char immbuffer[20] = "";
int flag = 1;//flag=1:two operand; flag=0:one operand
unsigned char prefix[0x80] = {0};
unsigned char prefix_buff[0x80] = {0};
unsigned char optype_addmethod;

char** p_register;

//-------------------------------------------------------------------------------------------------//
char * getimm(unsigned char * buff,int start,int size,int isunsigned)
{
	int imm = 0 ;
	for(int i = start+size-1;i>=start;i--)
	{
		imm = imm<<8;
		imm +=buff[i];
	}
	int sign;
	if(size == 1)
	{
		sign = imm&0x80;

		if(isunsigned)
			sprintf(immbuffer,"0x%02x",imm);
		else
			sprintf(immbuffer,sign?"-0x%02x":"+0x%02x",imm&0x7f);
	}
	else if (size == 4)
	{
		sign = imm&0x80000000;

		if(isunsigned)
			sprintf(immbuffer,"0x%08x",imm);
		else
			sprintf(immbuffer,sign?"-0x%08x":"+0x%08x",sign?(0-imm):imm);
	}else if(size == 2)
	{
		sign = imm&0x8000;

		if(isunsigned)
			sprintf(immbuffer,"0x%04x",imm);
		else
			sprintf(immbuffer,sign?"-0x%04x":"+0x%04x",sign?(0-imm):imm);
	}
	return immbuffer;
}

int findprefix(unsigned char pf)
{
	int len = sizeof(prefix)/sizeof(prefix[0]);
	
	for(int i = 0;i<len;i++)
	{
		if(prefix[i] == pf)
			return 1;
	}
	return 0;
}

int mysizeof(unsigned char * arr)
{
	int i;
	for(i = 0;i<=0x80;i++)
	{
		if(arr[i] == 0)
			break;
	}
	return i;
}

int detect_prefix_conflict(unsigned char pf,unsigned char pf_buff)
{
	int flag1 = 0 ,i,flag2 = 0;
	unsigned char segment_prefix[6] = {0x26,0x2e,0x36,0x3e,0x64,0x65};
	unsigned char rep_prefix[2] = {0xf2,0xf3};
	int len;
	len = sizeof(segment_prefix)/sizeof(segment_prefix[0]);
	for(i = 0; i< len ;i++)
	{
		if(segment_prefix[i] == pf)
			flag1 =1;
		if (segment_prefix[i] == pf_buff)
			flag2 = 1;
	}

	if((flag1 == 1) & (flag2 == 1))
		return 1;

	flag1 = 0;
	flag2 = 0;
	len = sizeof(rep_prefix)/sizeof(rep_prefix[0]);
	for(i = 0; i< len ;i++)
	{
		if(rep_prefix[i] == pf)
			flag1 = 1;
		if(rep_prefix[i] == pf_buff)
			flag2 = 1;
	}

	if((flag1 == 1) & (flag2 == 1))
		return 1;
	else
		return 0;

}
//-------------------------------------------------------------------------------------------------//
int mod_reg_rm(unsigned char *buff,char** p_register)
{
	int length = 2;
	unsigned char mod;
	unsigned char r_m;
	unsigned char reg;
	
	mod = (buff[1]>>6)&3;
	r_m = buff[1]&7;
	reg = (buff[1]>>3)&7;

	if((buff[0]==0x80) | (buff[0]==0x82) | (buff[0]==0x83) | (buff[0]==0xc0) | (buff[0]==0xc6) | (buff[0]==0xf6))
	{
		strcpy(immediate,getimm(buff,2,1,1));
		length += 1;
	}
	else if((buff[0]==0x81) | (buff[0]==0xc1) | (buff[0]==0xc7) | (buff[0]==0xf7))
	{
		if(findprefix(0x66))
		{
			strcpy(immediate,getimm(buff,2,2,1));
			length +=2;
		}
		else
		{
			strcpy(immediate,getimm(buff,2,4,1));
			length += 4;
		}
	}
	else
		strcpy(registername,p_register[reg]);


	switch(mod)
	{		

	case 0:
		if(findprefix(0x67))
		{
			if((buff[1]&7)==6)
			{
				strcpy(base,"");
				strcpy(displacement,(buff[1]&7)==6?getimm(buff,2,2,1):"");
				length += 2;
			}	
			else
				strcpy(base,addressmethod16bit[buff[1]&7]);
		}
		else
		{
			if(r_m == 4);
			else if(r_m == 5)
			{
				strcpy(displacement,getimm(buff,2,4,1));
				length += 4;
			}
			else
			{
				strcpy(base,register32bitname[r_m]);
			}
		}
		break;
	case 1:
		if(findprefix(0x67))
		{
			strcpy(base,addressmethod16bit[buff[1]&7]);
			strcpy(displacement,getimm(buff,2,1,0));
			length += 1;
		}
		else
		{
			if(r_m == 4)
			{
				strcpy(displacement,getimm(buff,3,1,0));
				length += 1;
			}
			else
			{
				strcpy(displacement,getimm(buff,2,1,0));
				strcpy(base,register32bitname[r_m]);
				length += 1;
			}
		}
		break;
	case 2:
		if(findprefix(0x67))
		{
			strcpy(base,addressmethod16bit[buff[1]&7]);
			strcpy(displacement,getimm(buff,2,4,0));
			length += 4;
		}
		else
		{
			if(r_m == 4)
			{
				strcpy(displacement,getimm(buff,3,4,0));
				length += 4;
			}
			else
			{
				strcpy(displacement,getimm(buff,2,4,0));
				strcpy(base,register32bitname[r_m]);
				length += 4;
			}
		}
		break;
	case 3:
		strcpy(base,p_register[r_m]);
		break;
	default:
		break;

	}
	
	return length;
}

int scale_index_base(unsigned char *buff,int length)
{
	if(findprefix(0x67))
		return length;


	unsigned char s;
	unsigned char i;
	unsigned char b;
	
	s = (buff[2]>>6)&3;
	b = buff[2]&7;
	i = (buff[2]>>3)&7;

	if(b==5)
	{
		unsigned char mod = (buff[1]>>6)&3;
		if(mod == 0)
		{
			strcpy(displacement,getimm(buff,3,4,0));
			length += 4;
		}
		else
			strcpy(base,"ebp+");
	}
	else
		strcpy(base,register32bitname[b]);

	if(i != 4)
	{
		sprintf(index,b==5?"%s":"+%s",register32bitname[i]);
		//sprintf(index,"+%s",register32bitname[i]);
		unsigned char product = 1;
		for(int j = 1;j<=s;j++)
			product = 2*product;
			
		sprintf(scale,s==0?"":"*%d",product);
	}
	
	return length+1;
}

//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//



int main()
{

	//--------------------------------------------------------------------------//
	char arr1[0x10] = "asd";
	int length;
	char instrname[0x80];
	
	int i;

	unsigned char *instrbuff;

	//instrbuff = (unsigned char *)&assembler;
	instrbuff = (unsigned char *)&main;

#if 1
	__asm
	{

		mov eax,0x01234568
		__emit(0x2e)
		__emit(0x3e)
		__emit(0x66)
		__emit(0x3e)
		__emit(0x2e)
		__emit(0xf2)
		__emit(0xf3)
		__emit(0xf3)
		mov eax,0x90909090
	}
#endif

	if (*instrbuff==0xe9)
	{
		instrbuff += 5+*(unsigned int*)(instrbuff+1);
	}
	
	unsigned char testarray[] =  {0x26,0x2e,0x26,0x66,0xf2,0xf3,0x26,0x67,0x2e,0x26,0x0f,0x38,0x01,0x01,0x46,0x01,0x02,0x3f,0x8f,0x22,0x3f,0x6e,0x83};
	instrbuff = testarray;
	for (i=0;i<100;i++)
	{
		length = assembler(instrbuff,instrname);
		/*
		for(int j=0;j<length;j++)
		{
			printf("%02x ",instrbuff[j]);
		}
		*/
		if (length==0)
		{
			break;
		}
		printf("%03d length=%02d 0x%08x %s\n",i,length,instrbuff,instrname);

		instrbuff+= length;
	}


	return 0;
}
