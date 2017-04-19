#include "../includes.h"
#include <stdlib.h>

int core_channel_save_cfg()
{

	return RETURN_OK;
}

int core_channel_load_cfg()
{

	return RETURN_OK;
}

int core_channel_init_cfg(int channel_index)
{
	if (channel_index < 0 || channel_index > g_av_platform_ctx.m_vichn_cnt + g_av_platform_ctx.m_filechn_cnt + g_av_platform_ctx.m_remotechn_cnt)
	{
		return AV_ERR_INVALID_PARAM;
	}
	channel_data_t *p_chn_data = &g_av_platform_ctx.m_all_channel_ptr[channel_index];
	channel_cfg_t *p_chn_cfg = &p_chn_data->m_cfg;
	p_chn_cfg->m_raw_fps = -1;
	p_chn_cfg->m_raw_width = -1;
	p_chn_cfg->m_raw_height = -1;
	p_chn_cfg->m_major_venc_fps = 25;
	p_chn_cfg->m_major_venc_width = 1920;
	p_chn_cfg->m_major_venc_height = 1080;
	p_chn_cfg->m_minor_venc_fps = 25;
	p_chn_cfg->m_minor_venc_width = 640;
	p_chn_cfg->m_minor_venc_height = 360;
	p_chn_cfg->m_minor2_venc_fps = 25;
	p_chn_cfg->m_minor2_venc_width = 960;
	p_chn_cfg->m_minor2_venc_height = 540;
	p_chn_cfg->m_vi_cfg.
	return RETURN_OK;
}

int core_channel_uninit_cfg()
{

	return RETURN_OK;
}

int core_channel_local_setup()
{
	
	
	return RETURN_OK;
}

int core_channel_local_desetup()
{

	return RETURN_OK;
}