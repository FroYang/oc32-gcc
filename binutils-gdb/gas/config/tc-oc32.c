/* tc-oc32.c -- Assemble code for oc32
   Copyright (C) 2023-2024 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to
   the Free Software Foundation, 51 Franklin Street - Fifth Floor,
   Boston, MA 02110-1301, USA.  */


#include "as.h"
#include "safe-ctype.h"
#include "opcode/oc32.h"
#include "elf/oc32.h"
#include "dw2gencfi.h"

/* See md_parse_option() for meanings of these options.  */
static int norelax; /* True if -norelax switch seen.  */

const char comment_chars[] = "#";
const char line_separator_chars[] = ";";
const char line_comment_chars[] = "#";
const char FLT_CHARS[] = "rRsSfFdDxXpP";
const char EXP_CHARS[] = "eE";

static int parse_err;
static char current_instruction[256]; /* 存储完整的指令行 */
static int allow_imm_empty;

/* Instruction type flags (use bit masks) */
#define T_C_A_B (1 << 0)
#define T_A_B_C (1 << 1)
#define T_B_A_SI18 (1 << 2)
#define T_A_B_SI18 (1 << 3)
#define T_A_SI18_B (1 << 4)
#define T_B_A_SI16 (1 << 5)
#define T_B_A_UI16 (1 << 6)
#define T_B_A_UI5 (1 << 7)
#define T_B_A (1 << 8)
#define T_A_B (1 << 9)
#define T_SI20_A (1 << 10)
#define T_A_SI20 (1 << 11)
#define T_A_UI18 (1 << 12)
#define T_UI18_A (1 << 13)
#define T_UI18_UI5 (1 << 14)
#define T_A_UI5 (1 << 15)
#define T_A_SI16 (1 << 16)
#define T_A_UI16 (1 << 17)
#define T_A_HI16 (1 << 18)
#define T_A (1 << 19)
#define T_SI30 (1 << 20)
#define T_UI16 (1 << 21)
#define T_B_UI5_UI5_A (1 << 22)
#define T_B_A_UI5_UI5 (1 << 23)
#define T_OP (1 << 24)

/* Indirect addressing flags */
#define D_IND1 (1 << 0)
#define D_IND2 (1 << 1)
#define D_IND3 (1 << 2)
#define S_IND1 (1 << 3)
#define S_IND2 (1 << 4)
#define S_IND3 (1 << 5)

/* instruction info */
typedef struct
{
  int Rop;
  int Iop;
  int type_flags;
  int ind_flags;
  int reloc_type;
} oc32_instruction_info_t;

/* Instruction table entry */
struct oc32_opcode_info
{
  const char *name;
  int Rop;
  int Iop;
  int type_flags;
  int ind_flags;
  int reloc_type;
};

