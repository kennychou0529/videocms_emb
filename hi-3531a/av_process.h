#ifndef __AV_PROCESS_H__
#define __AV_PROCESS_H__

#define AV_OK 0
#define AV_FALSE					-1
#define AV_ERR_NOMEMORY				-2
#define AV_ERR_INVALID_PARAM		-3
#define AV_ERR_OPEN_FILE_FAILED     -4
#define AV_ERR_FILE_EMPTY			-5
#define AV_ERR_PARSE_JSON_FAILED	-6
#define AV_ERR_OPEN_JSON_FAILED		-7

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct __av_platform_cfg_s
{

}av_platform_cfg_t;

typedef struct _av_platform_ctx_s
{
	int m_sys_align_width;					//_图像对齐跨度
	int m_major_width_max;					//_major最大分辨率宽
	int m_major_height_max;					//_major最大分辨率高
	int m_major_vb_cnt;						//_major缓存块个数
	int m_minor_width_max;					//_minor最大分辨率宽
	int m_minor_height_max;					//_minor最大分辨率高
	int m_minor_vb_cnt;						//_minor缓存块个数
	char m_venc_group_ddr_id;				//_venc的group内存id
	char m_venc_chn_ddr_id;					//_venc的chn内存id
	char m_vpss_group_ddr_id;				//_vpss的group内存id
	char m_vdec_chn_ddr_id;					//_vdec的chn内存id
	av_platform_cfg_t m_cfg;				//_av platform cfg
}av_platform_ctx_t, *pav_platform_ctx_t;

#ifdef 
}
#endif

#endif