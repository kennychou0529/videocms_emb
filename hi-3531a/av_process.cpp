#include "../includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AV_CFG_JSON_PATH  "/mnt/mtd/app/config"

typedef struct _av_vpss_group_info_s
{
	VPSS_GRP m_vpss_grp;				//group��
	char m_used_status;					//ʹ��״̬ 0:δʹ��  1:��ռ��
}av_vpss_group_info_t, *pav_vpss_group_info_t;

int av_init_cfg();

av_platform_ctx_t g_av_platform_ctx;
static av_vpss_group_info_t s_vpss_grp_arry[VPSS_MAX_GRP_NUM];

int av_cal_blk_size(int width, int height, int align_width, int pix_fmt, int comp_fmt)
{
	if (width <= 0 || height <= 0 || align_width < 0 || pix_fmt < 0)
	{
		return AV_ERR_INVALID_PARAM;
	}
	int tmp_width, tmp_height, blk_size, pic_head_size;

	tmp_width  = CEILING_2_POWER(width, align_width);
	tmp_height = CEILING_2_POWER(height,align_width);

	if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 == pix_fmt)
	{
		blk_size = tmp_width * tmp_height * 2;
	}
	else
	{
		blk_size = tmp_width * tmp_height * 3 / 2;
	}


	if(COMPRESS_MODE_SEG == comp_fmt)
	{
		VB_PIC_HEADER_SIZE(tmp_width, tmp_height, pix_fmt,pic_head_size);
	}

	blk_size += pic_head_size;

	return blk_size;
}

int av_register_vpss_group()
{
	int i = 0;

	for (i = 0;i < VPSS_MAX_GRP_NUM;i++)
	{
		if (0 == s_vpss_grp_arry[i].m_used_status)
		{
			pthread_mutex_lock(&g_av_platform_ctx.m_mtx);
			s_vpss_grp_arry[i].m_used_status = 1;
			pthread_mutex_unlock(&g_av_platform_ctx.m_mtx);
			return s_vpss_grp_arry[i].m_vpss_grp;
		}
	}
	return -1;
}

int av_unregister_vpss_group(int grp_number)
{
	if (grp_number < 0 || grp_number > VPSS_MAX_GRP_NUM)
	{
		return AV_ERR_INVALID_PARAM;
	}
	int i = 0;

	for (i = 0;i < VPSS_MAX_GRP_NUM;i++)
	{
		if (grp_number == s_vpss_grp_arry[i].m_vpss_grp)
		{
			pthread_mutex_lock(&g_av_platform_ctx.m_mtx);
			s_vpss_grp_arry[i].m_used_status = 0;
			pthread_mutex_unlock(&g_av_platform_ctx.m_mtx);
			return AV_OK;
		}
	}
	return AV_FALSE;
}

