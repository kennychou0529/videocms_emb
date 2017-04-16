#include "../includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AV_CFG_JSON_PATH  "/mnt/mtd/app/config"

typedef struct _av_vpss_group_info_s
{
	VPSS_GRP m_vpss_grp;				//group号
	char m_used_status;					//使用状态 0:未使用  1:已占用
}av_vpss_group_info_t, *pav_vpss_group_info_t;

int av_init_cfg();

av_platform_ctx_t g_av_platform_ctx;
static av_vpss_group_info_t s_vpss_grp_arry[VPSS_MAX_GRP_NUM];

VI_DEV_ATTR_S DEV_ATTR_7441_BT1120_STANDARD_BASE =
{
	/*interface mode*/
	VI_MODE_BT1120_STANDARD,
	/*work mode, 1/2/4 multiplex*/
	VI_WORK_MODE_1Multiplex,
	/* r_mask    g_mask    b_mask*/
	{0xFF000000,    0xFF0000},

	/* for single/double edge, must be set when double edge*/
	VI_CLK_EDGE_SINGLE_UP,

	/*AdChnId*/
	{-1, -1, -1, -1},
	//{0,0,0,0},
	/*enDataSeq, just support yuv*/
	VI_INPUT_DATA_UVUV,

	/*sync info*/
	{
		/*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
		VI_VSYNC_PULSE, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_NORM_PULSE,VI_VSYNC_VALID_NEG_HIGH,

			/*timing info*/
			/*hsync_hfb    hsync_act    hsync_hhb*/
		{0,            1920,        0,
		/*vsync0_vhb vsync0_act vsync0_hhb*/
		0,            1080,        0,
		/*vsync1_vhb vsync1_act vsync1_hhb*/
		0,            0,            0}
	},
		/*whether use isp*/
		VI_PATH_BYPASS,
		/*data type*/
		VI_DATA_TYPE_YUV

};

VI_DEV_ATTR_S DEV_ATTR_TP2823_720P_1MUX_BASE =
{
	/*interface mode*/
	VI_MODE_BT656,
	/*work mode, 1/2/4 multiplex*/
	VI_WORK_MODE_1Multiplex,
	/* r_mask    g_mask    b_mask*/
	{0xFF000000,    0x0},

	/* for single/double edge, must be set when double edge*/
	VI_CLK_EDGE_SINGLE_UP,

	/*AdChnId*/
	{-1, -1, -1, -1},
	/*enDataSeq, just support yuv*/
	VI_INPUT_DATA_YVYU,
	/*sync info*/
	{
		/*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
		VI_VSYNC_FIELD, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_VALID_SINGAL,VI_VSYNC_VALID_NEG_HIGH,

			/*timing info*/
			/*hsync_hfb    hsync_act    hsync_hhb*/
		{0,            0,        0,
		/*vsync0_vhb vsync0_act vsync0_hhb*/
		0,            0,        0,
		/*vsync1_vhb vsync1_act vsync1_hhb*/
		0,            0,            0}
	},
		/*whether use isp*/
		VI_PATH_BYPASS,
		/*data type*/
		VI_DATA_TYPE_YUV
};

static int av_set_vi_bt656(vi_cfg_t vi_cfg, VI_DEV_ATTR_S * stViDevAttr)
{
	if (!stViDevAttr)
	{
		return AV_ERR_INVALID_PARAM;
	}

	memcpy(stViDevAttr,&DEV_ATTR_TP2823_720P_1MUX_BASE,sizeof(stViDevAttr));

	stViDevAttr->au32CompMask[0] = vi_cfg->m_mask0;
	stViDevAttr->au32CompMask[1] = vi_cfg->m_mask1;


	if(vi_cfg.m_width >= 1920 && vi_cfg.m_fps > 30 )
	{
		stViDevAttr->enClkEdge = VI_CLK_EDGE_DOUBLE;
	}
	else
	{
		stViDevAttr->enClkEdge = VI_CLK_EDGE_SINGLE_UP;
	}

	stViDevAttr->enIntfMode = VI_MODE_BT656;

	stViDevAttr->stSynCfg.stTimingBlank.u32HsyncAct = vi_cfg.m_width;
	stViDevAttr->stSynCfg.stTimingBlank.u32VsyncVact = vi_cfg.m_height;

	return AV_OK;
}

static int av_set_vi_bt1120(vi_cfg_t vi_cfg, VI_DEV_ATTR_S * stViDevAttr)
{
	if (!stViDevAttr)
	{
		return AV_ERR_INVALID_PARAM;
	}

	memcpy(stViDevAttr,&DEV_ATTR_7441_BT1120_STANDARD_BASE,sizeof(stViDevAttr));

	stViDevAttr->au32CompMask[0] = vi_cfg.m_mask0;
	stViDevAttr->au32CompMask[1] = vi_cfg.m_mask1;


	if(vi_cfg.m_width == 1920 && vi_cfg.m_interface == BT1120I && vi_cfg.m_fps > 30)
	{
		stViDevAttr->enClkEdge = VI_CLK_EDGE_DOUBLE;
	}
	else if(vi_cfg.m_width > 1920 && vi_cfg.m_interface == BT1120S)
	{
		stViDevAttr->enClkEdge = VI_CLK_EDGE_DOUBLE;
	}
	else
	{
		stViDevAttr->enClkEdge = VI_CLK_EDGE_SINGLE_UP;
	}

	if(vi_cfg.m_interface == BT1120I)
		stViDevAttr->enIntfMode = VI_MODE_BT1120_INTERLEAVED;
	else if (vi_cfg.m_interface == BT1120S)
		stViDevAttr->enIntfMode = VI_MODE_BT1120_STANDARD;
	else
		stViDevAttr->enIntfMode = VI_MODE_BT1120_INTERLEAVED;

	stViDevAttr->stSynCfg.stTimingBlank.u32HsyncAct = vi_cfg.m_width;
	stViDevAttr->stSynCfg.stTimingBlank.u32VsyncVact = vi_cfg.m_height;

	return AV_OK;
}

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
	int vps_grp = -1;
	
	pthread_mutex_lock(&g_av_platform_ctx.m_mtx);
	for (i = 0;i < VPSS_MAX_GRP_NUM;i++)
	{
		if (0 == s_vpss_grp_arry[i].m_used_status)
		{
			s_vpss_grp_arry[i].m_used_status = 1;
			vps_grp = s_vpss_grp_arry[i].m_vpss_grp;
		}
	}
	pthread_mutex_unlock(&g_av_platform_ctx.m_mtx);
	return vps_grp;
}

