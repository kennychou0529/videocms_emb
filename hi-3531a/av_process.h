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
	int m_sys_align_width;					//_ͼ�������
	int m_major_width_max;					//_major���ֱ��ʿ�
	int m_major_height_max;					//_major���ֱ��ʸ�
	int m_major_vb_cnt;						//_major��������
	int m_minor_width_max;					//_minor���ֱ��ʿ�
	int m_minor_height_max;					//_minor���ֱ��ʸ�
	int m_minor_vb_cnt;						//_minor��������
	char m_venc_group_ddr_id;				//_venc��group�ڴ�id
	char m_venc_chn_ddr_id;					//_venc��chn�ڴ�id
	char m_vpss_group_ddr_id;				//_vpss��group�ڴ�id
	char m_vdec_chn_ddr_id;					//_vdec��chn�ڴ�id
	av_platform_cfg_t m_cfg;				//_av platform cfg
}av_platform_ctx_t, *pav_platform_ctx_t;

#ifdef 
}
#endif

#endif