int av_start_vo_dev(av_vo_cfg_t av_vo_cfg, VO_DEV vo_dev, VO_LAYER vo_layer)
{
	int tmp_width = 0, tmp_height = 0;
	VO_DEV VoDev = vo_dev;
	VO_LAYER VoLayer = vo_layer;
	VO_CHN VoChn;
	VO_PUB_ATTR_S stVoPubAttr_hd0, stVoPubAttr_hd1, stVoPubAttr_sd;
	VO_VIDEO_LAYER_ATTR_S stLayerAttr;
	HI_S32 s32Ret = HI_SUCCESS;

	if (av_vo_cfg.m_out_dev_type & OUT_DEV_TYPE_VGA)
	{
		stVoPubAttr_hd1.enIntfType =  VO_INTF_VGA;
	}
	if (av_vo_cfg.m_out_dev_type & OUT_DEV_TYPE_HDMI)
	{
		stVoPubAttr_hd1.enIntfType =  VO_INTF_HDMI;
	}
	if (av_vo_cfg.m_out_dev_type & OUT_DEV_TYPE_BT1120)
	{
		stVoPubAttr_hd1.enIntfType =  VO_INTF_BT1120;
	}
	stVoPubAttr_hd1.u32BgColor = av_vo_cfg.m_bg_color;
	if (FRAME_RATE_TYPE_60 == av_vo_cfg.m_frame_rate_type)
	{
		if (RESULOTION_TYPE_1920X1080 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P60;
			tmp_width = 1920; tmp_height = 1080;
		}
		else if (RESULOTION_TYPE_1440X900 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1440x900_60;
			tmp_width = 1440; tmp_height = 900;
		}
		else if (RESULOTION_TYPE_1280X1024 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1280x1024_60;
			tmp_width = 1280; tmp_height = 1024;
		}
		else if (RESULOTION_TYPE_1280X720 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_720P60;
			tmp_width = 1280; tmp_height = 768;
		}
		else if (RESULOTION_TYPE_1280X960 == av_vo_cfg.m_resulotion_type)
		{
			//stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_720P60;
		}
		else if (RESULOTION_TYPE_3840X2160 == av_platform_cfg.m_resulotion_type_ui)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_3840x2160_60;
			tmp_width = 3840; tmp_height = 2160;
		}
	}
	else if (FRAME_RATE_TYPE_50 == av_vo_cfg.m_frame_rate_type)
	{
		if (RESULOTION_TYPE_1920X1080 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P50;
			tmp_width = 1920; tmp_height = 1080;
		}
		else if (RESULOTION_TYPE_1440X900 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1440x900_50;
			tmp_width = 1440; tmp_height = 900;
		}
		else if (RESULOTION_TYPE_1280X1024 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1280x1024_50;
			tmp_width = 1280; tmp_height = 1024;
		}
		else if (RESULOTION_TYPE_1280X720 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_720P50;
			tmp_width = 1280; tmp_height = 768;
		}
		else if (RESULOTION_TYPE_1280X960 == av_vo_cfg.m_resulotion_type)
		{
			//stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_720P50;
		}
		else if (RESULOTION_TYPE_3840X2160 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_3840x2160_50;
			tmp_width = 3840; tmp_height = 2160;
		}
	}
	else if (FRAME_RATE_TYPE_30 == av_vo_cfg.m_frame_rate_type)
	{
		if (RESULOTION_TYPE_1920X1080 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P30;
			tmp_width = 1920; tmp_height = 1080;
		}
		else if (RESULOTION_TYPE_1440X900 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1440x900_30;
			tmp_width = 1440; tmp_height = 900;
		}
		else if (RESULOTION_TYPE_1280X1024 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1280x1024_30;
			tmp_width = 1280; tmp_height = 1024;
		}
		else if (RESULOTION_TYPE_1280X720 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_720P30;
			tmp_width = 1280; tmp_height = 768;
		}
		else if (RESULOTION_TYPE_1280X960 == av_vo_cfg.m_resulotion_type)
		{
			//stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_720P30;
		}
		else if (RESULOTION_TYPE_3840X2160 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_3840x2160_30;
			tmp_width = 3840; tmp_height = 2160;
		}
	}
	else if (FRAME_RATE_TYPE_25 == av_vo_cfg.m_frame_rate_type)
	{
		if (RESULOTION_TYPE_1920X1080 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P25;
			tmp_width = 1920; tmp_height = 1080;
		}
		else if (RESULOTION_TYPE_1440X900 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1440x900_25;
			tmp_width = 1440; tmp_height = 900;
		}
		else if (RESULOTION_TYPE_1280X1024 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1280x1024_25;
			tmp_width = 1280; tmp_height = 1024;
		}
		else if (RESULOTION_TYPE_1280X720 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_720P25;
			tmp_width = 1280; tmp_height = 768;
		}
		else if (RESULOTION_TYPE_1280X960 == av_vo_cfg.m_resulotion_type)
		{
			//stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_720P25;
		}
		else if (RESULOTION_TYPE_3840X2160 == av_vo_cfg.m_resulotion_type)
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_3840x2160_25;
			tmp_width = 3840; tmp_height = 2160;
		}
	}
	else
	{
		stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P60;
	}

	s32Ret = HI_MPI_VO_Disable(VoDev);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	s32Ret = HI_MPI_VO_SetPubAttr(VoDev, stVoPubAttr_hd1);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}

	s32Ret = HI_MPI_VO_Enable(VoDev);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	if (PIXEL_FMT_TYPE_YUV422 == av_vo_cfg.m_pixel_fmt_type)
		stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
	else
		stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
	stLayerAttr.u32DispFrmRt = 25;
	stLayerAttr.stDispRect.s32X       = 0;
	stLayerAttr.stDispRect.s32Y       = 0;
	stLayerAttr.stDispRect.u32Width   = tmp_width;
	stLayerAttr.stDispRect.u32Height  = tmp_height;
	stLayerAttr.stImageSize.u32Width  = tmp_width;
	stLayerAttr.stImageSize.u32Height = tmp_height;
	s32Ret = HI_MPI_VO_SetVideoLayerAttr(VoLayer, &stLayerAttr);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}

	s32Ret = HI_MPI_VO_EnableVideoLayer(VoLayer);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}

	return AV_OK;
}

int av_stop_vo_dev(VO_DEV vo_dev, VO_LAYER vo_layer)
{
	VO_DEV VoDev = vo_dev;
	VO_LAYER VoLayer = vo_layer;
	HI_S32 s32Ret = HI_SUCCESS;

	s32Ret = HI_MPI_VO_DisableVideoLayer(VoLayer);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	s32Ret = HI_MPI_VO_Disable(VoDev);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	return AV_OK;
}

int av_start_vo_chn(vo_chn_cfg_t vo_chn_cfg, VO_LAYER vo_layer)
{
	VO_CHN_ATTR_S stChnAttr;
	VO_CHN VoChn;
	HI_S32 s32Ret = HI_SUCCESS;

	stChnAttr.stRect.s32X       = ALIGN_BACK(vo_chn_cfg.m_x, 2);
	stChnAttr.stRect.s32Y       = ALIGN_BACK(vo_chn_cfg.m_y, 2);
	stChnAttr.stRect.u32Width   = ALIGN_BACK(vo_chn_cfg.m_width, 2);
	stChnAttr.stRect.u32Height  = ALIGN_BACK(vo_chn_cfg.m_height, 2);
	stChnAttr.u32Priority       = vo_chn_cfg.m_layer_id;
	stChnAttr.bDeflicker        = vo_chn_cfg.m_deflicker;
	VoChn = vo_chn_cfg.m_chn_id;

	s32Ret = HI_MPI_VO_SetChnAttr(vo_layer, VoChn, &stChnAttr);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("%s(%d):failed with %#x!\n",\
			__FUNCTION__,__LINE__,  s32Ret);
		return AV_FALSE;
	}

	s32Ret = HI_MPI_VO_EnableChn(vo_layer, VoChn);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	return AV_OK;
}

int av_stop_vo_chn(vo_chn_cfg_t vo_chn_cfg, VO_LAYER vo_layer)
{
	VO_CHN VoChn = vo_chn_cfg.m_chn_id;
	HI_S32 s32Ret = HI_SUCCESS;

	s32Ret = HI_MPI_VO_DisableChn(vo_layer, VoChn);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	return AV_OK;
}

