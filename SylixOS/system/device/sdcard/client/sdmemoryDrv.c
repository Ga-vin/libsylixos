/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: sdmemoryDrv.c
**
** ��   ��   ��: Zeng.Bo (����)
**
** �ļ���������: 2014 �� 10 �� 25 ��
**
** ��        ��: sd ���俨ע�ᵽ SDM ģ�������Դ�ļ�

** BUG:
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
/*********************************************************************************************************
  ����ü�֧��
*********************************************************************************************************/
#if (LW_CFG_DEVICE_EN > 0) && (LW_CFG_SDCARD_EN > 0)
#include "../SylixOS/fs/oemDisk/oemDisk.h"
#include "../core/sddrvm.h"
#include "sdmemory.h"
#include "sdmemoryDrv.h"
#include "../include/sddebug.h"
/*********************************************************************************************************
  �ڲ��궨��
*********************************************************************************************************/
#define __SDMEM_CACHE_BOOST    256
#define __SDMEM_CACHE_SIZE     (128 * LW_CFG_KB_SIZE)
/*********************************************************************************************************
  sdmem �豸˽������
*********************************************************************************************************/
struct __sdmem_priv {
    PLW_BLK_DEV         SDMEMPRIV_pblkdev;
    PLW_OEMDISK_CB      SDMEMPRIV_poemdisk;
};
typedef struct __sdmem_priv __SDMEM_PRIV;
/*********************************************************************************************************
  ǰ������
*********************************************************************************************************/
static INT   __sdmemDevCreate(SD_DRV *psddrv, PLW_SDCORE_DEVICE psdcoredev, VOID **ppvDevPriv);
static INT   __sdmemDevDelete(SD_DRV *psddrv, VOID *pvDevPriv);
/*********************************************************************************************************
  sdmem ��������
*********************************************************************************************************/
static SD_DRV  _G_sddrvMem;
/*********************************************************************************************************
** ��������: API_SdMemDrvInstall
** ��������: ��װ sd memory ����
** ��    ��: NONE
** ��    ��: ERROR CODE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
LW_API INT API_SdMemDrvInstall (VOID)
{
    _G_sddrvMem.SDDRV_cpcName        = "/dev/sdmem";
    _G_sddrvMem.SDDRV_pfuncDevCreate = __sdmemDevCreate;
    _G_sddrvMem.SDDRV_pfuncDevDelete = __sdmemDevDelete;
    
    API_SdmSdDrvRegister(&_G_sddrvMem);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __sdmemDevCreate
** ��������: sd memory �豸����
** ��    ��: psddrv       sd ����
**           psdcoredev   sd ���Ĵ������
**           ppvDevPriv   ���ڱ����豸�����ɹ�����豸˽������
** ��    ��: ERROR CODE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static INT  __sdmemDevCreate (SD_DRV *psddrv, PLW_SDCORE_DEVICE psdcoredev, VOID **ppvDevPriv)
{
    PLW_BLK_DEV       pblkdev;
    PLW_OEMDISK_CB    poemdisk;
    __SDMEM_PRIV     *psdmempriv;

    psdmempriv= (__SDMEM_PRIV *)__SHEAP_ALLOC(sizeof(__SDMEM_PRIV));
    if (!psdmempriv) {
        SDCARD_DEBUG_MSG(__ERRORMESSAGE_LEVEL, "system low memory.\r\n");
        return  (PX_ERROR);
    }

    /*
     * ������SDM��, Լ��: �����������ƺ��豸����Ϊ��ʱ, ��ʾcoredev �� SDM ����
     * ��ʱ, psdmemchan ָ���Ӧ��coredev
     */
    pblkdev = API_SdMemDevCreate(-1, LW_NULL, LW_NULL, (PLW_SDMEM_CHAN)psdcoredev);
    if (!pblkdev) {
        goto    __err1;
    }

    poemdisk = oemDiskMount("/sdcard",
                            pblkdev,
                            LW_NULL,
                            __SDMEM_CACHE_SIZE,
                            __SDMEM_CACHE_BOOST);
    if (!poemdisk) {
        printk("\nmount sd memory card failed.\r\n");
        goto    __err2;
    }


    printk("\nmount sd memory card successfully.\r\n");

    API_SdMemDevShow(pblkdev);

    psdmempriv->SDMEMPRIV_pblkdev  = pblkdev;
    psdmempriv->SDMEMPRIV_poemdisk = poemdisk;

    *ppvDevPriv = (VOID *)psdmempriv;

    return  (ERROR_NONE);

__err2:
    API_SdMemDevDelete(pblkdev);

__err1:
    __SHEAP_FREE(psdmempriv);

    return  (PX_ERROR);
}
/*********************************************************************************************************
** ��������: __sdmemDevDelete
** ��������: sd memory �豸ɾ��
** ��    ��: psddrv       sd ����
**           pvDevPriv    �豸˽������
** ��    ��: ERROR CODE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static INT  __sdmemDevDelete (SD_DRV *psddrv,  VOID *pvDevPriv)
{
    __SDMEM_PRIV  *psdmempriv = (__SDMEM_PRIV *)pvDevPriv;
    if (!psdmempriv) {
        return  (PX_ERROR);
    }

    oemDiskUnmount(psdmempriv->SDMEMPRIV_poemdisk);
    API_SdMemDevDelete(psdmempriv->SDMEMPRIV_pblkdev);

    __SHEAP_FREE(psdmempriv);

    return  (ERROR_NONE);
}
#endif                                                                  /*  (LW_CFG_DEVICE_EN > 0)      */
                                                                        /*  (LW_CFG_SDCARD_EN > 0)      */
/*********************************************************************************************************
  END
*********************************************************************************************************/