#include "../includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AV_CFG_JSON_PATH  "/mnt/mtd/app/config"

int av_init_cfg();

av_platform_ctx_t g_av_platform_ctx;

int av_init_mpp_sys(void)
{
	VB_CONF_S st_vb_conf;
	MPP_SYS_CONF_S st_mpp_sys_conf;

	memset(&st_vb_conf, 0, sizeof(st_vb_conf));
	memset(&st_mpp_sys_conf, 0, sizeof(st_mpp_sys_conf));
	st_vb_conf.astCommPool[0].u32BlkSize = g_av_platform_ctx.m_major_height_max * 

	return AV_OK;
}

int av_uninit_mpp_sys(void)
{


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
	cJSON *json_root = NULL, *json_tmp;
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
	json_fp = fopen(AV_CFG_JSON_PATH, "w+");
	if (!json_fp)
	{
		ret = AV_ERR_OPEN_FILE_FAILED;
	}
	str = cJSON_Print(json_root);
	if (str)
	{
		fwrite(str, strlen(str), 1, json_fp);
		free(str);
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

int av_init_cfg()
{

	return AV_OK;
}

int av_uninit_cfg()
{
	av_save_cfg();
	return AV_OK;
}

int av_startup()
{
	memset(&g_av_platform_ctx, 0, sizeof(av_platform_ctx_t));
	if (AV_OK != av_load_cfg())
	{
		av_init_cfg();
		av_save_cfg();
	}

	return AV_OK;
}

int  av_shutdown()
{
	av_uninit_cfg();
	return AV_OK;
}