int av_start_vpss(vpss_cfg_t vpss_cfg, VPSS_GRP vpss_grp, VPSS_CHN vpss_chn)
{
	VPSS_GRP VpssGrp = vpss_grp;
	VPSS_CHN VpssChn = vpss_chn;
	VPSS_GRP_ATTR_S stGrpAttr = {0};
	VPSS_CHN_ATTR_S stChnAttr = {0};
	VPSS_GRP_PARAM_S stVpssParam = {0};
	HI_S32 s32Ret;

	stGrpAttr.u32MaxW = vpss_cfg.m_max_width;
	stGrpAttr.u32Max = vpss_cfg.m_max_height;
	stGrpAttr.bNrEn = vpss_cfg.m_en_nr;
	stGrpAttr.bIeEn = vpss_cfg.m_en_ie;
	stGrpAttr.bDciEn = vpss_cfg.m_en_dci;
	stGrpAttr.bHistEn = vpss_cfg.m_en_hist;
	stGrpAttr.bEsEn = vpss_cfg.m_en_es;
	stGrpAttr.enDieMode = vpss_cfg.m_en_die_mode;
	if(PIXEL_FMT_TYPE_YUV422 == vpss_cfg.m_pixel_fmt_type)
	{
		stGrpAttr.enPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
	}
	else
	{
		stGrpAttr.enPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
	}
	s32Ret = HI_MPI_VPSS_CreateGrp(VpssGrp, &stGrpAttr);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("HI_MPI_VPSS_CreateGrp[%d] failed with %#x!\n", VpssGrp, s32Ret);
		return AV_FALSE;
	}

	/*** set vpss param ***/
	s32Ret = HI_MPI_VPSS_GetGrpParam(VpssGrp, &stVpssParam);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}

	stVpssParam.u32Contrast = vpss_cfg.m_contrast;
	stVpssParam.u32DieStrength = vpss_cfg.m_dieStrength;
	stVpssParam.u32IeStrength = vpss_cfg.m_ieStrength;
	stVpssParam.u32SfStrength = vpss_cfg.m_sfStrength;
	stVpssParam.u32TfStrength = vpss_cfg.m_cTfStrength;
	stVpssParam.u32CfStrength = vpss_cfg.m_cfStrength;
	stVpssParam.u32CTfStrength = vpss_cfg.m_cTfStrength;
	stVpssParam.u32CvbsStrength = vpss_cfg.m_cTfStrength;
	stVpssParam.u32DeMotionBlurring = vpss_cfg.m_deMotionBlurring;
	s32Ret = HI_MPI_VPSS_SetGrpParam(VpssGrp, &stVpssParam);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	/* Set Vpss Chn attr */
	stChnAttr.bSpEn = HI_FALSE;
	stChnAttr.bUVInvert = HI_FALSE;
	stChnAttr.bBorderEn = HI_TRUE;
	stChnAttr.stBorder.u32Color = 0xff00;
	stChnAttr.stBorder.u32LeftWidth = 2;
	stChnAttr.stBorder.u32RightWidth = 2;
	stChnAttr.stBorder.u32TopWidth = 2;
	stChnAttr.stBorder.u32BottomWidth = 2;

	s32Ret = HI_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &stChnAttr);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("HI_MPI_VPSS_SetChnAttr failed with %#x\n", s32Ret);
		return AV_FALSE;
	}
	s32Ret = HI_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("HI_MPI_VPSS_EnableChn failed with %#x\n", s32Ret);
		return AV_FALSE;
	}
	/*** start vpss group ***/
	s32Ret = HI_MPI_VPSS_StartGrp(VpssGrp);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("HI_MPI_VPSS_StartGrp failed with %#x\n", s32Ret);
		return AV_FALSE;
	}
	return AV_OK;
}

int av_stop_vpss(VPSS_GRP vpss_grp, VPSS_CHN vpss_chn)
{
	HI_S32 s32Ret = HI_SUCCESS;
	VPSS_GRP VpssGrp = vpss_grp;
	VPSS_CHN VpssChn = vpss_chn;

	s32Ret = HI_MPI_VPSS_StopGrp(VpssGrp);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	s32Ret = HI_MPI_VPSS_DisableChn(VpssGrp, VpssChn);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	s32Ret = HI_MPI_VPSS_DestroyGrp(VpssGrp);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	return AV_OK;
}

int av_start_hdmi()
{

	return AV_OK;
}

int av_stop_hdmi()
{

	return AV_OK;
}