int av_unregister_vpss_group(int grp_number)
{
	if (grp_number < 0 || grp_number > VPSS_MAX_GRP_NUM)
	{
		return AV_ERR_INVALID_PARAM;
	}
	int i = 0;
	pthread_mutex_lock(&g_av_platform_ctx.m_mtx);
	for (i = 0;i < VPSS_MAX_GRP_NUM;i++)
	{
		if (grp_number == s_vpss_grp_arry[i].m_vpss_grp)
		{
			s_vpss_grp_arry[i].m_used_status = 0;
			break;
		}
	}
	pthread_mutex_unlock(&g_av_platform_ctx.m_mtx);
	
	return AV_OK;
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
		else
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P60;
			tmp_width = 1920; tmp_height = 1080;
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
		else
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P50;
			tmp_width = 1920; tmp_height = 1080;
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
		else
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P30;
			tmp_width = 1920; tmp_height = 1080;
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
		else
		{
			stVoPubAttr_hd1.enIntfSync = VO_OUTPUT_1080P25;
			tmp_width = 1920; tmp_height = 1080;
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

int av_start_vi(vi_cfg_t vi_cfg)
{
	VI_DEV ViDev;
	VI_CHN ViChn;
	HI_S32 s32Ret;
	HI_S32 s32Ret;
	VI_DEV_ATTR_S stViDevAttr;
	VI_DEV ViDev;
	HI_S32 s32Ret;
	VI_CHN_ATTR_S stChnAttr;
	VI_CHN ViChn;

	memset(&stViDevAttr,0,sizeof(stViDevAttr));
	ViDev = vi_cfg.m_videv;
	ViChn = vi_cfg.m_vichn;
	if(vi_cfg.m_interface == BT1120S ||
		vi_cfg.m_interface == BT1120I)
	{
		av_set_vi_bt1120(vi_cfg, &stViDevAttr);
	}
	else
	{
		av_set_vi_bt656(vi_cfg, &stViDevAttr);
	}


	s32Ret = HI_MPI_VI_SetDevAttr(ViDev, &stViDevAttr);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("HI_MPI_VI_SetDevAttr[%d] failed with %#x!\n",ViDev, s32Ret);
		return AV_FALSE;
	}

	s32Ret = HI_MPI_VI_EnableDev(ViDev);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("HI_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}

	stChnAttr.stCapRect.u32Width = vi_cfg.m_width;
	stChnAttr.stCapRect.u32Height = vi_cfg.m_height;

	/* to show scale. this is a sample only, we want to show dist_size = D1 only */
	stChnAttr.stDestSize.u32Width = vi_cfg.m_width;
	stChnAttr.stDestSize.u32Height = vi_cfg.m_height;

	stChnAttr.enCapSel = VI_CAPSEL_BOTH;
	if(vi_cfg.m_colorspace == YUV420)
	{
		stChnAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
	}
	else if(vi_cfg.m_colorspace == YUV422)
	{
		stChnAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
	}
	else
	{
		stChnAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;   /* sp420 or sp422 */
	}


	stChnAttr.bMirror = (VI_CHN_SET_MIRROR & vi_cfg.m_mirrorflip)?HI_TRUE:HI_FALSE;
	stChnAttr.bFlip = (VI_CHN_SET_FILP & vi_cfg.m_mirrorflip)?HI_TRUE:HI_FALSE;
	if (vi_cfg.m_dst_fps != -1 && vi_cfg.m_src_fps != -1)
	{
		stChnAttr.s32SrcFrameRate = vi_cfg.m_src_fps;
		stChnAttr.s32DstFrameRate = vi_cfg.m_dst_fps;
	}
	else
	{
		stChnAttr.s32SrcFrameRate = -1;
		stChnAttr.s32DstFrameRate = -1;
	}

	if(vi_cfg.m_interlaced == 0)
	{
		stChnAttr.enScanMode = VI_SCAN_PROGRESSIVE;
	}
	else
	{
		stChnAttr.enScanMode = VI_SCAN_INTERLACED;
	}
	s32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}

	s32Ret = HI_MPI_VI_EnableChn(ViChn);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}

	return AV_OK;
}

