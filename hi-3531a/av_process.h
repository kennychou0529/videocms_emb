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
#define AV_ERR_WRITE_FILE_FAILD		-8

#ifdef __cplusplus
extern "C"
{
#endif

#include "../thirtyparty/pthread_win32/pthread.h"

#define	OUT_DEV_TYPE_VGA  1 << 0
#define	OUT_DEV_TYPE_HDMI  1 << 1
#define	OUT_DEV_TYPE_BT1120  1 << 2

typedef enum _resulotion_type_e
{
	RESULOTION_TYPE_1920X1080 = 0,
	RESULOTION_TYPE_1440X900 = 1,
	RESULOTION_TYPE_1280X1024 = 2,
	RESULOTION_TYPE_1280X720 = 3,
	RESULOTION_TYPE_1280X960 = 4,
	RESULOTION_TYPE_3840X2160 = 5,
}resulotion_type_t;

typedef enum _frame_rate_type_e
{
	FRAME_RATE_TYPE_60 = 0,
	FRAME_RATE_TYPE_50 = 1,
	FRAME_RATE_TYPE_30 = 2,
	FRAME_RATE_TYPE_25 = 3,
}frame_rate_type_t;

typedef enum _pixel_fmt_type_e
{
	PIXEL_FMT_TYPE_YUV422 = 0,
	PIXEL_FMT_TYPE_YUV420 = 1,
}pixel_fmt_type_t;

typedef enum _vpss_chn_type_e
{
	VPSS_CHN_TYPE_MAJOR = 0,
	VPSS_CHN_TYPE_MINOR = 1,
	VPSS_CHN_TYPE_MINOR2 = 2,
	VPSS_CHN_TYPE_RENDER = 3,
}vpss_chn_type_t;

typedef struct _vo_dev_cfg_s
{
	int m_dev_id;							//_�豸ID��0-DH0  1-DH1  2-SD0
	unsigned int m_out_dev_type;			//_����豸����
	resulotion_type_t m_resulotion_type;	//_����ֱ�������
	frame_rate_type_t m_frame_rate_type;	//_����豸֡������
	pixel_fmt_type_t m_pixel_fmt_type;		//_����豸YUV��ʽ����
	unsigned int m_bg_color;				//_���汳����ɫ
}av_vo_dev_cfg_t, *pav_vo_dev_cfg_t;

typedef struct _vo_chn_cfg_s
{
	int m_chn_id;							//_chn id
	char m_layer_id;						//_��ʾ���
	int m_x;								//_��ʼ��x����
	int m_y;								//_��ʼ��y����
	int m_width;							//_�����
	int m_height;							//_�����
	char m_deflicker;						//_����˸ʹ��
}vo_chn_cfg_t, *pvo_chn_cfg_t;

typedef struct _vpss_cfg_s
{
	unsigned int m_group_number;			//_vpss��group��
	unsigned int m_max_width;				//_vpss���ͼ����
	unsigned int m_max_height;				//_vpss���ͼ��߶�
	pixel_fmt_type_t m_pixel_fmt_type;		//_����豸YUV��ʽ����
	char m_en_die_mode;						//_����dieģʽ
	char m_en_ie;							//_����ie����
	char m_en_dci;							//_����dci����
	char m_en_nr;							//_����NR
	char m_en_hist;							//_����hist
	char m_en_es;							//_����es
	char m_contrast;						//_contrast
	char m_dieStrength;						//_dieStrength
	char m_ieStrength;						//_ieStrength
	char m_sfStrength;						//_sfStrength
	char m_cfStrength;						//_CfStrength
	char m_cTfStrength;						//_CTfStrength
	char m_cvbsStrength;					//_cvbsStrength
	char m_deMotionBlurring;				//_DeMotionBlurring
}vpss_cfg_t, *pvpss_cfg_t;

typedef struct __av_platform_cfg_s
{
	av_vo_dev_cfg_t m_vo_cfg_ui;			//_UI����豸����
	//vo_chn_cfg_t m_vo_chn_cfg_ui;			//_UI���VOͨ��
	vpss_cfg_t m_vpss_cfg_ui;				//_UI����豸VPSS����
	av_vo_dev_cfg_t m_vo_cfg_live;			//_LIVE����豸����
	//vo_chn_cfg_t m_vo_chn_cfg_live;			//_LIVE���VOͨ��
	vpss_cfg_t m_vpss_cfg_live;				//_LIVE����豸VPSS����
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
	char m_app_start_ok;					//_ϵͳ�Ѿ��������
	pthread_mutex_t m_mtx;					//_������
	av_platform_cfg_t m_cfg;				//_av platform cfg

}av_platform_ctx_t, *pav_platform_ctx_t;

int av_startup();
int  av_shutdown();

#ifdef __cplusplus
}
#endif

#endif