int av_init_mpp_sys(void)
{
	VB_CONF_S st_vb_conf;
	MPP_SYS_CONF_S st_mpp_sys_conf;
	HI_S32 s32Ret = HI_FAILURE;
	HI_S32 i;

	memset(&st_vb_conf, 0, sizeof(st_vb_conf));
	memset(&st_mpp_sys_conf, 0, sizeof(st_mpp_sys_conf));
	//major vb
	st_vb_conf.astCommPool[0].u32BlkSize = av_cal_blk_size(g_av_platform_ctx.m_major_width_max, g_av_platform_ctx.m_major_height_max, g_av_platform_ctx.m_sys_align_width, 0, 0); 
	st_vb_conf.astCommPool[0].u32BlkCnt = g_av_platform_ctx.m_major_vb_cnt;
	//minor vb
	st_vb_conf.astCommPool[1].u32BlkSize = av_cal_blk_size(g_av_platform_ctx.m_minor_width_max, g_av_platform_ctx.m_major_height_max, g_av_platform_ctx.m_sys_align_width, 0, 0); 
	st_vb_conf.astCommPool[1].u32BlkCnt = g_av_platform_ctx.m_minor_vb_cnt;

	st_vb_conf.astCommPool[2].u32BlkSize = 196 * 4;
	st_vb_conf.astCommPool[2].u32BlkCnt = 10;
	st_vb_conf.astCommPool[3].u32BlkSize = 196 * 4;
	st_vb_conf.astCommPool[3].u32BlkCnt = 10;
	HI_MPI_SYS_Exit();

	for(i=0;i<VB_MAX_USER;i++)
	{
		HI_MPI_VB_ExitModCommPool(i);
	}
	for(i=0; i<VB_MAX_POOLS; i++)
	{
		HI_MPI_VB_DestroyPool(i);
	}
	HI_MPI_VB_Exit();

	s32Ret = HI_MPI_VB_SetConf(&st_vb_conf);
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("HI_MPI_VB_SetConf failed! 0x%8x \n", s32Ret);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_VB_Init();
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("HI_MPI_VB_Init failed! 0x%8x \n", s32Ret);
		return HI_FAILURE;
	}

	st_mpp_sys_conf.u32AlignWidth = g_av_platform_ctx.m_sys_align_width;
	s32Ret = HI_MPI_SYS_SetConf(&st_mpp_sys_conf);
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("HI_MPI_SYS_SetConf failed 0x%8x \n", s32Ret);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_SYS_Init();
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("HI_MPI_SYS_Init failed 0x%8x \n", s32Ret);
		return HI_FAILURE;
	}
	//_���ý��뻺�棬��Ϊ�������Ƶ���ǹ�������ش��ݹ����ģ����Ե÷��������ģ�鹫�������
	memset(&st_vb_conf, 0, sizeof(st_vb_conf));
	//major vb
	st_vb_conf.astCommPool[0].u32BlkSize = av_cal_blk_size(g_av_platform_ctx.m_major_width_max, g_av_platform_ctx.m_major_height_max, g_av_platform_ctx.m_sys_align_width, 0, 0); 
	st_vb_conf.astCommPool[0].u32BlkCnt = g_av_platform_ctx.m_major_vb_cnt;
	s32Ret = HI_MPI_VB_ExitModCommPool(VB_UID_VDEC); 
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("VB_UID_VDEC HI_MPI_VB_ExitModCommPool  failed 0x%8x \n", s32Ret);
	}
	s32Ret = HI_MPI_VB_SetModPoolConf(VB_UID_VDEC, &st_vb_conf);
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("VB_UID_VDEC HI_MPI_VB_SetModPoolConf  failed 0x%8x \n", s32Ret);
	}
	s32Ret = HI_MPI_VB_InitModCommPool(VB_UID_VDEC);
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("VB_UID_VDEC HI_MPI_VB_InitModCommPool  failed 0x%8x \n", s32Ret);
	}
	return AV_OK;
}

int av_uninit_mpp_sys(void)
{
	int i = 0;
	HI_MPI_SYS_Exit();
	for(i=0;i<VB_MAX_USER;i++)
	{
		HI_MPI_VB_ExitModCommPool(i);
	}
	for(i=0; i<VB_MAX_POOLS; i++)
	{
		HI_MPI_VB_DestroyPool(i);
	}	
	HI_MPI_VB_Exit();

	return AV_OK;
}