int av_stop_vi(vi_cfg_t vi_cfg)
{
	VI_DEV ViDev;
	VI_CHN ViChn;
	HI_S32 s32Ret;
	HI_S32 s32Ret;
	VI_DEV_ATTR_S stViDevAttr;
	VI_DEV ViDev;
	HI_S32 s32Ret;
	VI_CHN_ATTR_S stChnAttr;
	VI_CHN ViChn;

	ViDev = vi_cfg.m_videv;
	ViChn = vi_cfg.m_vichn;

	s32Ret = HI_MPI_VI_DisableChn(ViChn);
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("SAMPLE_COMM_VI_StopChn failed with %#x\n",s32Ret);
		//return AV_FALSE;
	}
	s32Ret = HI_MPI_VI_DisableDev(ViDev);
	if (HI_SUCCESS != s32Ret)
	{
		DBG_PRT("SAMPLE_COMM_VI_StopDev failed with %#x\n", s32Ret);
		//return AV_FALSE;
	}
	return AV_OK;
}

int av_set_vpss_chn_mode(int channel_index, VPSS_GRP VpssGrp, int VpssChn)
{
	if (channel_index < 0 || channel_index > VI_CHN_START + g_av_platform_ctx.m_vichn_cnt + g_av_platform_ctx.m_filechn_cnt + g_av_platform_ctx.m_remotechn_cnt)
	{
		return AV_ERR_INVALID_PARAM;
	}
	
	VPSS_CHN_MODE_S pstVpssMode;
	HI_S32 s32Ret;
	channel_data_t *p_chn_dat = &g_av_platform_ctx.m_local_channel_ptr[channel_index];
	channel_cfg_t *p_chn_cfg = &p_chn_dat->m_cfg;
	vpss_cfg_t *p_vpss_cfg = &p_chn_cfg->m_vpss_cfg;

	if (VPSS_CHN_TYPE_MAJOR == VpssChn)
	{
		if (VPSS_CHN_MODE_USER == p_vpss_cfg->m_en_major_user_mode)
		{
			HI_MPI_VPSS_GetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			pstVpssMode.u32Width = p_chn_cfg->m_major_venc_width;
			pstVpssMode.u32Height = p_chn_cfg->m_major_venc_height;
			pstVpssMode.enPixelFormat = p_chn_cfg->m_pix_format;
			pstVpssMode.bDouble = HI_FALSE;
			pstVpssMode.enChnMode = VPSS_CHN_MODE_USER;
			s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			if (s32Ret != HI_SUCCESS)
			{
				DBG_PRT("HI_MPI_VPSS_SetChnMode USERMode with %#x\n", s32Ret);
				return AV_FALSE;
			}
		}
		else
		{
			pstVpssMode.enChnMode = VPSS_CHN_MODE_AUTO;
			s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			if (s32Ret != HI_SUCCESS)
			{
				DBG_PRT("HI_MPI_VPSS_SetChnMode AUTO_Mode with %#x\n", s32Ret);
				return AV_FALSE;
			}
		}

	}
	else if (VPSS_CHN_TYPE_MINOR == VpssChn)
	{
		if (VPSS_CHN_MODE_USER == p_vpss_cfg->m_en_minor_user_mode)
		{
			HI_MPI_VPSS_GetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			pstVpssMode.u32Width = p_chn_cfg->m_minor_venc_width;
			pstVpssMode.u32Height = p_chn_cfg->m_minor_venc_hight;
			pstVpssMode.enPixelFormat = p_chn_cfg->m_pix_format;
			pstVpssMode.bDouble = HI_FALSE;
			pstVpssMode.enChnMode = VPSS_CHN_MODE_USER;
			s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			if (s32Ret != HI_SUCCESS)
			{
				DBG_PRT("HI_MPI_VPSS_SetChnMode with %#x\n", s32Ret);
				return AV_FALSE;
			}
		}
		else
		{
			pstVpssMode.enChnMode = VPSS_CHN_MODE_AUTO;
			s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			if (s32Ret != HI_SUCCESS)
			{
				DBG_PRT("HI_MPI_VPSS_SetChnMode AUTO_Mode with %#x\n", s32Ret);
				return AV_FALSE;
			}
		}
	}
	else if (VPSS_CHN_TYPE_MINOR2 == VpssChn && )
	{
		if (VPSS_CHN_MODE_USER == p_vpss_cfg->m_en_minor2_user_mode)
		{
			HI_MPI_VPSS_GetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			pstVpssMode.u32Width = p_chn_cfg->m_minor2_venc_width;
			pstVpssMode.u32Height = p_chn_cfg->m_minor2_venc_height;
			pstVpssMode.enPixelFormat = p_chn_cfg->m_pix_format;
			pstVpssMode.bDouble = HI_FALSE;
			pstVpssMode.enChnMode = VPSS_CHN_MODE_USER;
			s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			if (s32Ret != HI_SUCCESS)
			{
				DBG_PRT("HI_MPI_VPSS_SetChnMode with %#x\n", s32Ret);
				return AV_FALSE;
			}
		}
		else
		{
			pstVpssMode.enChnMode = VPSS_CHN_MODE_AUTO;
			s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VpssChn, &pstVpssMode);
			if (s32Ret != HI_SUCCESS)
			{
				DBG_PRT("HI_MPI_VPSS_SetChnMode AUTO_Mode with %#x\n", s32Ret);
				return AV_FALSE;
			}
		}
	}
	else if (VPSS_CHN_TYPE_RENDER == VpssChn && VPSS_CHN_MODE_USER == p_vpss_cfg->m_en_render_user_mode)
	{
// 		HI_MPI_VPSS_GetChnMode(VpssGrp, VpssChn, &pstVpssMode);
// 		pstVpssMode.u32Width = p_chn_cfg->m_minor2_venc_width;
// 		pstVpssMode.u32Height = p_chn_cfg->m_minor2_venc_height;
// 		pstVpssMode.enPixelFormat = p_chn_cfg->m_pix_format;
// 		pstVpssMode.bDouble = HI_FALSE;
// 		pstVpssMode.enChnMode = VPSS_CHN_MODE_USER;
// 		s32Ret = HI_MPI_VPSS_SetChnMode(VpssGrp, VpssChn, &pstVpssMode);
// 		if (s32Ret != HI_SUCCESS)
// 		{
// 			DBG_PRT("HI_MPI_VPSS_SetChnMode with %#x\n", s32Ret);
// 			return AV_FALSE;
// 		}
	}

	return RETURN_OK;
}

