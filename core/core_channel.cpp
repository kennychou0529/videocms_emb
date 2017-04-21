#include "../includes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*****************************************************
_vi通道配置加载流程：
1、加载和初始化vi的json时，都以vi配置的jso数组作为接口的实参；
2、接口内部通过通道的vi_json_id去指定加载json数组中的固定的配置；
3、


*****************************************************/

int core_channel_init_vi_cfg(int channel_index, cJSON *arry_json)
{
	if (channel_index < 0 || channel_index >= g_av_platform_ctx.m_vichn_cnt || !arry_json)
	{
		return AV_ERR_INVALID_PARAM;
	}
	if (CHANNEL_TYPE_LOCAL_CHANNEL != g_av_platform_ctx.m_all_channel_ptr[channel_index]->m_channel_type)
	{
		DBG_PRT("chn[%d] is vi chn\n");
		return AV_ERR_INVALID_PARAM;
	}
	cJSON *sub_json;
	char str[32];
	vi_cfg_t *p_vi_cfg = (vi_cfg_t *)&g_av_platform_ctx.m_local_channel_ptr[channel_index].m_cfg.m_vi_cfg;
	
	p_vi_cfg->m_mask0 = 0xff0000;
	p_vi_cfg->m_mask1 = 0x0;
	p_vi_cfg->m_width = HD_WIDTH;
	p_vi_cfg->m_height = HD_HEIGHT;
	p_vi_cfg->m_fps = 30;
	p_vi_cfg->m_interface = BT1120I;
	p_vi_cfg->m_videv = channel_index - VI_CHN_START;
	p_vi_cfg->m_vichn = (channel_index - VI_CHN_START) * 4;
	p_vi_cfg->m_colorspace = YUV420;
	p_vi_cfg->m_mirrorflip = VI_CHN_SET_NORMAL;
	p_vi_cfg->m_interlaced = 0;//_逐行
	p_vi_cfg->m_src_fps = -1;
	p_vi_cfg->m_dst_fps = -1;
	sub_json = cJSON_CreateObject();
	if (!sub_json)
	{
		return AV_ERR_OPEN_JSON_FAILED;
	}
	snprintf(str, sizeof(str), "0x%x", p_vi_cfg->m_mask0);
	cJSON_AddStringToObject(sub_json,"mask0", str);
	memset(str, 0, sizeof(str));
	snprintf(str, sizeof(str), "0x%x", p_vi_cfg->m_mask1);
	cJSON_AddStringToObject(sub_json,"mask1", str);
	cJSON_AddNumberToObject(sub_json,"width", p_vi_cfg->m_width);
	cJSON_AddNumberToObject(sub_json,"height", p_vi_cfg->m_width);
	cJSON_AddNumberToObject(sub_json,"fps", p_vi_cfg->m_fps);
	cJSON_AddNumberToObject(sub_json,"interface", p_vi_cfg->m_interface);
	cJSON_AddNumberToObject(sub_json,"videv", p_vi_cfg->m_videv);
	cJSON_AddNumberToObject(sub_json,"vichn", p_vi_cfg->m_vichn);
	cJSON_AddNumberToObject(sub_json,"colorspace", p_vi_cfg->m_colorspace);
	cJSON_AddNumberToObject(sub_json,"mirrorflip", p_vi_cfg->m_mirrorflip);
	cJSON_AddNumberToObject(sub_json,"src_fps", p_vi_cfg->m_src_fps);
	cJSON_AddNumberToObject(sub_json,"dst_fps", p_vi_cfg->m_dst_fps);
	return AV_OK;
}

int core_channel_uninit_vi_cfg()
{

	return AV_OK;
}

