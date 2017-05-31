#include "../includes.h"
#include <string.h>
#include <stdio.h>

int av_start_vo_chn(vo_chn_info_t vo_chn_info, VO_LAYER vo_layer);
int av_handler_set_com_rect_by_mode(compound_cfg_t *p_set_chn, divison_mode_t division_mode) //���л�ģʽ
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
		// 		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		// 		p_set_chn->m_count = 1;
		// 		//for(i = 0;i < p_set_chn->m_count;i++)
		// 		{
		// 			p_set_chn->m_rect[i].m_width = 1920;
		// 			p_set_chn->m_rect[i].m_height = 1080;
		// 			p_set_chn->m_rect[i].m_x = 0;
		// 			p_set_chn->m_rect[i].m_y = 0;
		// 		}
		break;
	default:
		break;
	}
	return AV_OK;
}

//===========_����˵��===========
//compound_chn:_�ϳ�ͨ��ID
//index:_�ϳ�ͨ������ͨ����������
//chn:_��ƵԴͨ����
//===============================
int av_handler_set_com_chn(compound_chn_t compound_chn, int index, int chn)
{
	if (compound_chn < 0 || compound_chn > COMPOUND_CHN_VP || index >= COMPOUND_SUB_CHN_MAX \
		|| chn < 0 || chn > g_av_platform_ctx.m_vichn_cnt + g_av_platform_ctx.m_filechn_cnt \
		+ g_av_platform_ctx.m_remotechn_cnt + VI_CHN_START)
	{
		return AV_ERR_INVALID_PARAM;
	}
	compound_cfg_t *p_compound_cfg = &g_av_platform_ctx.m_cfg.m_compound_cfg[compound_chn];
	compound_cfg_t tmp_compound_cfg;
	
	memcpy(&tmp_compound_cfg, p_compound_cfg, sizeof(tmp_compound_cfg));
	tmp_compound_cfg.m_chn[index] = chn;
	av_stop_compound_effect(compound_chn);
	//_�豸��Դ���ޣ�����ֻ��DIVISON_MODE_4���µ�ģʽ��֧����Ч
	if (p_compound_cfg->m_division_mode < DIVISON_MODE_4 && g_tde_ctx.m_effect_data->m_mode > EFFECT_MODE_NONE)
	{
		av_start_compound_effect(compound_chn);
	}
	else
	{
		av_set_compound_vo_chn(compound_chn, &tmp_compound_cfg);
	}
	av_save_cfg();

	return AV_OK;
}

int av_bind_vo_and_vpss(int VpssGrp, int VpssChn, int VoDev, int VoChn)
{
	HI_S32 s32Ret = HI_SUCCESS;
	MPP_CHN_S stSrcChn, stDestChn;

	stSrcChn.enModId    = HI_ID_VPSS;
	stSrcChn.s32DevId   = VpssGrp;
	stSrcChn.s32ChnId   = VpssChn;

	stDestChn.enModId   = HI_ID_VOU;
	stDestChn.s32ChnId  = VoChn;
	stDestChn.s32DevId  = VoDev;
	if(HI_SUCCESS != (s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn)))
	{
		DBG_PRT("VpssGrp[%d] VpssChn[%d] VoDev[%d] VoChn[%d] HI_MPI_SYS_Bind failed with 0x%X\n", VpssGrp, VpssChn, VoDev, VoChn, s32Ret);
		return AV_FALSE;
	}

	return AV_OK;
}

int av_unbind_vo_and_vpss(int VpssGrp, int VpssChn, int VoDev, int VoChn)
{
	HI_S32 s32Ret = HI_SUCCESS;
	MPP_CHN_S stSrcChn, stDestChn;

	stSrcChn.enModId    = HI_ID_VPSS;
	stSrcChn.s32DevId   = VpssGrp;
	stSrcChn.s32ChnId   = VpssChn;

	stDestChn.enModId   = HI_ID_VOU;
	stDestChn.s32ChnId  = VoChn;
	stDestChn.s32DevId  = VoDev;
	if(HI_SUCCESS != (s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn)))
	{
		DBG_PRT("VpssGrp[%d] VpssChn[%d] VoDev[%d] VoChn[%d] HI_MPI_SYS_UnBind failed with %d\n", VpssGrp, VpssChn, VoDev, VoChn, s32Ret);
		return AV_FALSE;
	}

	return AV_OK;
}

int render_display_callback(RENDER_DIS_CFG_S render_dis_cfg)
{
	vo_chn_info_t vo_chn_info;
	VO_DEV VoDev;
	VO_LAYER VoLayer;

	av_get_vodev_and_volayer_by_id(&VoDev, &VoLayer, render_dis_cfg.m_client_info.m_VoDev);
	vo_chn_info.m_chn_id = render_dis_cfg.m_vo_chn;
	vo_chn_info.m_deflicker = 0;
	vo_chn_info.m_layer_id = 0;
	vo_chn_info.m_x = render_dis_cfg.m_rect.m_s32X;
	vo_chn_info.m_y = render_dis_cfg.m_rect.m_s32Y;
	vo_chn_info.m_width = render_dis_cfg.m_rect.m_u32Width;
	vo_chn_info.m_height = render_dis_cfg.m_rect.m_u32Height;
	av_start_vo_chn(vo_chn_info, VoLayer);
	HI_MPI_VO_SetChnFrameRate(VoLayer, vo_chn_info.m_chn_id, 60);

	av_bind_vo_and_vpss(render_dis_cfg.m_client_info.m_VpssGrp, render_dis_cfg.m_client_info.m_VpssChn, render_dis_cfg.m_client_info.m_VoDev, render_dis_cfg.m_vo_chn);

	return AV_OK;
}