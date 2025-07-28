#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <netinet/in.h>
#include "emulator.h"

#define XSTR(x) STR(x)		//can be used for MAX_ARG_LEN in sscanf
#define STR(x) #x

// These are some macro variables and function to simplify translation between 
// memory addresses and array indices.
#define ADDR_TEXT    0x00400000 //where the .text area starts in which the program lives
#define ADDR_DATA    0x10010000 //where the .text area starts in which the program lives
#define TEXT_POS(a)  ((a==ADDR_TEXT)?(0):(a - ADDR_TEXT)/4) //can be used to access text[]
#define TEXT_ADDR_POS(j)  (j*4 + ADDR_TEXT)                      //convert text index to address
#define DATA_POS(a)  ((a==ADDR_DATA)?(0):(a - ADDR_DATA)/4) //can be used to access data_str[]
#define DATA_ADDR_POS(j)  (j*4 + ADDR_DATA)                      //convert data index to address



const char *register_str[] = {"$zero",
                              "$at", "$v0", "$v1",
                              "$a0", "$a1", "$a2", "$a3",
                              "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                              "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
                              "$t8", "$t9",
                              "$k0", "$k1",
                              "$gp",
                              "$sp", "$fp", "$ra"};

/* These variables are used to store the string representation of the code by the 
 * load_program function */

/* Space for the instructions of the assembler program */
char prog_str[MAX_PROG_LEN][MAX_LINE_LEN];
int prog_len = 0;
/* Array to carry the data section of the assembly program */
char data_str[MAX_PROG_LEN][MAX_LINE_LEN];
int data_len = 0;

/* These arrays should store the content of the text and data sections of the program 
 * affer the make_bytecode function. */
unsigned int data[MAX_PROG_LEN] = {0}; // the text memory with our instructions
unsigned int text[MAX_PROG_LEN] = {0}; // the text memory with our instructions

/* Elements for running the emulator. */
unsigned int registers[MAX_REGISTER] = {0}; // the registers
unsigned int pc = 0;                        // the program counter

int error = 0;

/* function to create bytecode for instruction nop
   conversion result is passed in bytecode
   function always returns 0 (conversion OK) */
typedef int (*opcode_function)(unsigned int, unsigned int*, char*, char*, char*, char*);

int add_imi(unsigned int *bytecode, short int imi){
	if (imi<-32768 || imi>32767) return (-1);
	*bytecode|= (0xFFFF & imi);
	return(0);
}

int add_sht(unsigned int *bytecode, int sht){
	if (sht<0 || sht>31) return(-1);
	*bytecode|= (0x1F & sht) << 6;
	return(0);
}

int add_reg(unsigned int *bytecode, char *reg, int pos){
	int i;
	for(i=0;i<MAX_REGISTER;i++){
		if(!strcmp(reg,register_str[i])){
		*bytecode |= (i << pos);
			return(0);
		}
	}
	return(-1);
}

int add_addr(unsigned int *bytecode, int addr){
    *bytecode |= ((addr>>2) & 0x3FFFFF);
    return 0;
}

int add_lbl(unsigned int offset, unsigned int *bytecode, char *label){
	char l[MAX_ARG_LEN+1];
	int j=0;
	while(j<prog_len){
		memset(l,0,MAX_ARG_LEN+1);
		sscanf(&prog_str[j][0],"%" XSTR(MAX_ARG_LEN) "[^:]:", l);
		if (label!=NULL && !strcmp(l, label)) return(add_imi( bytecode, j-(offset+1)) );
		j++;
	}
	return (-1);
}

int add_text_addr(unsigned int *bytecode, char *label){
	char l[MAX_ARG_LEN+1];
	int j=0;
	while(j<prog_len){
		memset(l,0,MAX_ARG_LEN+1);
		sscanf(&prog_str[j][0],"%" XSTR(MAX_ARG_LEN) "[^:]:", l);
		if (label!=NULL && !strcmp(l, label)) return(add_addr( bytecode, TEXT_ADDR_POS(j)));
		j++;
	}
	return (-1);
}

int opcode_nop(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0;
	return (0);
}

int opcode_add(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0x20; 				// op,shamt,funct
	if (add_reg(bytecode,arg1,11)<0) return (-1); 	// destination register
	if (add_reg(bytecode,arg2,21)<0) return (-1);	// source1 register
	if (add_reg(bytecode,arg3,16)<0) return (-1);	// source2 register
	return (0);
}

