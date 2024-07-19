#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_LABEL_LENGTH 31 /* maximum length of label according to instructions */
#define MAX_INPUT_LINE 81 /* maximum length of line according to instructions */
#define MAX_LABELS 100 /* for convenience, we've decided on an array of 100 labels per program */
#define IC_START_ADDRESS 100 /* starting address for memory */
#define NUM_OF_COMMANDS 15 /* all legal commands */
#define MAX_CMD_NAME 5 /* length of max cmd name */
#define BASE_DECIMAL 10
#define MAX_OPERANDS 2

/* macro related */
#define MAX_MACRO_NAME 31 /* in forum discussion it was said macros name will be 31 chars at max */
#define MAX_BUFFER 256
#define RESERVED_NAMES 20 /* number of reserved names */
#define EOL_BUFFER 10 /* buffer to check end of line after macro def / endef */

#define IS_REGISTER 1
#define IS_P_REGISTER 2
#define IS_LABEL 3
#define IS_IMMEDIATE 4

#define LOWEST_REG_NUM '0'
#define MAX_REG_NUM '7'

/* shift bits */
#define SHIFT_TO_SOURCE 7
#define SHIFT_TO_TARGET 3
#define SHIFT_TO_OPCODE 11

/* addressing and bits man */
#define REGISTER_ADDRESSING 8
#define REGSTER_P_ADDRESSING 4
#define LABEL_ADDRESSING 2
#define IMMEDIATE_ADDRESSING 1
#define A_BIT 4
#define R_BIT 2
#define E_BIT 1
#define ZERO_UPPER_BITS 0x00007FFF

#endif