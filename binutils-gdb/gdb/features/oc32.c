/* THIS FILE IS GENERATED.  -*- buffer-read-only: t -*- vi:set ro:
  Original: oc32.xml */

#include "osabi.h"
#include "target-descriptions.h"

const_target_desc_up tdesc_oc32;
static void
initialize_tdesc_oc32 (void)
{
  target_desc_up result = allocate_target_description ();
  set_tdesc_architecture (result.get (), bfd_scan_arch ("oc32"));

  struct tdesc_feature *feature;

  feature = tdesc_create_feature (result.get (), "org.gnu.gdb.oc32.group0");


  tdesc_create_reg (feature, "R0", 0, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R1", 1, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R2", 2, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R3", 3, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R4", 4, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R5", 5, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R6", 6, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R7", 7, 1, NULL, 32, "code_ptr");
  tdesc_create_reg (feature, "R8", 8, 1, NULL, 32, "data_ptr");
  tdesc_create_reg (feature, "R9", 9, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R10", 10, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R11", 11, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R12", 12, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R13", 13, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R14", 14, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R15", 15, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R16", 16, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R17", 17, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R18", 18, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R19", 19, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R20", 20, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R21", 21, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R22", 22, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R23", 23, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R24", 24, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R25", 25, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R26", 26, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R27", 27, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R28", 28, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R29", 29, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R30", 30, 1, NULL, 32, "int");
  tdesc_create_reg (feature, "R31", 31, 1, NULL, 32, "data_ptr");
  tdesc_create_reg (feature, "PPC", 32, 1, NULL, 32, "code_ptr");
  tdesc_create_reg (feature, "NPC", 33, 1, NULL, 32, "code_ptr");

  tdesc_oc32 = std::move (result);
}