int opcode_addi(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0x20000000; 				// op
	if (add_reg(bytecode,arg1,16)<0) return (-1);	// destination register
	if (add_reg(bytecode,arg2,21)<0) return (-1);	// source1 register
	if (add_imi(bytecode,(short int)strtol(arg3, NULL, 0))) return (-1);	// constant
	return (0);
}

int opcode_andi(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0x30000000; 				// op
	if (add_reg(bytecode,arg1,16)<0) return (-1); 	// destination register
	if (add_reg(bytecode,arg2,21)<0) return (-1);	// source1 register
	if (add_imi(bytecode,(short int)strtol(arg3, NULL, 0))) return (-1);	// constant
	return (0);
}

int opcode_bne(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0x14000000; 				// op
	if (add_reg(bytecode,arg1,21)<0) return (-1); 	// register1
	if (add_reg(bytecode,arg2,16)<0) return (-1);	// register2
	if (add_lbl(offset,bytecode,arg3)) return (-1); // jump
	return (0);
}

int opcode_srl(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0x2; 					// op
	if (add_reg(bytecode,arg1,11)<0) return (-1);   // destination register
	if (add_reg(bytecode,arg2,16)<0) return (-1);   // source1 register
	if (add_sht(bytecode,atoi(arg3))<0) return (-1);// shift
	return(0);
}

int opcode_sll(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0; 					// op
	if (add_reg(bytecode,arg1,11)<0) return (-1);	// destination register
	if (add_reg(bytecode,arg2,16)<0) return (-1); 	// source1 register
	if (add_sht(bytecode,atoi(arg3))<0) return (-1);// shift
	return(0);
}

int opcode_lui(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0x3c000000; 					// op
	if (add_reg(bytecode,arg1,16)<0) return (-1);	// destination register
	if (add_imi(bytecode,(short int)strtol(arg2, NULL, 0))) return (-1);	// constant
	return(0);
}

int opcode_lw(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0x8c000000; 					// op
	if (add_reg(bytecode,arg1,16)<0) return (-1);	// destination register
	if (add_reg(bytecode,arg2,21)<0) return (-1);	// source register
	if (add_imi(bytecode,(short int)strtol(arg3, NULL, 0))) return (-1);	// constant
	return(0);
}

int opcode_sw(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0xAC000000; 					// op
	if (add_reg(bytecode,arg1,16)<0) return (-1);	// destination register
	if (add_reg(bytecode,arg2,21)<0) return (-1);	// source register
	if (add_imi(bytecode,(short int)strtol(arg3, NULL, 0))) return (-1);	// constant
	return(0);
}

int opcode_lhu(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0x94000000; 					// op
	if (add_reg(bytecode,arg1,16)<0) return (-1);	// destination register
	if (add_reg(bytecode,arg2,21)<0) return (-1);	// source register
	if (add_imi(bytecode,(short int)strtol(arg3, NULL, 0))) return (-1);	// constant
	return(0);
}

int opcode_sh(unsigned int offset, unsigned int *bytecode, char *opcode, char *arg1, char *arg2, char *arg3 ){
	*bytecode=0xA4000000; 					// op
	if (add_reg(bytecode,arg1,16)<0) return (-1);	// destination register
	if (add_reg(bytecode,arg2,21)<0) return (-1);	// source register
	if (add_imi(bytecode,(short int)strtol(arg3, NULL, 0))) return (-1);	// constant
	return(0);
}


/* These arrays can be used to make your make_bytecode code more readable. You can use the array opcode_funct to store pointers 
 * to instruction serialization functions. You can then use the array opcode_str in order to match instruction 
 * names to function pointers. */
const char *opcode_str[] = {"nop", "add", "addi", "andi", "bne", "srl", "sll", "lui", "lw", "sw", "lhu", "sh"};
opcode_function opcode_func[] = {&opcode_nop, &opcode_add, &opcode_addi, &opcode_andi, &opcode_bne, &opcode_srl, &opcode_sll, &opcode_lui, &opcode_lw, &opcode_sw, &opcode_lhu, &opcode_sh};

/* a function to print the state of the machine */
int print_registers() {
  int i;
  printf("registers:\n");
  for (i = 0; i < MAX_REGISTER; i++) {
    printf(" %d: %d\n", i, registers[i]);
  }
  printf(" Program Counter: 0x%08x\n", pc);
  return (0);
}

