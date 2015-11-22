#*********************************************************************************************************
#
#                                    中国软件开源组织
#
#                                   嵌入式实时操作系统
#
#                                SylixOS(TM)  LW : long wing
#
#                               Copyright All Rights Reserved
#
#--------------文件信息--------------------------------------------------------------------------------
#
# 文   件   名: config.mk
#
# 创   建   人: RealEvo-IDE
#
# 文件创建日期: 2015 年 05 月 11 日
#
# 描        述: 本文由RealEvo-IDE生成，用于配置Makefile功能，请勿手动修改
#*********************************************************************************************************
#*********************************************************************************************************
# SylixOS Base Project path
#*********************************************************************************************************
SYLIXOS_BASE_PATH = ..

#*********************************************************************************************************
# Toolchain prefix
#*********************************************************************************************************
TOOLCHAIN_PREFIX = arm-sylixos-eabi-

#*********************************************************************************************************
# Debug options (debug or release)
#*********************************************************************************************************
DEBUG_LEVEL = debug

#*********************************************************************************************************
# configure area you can set the following config to you own system
# FPUFLAGS (-mfloat-abi=softfp -mfpu=vfpv3 ...)
# CPUFLAGS (-mcpu=arm920t ...)
# NOTICE: libsylixos, BSP and other kernel modules projects CAN NOT use vfp!
#*********************************************************************************************************
FPUFLAGS = -mfloat-abi=softfp -mfpu=vfpv3
CPUFLAGS = -mcpu=arm920t