/* Instruction table */
static const struct oc32_opcode_info oc32_opcode_table[] =
    {
        {"ADD", OP_ADD_R, OP_ADD_I, T_C_A_B | T_B_A_SI16, 0, BFD_RELOC_OC32_D_ABSLO},
        {"ADDC", OP_ADDC_R, OP_ADDC_I, T_C_A_B | T_B_A_SI16, 0, 0},
        {"SUBB", OP_SUBB_R, OP_SUBB_I, T_C_A_B | T_B_A_SI16, 0, 0},
        {"MUL", OP_MUL_R, OP_MUL_I, T_C_A_B | T_B_A_SI16, 0, 0},
        {"MULU", OP_MULU_R, OP_MULU_I, T_C_A_B | T_B_A_SI16, 0, 0},
        {"DIV", OP_DIV_R, OP_DIV_I, T_C_A_B | T_B_A_SI16, 0, 0},
        {"DIVU", OP_DIVU_R, OP_DIVU_I, T_C_A_B | T_B_A_SI16, 0, 0},
        {"AND", OP_AND_R, OP_AND_I, T_C_A_B | T_B_A_UI16, 0, 0},
        {"OR", OP_OR_R, OP_OR_I, T_C_A_B | T_B_A_UI16, 0, BFD_RELOC_OC32_D_ABSLO},
        {"XOR", OP_XOR_R, OP_XOR_I, T_C_A_B | T_B_A_UI16, 0, 0},
        {"SUB", OP_SUB_R, 0, T_C_A_B, 0, 0},
        {"NAND", OP_NAND_R, 0, T_C_A_B, 0, 0},
        {"NOR", OP_NOR_R, 0, T_C_A_B, 0, 0},
        {"MOVZ", OP_MOVZ_R, 0, T_C_A_B, 0, 0},
        {"MOVNZ", OP_MOVNZ_R, 0, T_C_A_B, 0, 0},
        {"RR", OP_RR_R, OP_RR_N, T_C_A_B | T_B_A_UI5, 0, 0},
        {"SL", OP_SL_R, OP_SL_N, T_C_A_B | T_B_A_UI5, 0, 0},
        {"SR", OP_SR_R, OP_SR_N, T_C_A_B | T_B_A_UI5, 0, 0},
        {"SA", OP_SA_R, OP_SA_N, T_C_A_B | T_B_A_UI5, 0, 0},
        {"JZ", OP_JZ_R, OP_JZ_P, T_A_B | T_SI20_A, D_IND1, BFD_RELOC_OC32_I_CON20},
        {"JNZ", OP_JNZ_R, OP_JNZ_P, T_A_B | T_SI20_A, D_IND1, BFD_RELOC_OC32_I_CON20},
        {"RDSR", OP_RDSR_R, OP_RDSR_A, T_B_A | T_A_UI18, S_IND1, 0},
        {"WRSR", OP_WRSR_R, OP_WRSR_A, T_A_B | T_UI18_A, D_IND1, 0},
        {"SBSR", OP_SBSR_R, OP_SBSR_A, T_A_UI5 | T_UI18_UI5, D_IND1, 0},
        {"CBSR", OP_CBSR_R, OP_CBSR_A, T_A_UI5 | T_UI18_UI5, D_IND1, 0},
        {"TBSR", OP_TBSR_R, OP_TBSR_A, T_A_UI5 | T_UI18_UI5, D_IND1, 0},
        {"CMP", OP_CMP_R, OP_CMP_I, T_A_B | T_A_SI16, 0, 0},
        {"CMPU", OP_CMPU_R, OP_CMPU_I, T_A_B | T_A_UI16, 0, 0},
        {"MOVH", 0, OP_MOVH_I, T_A_HI16, 0, BFD_RELOC_OC32_D_ABSHI},
        {"CLO", OP_CLO_R, 0, T_B_A, 0, 0},
        {"CLZ", OP_CLZ_R, 0, T_B_A, 0, 0},
        {"SEB", OP_SEB_R, 0, T_B_A, 0, 0},
        {"SEH", OP_SEH_R, 0, T_B_A, 0, 0},
        {"RV", OP_RV_R, 0, T_B_A, 0, 0},
        {"RVB", OP_RVB_R, 0, T_B_A, 0, 0},
        {"J", OP_J_P, OP_J_O, T_SI30 | T_A_SI20, D_IND3, BFD_RELOC_OC32_I_REL30},
        {"JL", OP_JL_P, OP_JL_O, T_SI30 | T_A_SI20, D_IND3, BFD_RELOC_OC32_I_REL30},
        {"SYSC", 0, OP_SYSC, T_UI16, 0, 0},
        {"TRAPZ", OP_TRAPZ, 0, T_A, 0, 0},
        {"TRAPNZ", OP_TRAPNZ, 0, T_A, 0, 0},
        {"MOV", OP_MOV_PSR, OP_MOV_EIA, T_A, 0, 0},
        {"LW", OP_LW_R, OP_LW_O, T_C_A_B | T_B_A_SI18, S_IND2 | S_IND3, 0},
        {"LWI", OP_LWI_R, OP_LWI_O, T_C_A_B | T_B_A_SI18, S_IND1, 0},
        {"LWAS", 0, OP_LWAS_O, T_B_A_SI18, S_IND3, 0},
        {"LB", OP_LB_R, 0, T_C_A_B, S_IND1, 0},
        {"LBU", OP_LBU_R, 0, T_C_A_B, S_IND1, 0},
        {"SW", OP_SW_R, OP_SW_O, T_A_B_C | T_A_SI18_B, D_IND2 | D_IND3, 0},
        {"SWI", OP_SWI_R, OP_SWI_O, T_A_B_C | T_A_B_SI18, D_IND1, 0},
        {"SWAT", 0, OP_SWAT_O, T_A_SI18_B, D_IND3, 0},
        {"SB", OP_SB_R, 0, T_A_B_C, D_IND1, 0},
        {"EBF", OP_EBF_R, 0, T_B_A_UI5_UI5, 0, 0},
        {"MBF", OP_MBF_R, 0, T_B_UI5_UI5_A, 0, 0},
        {"RETE", OP_RETE, 0, T_OP, 0, 0},
        {"NOP", OP_NOP, 0, T_OP, 0, 0},
        {NULL, 0, 0, 0, 0, 0}};

#define OC32_SHORTOPTS ""
const char md_shortopts[] = OC32_SHORTOPTS;

const struct option md_longopts[] =
    {
        {NULL, no_argument, NULL, 0}};
const size_t md_longopts_size = sizeof(md_longopts);

int md_parse_option(int c ATTRIBUTE_UNUSED, const char *arg ATTRIBUTE_UNUSED)
{
  return 0;
}

void md_show_usage(FILE *stream ATTRIBUTE_UNUSED)
{
}

const pseudo_typeS md_pseudo_table[] =
    {
        {"word", cons, 4}, /* OC32 .word is 32-bit (4 bytes) */
        {0, 0, 0}};

/* This function is called once, at assembler startup time.  */

void md_begin(void)
{
  bfd_set_arch_mach(stdoutput, TARGET_ARCH, 0);
  if (!norelax)
    linkrelax = 1;
}

/* Parse an expression and then restore the input line pointer.  */

static char *
parse_exp_save_ilp(char *s, expressionS *op)
{
  char *save = input_line_pointer;

  input_line_pointer = s;
  expression(op);
  s = input_line_pointer;
  input_line_pointer = save;
  return s;
}

/* Helper macros for instruction encoding */
#define ENCODE_REG(reg, shift) ((reg) << (shift))
#define ENCODE_IMM(imm, mask, shift) (((imm) & (mask)) << (shift))
#define ENCODE_IMM_SHIFT(imm, mask, shift, shr) ((((imm) & (mask)) >> (shr)) << (shift))

