/*
 * GENERATED FILE - DO NOT EDIT
 * (c) Code Red Technologies Ltd, 2008-2013
 * (c) NXP Semiconductors 2013-2013
 * Linker script memory definitions
 * Created from LinkMemoryTemplate
 * By LPCXpresso v6.0.2 [Build 151] [2013-09-18]  on Tue Nov 19 15:45:06 CST 2013)
*/

MEMORY
{
  /* Define each memory region */
  MFlash128 (rx) : ORIGIN = 0x2000, LENGTH = 0x1E000 /* 128k */
  RamLoc8 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x2000 /* 8k */
  RamUsb2 (rwx) : ORIGIN = 0x20004000, LENGTH = 0x800 /* 2k */
  SRAM1_2 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x800 /* 2k */


}
  /* Define a symbol for the top of each memory region */
  __top_MFlash128 = 0x2000 + 0x1E000;
  __top_RamLoc8 = 0x10000000 + 0x2000;
  __top_RamUsb2 = 0x20004000 + 0x800;
  __top_SRAM1_2 = 0x20000000 + 0x800;