int av_start_vpss(int channel_index, VPSS_GRP vpss_grp)
{
	VPSS_GRP VpssGrp = vpss_grp;
	VPSS_CHN VpssChn = vpss_chn;
	VPSS_GRP_ATTR_S stGrpAttr = {0};
	VPSS_CHN_ATTR_S stChnAttr = {0};
	VPSS_GRP_PARAM_S stVpssParam = {0};
	HI_S32 s32Ret;
	int i = 0;
	channel_data_t *p_chn_dat = &g_av_platform_ctx.m_local_channel_ptr[channel_index];
	channel_cfg_t *p_chn_cfg = &p_chn_dat->m_cfg;
	vpss_cfg_t *p_vpss_cfg = &p_chn_cfg->m_vpss_cfg;
	//输出给DH0 设备的VPSS信息
	if (SPECIAL_LIVE_CHN == channel_index)
	{
		p_vpss_cfg = &g_av_platform_ctx.m_cfg.m_vpss_cfg_ui;
	}
	

	stGrpAttr.u32MaxW = p_vpss_cfg->m_max_width;
	stGrpAttr.u32Max = p_vpss_cfg->m_max_height;
	stGrpAttr.bNrEn = p_vpss_cfg->m_en_nr;
	stGrpAttr.bIeEn = p_vpss_cfg->m_en_ie;
	stGrpAttr.bDciEn = p_vpss_cfg->m_en_dci;
	stGrpAttr.bHistEn = p_vpss_cfg->m_en_hist;
	stGrpAttr.bEsEn = p_vpss_cfg->m_en_es;
	stGrpAttr.enDieMode = p_vpss_cfg->m_en_die_mode;
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

	stVpssParam.u32Contrast = p_vpss_cfg->m_contrast;
	stVpssParam.u32DieStrength = p_vpss_cfg->m_dieStrength;
	stVpssParam.u32IeStrength = p_vpss_cfg->m_ieStrength;
	stVpssParam.u32SfStrength = p_vpss_cfg->m_sfStrength;
	stVpssParam.u32TfStrength = p_vpss_cfg->m_cTfStrength;
	stVpssParam.u32CfStrength = p_vpss_cfg->m_cfStrength;
	stVpssParam.u32CTfStrength = p_vpss_cfg->m_cTfStrength;
	stVpssParam.u32CvbsStrength = p_vpss_cfg->m_cTfStrength;
	stVpssParam.u32DeMotionBlurring = p_vpss_cfg->m_deMotionBlurring;
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

	for (i = 0; i < VPSS_MAX_CHN_NUM;i++)
	{
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
		av_set_vpss_chn_mode(channel_index, VpssGrp, i);
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

int av_stop_vpss(VPSS_GRP vpss_grp)
{
	HI_S32 s32Ret = HI_SUCCESS;
	VPSS_GRP VpssGrp = vpss_grp;

	int i;

	s32Ret = HI_MPI_VPSS_StopGrp(VpssGrp);
	if (s32Ret != HI_SUCCESS)
	{
		DBG_PRT("failed with %#x!\n", s32Ret);
		return AV_FALSE;
	}
	for (i = 0; i < VPSS_MAX_CHN_NUM;i++)
	{
		s32Ret = HI_MPI_VPSS_DisableChn(VpssGrp, i);
		if (s32Ret != HI_SUCCESS)
		{
			DBG_PRT("failed with %#x!\n", s32Ret);
			return AV_FALSE;
		}
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
	//_设置解码缓存，因为解码的视频不是公共缓存池传递过来的，所以得分配独立的模块公共缓存池
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
	cJSON_AddNumberToObject(json_tmp, "vpss_sfStrength_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_sfStrength);
	cJSON_AddNumberToObject(json_tmp, "vpss_ieStrength_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_ieStrength);
	cJSON_AddNumberToObject(json_tmp, "vpss_max_height_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_height);
	cJSON_AddNumberToObject(json_tmp, "vpss_max_width_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_width);
	cJSON_AddNumberToObject(json_tmp, "vpss_pixel_fmt_type_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_pixel_fmt_type);
	

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
	{
		g_av_platform_ctx.m_major_width_max = 1080;
		cJSON_AddNumberToObject(json_root, "major_width_max", g_av_platform_ctx.m_major_width_max);
	}
	json_tmp = cJSON_GetObjectItem(json_root, "major_height_max");
	if (json_tmp)
		g_av_platform_ctx.m_major_height_max = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_major_height_max = 1920;
		cJSON_AddNumberToObject(json_root, "major_height_max", g_av_platform_ctx.m_major_height_max);
	}
	json_tmp = cJSON_GetObjectItem(json_root, "major_vb_cnt");
	if (json_tmp)
		g_av_platform_ctx.m_major_vb_cnt = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_major_vb_cnt = 80;
		cJSON_AddNumberToObject(json_root, "major_vb_cnt", g_av_platform_ctx.m_major_vb_cnt);
	}
	json_tmp = cJSON_GetObjectItem(json_root, "minor_width_max");
	if (json_tmp)
		g_av_platform_ctx.m_minor_width_max = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_minor_width_max = 960;
		cJSON_AddNumberToObject(json_root, "minor_width_max", g_av_platform_ctx.m_minor_width_max);
	}

	json_tmp = cJSON_GetObjectItem(json_root, "minor_height_max");
	if (json_tmp)
		g_av_platform_ctx.m_minor_height_max = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_minor_height_max = 540;
		cJSON_AddNumberToObject(json_root, "minor_height_max", g_av_platform_ctx.m_minor_height_max);
	}
		

	json_tmp = cJSON_GetObjectItem(json_root, "minor_vb_cnt");
	if (json_tmp)
		g_av_platform_ctx.m_minor_vb_cnt = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_minor_vb_cnt = 50;
		cJSON_AddNumberToObject(json_root, "minor_vb_cnt", g_av_platform_ctx.m_minor_vb_cnt);
	}

	json_tmp = cJSON_GetObjectItem(json_root, "vpss_group_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_vpss_group_ddr_id = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_vpss_group_ddr_id = 0;
		cJSON_AddNumberToObject(json_root, "vpss_group_ddr_id", g_av_platform_ctx.m_vpss_group_ddr_id);
	}
		

	json_tmp = cJSON_GetObjectItem(json_root, "venc_group_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_venc_group_ddr_id = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_venc_group_ddr_id = 0;
		cJSON_AddNumberToObject(json_root, "venc_group_ddr_id", g_av_platform_ctx.m_venc_group_ddr_id);
	}

	json_tmp = cJSON_GetObjectItem(json_root, "venc_chn_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_venc_chn_ddr_id = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_venc_chn_ddr_id = 0;
		cJSON_AddNumberToObject(json_root, "venc_chn_ddr_id", g_av_platform_ctx.m_venc_chn_ddr_id);
	}

	json_tmp = cJSON_GetObjectItem(json_root, "vdec_chn_ddr_id");
	if (json_tmp)
		g_av_platform_ctx.m_vdec_chn_ddr_id = json_tmp->valueint;
	else
	{
		g_av_platform_ctx.m_vdec_chn_ddr_id = 0;
		cJSON_AddNumberToObject(json_root, "vdec_chn_ddr_id", g_av_platform_ctx.m_vdec_chn_ddr_id);
	}

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
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_dev_id = 0;
			cJSON_AddNumberToObject(_json, "dev_id_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_dev_id);
		}

		json_tmp = cJSON_GetObjectItem(_json, "out_dev_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type = json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type = OUT_DEV_TYPE_VGA;
			cJSON_AddNumberToObject(_json, "out_dev_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type);
		}	

		json_tmp = cJSON_GetObjectItem(_json, "resulotion_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type = (resulotion_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type = RESULOTION_TYPE_1920X1080;
			cJSON_AddNumberToObject(_json, "resulotion_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type);
		}

		json_tmp = cJSON_GetObjectItem(_json, "frame_rate_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type = (frame_rate_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type = FRAME_RATE_TYPE_60;
			cJSON_AddNumberToObject(_json, "frame_rate_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type);
		}
			

		json_tmp = cJSON_GetObjectItem(_json, "pixel_fmt_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type = (pixel_fmt_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
			cJSON_AddNumberToObject(_json, "pixel_fmt_type_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type);
		}
			

		json_tmp = cJSON_GetObjectItem(_json, "bg_color_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color = (pixel_fmt_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color = 0x0;
			cJSON_AddNumberToObject(_json, "bg_color_ui", g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color);
		}	
		
		json_tmp = cJSON_GetObjectItem(_json, "vpss_pixel_fmt_type_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_pixel_fmt_type = (pixel_fmt_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
			cJSON_AddNumberToObject(_json, "vpss_pixel_fmt_type_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_pixel_fmt_type);
		}
		json_tmp = cJSON_GetObjectItem(_json, "vpss_max_height_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_height = (pixel_fmt_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_height = 1080;
			cJSON_AddNumberToObject(json_tmp, "vpss_max_height_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_height);
		}
		json_tmp = cJSON_GetObjectItem(_json, "vpss_max_width_ui");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_width = (pixel_fmt_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_width = 1920;
			cJSON_AddNumberToObject(json_tmp, "vpss_max_width_ui", g_av_platform_ctx.m_cfg.m_vpss_cfg_ui.m_max_width);
		}
		//____________________________________________
		
		json_tmp = cJSON_GetObjectItem(_json, "dev_id_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id = json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id = 1;//DH1
			cJSON_AddNumberToObject(_json, "dev_id_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id);
		}
			
		
		json_tmp = cJSON_GetObjectItem(_json, "out_dev_type_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type = json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type = (OUT_DEV_TYPE_HDMI | OUT_DEV_TYPE_BT1120);
			cJSON_AddNumberToObject(_json, "out_dev_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type);
		}
			

		json_tmp = cJSON_GetObjectItem(_json, "resulotion_type_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type = (resulotion_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type = RESULOTION_TYPE_1920X1080;
			cJSON_AddNumberToObject(_json, "resulotion_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type);
		}

			

		json_tmp = cJSON_GetObjectItem(_json, "frame_rate_type_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type = (frame_rate_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type = FRAME_RATE_TYPE_60;
			cJSON_AddNumberToObject(_json, "frame_rate_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type);
		}
			

		json_tmp = cJSON_GetObjectItem(_json, "pixel_fmt_type_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type = (pixel_fmt_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
			cJSON_AddNumberToObject(_json, "pixel_fmt_type_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type);
		}
			

		json_tmp = cJSON_GetObjectItem(_json, "bg_color_live");
		if (json_tmp)
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color = (pixel_fmt_type_t)json_tmp->valueint;
		else
		{
			g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color = 0x0;
			cJSON_AddNumberToObject(_json, "bg_color_live", g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color);
		}
			
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

int av_get_vodev_and_volayer_by_id(int *VoDev, int *VoLayer, int dev_id)
{
	if (NULL == VoDev || NULL == VoLayer)
	{
		return AV_ERR_INVALID_PARAM;
	}
	
	if (0 == dev_id)
	{
		*VoDev = SAMPLE_VO_DEV_DHD0;
		GET_VO_LAYER(*VoLayer, *VoDev);
	}
	else if (1 == dev_id)
	{
		*VoDev = SAMPLE_VO_DEV_DHD1;
		GET_VO_LAYER(*VoLayer, *VoDev);
	}
	else if (2 == dev_id)
	{
		*VoDev = SAMPLE_VO_DEV_DSD0;
		GET_VO_LAYER(*VoLayer, *VoDev);
	}
	else if (3 == dev_id)
	{
		*VoDev = SAMPLE_VO_DEV_VIRT0;
		GET_VO_LAYER(*VoLayer, *VoDev);
	}
	else if (4 == dev_id)
	{
		*VoDev = SAMPLE_VO_DEV_VIRT1;
		GET_VO_LAYER(*VoLayer, *VoDev);
	}
	else if (5 == dev_id)
	{
		*VoDev = SAMPLE_VO_DEV_VIRT2;
		GET_VO_LAYER(*VoLayer, *VoDev);
	}
	else if (6 == dev_id)
	{
		*VoDev = SAMPLE_VO_DEV_VIRT3;
		GET_VO_LAYER(*VoLayer, *VoDev);
	}
	else
	{
		*VoDev = SAMPLE_VO_LAYER_VHD0;
		GET_VO_LAYER(*VoLayer, *VoDev);
	}
}
//_输出用户操作界面
int av_start_ui_out(av_platform_cfg_t av_platform_cfg)
{
	VO_DEV VoDev = 0;
	VO_LAYER VoLayer = 0;
	av_get_vodev_and_volayer_by_id(&VoDev, &VoLayer, av_platform_cfg.m_vo_cfg_ui.m_dev_id);

	if(AV_OK != av_start_vo_dev(av_platform_cfg.m_vo_cfg_ui, VoDev, VoLayer))
		DBG_PRT("av_start_vo_dev is failed\n");
	return AV_OK;
}

int av_stop_ui_out(av_platform_cfg_t av_platform_cfg)
{
	VO_DEV VoDev = 0;
	VO_LAYER VoLayer = 0;

	av_get_vodev_and_volayer_by_id(&VoDev, &VoLayer, av_platform_cfg.m_vo_cfg_ui.m_dev_id);
	av_stop_vo_dev(VoDev, VoLayer);
	return AV_OK;
}

//_输出直播画面
int av_start_live_out(av_platform_cfg_t av_platform_cfg)
{
	VO_DEV VoDev;
	VO_LAYER VoLayer;
	MPP_CHN_S stSrcChn, stDestChn;
	vo_chn_cfg_t vo_chn_cfg;

	av_get_vodev_and_volayer_by_id(&VoDev, &VoLayer, av_platform_cfg.m_vo_cfg_live.m_dev_id);

	if(AV_OK != av_start_vo_dev(av_platform_cfg.m_vo_cfg_live, VoDev, VoLayer))
		DBG_PRT("av_start_vo_dev is failed\n");
	av_platform_cfg.m_vpss_cfg_live.m_group_number = av_register_vpss_group();
	DBG_PRT("ui out vpss grp is %d\n", av_platform_cfg.m_vpss_cfg_live.m_group_number);
	av_start_vpss(SPECIAL_LIVE_CHN, av_platform_cfg.m_vpss_cfg_live.m_group_number);
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
	vo_chn_cfg_t vo_chn_cfg;

	vo_chn_cfg.m_chn_id = 0;
	vo_chn_cfg.m_deflicker = 0;
	vo_chn_cfg.m_layer_id = 0;
	vo_chn_cfg.m_x = 20;
	vo_chn_cfg.m_y = 20;
	vo_chn_cfg.m_width = 1920;
	vo_chn_cfg.m_height = 1080;
	av_get_vodev_and_volayer_by_id(&VoDev, &VoLayer, av_platform_cfg.m_vo_cfg_live.m_dev_id);
	stSrcChn.enModId    = HI_ID_VIU;
	stSrcChn.s32DevId   = 0;
	stSrcChn.s32ChnId   = VPSS_CHN_TYPE_RENDER;

	stDestChn.enModId   = HI_ID_VOU;
	stDestChn.s32ChnId  = vo_chn_cfg.m_chn_id;
	stDestChn.s32DevId  = VoLayer;
	HI_MPI_SYS_UnBind(stSrcChn, stDestChn);

	av_stop_vpss(av_platform_cfg.m_vpss_cfg_live.m_group_number);
	av_stop_vo_chn(vo_chn_cfg, VoLayer);
	av_stop_vo_dev(VoDev, VoLayer);
	return AV_OK;
}

int av_start_vir_vo(av_platform_cfg_t av_platform_cfg)
{
	VO_DEV VoDev;
	VO_LAYER VoLayer;
	MPP_CHN_S stSrcChn, stDestChn;
	vo_chn_cfg_t vo_chn_cfg;

	av_get_vodev_and_volayer_by_id(&VoDev, &VoLayer, av_platform_cfg.m_vo_cfg_live.m_dev_id);

	if(AV_OK != av_start_vo_dev(av_platform_cfg.m_vo_cfg_live, VoDev, VoLayer))
		DBG_PRT("av_start_vo_dev is failed\n");
	av_platform_cfg.m_vpss_cfg_live.m_group_number = av_register_vpss_group();
	DBG_PRT("ui out vpss grp is %d\n", av_platform_cfg.m_vpss_cfg_live.m_group_number);
	av_start_vpss(SPECIAL_LIVE_CHN, av_platform_cfg.m_vpss_cfg_live.m_group_number);
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

	return AV_OK;
}

int av_stop_vir_vo(av_platform_cfg_t av_platform_cfg)
{

	return AV_OK;
}

int av_set_com_rect(compound_cfg_t *p_set_chn, divison_mode_t division_mode) //画中画模式
{
	int i = 0;
	p_set_chn->m_division_mode = division_mode;
	switch(division_mode)
	{
	case DIVISON_MODE_0:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 1;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		break;
	case DIVISON_MODE_1:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 2;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height*2/3;
		break;
	case DIVISON_MODE_2:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 2;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height*2/3;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height*2/3;
		break;
	case DIVISON_MODE_3:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 2;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = p_set_chn->m_height/4;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[1].m_x = p_set_chn->m_width/2;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/4;
		break;
	case DIVISON_MODE_4:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 3;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height;
		p_set_chn->m_rect[0].m_x = p_set_chn->m_width/3;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = 0;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/2-p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_x = 0;
		p_set_chn->m_rect[2].m_y = p_set_chn->m_height/2;
		break;
	case DIVISON_MODE_5:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 3;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/2-p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[2].m_y = p_set_chn->m_height/2;
		break;
	case DIVISON_MODE_6:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 3;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[0].m_x = p_set_chn->m_width/4;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[1].m_x = 0;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/2;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[2].m_x = p_set_chn->m_width/2;
		p_set_chn->m_rect[2].m_y = p_set_chn->m_height/2;
		break;
	case DIVISON_MODE_7:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 4;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[1].m_x = 0;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/2;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[2].m_x = p_set_chn->m_width/2;
		p_set_chn->m_rect[2].m_y = 0;
		p_set_chn->m_rect[3].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[3].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[3].m_x = p_set_chn->m_width/2;
		p_set_chn->m_rect[3].m_y = p_set_chn->m_height/2;
		break;
	case DIVISON_MODE_8:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 6;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height*2/3;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = 0;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height*2/3;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_x = p_set_chn->m_width/3;
		p_set_chn->m_rect[2].m_y = p_set_chn->m_height*2/3;
		p_set_chn->m_rect[3].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[3].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[3].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[3].m_y = 0;
		p_set_chn->m_rect[4].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[4].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[4].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[4].m_y = p_set_chn->m_height/3;
		p_set_chn->m_rect[5].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[5].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[5].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[5].m_y = p_set_chn->m_height*2/3;
		break;
	case DIVISON_MODE_USER1:

		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 1;
		//for(i = 0;i < p_set_chn->m_count;i++)
		{
			p_set_chn->m_rect[i].m_width = 1920;
			p_set_chn->m_rect[i].m_height = 1080;
			p_set_chn->m_rect[i].m_x = 0;
			p_set_chn->m_rect[i].m_y = 0;
		}
		break;
	default:
		break;
	}
	return AV_OK;
}

int av_init_cfg()
{
	int i = 0, j = 0;
	memset(&g_av_platform_ctx, 0, sizeof(g_av_platform_ctx));
	g_av_platform_ctx.m_major_height_max = 1080;
	g_av_platform_ctx.m_major_width_max = 1920;
	g_av_platform_ctx.m_major_vb_cnt = 80;
	g_av_platform_ctx.m_minor_width_max = 960;
	g_av_platform_ctx.m_minor_height_max = 540;
	g_av_platform_ctx.m_minor_vb_cnt = 50;
	g_av_platform_ctx.m_vpss_group_ddr_id = 0;
	g_av_platform_ctx.m_venc_group_ddr_id = 0;
	g_av_platform_ctx.m_venc_chn_ddr_id = 0;
	g_av_platform_ctx.m_vdec_chn_ddr_id = 0;
	g_av_platform_ctx.m_vichn_cnt = 6;	
	g_av_platform_ctx.m_filechn_cnt = 2;
	g_av_platform_ctx.m_remotechn_cnt = 4;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_dev_id = VO_DEV_DHD0;	//DH0
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_out_dev_type = OUT_DEV_TYPE_VGA;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_resulotion_type = RESULOTION_TYPE_1920X1080;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_frame_rate_type = FRAME_RATE_TYPE_60;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
	g_av_platform_ctx.m_cfg.m_vo_cfg_ui.m_bg_color = 0x0;
	//___________VPSS参数
	//_Group参数:这部分参数如果从文件中加在失败，则始终使用从系统中获取的参数
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
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_dev_id = VO_DEV_DHD1;//DH1
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_out_dev_type = (OUT_DEV_TYPE_HDMI | OUT_DEV_TYPE_BT1120);
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_resulotion_type = RESULOTION_TYPE_1920X1080;
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_frame_rate_type = FRAME_RATE_TYPE_60;
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
	g_av_platform_ctx.m_cfg.m_vo_cfg_live.m_bg_color = 0x0;
	//compound
	//movie
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_width = 1920;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_height = 1080;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_count = 0;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_chn[0] = VI_CHN_START + 1;//_默认显示第二路vi画面
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_chn[1] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_chn[2] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_chn[3] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_chn[4] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_chn[5] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_show_mode = COMPOUND_SHOW_MODE_SCALE;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_division_mode = DIVISON_MODE_0;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_vo_dev_cfg.m_dev_id = VO_DEV_VIRT0;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_vo_dev_cfg.m_out_dev_type = OUT_DEV_TYPE_BT1120;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_vo_dev_cfg.m_resulotion_type = RESULOTION_TYPE_1920X1080;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_vo_dev_cfg.m_frame_rate_type = FRAME_RATE_TYPE_60;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_MOVIE].m_vo_dev_cfg.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
	//pvw
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_width = 960;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_height = 540;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_count = 0;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[0] = VI_CHN_START + 1;//_默认显示第二路vi画面
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[1] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[2] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[3] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[4] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[5] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_show_mode = COMPOUND_SHOW_MODE_SCALE;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_division_mode = DIVISON_MODE_0;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_dev_id = VO_DEV_VIRT1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_out_dev_type = OUT_DEV_TYPE_BT1120;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_resulotion_type = RESULOTION_TYPE_1280X720;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_frame_rate_type = FRAME_RATE_TYPE_50;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;

	//vp
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_width = 960;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_height = 540;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_count = 0;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[0] = VI_CHN_START + 1;//_默认显示第2路vi画面
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[1] = VI_CHN_START + 3;//_默认显示第3路vi画面
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[2] = VI_CHN_START + 4;//_默认显示第4路vi画面
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[3] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[4] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_chn[5] = -1;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_show_mode = COMPOUND_SHOW_MODE_SCALE;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_division_mode = DIVISON_MODE_4;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_dev_id = VO_DEV_VIRT2;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_out_dev_type = OUT_DEV_TYPE_BT1120;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_resulotion_type = RESULOTION_TYPE_1920X1080;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_frame_rate_type = FRAME_RATE_TYPE_60;
	g_av_platform_ctx.m_cfg.m_compound_cfg[COMPOUND_CHN_PVW].m_vo_dev_cfg.m_pixel_fmt_type = PIXEL_FMT_TYPE_YUV420;
	
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
	int vir_and_vi_chn_cnt;
	vi_cfg_t vi_cfg;
	channel_data_t *p_chn_dat;
	channel_cfg_t *p_chn_cfg;

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
	//_申请通道参数空间
	g_av_platform_ctx.m_local_channel_ptr = (channel_data_t *)calloc(1, VI_CHN_START + g_av_platform_ctx.m_vichn_cnt);
	for (i = 0;i < g_av_platform_ctx.m_vichn_cnt; i++)
	{
		g_av_platform_ctx.m_all_channel_ptr[i] = &(g_av_platform_ctx.m_local_channel_ptr[i]);
	}
	g_av_platform_ctx.m_local_chn_index_offset = 0;
	g_av_platform_ctx.m_file_channel_ptr = (channel_data_t *)calloc(1, g_av_platform_ctx.m_filechn_cnt);
	for (i = 0; i < g_av_platform_ctx.m_filechn_cnt;i++)
	{
		g_av_platform_ctx.m_all_channel_ptr[g_av_platform_ctx.m_vichn_cnt + i] = &(g_av_platform_ctx.m_file_channel_ptr[i]);
	}
	g_av_platform_ctx.m_file_chn_index_offset = g_av_platform_ctx.m_filechn_cnt;
	core_channel_local_setup();
	for(i = VI_CHN_START; i < g_av_platform_ctx.m_vichn_cnt; i++)
	{
		memcpy(&vi_cfg, &g_av_platform_ctx.m_local_channel_ptr[i], sizeof(vi_cfg));
		av_start_vi(vi_cfg);
	}
	vir_and_vi_chn_cnt = VI_CHN_START + g_av_platform_ctx.m_vichn_cnt;
	for (i = 0; i < vir_and_vi_chn_cnt;i++)
	{
		p_chn_dat = &g_av_platform_ctx.m_local_channel_ptr[i];
		p_chn_cfg = &p_chn_dat->m_cfg;
		p_chn_cfg->m_vpss_cfg.m_group_number = av_register_vpss_group();
		av_start_vpss(i, p_chn_cfg->m_vpss_cfg.m_group_number);
	}
	
	return AV_OK;
}

int  av_shutdown()
{
	int i = 0;
	vi_cfg_t vi_cfg;

	for(i = VI_CHN_START; i < g_av_platform_ctx.m_vichn_cnt; i++)
	{
		memcpy(&vi_cfg, &g_av_platform_ctx.m_local_channel_ptr[i], sizeof(vi_cfg));
		av_start_vi(vi_cfg);
	}
	av_stop_ui_out(g_av_platform_ctx.m_cfg);
	av_stop_live_out(g_av_platform_ctx.m_cfg);
	av_stop_hdmi();
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