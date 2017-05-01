#include "../includes.h"
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

tde_ctx_t g_tde_ctx;

void *av_tde_process_thread(void * arg)
{
	int s32Ret = HI_SUCCESS;
	VIDEO_FRAME_INFO_S pstVideoFrame;
	tde_effect_data_t *tde_effect_data = (tde_effect_data_t *)arg;
	compound_cfg_t *p_compoun_cfg = &g_av_platform_ctx.m_cfg.m_compound_cfg[tde_effect_data->m_vo_id];
	channel_data_t *p_chn_dat = g_av_platform_ctx.m_all_channel_ptr[tde_effect_data->m_vo_id];
	channel_cfg_t *p_chn_cfg = &p_chn_dat->m_cfg;
	DBG_PRT("tde_effect_data->m_vo_id = %d  grp = %d\n", tde_effect_data->m_vo_id, p_compoun_cfg->m_vpss_cfg.m_group_number);
	while (1)
	{
// 		s32Ret=HI_MPI_VPSS_GetChnFrame(p_compoun_cfg->m_vpss_cfg.m_group_number, VPSS_CHN_TYPE_RENDER, &pstVideoFrame, 5);
// 		DBG_PRT("[%d]HI_MPI_VPSS_UserGetFrame VPSS[%d] failed with %08X!\n", tde_effect_data->m_vo_id, p_compoun_cfg->m_vpss_cfg.m_group_number, s32Ret);
		
		while(HI_SUCCESS != (s32Ret=HI_MPI_VPSS_GetChnFrame(p_compoun_cfg->m_vpss_cfg.m_group_number, VPSS_CHN_TYPE_RENDER, &pstVideoFrame, 5)) && (TDE_EXIT_CMD_START == g_tde_ctx.m_exit_cmd) )
		{
			if(s32Ret != HI_ERR_VPSS_NOBUF)
			{
				DBG_PRT("[%d]HI_MPI_VPSS_GetChnFrame VPSS[%d] failed with %08X!\n", tde_effect_data->m_vo_id, p_compoun_cfg->m_vpss_cfg.m_group_number, s32Ret);
			}
		}
		if (TDE_EXIT_CMD_STOP == g_tde_ctx.m_exit_cmd)
		{
			break;
		}
		//_发送给通道VPSS
		s32Ret = HI_MPI_VPSS_SendFrame(p_chn_cfg->m_vpss_cfg.m_group_number, &pstVideoFrame,  0);
		if(s32Ret != HI_SUCCESS)
		{
			DBG_PRT("compound[%d] chn_group[%d] HI_MPI_VPSS_UserSendFrameTimeout failed with 0x%08X\n", tde_effect_data->m_vo_id,p_chn_cfg->m_vpss_cfg.m_group_number, s32Ret);
		}
		//_发给直播输出设备绑定的vpss
		if (0 == tde_effect_data->m_vo_id)
		{
			s32Ret = HI_MPI_VPSS_SendFrame(g_av_platform_ctx.m_cfg.m_vpss_cfg_live.m_group_number, &pstVideoFrame,  0);
			if(s32Ret != HI_SUCCESS)
			{
				DBG_PRT("compound[%d]HI_MPI_VPSS_UserSendFrameTimeout failed with 0x%08X\n", tde_effect_data->m_vo_id, s32Ret);
			}
		}
		s32Ret = HI_MPI_VPSS_ReleaseChnFrame (p_compoun_cfg->m_vpss_cfg.m_group_number, VPSS_CHN_TYPE_RENDER,  &pstVideoFrame);
		if (s32Ret != HI_SUCCESS)
		{
			DBG_PRT("[%d]HI_MPI_VPSS_UserReleaseFrame VPSS[%d] failed with %08X!\n",tde_effect_data->m_vo_id, p_compoun_cfg->m_vpss_cfg.m_group_number, s32Ret);
		}
	}
	DBG_PRT("exit av_tde_process_thread[%d]\n", tde_effect_data->m_vo_id);
	return NULL;
}

int av_tde_set_cfg(int index, tde_effect_cfg_t effect_cfg)
{
	if (index < 0 || index > VIR_VO_DEV_MAX)
	{
		return AV_ERR_INVALID_PARAM;
	}
	pthread_mutex_lock(&g_tde_ctx.m_effect_data[index].m_mutex);
	memcpy(&g_tde_ctx.m_tde_cfg.m_effect_cfg[index], &effect_cfg, sizeof(g_tde_ctx.m_tde_cfg.m_effect_cfg[index]));
	pthread_mutex_unlock(&g_tde_ctx.m_effect_data[index].m_mutex);
	return AV_OK;
}

int av_tde_load_cfg()
{

	return AV_OK;
}

int av_tde_save_cfg(tde_cfg_t *ptde_cfg)
{
	if (!ptde_cfg)
	{
		return AV_ERR_INVALID_PARAM;
	}

	return AV_OK;
}

int av_tde_start_effect_by_chn(compound_chn_t compound_chn)
{

	return AV_OK;
}

int av_tde_stop_effect_by_chn(compound_chn_t compound_chn)
{

	return AV_OK;
}

int av_tde_init()
{

	return AV_OK;
}

int av_tde_unint()
{

	return AV_OK;
}

int av_tde_startup()
{
	int i;

	memset(&g_tde_ctx, 0 ,sizeof(g_tde_ctx));
	for (i = 0;i < VIR_VO_DEV_MAX;i++)
	{
		g_tde_ctx.m_effect_data[i].m_vo_id = i;
		pthread_mutex_init(&g_tde_ctx.m_effect_data[i].m_mutex, NULL);
		pthread_create(&g_tde_ctx.m_effect_data[i].m_thread, NULL, av_tde_process_thread, (void *)(&g_tde_ctx.m_effect_data[i]));
		if(g_tde_ctx.m_effect_data[i].m_thread == 0)
		{
			DBG_PRT("pthread_create vir_vo[%d]  tde process is failed\n", i);
			continue;
		}
	}
	return AV_OK;
}

int av_tde_shutdown()
{

	return AV_OK;
}

int av_start_compound_effect(compound_chn_t compound_chn)
{

	return AV_OK;
}

int av_stop_compound_effect(compound_chn_t compound_chn)
{

	return AV_OK;
}