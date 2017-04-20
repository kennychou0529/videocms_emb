#include "../includes.h"
#include <string.h>

tde_ctx_t g_tde_ctx;

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
	memset(&g_tde_ctx, 0 ,sizeof(g_tde_ctx));
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