/* Handle t_si20_a type instruction */
static void
handle_t_si20_a(oc32_instruction_info_t *instinfo, int imm1, int ra, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM_SHIFT(imm1, 0xFFFFF, 5, 2);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_ui18_a type instruction */
static void
handle_t_ui18_a(oc32_instruction_info_t *instinfo, int imm1, int ra, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM_SHIFT(imm1, 0x3FFFF, 5, 2);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_b_a_si18 / t_a_si18_b type instruction */
static void
handle_t_b_a_si18(oc32_instruction_info_t *instinfo, int imm1, int ra, int rb, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM_SHIFT(imm1, 0x3FFFF, 10, 2);
  *b |= ENCODE_REG(rb, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_ui18_ui5 type instruction */
static void
handle_t_ui18_ui5(oc32_instruction_info_t *instinfo, int imm1, int imm2, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM_SHIFT(imm1, 0x3FFFF, 5, 2);
  *b |= ENCODE_IMM(imm2, 0x1F, 0);
}

/* Handle t_si30 type instruction */
static void
handle_t_si30(oc32_instruction_info_t *instinfo, int imm1, unsigned int *b)
{
  *b |= instinfo->Rop;
  *b |= ENCODE_IMM_SHIFT(imm1, 0x3FFFFFFF, 0, 2);
}

/* Handle t_ui16 type instruction */
static void
handle_t_ui16(oc32_instruction_info_t *instinfo, int imm1, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM(imm1, 0xFFFF, 0);
}

/* Handle t_c_b_a / t_a_b_c type instruction */
static void
handle_t_c_b_a(oc32_instruction_info_t *instinfo, int ra, int rb, int rc, unsigned int *b)
{
  *b |= instinfo->Rop;
  *b |= ENCODE_REG(rc, 10);
  *b |= ENCODE_REG(rb, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_b_a_si16 type instruction */
static void
handle_t_b_a_si16(oc32_instruction_info_t *instinfo, int imm1, int ra, int rb, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM(imm1, 0xFFFF, 10);
  *b |= ENCODE_REG(rb, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_b_a_ui16 type instruction */
static void
handle_t_b_a_ui16(oc32_instruction_info_t *instinfo, int imm1, int ra, int rb, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM(imm1, 0xFFFF, 10);
  *b |= ENCODE_REG(rb, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_b_a_ui5 type instruction */
static void
handle_t_b_a_ui5(oc32_instruction_info_t *instinfo, int imm1, int ra, int rb, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM(imm1, 0x1F, 10);
  *b |= ENCODE_REG(rb, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_b_a type instruction */
static void
handle_t_b_a(oc32_instruction_info_t *instinfo, int ra, int rb, unsigned int *b)
{
  *b |= instinfo->Rop;
  *b |= ENCODE_REG(rb, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_a_ui18 type instruction */
static void
handle_t_a_ui18(oc32_instruction_info_t *instinfo, int imm1, int ra, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM_SHIFT(imm1, 0x3FFFF, 5, 2);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_a_ui5 type instruction */
static void
handle_t_a_ui5(oc32_instruction_info_t *instinfo, int imm1, int ra, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM(imm1, 0x1F, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_a_si16 type instruction */
static void
handle_t_a_si16(oc32_instruction_info_t *instinfo, int imm1, int ra, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM(imm1, 0xFFFF, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_a_ui16 type instruction */
static void
handle_t_a_ui16(oc32_instruction_info_t *instinfo, int imm1, int ra, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM(imm1, 0xFFFF, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_a_hi16 type instruction */
static void
handle_t_a_hi16(oc32_instruction_info_t *instinfo, int imm1, int ra, unsigned int *b)
{
  *b |= instinfo->Iop;
  *b |= ENCODE_IMM((imm1 >> 16), 0xFFFF, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_a type instruction */
static void
handle_t_a(oc32_instruction_info_t *instinfo, int ra, unsigned int *b, unsigned int select_op)
{
  if (select_op == 0)
  {
    *b |= instinfo->Rop;
  }
  else
  {
    *b |= instinfo->Iop;
  }
  *b |= ENCODE_REG(ra, 0);
}

/* Handle t_b_ui5_ui5_a / t_b_a_ui5_ui5 type instruction */
static void
handle_t_b_ui5_ui5_a(oc32_instruction_info_t *instinfo, int imm1, int imm2, int ra, int rb, unsigned int *b)
{
  *b |= instinfo->Rop;
  *b |= ENCODE_IMM(imm1, 0x1F, 15);
  *b |= ENCODE_IMM(imm2, 0x1F, 10);
  *b |= ENCODE_REG(rb, 5);
  *b |= ENCODE_REG(ra, 0);
}

/* Parse immediate value and set parse_err if not immediate */
static int
parse_imm(char **ptr, oc32_instruction_info_t *instinfo, char *output, int reloc_valid)
{
  char *s = *ptr;
  int imm1 = 0;
  int is_negative = 0;
  int reloc_type_offset = 0;


  if (parse_err)
  {
    return 0;
  }

  /* Skip any leading whitespace */
  while ((*s == ',') || (*s == ' ') || (*s == '\t'))
    s++;

  /* check for BFD_RELOC_OC32_D_ABSHA mark */
  //if ((*s == 'H') && (s[1] == 'A') && (s[2] == ':'))
  if (strncasecmp(s, "HA:", 3) == 0)
  {
    reloc_type_offset = 1;
    s = s + 3;
  }
  /* not support yet */
  else if (strncasecmp(s, "GOT:", 4) == 0)
  {
    reloc_type_offset = 3;
    s = s + 4;
  }

  /* Check for negative sign */
  if (*s == '-')
  {
    is_negative = 1;
    s++;
  }
  else if (*s == '+')
  {
    s++;
  }

  /* Check if it's a number (immediate value) */
  if (ISDIGIT(*s))
  {
    /* Check for hex prefix 0x or 0X */
    //if (*s == '0' && (s[1] == 'x' || s[1] == 'X'))
    if (strncasecmp(s, "0x", 2) == 0)
    {
      s += 2;
      while (ISXDIGIT(*s))
      {
        imm1 = imm1 * 16;
        if (ISDIGIT(*s))
          imm1 += *s - '0';
        else if (*s >= 'a' && *s <= 'f')
          imm1 += *s - 'a' + 10;
        else
          imm1 += *s - 'A' + 10;
        s++;
      }
    }
    /* Check for binary prefix 0b or 0B */
    //else if (*s == '0' && (s[1] == 'b' || s[1] == 'B'))
    else if (strncasecmp(s, "0b", 2) == 0)
    {
      s += 2;
      while (*s == '0' || *s == '1')
      {
        imm1 = imm1 * 2 + (*s - '0');
        s++;
      }
    }
    /* Check for octal prefix 0 */
    else if (*s == '0' && ISDIGIT(s[1]))
    {
      s++;
      while (*s >= '0' && *s <= '7')
      {
        imm1 = imm1 * 8 + (*s - '0');
        s++;
      }
    }
    /* Decimal number */
    else
    {
      while (ISDIGIT(*s))
      {
        imm1 = imm1 * 10 + (*s - '0');
        s++;
      }
    }

    if (is_negative)
      imm1 = -imm1;
  }
  else if (allow_imm_empty != 0)
  {
    /* for J/JL/LW/LWAS/SW/SWAT [Reg+offset] type, when [Reg] only is allowed */
    imm1 = 0;
  }
  else
  {
    /* Not a number, try to parse as symbol reference */
    expressionS expr;
    char *new_s = parse_exp_save_ilp(s, &expr);

    /* Update pointer to after the expression */
    s = new_s;

    /* Handle different expression types */
    switch (expr.X_op)
    {
    case O_constant:
      /* Handle constant expression */
      imm1 = expr.X_add_number;
      break;

    case O_symbol:
    {
      /* Debug: Log symbol reference to file */
      /*
      {
        FILE *debug_fp = fopen("./symbol.log", "a");
        if (debug_fp)
        {
          const char *sym_name = S_GET_NAME(expr.X_add_symbol);
          fprintf(debug_fp, "<%s>,  type=%d  \t// %s\n",
                  sym_name ? sym_name : "<null>", (instinfo->reloc_type + absha), current_instruction);
          fclose(debug_fp);
        }
      } 
      */

      if ((instinfo->reloc_type) && (reloc_valid != 0))
      {
        /* Create fixup for symbol reference, type+1 if it is BFD_RELOC_OC32_D_ABSHA
           BFD_RELOC_OC32_D_ABSHA=BFD_RELOC_OC32_D_ABSHI+1 */
        bfd_reloc_code_real_type reloc_type = instinfo->reloc_type + reloc_type_offset;
        fixS *fix = fix_new(frag_now, output - frag_now->fr_literal, 4,
                            expr.X_add_symbol, expr.X_add_number, 0, reloc_type);

        if (fix == NULL)
        {
          parse_err = 1;
          return -1;
        }
      }
      else
      {
        /* Handle other expression types - error */
        parse_err = 1;
        return -1;
      }
    }
    break;

    default:
      /* Handle other expression types - error */
      parse_err = 1;
      return -1;
    }
  }

  /* Skip any trailing whitespace */
  /* Skip any leading whitespace, connection char */
  while ((*s == ',') || (*s == ' ') || (*s == '\t') || (*s == ']'))
    s++;

  /* Update the pointer to point after the immediate value */
  *ptr = s;
  return imm1;
}

/* Helper function to check if character is a delimiter */
static int
is_delimiter(char c)
{
  return (c == ',' || c == ' ' || c == '\t' || c == '+' ||
          c == ']' || c == '\0' || c == '\n' || c == '\r');
}

/* Parse a gpr and set parse_err if not a gpr */
static int
parse_gpr(char **ptr, int imm0_is_r0)
{
  int reg;
  char *s = *ptr;

  if (parse_err)
  {
    return 0;
  }

  /* Skip any leading whitespace, connection char */
  while ((*s == ',') || (*s == ' ') || (*s == '\t') || (*s == '+') || (*s == '['))
    s++;

  if (*s != 'R' && *s != 'r')
  {
    /* Case 1: empty char stands for R0*/
    if ((*s == '\r') || (*s == '\n') || (*s == '\0'))
    {
      *ptr = s;
      parse_err = 1;
      return 2;
    }
    /* Case 2: Number 0 stands for R0 */
    else if (*s == '0' && is_delimiter(*(s + 1)) && imm0_is_r0)
    {
      reg = 0;
      s += 1;
      goto done;
    }
    /* Case 3: PC ==> R1 */
    else if (strncasecmp(s, "PC", 2) == 0)
      {
      reg = 1;
      s += 2;
      goto done;
    }
    /* Case 4: OV, DZ ==> R2 */
    else if ((strncasecmp(s, "OV", 2) == 0) || (strncasecmp(s, "DZ", 2) == 0))
      {
      reg = 2;
      s += 2;
      goto done;
    }
    /* Case 5: EQ ==> R3 */
    else if (strncasecmp(s, "EQ", 2) == 0)
      {
      reg = 3;
      s += 2;
      goto done;
    }
    /* Case 6: LT ==> R4 */
    else if (strncasecmp(s, "LT", 2) == 0)
      {
      reg = 4;
      s += 2;
      goto done;
    }
    /* Case 7: GT ==> R5 */
    else if (strncasecmp(s, "GT", 2) == 0)
      {
      reg = 5;
      s += 2;
      goto done;
    }
    /* Case 8: HP,RM ==> R6 */
    else if ((strncasecmp(s, "HP", 2) == 0) || (strncasecmp(s, "RM", 2) == 0))
      {
      reg = 6;
      s += 2;
      goto done;
    }  
    /* Case 9: LK ==> R7 */
    else if (strncasecmp(s, "LK", 2) == 0)
      {
      reg = 7;
      s += 2;
      goto done;
    } 
    /* Case 10: SP ==> R8 */
    else if (strncasecmp(s, "SP", 2) == 0)
      {
      reg = 8;
      s += 2;
      goto done;
    }   
    /* Case none: not Rx */
    else
    {
      parse_err = 1;
      return -1;
    }
  }

  if (ISDIGIT(s[1]))
  {
    reg = s[1] - '0';
    if (ISDIGIT(s[2]))
    {
      reg = reg * 10 + (s[2] - '0');
      if (reg > 31)
      {
        parse_err = 1;
        return -1;
      }
      s += 3;
    }
    else
    {
      s += 2;
    }
  }
  else
  {
    parse_err = 1;
    return -1;
  }

done:
  /* Skip any leading whitespace, connection char */
  while ((*s == ',') || (*s == ' ') || (*s == '\t') || (*s == ']'))
    s++;

  /* Update the pointer to point after the register */
  *ptr = s;

  return reg;
}

/* Parse a special core register and set parse_err if not a gpr */
static int
parse_scr(char **ptr)
{
  char *s = *ptr;

  /* PSR */
  if ((s[0] == 'P' || s[0] == 'p') && (s[1] == 'S' || s[1] == 's') && (s[2] == 'R' || s[2] == 'r'))
  {
    return 0;
  }

  /* EIA */
  if ((s[0] == 'E' || s[0] == 'e') && (s[1] == 'I' || s[1] == 'i') && (s[2] == 'A' || s[2] == 'a'))
  {
    return 1;
  }

  parse_err = 1;
  return -1;
}

/* Look up instruction by name */
static const struct oc32_opcode_info *
oc32_lookup_opcode(const char *name)
{
  const struct oc32_opcode_info *op;

  for (op = oc32_opcode_table; op->name != NULL; op++)
  {
    if (strcasecmp(name, op->name) == 0)
      return op;
  }
  return NULL;
}

/* Identify instruction and return instruction info */
static oc32_instruction_info_t
oc32_identify_instruction(const char *instname)
{
  oc32_instruction_info_t info = {0};
  const struct oc32_opcode_info *op;

  op = oc32_lookup_opcode(instname);
  if (op == NULL)
    return info;

  info.Rop = op->Rop;
  info.Iop = op->Iop;
  info.type_flags = op->type_flags;
  info.ind_flags = op->ind_flags;
  info.reloc_type = op->reloc_type;

  return info;
}

/* This is the guts of the machine-dependent assembler.  STR points to
   a machine dependent instruction.  This function is supposed to emit
   the frags/bytes it assembles to.  */

void md_assemble(char *str)
{
  char *op_start;
  char *op_end;
  char *output;
  int idx = 0;
  char pend;
  int nlen = 0;
  unsigned int b = 0;

  /* 保存完整的指令行 */
  strncpy(current_instruction, str, sizeof(current_instruction) - 1);
  current_instruction[sizeof(current_instruction) - 1] = '\0'; /* 确保字符串结束 */

  /* Drop leading whitespace.  */
  while (*str == ' ' || *str == '\t')
    str++;

  /* Find the op code end.  */
  op_start = str;
  for (op_end = str;
       *op_end && !ISSPACE(*op_end) && *op_end != '.';
       op_end++)
    nlen++;

  pend = *op_end;
  *op_end = 0;

  if (nlen == 0)
  {
    as_bad(_("can't find opcode "));
    return;
  }

  /* Skip space after opcode */
  char *operand_start = op_end + 1;
  while (ISSPACE(*operand_start))
    operand_start++;

  /* Process instruction */
  char *instname = op_start;
  char *current_operand;
  int ra, rb, rc, imm1, imm2;
  int allow_r0_empty;
  unsigned int select_op;

  // identify instruction name
  oc32_instruction_info_t instinfo = oc32_identify_instruction(instname);

  /* Allocate output frag before parsing operands (needed for fixups) */
  output = frag_more(4);

  // parse the instructions, it will parse_again if op_num=2 and failed at 1st time
parse_again:
  if (instinfo.type_flags == 0)
  {
  asm_err:
    as_bad(_("OC32 does not support <%s> or it has wrong operands"), current_instruction);
    *op_end = pend;
    return;
  }

  // allow empty imm type
  allow_imm_empty = instinfo.type_flags & (T_A_SI20 | T_B_A_SI18 | T_A_SI18_B);

  // allow empty R0 type
  allow_r0_empty = (instinfo.Rop == OP_SW_R) || (instinfo.Rop == OP_SWI_R) || (instinfo.Rop == OP_SB_R) ||
                   (instinfo.Rop == OP_LW_R) || (instinfo.Rop == OP_LWI_R) || (instinfo.Rop == OP_LB_R) ||
                   (instinfo.Rop == OP_LBU_R);

  parse_err = 0;
  ra = 0;
  rb = 0;
  rc = 0;
  imm1 = 0;
  imm2 = 0;
  b = 0;
  select_op = 0;
  current_operand = operand_start;

  // parse base on different types of instructions

  // Do not change the if order!

  // type: inst
  if (instinfo.type_flags & T_OP)
  {
    b = instinfo.Rop;
  }

  // type: inst Rc, Ra, Rb
  // load: LW/LB/LBU Rc, [Ra+R0] => LW/LB/LBU Rc, [Ra]
  else if (instinfo.type_flags & T_C_A_B)
  {
    rc = parse_gpr(&current_operand, 1);
    ra = parse_gpr(&current_operand, 1);
    rb = parse_gpr(&current_operand, (instinfo.reloc_type == 0));
    /* check skip R0 or not */
    if ((rb == 2) && (parse_err == 1) && allow_r0_empty)
    {
      rb = 0;
      parse_err = 0;
    }

    handle_t_c_b_a(&instinfo, ra, rb, rc, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_C_A_B;
      goto parse_again;
    }
  }

  // type: inst Ra, Rb, Rc
  // store: SW/SWI/SB [Ra+R0], Rc => SW/SWI/SB [Ra], Rc
  else if (instinfo.type_flags & T_A_B_C)
  {
    ra = parse_gpr(&current_operand, 1);
    rb = parse_gpr(&current_operand, 1);
    rc = parse_gpr(&current_operand, 1);
    /* check skip R0 or not */
    if ((rc == 2) && (parse_err == 1) && allow_r0_empty)
    {
      rc = rb;
      rb = 0;
      parse_err = 0;
    }

    handle_t_c_b_a(&instinfo, ra, rb, rc, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_B_C;
      goto parse_again;
    }
  }

  // type: inst Rb, Ra
  else if (instinfo.type_flags & T_B_A)
  {
    rb = parse_gpr(&current_operand, 1);
    ra = parse_gpr(&current_operand, 1);
    handle_t_b_a(&instinfo, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_B_A;
      goto parse_again;
    }
  }

  // type: inst Ra, Rb
  else if (instinfo.type_flags & T_A_B)
  {
    ra = parse_gpr(&current_operand, (instinfo.reloc_type == 0));
    rb = parse_gpr(&current_operand, 1);
    handle_t_b_a(&instinfo, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_B;
      goto parse_again;
    }
  }

  // type: inst Ra
  else if (instinfo.type_flags & T_A)
  {
    ra = parse_gpr(&current_operand, 1);
    select_op = parse_scr(&current_operand);
    handle_t_a(&instinfo, ra, &b, select_op);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A;
      goto parse_again;
    }
  }

  // type: inst Rb, Ra, simm18
  else if (instinfo.type_flags & T_B_A_SI18)
  {
    rb = parse_gpr(&current_operand, 1);
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_b_a_si18(&instinfo, imm1, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_B_A_SI18;
      goto parse_again;
    }
  }

  // type: inst Ra, Rb, simm18
  else if (instinfo.type_flags & T_A_B_SI18)
  {
    ra = parse_gpr(&current_operand, 1);
    rb = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_b_a_si18(&instinfo, imm1, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_B_SI18;
      goto parse_again;
    }
  }

  // type: inst Rb, Ra, simm16
  else if (instinfo.type_flags & T_B_A_SI16)
  {
    rb = parse_gpr(&current_operand, 1);
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, instinfo.reloc_type);
    handle_t_b_a_si16(&instinfo, imm1, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_B_A_SI16;
      goto parse_again;
    }
  }

  // type: inst Rb, Ra, uimm16
  else if (instinfo.type_flags & T_B_A_UI16)
  {
    rb = parse_gpr(&current_operand, 1);
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, instinfo.reloc_type);
    handle_t_b_a_ui16(&instinfo, imm1, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_B_A_UI16;
      goto parse_again;
    }
  }

  // type: inst Rb, Ra, uimm5, uimm5
  else if (instinfo.type_flags & T_B_A_UI5_UI5)
  {
    rb = parse_gpr(&current_operand, 1);
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    imm2 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_b_ui5_ui5_a(&instinfo, (imm1 - 1), imm2, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_B_A_UI5_UI5;
      goto parse_again;
    }
  }

  // type: inst Rb, Ra, uimm5
  else if (instinfo.type_flags & T_B_A_UI5)
  {
    rb = parse_gpr(&current_operand, 1);
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_b_a_ui5(&instinfo, imm1, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_B_A_UI5;
      goto parse_again;
    }
  }

  // type: inst Ra, simm18, Rb
  else if (instinfo.type_flags & T_A_SI18_B)
  {
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    rb = parse_gpr(&current_operand, 1);
    handle_t_b_a_si18(&instinfo, imm1, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_SI18_B;
      goto parse_again;
    }
  }

  // type: inst Ra, simm20
  else if (instinfo.type_flags & T_A_SI20)
  {
    ra = parse_gpr(&current_operand, (instinfo.reloc_type == 0));
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_si20_a(&instinfo, imm1, ra, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_SI20;
      goto parse_again;
    }
  }

  // type: inst Ra, uimm18
  else if (instinfo.type_flags & T_A_UI18)
  {
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_a_ui18(&instinfo, imm1, ra, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_UI18;
      goto parse_again;
    }
  }

  // type: inst Ra, uimm5
  else if (instinfo.type_flags & T_A_UI5)
  {
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_a_ui5(&instinfo, imm1, ra, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_UI5;
      goto parse_again;
    }
  }

  // type: inst Ra, simm16
  else if (instinfo.type_flags & T_A_SI16)
  {
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_a_si16(&instinfo, imm1, ra, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_SI16;
      goto parse_again;
    }
  }

  // type: inst Ra, uimm16
  else if (instinfo.type_flags & T_A_UI16)
  {
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_a_ui16(&instinfo, imm1, ra, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_UI16;
      goto parse_again;
    }
  }

  // type: inst Ra, himm16
  else if (instinfo.type_flags & T_A_HI16)
  {
    ra = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, instinfo.reloc_type);
    handle_t_a_hi16(&instinfo, imm1, ra, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_A_HI16;
      goto parse_again;
    }
  }

  // type: inst Rb, uimm5, uimm5, Ra
  else if (instinfo.type_flags & T_B_UI5_UI5_A)
  {
    rb = parse_gpr(&current_operand, 1);
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    imm2 = parse_imm(&current_operand, &instinfo, output, 0);
    ra = parse_gpr(&current_operand, 1);
    handle_t_b_ui5_ui5_a(&instinfo, imm1 - 1, imm2, ra, rb, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_B_UI5_UI5_A;
      goto parse_again;
    }
  }

  // type: inst simm20, Ra
  else if (instinfo.type_flags & T_SI20_A)
  {
    imm1 = parse_imm(&current_operand, &instinfo, output, instinfo.reloc_type);
    ra = parse_gpr(&current_operand, 1);
    handle_t_si20_a(&instinfo, imm1, ra, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_SI20_A;
      goto parse_again;
    }
  }

  // type: inst uimm18, Ra
  else if (instinfo.type_flags & T_UI18_A)
  {
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    ra = parse_gpr(&current_operand, 1);
    handle_t_ui18_a(&instinfo, imm1, ra, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_UI18_A;
      goto parse_again;
    }
  }

  // type: inst uimm18,uimm5
  else if (instinfo.type_flags & T_UI18_UI5)
  {
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    imm2 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_ui18_ui5(&instinfo, imm1, imm2, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_UI18_UI5;
      goto parse_again;
    }
  }

  // type: inst simm30
  else if (instinfo.type_flags & T_SI30)
  {
    imm1 = parse_imm(&current_operand, &instinfo, output, instinfo.reloc_type);
    handle_t_si30(&instinfo, imm1, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_SI30;
      goto parse_again;
    }
  }

  // type: inst uimm16
  else if (instinfo.type_flags & T_UI16)
  {
    imm1 = parse_imm(&current_operand, &instinfo, output, 0);
    handle_t_ui16(&instinfo, imm1, &b);
    if (parse_err)
    {
      instinfo.type_flags &= ~T_UI16;
      goto parse_again;
    }
  }

  // Error
  else
  {
    goto asm_err;
  }

  /* Output instruction */
  output[idx++] = 0xff & (b >> 0);
  output[idx++] = 0xff & (b >> 8);
  output[idx++] = 0xff & (b >> 16);
  output[idx++] = 0xff & (b >> 24);
  dwarf2_emit_insn(4);

  /* Restore original end character */
  *op_end = pend;
}

/* Turn a string in input_line_pointer into a floating point constant of type
   type, and store the appropriate bytes in *litP.  The number of LITTLENUMS
   emitted is stored in *sizeP .  An error message is returned, or NULL on OK.  */

const char *
md_atof(int type, char *litP, int *sizeP)
{
  return ieee_md_atof(type, litP, sizeP, true);
}

/* Convert from host byte order to target byte order.  */

void md_number_to_chars(char *buf, valueT val, int n)
{
  number_to_chars_littleendian(buf, val, n);
}

/* The syntax in the manual says constants begin with '#'.
   We just ignore it.  */

void md_operand(expressionS *expressionP)
{
  if (*input_line_pointer == '#')
  {
    input_line_pointer++;
    expression(expressionP);
  }
}

/* Apply a fixup.  */

void md_apply_fix(fixS *fixP, valueT *valP, segT seg ATTRIBUTE_UNUSED)
{
  char *location = fixP->fx_frag->fr_literal + fixP->fx_where;
  valueT val = *valP;
  unsigned int insn;
  int i;

  /* Read current instruction */
  insn = 0;
  for (i = 0; i < 4; i++)
    insn |= ((unsigned int)(unsigned char)location[i]) << (i * 8);

  switch (fixP->fx_r_type)
  {
  // ========== 标准的 8/16/32 位数据重定位 ==========
  case BFD_RELOC_8:
    /* 8-bit absolute data (e.g., .byte symbol) */
    location[0] = (val & 0x000000ff);
    fixP->fx_done = 1;
    return;

  case BFD_RELOC_16:
    /* 16-bit absolute data (e.g., .short symbol) */
    location[0] = (val & 0x000000ff);
    location[1] = (val & 0x0000ff00) >> 8;
    fixP->fx_done = 1;
    return;

  case BFD_RELOC_32:
    /* 32-bit absolute data (e.g., .long symbol) */
    location[0] = (val & 0x000000ff);
    location[1] = (val & 0x0000ff00) >> 8;
    location[2] = (val & 0x00ff0000) >> 16;
    location[3] = (val & 0xff000000) >> 24;
    fixP->fx_done = 1;
    return;

  // ========== OC32 指令重定位（保持原有代码） ==========
  case BFD_RELOC_OC32_I_REL30:
    /* J/JL rel30: 30-bit relative relocation */
    /* Shift by 2 because instructions are 4-byte aligned */
    val >>= 2;
    /* Clear the lower 28 bits and set the new value */
    insn &= ~0x0fffffff;
    insn |= (val & 0x0fffffff);
    break;

  case BFD_RELOC_OC32_I_CON20:
    /* JZ/JNZ rel20: 20-bit relative relocation */
    /* Shift by 2 because instructions are 4-byte aligned */
    val >>= 2;
    /* Clear the lower 20 bits and set the new value */
    insn &= ~(0x3ffff << 5);
    insn |= ((val & 0x3ffff) << 5);
    break;

  case BFD_RELOC_OC32_D_ABSLO:
  case BFD_RELOC_OC32_D_GOTLO:
    /* OR Ra, R0, imm: 16-bit absolute low relocation */
    /* Clear the lower 16 bits and set the new value */
    insn &= ~(0xffff << 10);
    insn |= ((val & 0xffff) << 10);
    break;

  case BFD_RELOC_OC32_D_ABSHI:
  case BFD_RELOC_OC32_D_ABSHA:
  case BFD_RELOC_OC32_D_GOTHI:
    /* MOVH Ra, Ra, imm: 16-bit absolute high relocation */
    /* Shift by 16 to get the high 16 bits */
    val >>= 16;
    /* Clear the lower 16 bits and set the new value */
    insn &= ~(0xffff << 10);
    insn |= ((val & 0xffff) << 10);
    break;

  default:
    /* Unknown relocation type */
    as_bad_where(fixP->fx_file, fixP->fx_line,
                 _("unknown relocation type %d"), fixP->fx_r_type);
    return;
  }

  /* Write back the modified instruction */
  for (i = 0; i < 4; i++)
    location[i] = (insn >> (i * 8)) & 0xff;

  fixP->fx_done = 1;
}

/* Functions concerning relocs.  */

/* The location from which a PC relative jump should be calculated,
   given a PC relative reloc.  */

long md_pcrel_from_section(fixS *fixP, segT sec)
{
  if (fixP->fx_addsy != NULL && (!S_IS_DEFINED(fixP->fx_addsy) || (S_GET_SEGMENT(fixP->fx_addsy) != sec) || S_IS_EXTERNAL(fixP->fx_addsy) || S_IS_WEAK(fixP->fx_addsy)))
  {
    /* The symbol is undefined (or is defined but not in this section).
     Let the linker figure it out.  */
    return 0;
  }

  return fixP->fx_frag->fr_address + fixP->fx_where;
}


#define GOT_NAME "_GLOBAL_OFFSET_TABLE_"

arelent *
tc_gen_reloc(asection *section, fixS *fixp)
{
  arelent *reloc;
  bfd_reloc_code_real_type code;

  reloc = notes_alloc(sizeof(arelent));
  reloc->sym_ptr_ptr = notes_alloc(sizeof(asymbol *));
  *reloc->sym_ptr_ptr = symbol_get_bfdsym(fixp->fx_addsy);
  reloc->address = fixp->fx_frag->fr_address + fixp->fx_where;

  if (fixp->fx_pcrel)
  {
    if (section->use_rela_p)
      fixp->fx_offset -= md_pcrel_from_section(fixp, section);
    else
      fixp->fx_offset = reloc->address;
  }
  reloc->addend = fixp->fx_offset;
  code = fixp->fx_r_type;
  reloc->howto = bfd_reloc_type_lookup(stdoutput, code);

  if (reloc->howto == NULL)
  {
    as_bad_where(fixp->fx_file, fixp->fx_line,
                 _("cannot represent %s relocation in this object file format"),
                 bfd_get_reloc_code_name(code));
    as_bad(_("OC32 does not support <%s> with relocation"), current_instruction);
    return NULL;
  }

  return reloc;
}

/* Standard calling conventions leave the CFA at SP on entry.  */

void oc32_cfi_frame_initial_instructions(void)
{
  /* stack pointer register */
  cfi_add_CFA_def_cfa_register(8);
}

/* Validate fixup with fx_subsy */
int oc32_validate_fix_sub(struct fix *fix)
{
  segT add_symbol_segment, sub_symbol_segment;

  /* If no add symbol, not valid */
  if (fix->fx_addsy == NULL)
    return 0;

  add_symbol_segment = S_GET_SEGMENT(fix->fx_addsy);
  sub_symbol_segment = S_GET_SEGMENT(fix->fx_subsy);

  /* Allow if both symbols are in the same section */
  return (sub_symbol_segment == add_symbol_segment);
}

/* TC_FORCE_RELOCATION hook */

/* If linkrelax is turned on, and the symbol to relocate
   against is in a relaxable segment, don't compute the value -
   generate a relocation instead.  */

int oc32_force_relocation(fixS *fix)
{

  return generic_force_reloc(fix);
}

/* Allow local subtraction - needed for DWARF debug info */
bool oc32_allow_local_subtract(expressionS *left ATTRIBUTE_UNUSED,
                               expressionS *right ATTRIBUTE_UNUSED,
                               segT section ATTRIBUTE_UNUSED)
{
  /* Always allow subtraction of symbols in the same section */
  return true;
}