/* a function to print the content of the memory in hex */
int print_memory(int start, int len) {
	int i, end = start + len;
  printf("Memory region (0x%08x):\n\t\t", start);
  for (i = 0; i < 8; i++) 
	printf("\tValue(+%02x)", (i*4));
  printf("\n");
  for (; start < end; start += 0x20) {
	printf("0x%08x\t", start);
  	for (i = 0; i < 8; i++) 
		printf("\t0x%08x", ntohl(data[DATA_POS(start + i*4)]));
	printf("\n");
  }
  return (0);
}
/*A Function that excutes the r type instruction for add, sll, srl. Takes in the first source register (sr); the second source register (sr2);
the destination register (dr); the shift amount (sa); and the function (fc) for an r type instruction.*/
void executeRType(unsigned int sr, unsigned int sr2, unsigned int dr, unsigned int sa, unsigned int fc){
    switch (fc) //Switch used to cycle through the function code of the r-type instruction and excute the correct r-type instruction when called
    {
    case 0x20://ADD case 
        registers[dr] = registers[sr] + registers[sr2];
        break;
    case 0x00://SLL case
        registers[dr] = ( registers[sr2] << sa);
        break;
    case 0x02://SRL case
        registers[dr] = (registers[sr2] >> sa);
        break;
    default:
        break;
    }

}

/*A function that checks if a value is negative or not and output the correct +/- integer*/
unsigned int checkNegative(unsigned int constant){
	if((constant >> 15) == 1){              //Calculates and outputs the negative value of the constant assigned if negative
		constant = -((constant^0xffff) + 1);
		return constant;
	}
	else{                                   //Outputs the positive constant
		return constant;
	}
}

/*This function execute the i-type addi, andi, bne, lhu, lui, lw, sh,sw*/
void executeIType(unsigned int opcode, unsigned int sr, unsigned int dr, unsigned int constant){
    switch (opcode)
    {
    case 0x08://ADDI function preformed
        registers[dr] = registers[sr] + checkNegative(constant);
        break;
    case 0x0C://ANDI function preformed
        registers[dr] = registers[sr] & constant;
        break;
    case 0x05://BNE function
        if(registers[sr] != registers[dr]){
          pc += checkNegative(constant)*4;
        }
        break;
    case 0x25://LHU function, checks that the data position is mod 2 or 4 then executes masking or logical shift to extract the two bytes from the data position in that location
        if((registers[sr] + checkNegative(constant)) % 4 == 0){
          registers[dr] = htonl(data[DATA_POS(registers[sr] + checkNegative(constant))])& 0xffff;
        }else if((registers[sr] + checkNegative(constant))% 2 == 0){
          registers[dr] = htonl(data[DATA_POS(registers[sr] + checkNegative(constant))])>>16;
        }else{          //else statement if trying access the wrong data segment throw eror
          printf("load halfword error\n");
          error = 1;
        }
        break;
    case 0x0F://LUI function
        registers[dr] = constant<<16;
        break;
    case 0x23://LW function, extracts the data store in memory at a specific positon to a register after converting to big endian format
        if(constant % 4 == 0){
          registers[dr] = htonl(data[DATA_POS(registers[sr] + checkNegative(constant))]);
        }else{
          printf("load error\n");
          error = 1;
        }
        break;
    case 0x29://SH function, check to see if adding to the first half or second half of the data and then adds the new 2 byte data to the selected position along with the other half
        if((registers[sr] + checkNegative(constant)) % 4 == 0){
          unsigned int val = (htonl(data[DATA_POS(registers[sr] + checkNegative(constant))])>>16)<<16; //extract data in the position and clears the last 16bits of data
          data[DATA_POS(registers[sr] + checkNegative(constant))] =  ntohl(val +(registers[dr] & 0xffff));//adds the extracted data to the ending 2bytes of data held in the dr register 
        }else if(registers[sr] + checkNegative(constant) % 2 == 0){
          unsigned int val= htonl(data[DATA_POS(registers[sr] + checkNegative(constant-2))])& 0xffff; //extract the second half of the data held in the data position looked at
          data[DATA_POS(registers[sr] + checkNegative(constant))] = ntohl((registers[dr]<<16) + val); //shift the data dr register to take up the first 16bits and add the extracted then save in the selected data position
        }else{
          printf("store halfword error\n");
          error = 1;
        }
        break;
    case 0x2B://SW function, add the value stored in register dr after coverting to little endian to the location specified in memory  
        if(constant % 4 == 0){
            data[DATA_POS(registers[sr] + checkNegative(constant))] = ntohl(registers[dr] & 0xffff);
        }else{
          printf("store error\n");
          error = 1;
        }
        break;
    default:
        break;
    }

}


