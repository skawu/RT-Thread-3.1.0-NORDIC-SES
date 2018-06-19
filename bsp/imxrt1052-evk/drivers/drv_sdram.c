/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-12-04     Tanek        first implementation
 */
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

static uint32_t _RDWORD(uint32_t address)
{    
    return *((uint32_t *)address);
}

static void _WDWORD(uint32_t address, uint32_t value)
{
    *((uint32_t *)address) = value;
}

static void SDRAM_WaitIpCmdDone(void)
{
  unsigned long reg;
  do
  {
    reg = _RDWORD(0x402F003C);
  }while((reg & 0x3) == 0);
}

static void _clock_init(void)
{
    // Enable all clocks
    _WDWORD(0x400FC068,0xffffffff);
    _WDWORD(0x400FC06C,0xffffffff);
    _WDWORD(0x400FC070,0xffffffff);
    _WDWORD(0x400FC074,0xffffffff);
    _WDWORD(0x400FC078,0xffffffff);
    _WDWORD(0x400FC07C,0xffffffff);
    _WDWORD(0x400FC080,0xffffffff);

    _WDWORD(0x400D8030,0x00002001); 
    _WDWORD(0x400D8100,0x001d0000);   
    _WDWORD(0x400FC014,0x00010D40);   
}

static void _sdr_Init(void)
{
    // Config IOMUX
    _WDWORD(0x401F8014, 0x00000000);
    _WDWORD(0x401F8018, 0x00000000);
    _WDWORD(0x401F801C, 0x00000000);
    _WDWORD(0x401F8020, 0x00000000);
    _WDWORD(0x401F8024, 0x00000000);
    _WDWORD(0x401F8028, 0x00000000);
    _WDWORD(0x401F802C, 0x00000000);
    _WDWORD(0x401F8030, 0x00000000);
    _WDWORD(0x401F8034, 0x00000000);
    _WDWORD(0x401F8038, 0x00000000);
    _WDWORD(0x401F803C, 0x00000000);
    _WDWORD(0x401F8040, 0x00000000);
    _WDWORD(0x401F8044, 0x00000000);
    _WDWORD(0x401F8048, 0x00000000);
    _WDWORD(0x401F804C, 0x00000000);
    _WDWORD(0x401F8050, 0x00000000);
    _WDWORD(0x401F8054, 0x00000000);
    _WDWORD(0x401F8058, 0x00000000);
    _WDWORD(0x401F805C, 0x00000000);
    _WDWORD(0x401F8060, 0x00000000);
    _WDWORD(0x401F8064, 0x00000000);
    _WDWORD(0x401F8068, 0x00000000);
    _WDWORD(0x401F806C, 0x00000000);
    _WDWORD(0x401F8070, 0x00000000);
    _WDWORD(0x401F8074, 0x00000000);
    _WDWORD(0x401F8078, 0x00000000);
    _WDWORD(0x401F807C, 0x00000000);
    _WDWORD(0x401F8080, 0x00000000);
    _WDWORD(0x401F8084, 0x00000000);
    _WDWORD(0x401F8088, 0x00000000);
    _WDWORD(0x401F808C, 0x00000000);
    _WDWORD(0x401F8090, 0x00000000);
    _WDWORD(0x401F8094, 0x00000000);
    _WDWORD(0x401F8098, 0x00000000);
    _WDWORD(0x401F809C, 0x00000000);
    _WDWORD(0x401F80A0, 0x00000000);
    _WDWORD(0x401F80A4, 0x00000000);
    _WDWORD(0x401F80A8, 0x00000000);
    _WDWORD(0x401F80AC, 0x00000000);
    _WDWORD(0x401F80B0, 0x00000010); // EMC_39, DQS PIN, enable SION
    _WDWORD(0x401F80B4, 0x00000000);
    _WDWORD(0x401F80B8, 0x00000000);

    // PAD ctrl
    // drive strength = 0x7 to increase drive strength
    // otherwise the data7 bit may fail.
    _WDWORD(0x401F8204, 0x000110F9);
    _WDWORD(0x401F8208, 0x000110F9);
    _WDWORD(0x401F820C, 0x000110F9);
    _WDWORD(0x401F8210, 0x000110F9);
    _WDWORD(0x401F8214, 0x000110F9);
    _WDWORD(0x401F8218, 0x000110F9);
    _WDWORD(0x401F821C, 0x000110F9);
    _WDWORD(0x401F8220, 0x000110F9);
    _WDWORD(0x401F8224, 0x000110F9);
    _WDWORD(0x401F8228, 0x000110F9);
    _WDWORD(0x401F822C, 0x000110F9);
    _WDWORD(0x401F8230, 0x000110F9);
    _WDWORD(0x401F8234, 0x000110F9);
    _WDWORD(0x401F8238, 0x000110F9);
    _WDWORD(0x401F823C, 0x000110F9);
    _WDWORD(0x401F8240, 0x000110F9);
    _WDWORD(0x401F8244, 0x000110F9);
    _WDWORD(0x401F8248, 0x000110F9);
    _WDWORD(0x401F824C, 0x000110F9);
    _WDWORD(0x401F8250, 0x000110F9);
    _WDWORD(0x401F8254, 0x000110F9);
    _WDWORD(0x401F8258, 0x000110F9);
    _WDWORD(0x401F825C, 0x000110F9);
    _WDWORD(0x401F8260, 0x000110F9);
    _WDWORD(0x401F8264, 0x000110F9);
    _WDWORD(0x401F8268, 0x000110F9);
    _WDWORD(0x401F826C, 0x000110F9);
    _WDWORD(0x401F8270, 0x000110F9);
    _WDWORD(0x401F8274, 0x000110F9);
    _WDWORD(0x401F8278, 0x000110F9);
    _WDWORD(0x401F827C, 0x000110F9);
    _WDWORD(0x401F8280, 0x000110F9);
    _WDWORD(0x401F8284, 0x000110F9);
    _WDWORD(0x401F8288, 0x000110F9);
    _WDWORD(0x401F828C, 0x000110F9);
    _WDWORD(0x401F8290, 0x000110F9);
    _WDWORD(0x401F8294, 0x000110F9);
    _WDWORD(0x401F8298, 0x000110F9);
    _WDWORD(0x401F829C, 0x000110F9);
    _WDWORD(0x401F82A0, 0x000110F9);
    _WDWORD(0x401F82A4, 0x000110F9);
    _WDWORD(0x401F82A8, 0x000110F9);

    // Config SDR Controller Registers/
    _WDWORD(0x402F0000,0x10000004); // MCR
    _WDWORD(0x402F0008,0x00030524); // BMCR0
    _WDWORD(0x402F000C,0x06030524); // BMCR1
    _WDWORD(0x402F0010,0x8000001B); // BR0, 32MB
    _WDWORD(0x402F0014,0x8200001B); // BR1, 32MB
    _WDWORD(0x402F0018,0x8400001B); // BR2, 32MB
    _WDWORD(0x402F001C,0x8600001B); // BR3, 32MB
    _WDWORD(0x402F0020,0x90000021); // BR4,
    _WDWORD(0x402F0024,0xA0000019); // BR5,
    _WDWORD(0x402F0028,0xA8000017); // BR6,
    _WDWORD(0x402F002C,0xA900001B); // BR7,
    _WDWORD(0x402F0030,0x00000021); // BR8,
    _WDWORD(0x402F0004,0x000079A8);  //IOCR,SEMC_CCSX0 as NOR CE, SEMC_CSX1 as PSRAM CE, SEMC_CSX2 as NAND CE, SEMC_CSX3 as DBI CE.

    // _WDWORD(0x402F0004,0x00000008); // IOCR, SEMC_CCSX0 as SDRAM_CS1
    _WDWORD(0x402F0040,0x00000F31); // SDRAMCR0
    _WDWORD(0x402F0044,0x00652922); // SDRAMCR1
    _WDWORD(0x402F0048,0x00010920); // SDRAMCR2
    _WDWORD(0x402F004C,0x50210A08); // SDRAMCR3

    _WDWORD(0x402F0080,0x00000021); // DBICR0
    _WDWORD(0x402F0084,0x00888888); // DBICR1
    _WDWORD(0x402F0094,0x00000002); // IPCR1
    _WDWORD(0x402F0098,0x00000000); // IPCR2

    _WDWORD(0x402F0090,0x80000000); // IPCR0
    _WDWORD(0x402F009C,0xA55A000F); // IPCMD, SD_CC_IPREA
    SDRAM_WaitIpCmdDone();
    _WDWORD(0x402F0090,0x80000000); // IPCR0
    _WDWORD(0x402F009C,0xA55A000C); // SD_CC_IAF
    SDRAM_WaitIpCmdDone();
    _WDWORD(0x402F0090,0x80000000); // IPCR0
    _WDWORD(0x402F009C,0xA55A000C); // SD_CC_IAF
    SDRAM_WaitIpCmdDone();
    _WDWORD(0x402F00A0,0x00000033); // IPTXDAT
    _WDWORD(0x402F0090,0x80000000); // IPCR0
    _WDWORD(0x402F009C,0xA55A000A); // SD_CC_IMS
    SDRAM_WaitIpCmdDone();
    _WDWORD(0x402F004C,0x50210A09 ); // enable sdram self refresh again after initialization done.
}

int imxrt_sdram_init(void)
{
    _clock_init();
    _sdr_Init();
    
    return 0;
}

/*@}*/