int av_save_cfg()
{
	FILE *json_fp;
	int file_size;
	char *file_buf = NULL;
	cJSON *json_root = NULL,*json_arry, *json_tmp;
	int ret = -1;
	char *str;

	json_fp = fopen(AV_CFG_JSON_PATH, "w+");
	if (!json_fp)
	{
		return AV_ERR_OPEN_FILE_FAILED;
	}
	json_root = cJSON_CreateObject();
	if (!json_root)
	{
		goto AV_SAVE_ERR;
	}
	cJSON_AddNumberToObject(json_root, "major_width_max", g_av_platform_ctx.m_major_width_max);
	cJSON_AddNumberToObject(json_root, "major_height_max", g_av_platform_ctx.m_major_height_max);
	cJSON_AddNumberToObject(json_root, "major_vb_cnt", g_av_platform_ctx.m_major_vb_cnt);
	cJSON_AddNumberToObject(json_root, "minor_width_max", g_av_platform_ctx.m_minor_width_max);
	cJSON_AddNumberToObject(json_root, "minor_height_max", g_av_platform_ctx.m_minor_height_max);
	cJSON_AddNumberToObject(json_root, "minor_vb_cnt", g_av_platform_ctx.m_minor_vb_cnt);
	cJSON_AddNumberToObject(json_root, "vpss_group_ddr_id", g_av_platform_ctx.m_vpss_group_ddr_id);
	cJSON_AddNumberToObject(json_root, "venc_group_ddr_id", g_av_platform_ctx.m_venc_group_ddr_id);
	cJSON_AddNumberToObject(json_root, "venc_chn_ddr_id", g_av_platform_ctx.m_venc_chn_ddr_id);
	cJSON_AddNumberToObject(json_root, "vdec_chn_ddr_id", g_av_platform_ctx.m_vdec_chn_ddr_id);
	json_arry = cJSON_CreateArray();
	if (!json_arry)
	{
		ret = AV_ERR_OPEN_JSON_FAILED;
		goto AV_SAVE_ERR;
	}
	json_tmp = cJSON_CreateObject();
	if (!json_tmp)
	{
		cJSON_Delete(json_arry);
		ret = AV_ERR_OPEN_JSON_FAILED;
		goto AV_SAVE_ERR;
	}
	cJSON_AddNumberToObject(json_tmp, "dev_id_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_dev_id);
	cJSON_AddNumberToObject(json_tmp, "out_dev_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type);
	cJSON_AddNumberToObject(json_tmp, "resulotion_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type);
	cJSON_AddNumberToObject(json_tmp, "frame_rate_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type);
	cJSON_AddNumberToObject(json_tmp, "pixel_fmt_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type);
	cJSON_AddNumberToObject(json_tmp, "bg_color_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color);
	cJSON_AddNumberToObject(json_tmp, "vpss_cfStrength_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_cfStrength);
	cJSON_AddNumberToObject(json_tmp, "vpss_contrast_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_contrast);
	cJSON_AddNumberToObject(json_tmp, "vpss_cTfStrength_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_cTfStrength);
	cJSON_AddNumberToObject(json_tmp, "vpss_cvbsStrength_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_cvbsStrength);
	cJSON_AddNumberToObject(json_tmp, "vpss_deMotionBlurring_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_deMotionBlurring);
	cJSON_AddNumberToObject(json_tmp, "vpss_cvbsStrength_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_cvbsStrength);
	cJSON_AddNumberToObject(json_tmp, "vpss_deMotionBlurring_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_dieStrength);
	cJSON_AddNumberToObject(json_tmp, "vpss_en_dci_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_dci);
	cJSON_AddNumberToObject(json_tmp, "vpss_en_die_mode_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_die_mode);
	cJSON_AddNumberToObject(json_tmp, "vpss_en_es_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_es);
	cJSON_AddNumberToObject(json_tmp, "vpss_en_hist_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_hist);
	cJSON_AddNumberToObject(json_tmp, "vpss_en_ie_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_ie);
	cJSON_AddNumberToObject(json_tmp, "vpss_en_nr_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_nr);
	//cJSON_AddNumberToObject(json_tmp, "vpss_group_number_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_group_number);
	cJSON_AddNumberToObject(json_tmp, "vpss_ieStrength_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_ieStrength);
	cJSON_AddNumberToObject(json_tmp, "vpss_max_height_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_height);
	cJSON_AddNumberToObject(json_tmp, "vpss_max_width_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_width);
	cJSON_AddNumberToObject(json_tmp, "vpss_pixel_fmt_type_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_pixel_fmt_type);
	cJSON_AddNumberToObject(json_tmp, "vpss_sfStrength_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_sfStrength);
	

	cJSON_AddNumberToObject(json_tmp, "dev_id_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id);
	cJSON_AddNumberToObject(json_tmp, "out_dev_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type);
	cJSON_AddNumberToObject(json_tmp, "resulotion_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type);
	cJSON_AddNumberToObject(json_tmp, "frame_rate_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type);
	cJSON_AddNumberToObject(json_tmp, "pixel_fmt_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type);
	cJSON_AddNumberToObject(json_tmp, "bg_color_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color);
	if(1 == g_av_platform_ctx.m_app_start_ok)
	{
		cJSON_AddNumberToObject(json_tmp, "vpss_cfStrength_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_cfStrength);
		cJSON_AddNumberToObject(json_tmp, "vpss_contrast_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_contrast);
		cJSON_AddNumberToObject(json_tmp, "vpss_cTfStrength_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_cTfStrength);
		cJSON_AddNumberToObject(json_tmp, "vpss_cvbsStrength_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_cvbsStrength);
		cJSON_AddNumberToObject(json_tmp, "vpss_deMotionBlurring_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_deMotionBlurring);
		cJSON_AddNumberToObject(json_tmp, "vpss_cvbsStrength_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_cvbsStrength);
		cJSON_AddNumberToObject(json_tmp, "vpss_deMotionBlurring_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_dieStrength);
		cJSON_AddNumberToObject(json_tmp, "vpss_en_dci_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_en_dci);
		cJSON_AddNumberToObject(json_tmp, "vpss_en_die_mode_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_en_die_mode);
		cJSON_AddNumberToObject(json_tmp, "vpss_en_es_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_en_es);
		cJSON_AddNumberToObject(json_tmp, "vpss_en_hist_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_en_hist);
		cJSON_AddNumberToObject(json_tmp, "vpss_en_ie_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_en_ie);
		cJSON_AddNumberToObject(json_tmp, "vpss_en_nr_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_en_nr);
		//cJSON_AddNumberToObject(json_tmp, "vpss_group_number_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_group_number);
		cJSON_AddNumberToObject(json_tmp, "vpss_ieStrength_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_ieStrength);
		cJSON_AddNumberToObject(json_tmp, "vpss_max_height_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_max_height);
		cJSON_AddNumberToObject(json_tmp, "vpss_max_width_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_max_width);
		cJSON_AddNumberToObject(json_tmp, "vpss_pixel_fmt_type_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_pixel_fmt_type);
		cJSON_AddNumberToObject(json_tmp, "vpss_sfStrength_live", g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_sfStrength);
	}
	cJSON_AddItemToArray(json_arry, json_tmp);
	cJSON_AddItemReferenceToObject(json_root, "av_cfg", json_arry);
	str = cJSON_Print(json_root);
	if (str)
	{
		fwrite(str, strlen(str), 1, json_fp);
		free(str);
	}
	ret = AV_OK;

AV_SAVE_ERR:
	if (json_fp)
		fclose(json_fp);
	if(json_root)
		cJSON_Delete(json_root);

	return ret;
}

int av_load_cfg()
{
	FILE *json_fp;
	int file_size;
	char *file_buf = NULL;
	cJSON *json_root = NULL, *json_tmp, *json_arry;
	int ret = -1;
	char *str;

	json_fp = fopen(AV_CFG_JSON_PATH, "r+");
	if (!json_fp)
	{
		return AV_ERR_OPEN_FILE_FAILED;
	}
	fseek(json_fp, 0, SEEK_END);
	file_size = ftell(json_fp);
	if (!file_size)
	{
		ret = AV_ERR_FILE_EMPTY;
		goto AV_INIT_ERR;
	}
	fseek(json_fp, 0, SEEK_SET);
	file_buf = (char *)malloc(file_size);
	if (!file_buf)
	{
		ret = AV_ERR_NOMEMORY;
		goto AV_INIT_ERR;
	}
	fread(file_buf, file_size, 1, json_fp);
	json_root = cJSON_Parse(file_buf);
	if (!json_root)
	{
		ret = AV_ERR_PARSE_JSON_FAILED;
		goto AV_INIT_ERR;
	}
	fclose(json_fp);

	json_tmp = cJSON_GetObjectItem(json_root, "major_width_max");
	if (json_tmp)
		g_av_platform_ctx.m_major_width_max = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "major_width_max", g_av_platform_ctx.m_major_width_max);
	json_tmp = cJSON_GetObjectItem(json_root, "major_height_max");
	if (json_tmp)
		g_av_platform_ctx.m_major_height_max = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "major_height_max", g_av_platform_ctx.m_major_height_max);
	json_tmp = cJSON_GetObjectItem(json_root, "major_vb_cnt");
	if (json_tmp)
		g_av_platform_ctx.m_major_vb_cnt = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "major_vb_cnt", g_av_platform_ctx.m_major_vb_cnt);
	json_tmp = cJSON_GetObjectItem(json_root, "minor_width_max");
	if (json_tmp)
		g_av_platform_ctx.m_minor_width_max = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "minor_width_max", g_av_platform_ctx.m_minor_width_max);

	json_tmp = cJSON_GetObjectItem(json_root, "minor_height_max");
	if (json_tmp)
		g_av_platform_ctx.m_minor_height_max = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "minor_height_max", g_av_platform_ctx.m_minor_height_max);

	json_tmp = cJSON_GetObjectItem(json_root, "minor_vb_cnt");
	if (json_tmp)
		g_av_platform_ctx.m_minor_vb_cnt = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "minor_vb_cnt", g_av_platform_ctx.m_minor_vb_cnt);

	json_tmp = cJSON_GetObjectItem(json_root, "vpss_group_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_vpss_group_ddr_id = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "vpss_group_ddr_id", g_av_platform_ctx.m_vpss_group_ddr_id);

	json_tmp = cJSON_GetObjectItem(json_root, "venc_group_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_venc_group_ddr_id = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "venc_group_ddr_id", g_av_platform_ctx.m_venc_group_ddr_id);

	json_tmp = cJSON_GetObjectItem(json_root, "venc_chn_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_venc_chn_ddr_id = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "venc_chn_ddr_id", g_av_platform_ctx.m_venc_chn_ddr_id);

	json_tmp = cJSON_GetObjectItem(json_root, "vdec_chn_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_venc_chn_ddr_id = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "vdec_chn_ddr_id", g_av_platform_ctx.m_vdec_chn_ddr_id);

	json_tmp = cJSON_GetObjectItem(json_root, "vdec_chn_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_venc_chn_ddr_id = json_tmp->valueint;
	else
		cJSON_AddNumberToObject(json_root, "vdec_chn_ddr_id", g_av_platform_ctx.m_vdec_chn_ddr_id);

	json_tmp = cJSON_GetObjectItem(json_root, "av_cfg");
	if (!json_tmp)
	{
		json_arry = cJSON_CreateArray();
		if (!json_arry)
		{
			ret = AV_ERR_OPEN_JSON_FAILED;
			goto AV_INIT_ERR;
		}
		json_tmp = cJSON_CreateObject();
		if (!json_tmp)
		{
			cJSON_Delete(json_arry);
			ret = AV_ERR_OPEN_JSON_FAILED;
			goto AV_INIT_ERR;
		}
		cJSON_AddNumberToObject(json_tmp, "dev_id_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_dev_id);
		cJSON_AddNumberToObject(json_tmp, "out_dev_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type);
		cJSON_AddNumberToObject(json_tmp, "resulotion_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type);
		cJSON_AddNumberToObject(json_tmp, "frame_rate_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type);
		cJSON_AddNumberToObject(json_tmp, "pixel_fmt_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type);
		cJSON_AddNumberToObject(json_tmp, "bg_color_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color);
		cJSON_AddNumberToObject(json_tmp, "dev_id_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id);
		cJSON_AddNumberToObject(json_tmp, "out_dev_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type);
		cJSON_AddNumberToObject(json_tmp, "resulotion_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type);
		cJSON_AddNumberToObject(json_tmp, "frame_rate_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type);
		cJSON_AddNumberToObject(json_tmp, "pixel_fmt_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type);
		cJSON_AddNumberToObject(json_tmp, "bg_color_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color);
		cJSON_AddItemToArray(json_arry, json_tmp);
		cJSON_AddItemReferenceToObject(json_root, "av_cfg", json_arry);
	}
	else
	{
		cJSON *_json = cJSON_GetArrayItem(json_arry, 0);
		json_tmp = cJSON_GetObjectItem(_json, "dev_id_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_dev_id = json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "dev_id_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_dev_id);

		json_tmp = cJSON_GetObjectItem(_json, "out_dev_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type = json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "out_dev_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type);

		json_tmp = cJSON_GetObjectItem(_json, "resulotion_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type = (resulotion_type_t)json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "resulotion_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type);

		json_tmp = cJSON_GetObjectItem(_json, "frame_rate_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type = (frame_rate_type_t)json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "frame_rate_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type);

		json_tmp = cJSON_GetObjectItem(_json, "pixel_fmt_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type = (pixel_fmt_type_t)json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "pixel_fmt_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type);

		json_tmp = cJSON_GetObjectItem(_json, "bg_color_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color = (pixel_fmt_type_t)json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "pixel_fmt_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color);

		json_tmp = cJSON_GetObjectItem(_json, "dev_id_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id = json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "dev_id_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id);
		
		json_tmp = cJSON_GetObjectItem(_json, "out_dev_type_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type = json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "out_dev_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type);

		json_tmp = cJSON_GetObjectItem(_json, "resulotion_type_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type = (resulotion_type_t)json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "resulotion_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type);

		json_tmp = cJSON_GetObjectItem(_json, "frame_rate_type_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type = (frame_rate_type_t)json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "frame_rate_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type);

		json_tmp = cJSON_GetObjectItem(_json, "pixel_fmt_type_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type = (pixel_fmt_type_t)json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "pixel_fmt_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type);

		json_tmp = cJSON_GetObjectItem(_json, "bg_color_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color = (pixel_fmt_type_t)json_tmp->valueint;
		else
			cJSON_AddNumberToObject(_json, "bg_color_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color);
	}

	json_fp = fopen(AV_CFG_JSON_PATH, "w+");
	if (!json_fp)
	{
		ret = AV_ERR_OPEN_FILE_FAILED;
		goto AV_INIT_ERR;
	}
	str = cJSON_Print(json_root);
	if (str)
	{
		fwrite(str, strlen(str), 1, json_fp);
		free(str);
	}
	else
	{
		ret = AV_ERR_FILE_EMPTY;
		goto AV_INIT_ERR;
	}
	ret = AV_OK;
AV_INIT_ERR:
	if (json_fp)
		fclose(json_fp);
	if(file_buf)
		free(file_buf);
	if(json_root)
		cJSON_Delete(json_root);
	return ret;
}

int av_unload_cfg()
{
	av_save_cfg();
	return AV_OK;
}

//_����û���������
int av_start_ui_out(av_platform_cfg_t av_platform_cfg)
{
	VO_DEV VoDev = 0;
	VO_LAYER VoLayer = 0;
	if (0 == av_platform_cfg.m_vo_cfg_ui.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DHD0;
		VoLayer = SAMPLE_VO_LAYER_VHD0;
	}
	else if (1 == av_platform_cfg.m_vo_cfg_ui.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DHD1;
		VoLayer = SAMPLE_VO_LAYER_VHD1;
	}
	else if (2 == av_platform_cfg.m_vo_cfg_ui.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DSD0;
		VoLayer = SAMPLE_VO_LAYER_VSD0;
	}

	if(AV_OK != av_start_vo_dev(av_platform_cfg.m_vo_cfg_ui, VoDev, VoLayer))
		DBG_PRT("av_start_vo_dev is failed\n");
	return AV_OK;
}

int av_stop_ui_out(av_platform_cfg_t av_platform_cfg)
{
	VO_DEV VoDev = 0;
	VO_LAYER VoLayer = 0;
	if (0 == av_platform_cfg.m_vo_cfg_ui.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DHD0;
		VoLayer = SAMPLE_VO_LAYER_VHD0;
	}
	else if (1 == av_platform_cfg.m_vo_cfg_ui.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DHD1;
		VoLayer = SAMPLE_VO_LAYER_VHD1;
	}
	else if (2 == av_platform_cfg.m_vo_cfg_ui.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DSD0;
		VoLayer = SAMPLE_VO_LAYER_VSD0;
	}
	av_stop_vo_dev(VoDev, VoLayer);
	return AV_OK;
}

//_���ֱ������
int av_start_live_out(av_platform_cfg_t av_platform_cfg)
{
	VO_DEV VoDev;
	VO_LAYER VoLayer;
	MPP_CHN_S stSrcChn, stDestChn;
	vo_chn_cfg_t vo_chn_cfg;

	if (0 == av_platform_cfg.m_vo_cfg_live.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DHD0;
		VoLayer = SAMPLE_VO_LAYER_VHD0;
	}
	else if (1 == av_platform_cfg.m_vo_cfg_live.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DHD1;
		VoLayer = SAMPLE_VO_LAYER_VHD1;
	}
	else if (2 == av_platform_cfg.m_vo_cfg_live.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DSD0;
		VoLayer = SAMPLE_VO_LAYER_VSD0;
	}
	else
	{
		VoDev = SAMPLE_VO_DEV_DHD0;
		VoLayer = SAMPLE_VO_LAYER_VHD0;
	}

	if(AV_OK != av_start_vo_dev(av_platform_cfg.m_vo_cfg_live, VoDev, VoLayer))
		DBG_PRT("av_start_vo_dev is failed\n");
	av_platform_cfg.m_vpss_cfg_live.m_group_number = av_register_vpss_group();
	DBG_PRT("ui out vpss grp is %d\n", av_platform_cfg.m_vpss_cfg_live.m_group_number);
	av_start_vpss(av_platform_cfg.m_vpss_cfg_ui, av_platform_cfg.m_vpss_cfg_live.m_group_number , VPSS_CHN_TYPE_RENDER);
	vo_chn_cfg.m_chn_id = 0;
	vo_chn_cfg.m_deflicker = 0;
	vo_chn_cfg.m_layer_id = 0;
	vo_chn_cfg.m_x = 20;
	vo_chn_cfg.m_y = 20;
	vo_chn_cfg.m_width = 1920;
	vo_chn_cfg.m_height = 1080;
	av_start_vo_chn(vo_chn_cfg, VoLayer);
	stSrcChn.enModId    = HI_ID_VIU;
	stSrcChn.s32DevId   = 0;
	stSrcChn.s32ChnId   = VPSS_CHN_TYPE_RENDER;

	stDestChn.enModId   = HI_ID_VOU;
	stDestChn.s32ChnId  = vo_chn_cfg.m_chn_id;
	stDestChn.s32DevId  = VoLayer;
	HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);

	return AV_OK;
}

int av_stop_live_out(av_platform_cfg_t av_platform_cfg)
{
	VO_DEV VoDev;
	VO_LAYER VoLayer;
	MPP_CHN_S stSrcChn, stDestChn;

	if (0 == av_platform_cfg.m_vo_cfg_live.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DHD0;
		VoLayer = SAMPLE_VO_LAYER_VHD0;
	}
	else if (1 == av_platform_cfg.m_vo_cfg_live.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DHD1;
		VoLayer = SAMPLE_VO_LAYER_VHD1;
	}
	else if (2 == av_platform_cfg.m_vo_cfg_live.m_dev_id)
	{
		VoDev = SAMPLE_VO_DEV_DSD0;
		VoLayer = SAMPLE_VO_LAYER_VSD0;
	}
	else
	{
		VoDev = SAMPLE_VO_DEV_DHD0;
		VoLayer = SAMPLE_VO_LAYER_VHD0;
	}
	stSrcChn.enModId    = HI_ID_VIU;
	stSrcChn.s32DevId   = 0;
	stSrcChn.s32ChnId   = VPSS_CHN_TYPE_RENDER;

	stDestChn.enModId   = HI_ID_VOU;
	stDestChn.s32ChnId  = av_platform_cfg.m_vo_chn_cfg_ui.m_chn_id;
	stDestChn.s32DevId  = VoLayer;
	HI_MPI_SYS_UnBind(stSrcChn, stDestChn);

	av_stop_vpss(av_platform_cfg.m_vpss_cfg_live.m_group_number , VPSS_CHN_TYPE_RENDER);
	av_stop_vo_chn(av_platform_cfg.m_vo_chn_cfg_ui, VoLayer);
	av_stop_vo_dev(VoDev, VoLayer);
	return AV_OK;
}

int av_init_cfg()
{
	memset(&g_av_platform_ctx, 0, sizeof(g_av_platform_ctx));
	g_av_platform_ctx.m_major_height_max = 1920;
	g_av_platform_ctx.m_major_width_max = 1080;
	g_av_platform_ctx.m_major_vb_cnt = 80;
	g_av_platform_ctx.m_minor_width_max = 960;
	g_av_platform_ctx.m_minor_height_max = 540;
	g_av_platform_ctx.m_minor_vb_cnt = 50;
	g_av_platform_ctx.m_vpss_group_ddr_id = 0;
	g_av_platform_ctx.m_venc_group_ddr_id = 0;
	g_av_platform_ctx.m_venc_chn_ddr_id = 0;
	g_av_platform_ctx.m_vdec_chn_ddr_id = 0;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_dev_id = 0;	//DH0
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type = OUT_DEV_TYPE_VGA;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type = RESULOTION_TYPE_1920X1080;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type = FRAME_RATE_TYPE_60;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color = 0x0;
	//___________VPSS����
	//_Group����:�ⲿ�ֲ���������ļ��м���ʧ�ܣ���ʼ��ʹ�ô�ϵͳ�л�ȡ�Ĳ���
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_contrast = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_cfStrength = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_cTfStrength = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_cvbsStrength = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_deMotionBlurring = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_dieStrength = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_dci = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_die_mode = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_es = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_hist = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_ie = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_en_nr = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_ieStrength = 0;
// 	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_sfStrength = 0;
	//_Group end
	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_width = 1920;
	g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_height = 1080;
	//____________________________________________
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id = 1;//DH1
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type = (OUT_DEV_TYPE_HDMI | OUT_DEV_TYPE_BT1120);
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type = RESULOTION_TYPE_1920X1080;
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type = FRAME_RATE_TYPE_60;
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color = 0x0;
	return AV_OK;
}

int av_uninit_cfg()
{
	av_save_cfg();
	return AV_OK;
}

int av_startup()
{
	int i = 0;

	memset(&g_av_platform_ctx, 0, sizeof(av_platform_ctx_t));
	for (i = 0;i < VPSS_MAX_GRP_NUM;i++)
	{
		s_vpss_grp_arry[i].m_used_status = 0;
		s_vpss_grp_arry[i].m_vpss_grp = i;
	}
	if (AV_OK != av_load_cfg())
	{
		av_init_cfg();
		av_save_cfg();
	}
	pthread_mutex_init(&g_av_platform_ctx.m_mtx, NULL);
	av_init_mpp_sys();
	av_start_hdmi();
	av_start_ui_out(g_av_platform_ctx.m_cfg);
	av_start_live_out(g_av_platform_ctx.m_cfg);
	return AV_OK;
}

int  av_shutdown()
{
	av_uninit_mpp_sys();
	av_uninit_cfg();
	pthread_mutex_destroy(&g_av_platform_ctx.m_mtx);
	return AV_OK;
}

int av_get_cfg(av_platform_cfg_t *pav_platform_cfg)
{
	if (!pav_platform_cfg)
	{
		return AV_ERR_INVALID_PARAM;
	}
	memcpy(pav_platform_cfg, &g_av_platform_ctx.m_cfg, sizeof(av_platform_cfg_t));

	return AV_OK;
}

int av_set_cfg(av_platform_cfg_t av_platform_cfg)
{
	memcpy(&g_av_platform_ctx.m_cfg, &av_platform_cfg, sizeof(av_platform_cfg_t));

	return AV_OK;
}