//function mask the bytcode with respect to the type of instruction
void mask(unsigned int bytecode){
    unsigned int opcode = (bytecode >> 26);
    if(opcode == 0){//masking for r-type instruction if opcode equal to zero
        unsigned int sr = (bytecode >> 21) & 0x1f; //extract the first source register
        unsigned int sr2 = (bytecode >> 16) & 0x1f; //extract the second source register
        unsigned int dr = (bytecode >> 11) & 0x1f; //extract the destination register
        unsigned int sa = (bytecode >> 6) & 0x1f; //extract the shift amount
        unsigned int fc = bytecode & 0x3f; //extract function to be preformed 
		executeRType(sr,sr2,dr,sa,fc);//call to execute the r-type instruction
    }else{//masking for i-type instruction when opcode not 0
        unsigned int sr = (bytecode >> 21) & 0x1f; //extract source register
        unsigned int dr = (bytecode >> 16) & 0x1f; //extract destination register
        unsigned int constant = bytecode  & 0xffff; //extract immediate of i type instruction
		executeIType(opcode,sr,dr,constant); //call to execute i-type instructions

    }
}

/* function to execute bytecode */
int exec_bytecode() {
	printf("EXECUTING PROGRAM ...\n");
   	// set program counter to the start of our program
  	for(pc = ADDR_TEXT; pc < ADDR_TEXT+prog_len*4;pc+=4){ //for loop to loop through the array of bytecode instruction
		unsigned int bytecode = text[TEXT_POS(pc)]; //extract the bytecode from the array
		if(bytecode != 0){ //checks that the bytecode doesn't express a nop instruction, since nop functions do nothing they can be skipped over
			mask(bytecode); //Calls to mask bytecode
		}
    if(error != 0){//if to check if an error has occurred and stop the excution of the program if they had been an error with bytecode instructtions
      printf("Error has ocurred, halting program...\n");
      break;
    }
    print_registers();//prints registers after each line of bytecode is executed
 	}
  pc+=4; //increments pc by 4 atthe end since pc would move to next argument after reading all lines of code to executed
  print_registers(); // prints registers at the end
	print_memory(ADDR_DATA, 0x40); // prints the memory of the computer
  	printf("... DONE!\n");
  	return (0);
}