int core_channel_load_vi_cfg(int channel_index, int vi_josn_id, cJSON *arry_json)
{
	if (channel_index < 0 || channel_index >= g_av_platform_ctx.m_vichn_cnt || \
		vi_josn_id < 0 || vi_josn_id >= g_av_platform_ctx.m_vichn_cnt || arry_json)
	{
		return AV_ERR_INVALID_PARAM;
	}
	if (CHANNEL_TYPE_LOCAL_CHANNEL != g_av_platform_ctx.m_all_channel_ptr[channel_index]->m_channel_type)
	{
		DBG_PRT("chn[%d] is vi chn\n");
		return AV_ERR_INVALID_PARAM;
	}
	cJSON *json, *sub_json;
	char str[32];
	vi_cfg_t *p_vi_cfg = (vi_cfg_t *)&g_av_platform_ctx.m_local_channel_ptr[channel_index].m_cfg.m_vi_cfg;

	json = cJSON_GetArrayItem(arry_json, vi_josn_id);
	if (!sub_json)
	{
		return AV_FALSE;
	}
	sub_json = cJSON_GetObjectItem(json, "mask0");
	if (!sub_json)
	{
		p_vi_cfg->m_mask0 = 0xff0000;
		snprintf(str, sizeof(str), "0x%x", p_vi_cfg->m_mask0);
		cJSON_AddStringToObject(sub_json,"mask0", str);
	}
	else
	{
		StrToNumber(sub_json->valuestring, &p_vi_cfg->m_mask0);
	}
	sub_json = cJSON_GetObjectItem(json, "mask1");
	if (!sub_json)
	{
		p_vi_cfg->m_mask0 = 0xff0000;
		snprintf(str, sizeof(str), "0x%x", p_vi_cfg->m_mask0);
		cJSON_AddStringToObject(sub_json,"mask1", str);
	}
	else
	{
		StrToNumber(sub_json->valuestring, &p_vi_cfg->m_mask1);
	}
	sub_json = cJSON_GetObjectItem(json, "width");
	if (!sub_json)
	{
		p_vi_cfg->m_width = HD_WIDTH;
		cJSON_AddNumberToObject(sub_json,"width", p_vi_cfg->m_width);
	}
	else
	{
		p_vi_cfg->m_width = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "height");
	if (!sub_json)
	{
		p_vi_cfg->m_height = HD_HEIGHT;
		cJSON_AddNumberToObject(sub_json,"height", p_vi_cfg->m_width);
	}
	else
	{
		p_vi_cfg->m_height = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "fps");
	if (!sub_json)
	{
		p_vi_cfg->m_fps = 30;
		cJSON_AddNumberToObject(sub_json,"fps", p_vi_cfg->m_width);
	}
	else
	{
		p_vi_cfg->m_fps = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "interface");
	if (!sub_json)
	{
		p_vi_cfg->m_interface = BT1120I;
		cJSON_AddNumberToObject(sub_json,"interface", p_vi_cfg->m_interface);
	}
	else
	{
		p_vi_cfg->m_interface = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "videv");
	if (!sub_json)
	{
		p_vi_cfg->m_videv = channel_index - VI_CHN_START;
		cJSON_AddNumberToObject(sub_json,"videv", p_vi_cfg->m_videv);
	}
	else
	{
		p_vi_cfg->m_videv = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "vichn");
	if (!sub_json)
	{
		p_vi_cfg->m_vichn = (channel_index - VI_CHN_START) * 4;
		cJSON_AddNumberToObject(sub_json,"vichn", p_vi_cfg->m_vichn);
	}
	else
	{
		p_vi_cfg->m_vichn = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "colorspace");
	if (!sub_json)
	{
		p_vi_cfg->m_colorspace = YUV420;
		cJSON_AddNumberToObject(sub_json,"colorspace", p_vi_cfg->m_colorspace);
	}
	else
	{
		p_vi_cfg->m_colorspace = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "mirrorflip");
	if (!sub_json)
	{
		p_vi_cfg->m_mirrorflip = VI_CHN_SET_NORMAL;
		cJSON_AddNumberToObject(sub_json,"mirrorflip", p_vi_cfg->m_mirrorflip);
	}
	else
	{
		p_vi_cfg->m_mirrorflip = (vi_chn_set_e)sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "interlaced");
	if (!sub_json)
	{
		p_vi_cfg->m_interlaced = 0;
		cJSON_AddNumberToObject(sub_json,"interlaced", p_vi_cfg->m_interlaced);
	}
	else
	{
		p_vi_cfg->m_interlaced = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "src_fps");
	if (!sub_json)
	{
		p_vi_cfg->m_src_fps = 0;
		cJSON_AddNumberToObject(sub_json,"src_fps", p_vi_cfg->m_src_fps);
	}
	else
	{
		p_vi_cfg->m_src_fps = sub_json->valueint;
	}
	sub_json = cJSON_GetObjectItem(json, "dst_fps");
	if (!sub_json)
	{
		p_vi_cfg->m_dst_fps = 0;
		cJSON_AddNumberToObject(sub_json,"dst_fps", p_vi_cfg->m_dst_fps);
	}
	else
	{
		p_vi_cfg->m_dst_fps = sub_json->valueint;
	}
	
	return AV_OK;
}

// int core_channel_init_vpss_cfg()
// {
// 
// 	return AV_OK;
// }
// 
// int core_channel_unint_vpss_cfg()
// {
// 
// 	return AV_OK;
// }
// 
// int core_channel_load_vpss_cfg()
// {
// 
// 	return AV_OK;
// }

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
	cJSON *hardware_json;
	FILE *hardware_fp;
	channel_data_t *p_chn_data = g_av_platform_ctx.m_all_channel_ptr[channel_index];
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
	p_chn_cfg->m_pix_format = YUV420;
	if (channel_index < VI_CHN_START || channel_index >= g_av_platform_ctx.m_vichn_cnt)
	{
		p_chn_cfg->vi_json_id = -1;
	}
	else
	{
		p_chn_cfg->vi_json_id = channel_index - VI_CHN_START;
	}
	core_channel_init_vi_cfg(channel_index);
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