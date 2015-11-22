/**********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: mips32Cache.h
**
** 创   建   人: Ryan.Xin (信金龙)
**
** 文件创建日期: 2015 年 10 月 12 日
**
** 描        述: MIPS32 体系构架 Cache 驱动.
*********************************************************************************************************/

#ifndef __MIPS32_CACHE_H
#define __MIPS32_CACHE_H

VOID  mips32CacheInit(LW_CACHE_OP *pcacheop,
                      CACHE_MODE   uiInstruction,
                      CACHE_MODE   uiData,
                      CPCHAR       pcMachineName);

VOID  mips32CacheReset(CPCHAR  pcMachineName);

#endif                                                                  /*  __MIPS32_CACHE_H            */
/*********************************************************************************************************
  END
*********************************************************************************************************/