/* function to create bytecode */
int make_bytecode() {
  unsigned int
      bytecode; // holds the bytecode for each converted program instruction
  int i, j = 0;    // instruction counter (equivalent to program line)

  char label[MAX_ARG_LEN + 1];
  char opcode[MAX_ARG_LEN + 1];
  char arg1[MAX_ARG_LEN + 1];
  char arg2[MAX_ARG_LEN + 1];
  char arg3[MAX_ARG_LEN + 1];
	char data_line[MAX_LINE_LEN], *val;

  printf("\nASSEMBLING PROGRAM ...\n");
  printf("DATA AREA:\n");

  // First need to parse the data section and store the data in array data.
  i = 0;
  while (j < data_len) {
      if (sscanf(
              &data_str[j][0],
              "%" XSTR(MAX_ARG_LEN) "[^:]: .word %[^\n]",
              label, data_line) < 2) { // parse the line with label
        printf("parse data error line %d %s %s\n", j, label, data_line);
        return (-1);
      }

      // parsing data entry values
      val = strtok(data_line, " ");
      while (val != NULL) {
	      *(data + (i++)) = htonl(strtol(val, NULL, 0));
	      printf("%08x: %08x\n", ADDR_DATA + i*4, ntohl(*(data + i - 1)));
	      val = strtok(NULL, " ");
      };
      j++;
   } 

	j = 0;
  printf("TEXT AREA:\n");
  while (j < prog_len) {
  	// These variables will store the elements of each instruction. 
    memset(label, 0, sizeof(label));
    memset(opcode, 0, sizeof(opcode));
    memset(arg1, 0, sizeof(arg1));
    memset(arg2, 0, sizeof(arg2));
    memset(arg3, 0, sizeof(arg3));

    bytecode = 0;

    //First, parse and remove label. 
    if ((val = strchr(&prog_str[j][0], ':')) != NULL) { // check if the line contains a label
			*val = '\0';
			strcpy(label, &prog_str[j][0]);
			val++;
		} else {
			val = &prog_str[j][0];
		}

    // Parse a load/store instruction using an appropriate pattern match
    // We match a 2 strings up to MAX_ARG_LEN characters, a string of chars and a string
    // that should not contain a closing parentheses.
    // If we do not read 4 arguments, the instruction is not load/store
		if (sscanf(val,"%" XSTR(MAX_ARG_LEN) "s %" XSTR(MAX_ARG_LEN) "s %[A-Fa-f0-9-](%[^)]",
          opcode, arg1, arg3, arg2) < 4) { 
      // Reset the last two arguments to empty strings
      memset(arg2, 0, sizeof(arg2));
    	memset(arg3, 0, sizeof(arg3));

    	// Scan using attern for any other supported instruction
    	// The pattern scans for 4 string of max length MAX_ARG_LEN. 
    	// The scanning will return the number matched fields and our instructions 
    	// need at least two arguments.
      if (sscanf(val, "%" XSTR(MAX_ARG_LEN) "s %" XSTR(
            	MAX_ARG_LEN) "s %" XSTR(MAX_ARG_LEN) "s %" XSTR(MAX_ARG_LEN) "s",
          	opcode, arg1, arg2, arg3) < 1) { // parse the line with label
      	printf("parse error line %d\n", j);
      	return (-1);
      }
    }

    for (i=0; i<MAX_OPCODE; i++){
        if (!strcmp(opcode, opcode_str[i]) && ((*opcode_func[i]) != NULL))
        {
            if ((*opcode_func[i])(j, &bytecode, opcode, arg1, arg2, arg3) < 0)
            {
                printf("ERROR: line %d opcode error (assembly: %s %s %s %s)\n", j, opcode, arg1, arg2, arg3);
                return (-1);
            }
            else
            {
                printf("0x%08x 0x%08x\n", ADDR_TEXT + 4 * j, bytecode);
                text[j] = bytecode;
                break;
            }
        }
        if (i == (MAX_OPCODE - 1))
        {
            printf("ERROR: line %d unknown opcode\n", j);
            return (-1);
        }
    }

    j++;
  }
  printf("... DONE!\n");
  return (0);
}

// A helper function to trim whitespace from front and back of a string
char *trimwhitespace(char *str) {
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

/* loading the program into memory */
int load_program(char *filename) {
  int j = 0;
  FILE *f;
  char line[MAX_LINE_LEN], *str;

  printf("LOADING PROGRAM %s ...\n", filename);

  f = fopen(filename, "r");
  if (f == NULL) {
      printf("ERROR: Cannot open program %s...\n", filename);
      return -1;
  }
	
  // skip any empty lines 
  do {
		if (fgets(&line[0], MAX_LINE_LEN, f) == NULL) {
			fprintf(stderr, "ERROR: Input assembly file is empty\n");
			exit(1);
		}
		str = trimwhitespace(line);
	}	while(!strlen(str));

	// Reading the data section of the file
	if (!strcmp(str, ".data")) {
		while (fgets(&line[0], MAX_LINE_LEN, f) != NULL) {

			// Remove whitespaces
			str = trimwhitespace(line);

			// Skip empty lines
			if(!strlen(str)) continue;

			// terminate loop if beginning of data section found
			if (strcmp(str, ".text") == 0) break;
			strcpy(data_str[data_len++], str);
		}
	}
	
	// Reading the assembly text section

	// Skip the .text macro and get the first instruction
	if(!strcmp(str, ".text")) {
		if (fgets(&line[0], MAX_LINE_LEN, f) == NULL) {
			fprintf(stderr, "ERROR: Input assembly file has no instructions\n");
			exit(1);
		}
	}

	// copy instructions to final prog_str array
	do {
		str = trimwhitespace(line);
		if (!strlen(str)) break;
		strcpy(prog_str[prog_len++], str);
	} while (fgets(&line[0], MAX_LINE_LEN, f) != NULL);

  printf("DATA:\n");
  for (j = 0; j < data_len; j++) {
    printf("%d: %s\n", j, &data_str[j][0]);
  }

  printf("PROGRAM:\n");
  for (j = 0; j < prog_len; j++) {
    printf("%d: %s\n", j, &prog_str[j][0]);
  }

  return (0);
}

