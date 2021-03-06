/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: sddrvm.h
**
** 创   建   人: Zeng.Bo (曾波)
**
** 文件创建日期: 2014 年 10 月 24 日
**
** 描        述: sd drv manager layer

** BUG:
2015.09.18  增加控制器扩展选项设置, 以适应更多实际应用的场合
*********************************************************************************************************/

#ifndef __SDDRVM_H
#define __SDDRVM_H

#include "sdcore.h"

/*********************************************************************************************************
  SDM 事件类型 for API_SdmEventNotify()
*********************************************************************************************************/

#define SDM_EVENT_DEV_INSERT            0
#define SDM_EVENT_DEV_REMOVE            1
#define SDM_EVENT_SDIO_INTERRUPT        2

/*********************************************************************************************************
  前置声明
*********************************************************************************************************/
struct sd_drv;
struct sd_host;

typedef struct sd_drv     SD_DRV;
typedef struct sd_host    SD_HOST;

/*********************************************************************************************************
  sd 驱动(用于sd memory 和 sdio base)
*********************************************************************************************************/

struct sd_drv {
    LW_LIST_LINE  SDDRV_lineManage;                               /*  驱动挂载链                        */

    CPCHAR        SDDRV_cpcName;

    INT         (*SDDRV_pfuncDevCreate)(SD_DRV *psddrv, PLW_SDCORE_DEVICE psdcoredev, VOID **ppvDevPriv);
    INT         (*SDDRV_pfuncDevDelete)(SD_DRV *psddrv, VOID *pvDevPriv);

    atomic_t      SDDRV_atomicDevCnt;

    VOID         *SDDRV_pvSpec;
};

/*********************************************************************************************************
  sd host 信息结构
*********************************************************************************************************/

#ifdef  __cplusplus
typedef INT     (*SD_CALLBACK)(...);
#else
typedef INT     (*SD_CALLBACK)();
#endif

struct sd_host {
    CPCHAR        SDHOST_cpcName;

    INT           SDHOST_iType;
#define SDHOST_TYPE_SD                  0
#define SDHOST_TYPE_SPI                 1

    INT           SDHOST_iCapbility;                                /*  主动支持的特性                  */
#define SDHOST_CAP_HIGHSPEED            (1 << 0)                    /*  支持高速传输                    */
#define SDHOST_CAP_DATA_4BIT            (1 << 1)                    /*  支持4位数据传输                 */
#define SDHOST_CAP_DATA_8BIT            (1 << 2)                    /*  支持8位数据传输                 */
#define SDHOST_CAP_DATA_4BIT_DDR        (1 << 3)                    /*  支持4位ddr数据传输              */
#define SDHOST_CAP_DATA_8BIT_DDR        (1 << 4)                    /*  支持8位ddr数据传输              */
#define SDHOST_CAP_MMC_FORCE_1BIT       (1 << 5)                    /*  MMC卡 强制使用 1 位总线         */

    VOID          (*SDHOST_pfuncSpicsEn)(SD_HOST *psdhost);
    VOID          (*SDHOST_pfuncSpicsDis)(SD_HOST *psdhost);
    INT           (*SDHOST_pfuncCallbackInstall)
                  (
                  SD_HOST          *psdhost,
                  INT               iCallbackType,                  /*  安装的回调函数的类型            */
                  SD_CALLBACK       callback,                       /*  回调函数指针                    */
                  PVOID             pvCallbackArg                   /*  回调函数的参数                  */
                  );

    INT           (*SDHOST_pfuncCallbackUnInstall)
                  (
                  SD_HOST          *psdhost,
                  INT               iCallbackType                   /*  安装的回调函数的类型            */
                  );
#define SDHOST_CALLBACK_CHECK_DEV       0                           /*  卡状态检测                      */
#define SDHOST_DEVSTA_UNEXIST           0                           /*  卡状态:不存在                   */
#define SDHOST_DEVSTA_EXIST             1                           /*  卡状态:存在                     */

    VOID          (*SDHOST_pfuncSdioIntEn)(SD_HOST *psdhost, BOOL bEnable);
    BOOL          (*SDHOST_pfuncIsCardWp)(SD_HOST *psdhost);

    VOID          (*SDHOST_pfuncDevAttach)(SD_HOST *psdhost, CPCHAR cpcDevName);
    VOID          (*SDHOST_pfuncDevDetach)(SD_HOST *psdhost);
};


/*********************************************************************************************************
  sd host 扩展选项,这些选项均仅对指定的一个硬件控制器通道有效
*********************************************************************************************************/

#define SDHOST_EXTOPT_RESERVE_SECTOR_SET    0
#define SDHOST_EXTOPT_RESERVE_SECTOR_GET    1

#define SDHOST_EXTOPT_MAXBURST_SECTOR_SET   2
#define SDHOST_EXTOPT_MAXBURST_SECTOR_GET   3

#define SDHOST_EXTOPT_CACHE_SIZE_SET        4
#define SDHOST_EXTOPT_CACHE_SIZE_GET        5

/*********************************************************************************************************
  API
*********************************************************************************************************/

LW_API INT   API_SdmLibInit(VOID);

LW_API PVOID API_SdmHostRegister(SD_HOST *psdhost);
LW_API INT   API_SdmHostUnRegister(PVOID  pvSdmHost);

LW_API INT   API_SdmHostCapGet(PLW_SDCORE_DEVICE psdcoredev, INT *piCapbility);
LW_API VOID  API_SdmHostInterEn(PLW_SDCORE_DEVICE psdcoredev, BOOL bEnable);
LW_API BOOL  API_SdmHostIsCardWp(PLW_SDCORE_DEVICE psdcoredev);

LW_API INT   API_SdmSdDrvRegister(SD_DRV *psddrv);
LW_API INT   API_SdmSdDrvUnRegister(SD_DRV *psddrv);

LW_API INT   API_SdmEventNotify(PVOID pvSdmHost, INT iEvtType);

/*********************************************************************************************************
  扩展选项设置 API
  API_SdmHostExtOptSet 用于驱动设置控制器的扩展选项
  API_SdmHostExtOptGet 用于协议层获取控制器的扩展选项
*********************************************************************************************************/

LW_API INT   API_SdmHostExtOptSet(PVOID pvSdmHost, INT  iOption, LONG  lArg);
LW_API INT   API_SdmHostExtOptGet(PLW_SDCORE_DEVICE psdcoredev, INT  iOption, LONG  lArg);

#endif                                                              /*  __SDDRVM_H                      */
/*********************************************************************************************************
  END
*********************************************************************************************************/
