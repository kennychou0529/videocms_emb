#ifndef __CORE_CHANNEL_H__
#define __CORE_CHANNEL_H__

#ifdef __cplusplus
extern "C"
{
#endif

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

typedef enum _vpss_chn_type_e
{
	VPSS_CHN_TYPE_MAJOR = 0,
	VPSS_CHN_TYPE_MINOR = 1,
	VPSS_CHN_TYPE_MINOR2 = 2,
	VPSS_CHN_TYPE_RENDER = 3,
}vpss_chn_type_t;

typedef enum _vi_chn_set_e
{
	VI_CHN_SET_NORMAL = 0, /* mirror, filp close */ 
	VI_CHN_SET_MIRROR = 1,      /* open MIRROR */   
	VI_CHN_SET_FILP  = 2      /* open filp */
}vi_chn_set_e;

typedef enum _vpss_chn_mode_e
{
	VPSS_CHN_MODE_AUTO = 0,
	VPSS_CHN_MODE_USER = 1,
}vpss_chn_mode_t;

typedef struct _CHN_RECT
{
	int m_x;
	int m_y;
	int m_width;
	int m_height;
}CHN_RECT;

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

typedef struct _vi_cfg_s
{
	unsigned int m_mask0;
	unsigned int m_mask1;
	int m_width;
	int m_height;
	int m_fps;
	char m_interface;
	int m_videv;
	int m_vichn;
	int m_colorspace;
	vi_chn_set_e m_mirrorflip;
	char m_interlaced;
	int m_src_fps;
	int m_dst_fps;
}vi_cfg_t, *pvi_cfg_t;

typedef struct _vpss_cfg_s
{
	unsigned int m_group_number;			//_vpss��group��
	unsigned int m_max_width;				//_vpss���ͼ����
	unsigned int m_max_height;				//_vpss���ͼ��߶�
	pixel_fmt_type_t m_pixel_fmt_type;		//_����豸YUV��ʽ����
	int m_vpss_bind_cnt[4];					//_��������vpssͨ���󶨴�����0:major 1:minor 2:minor2 3:render
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

typedef struct _channel_cfg_s
{
	int m_raw_fps;
	int m_raw_width;
	int m_raw_height;

	int m_major_venc_fps;
	int m_major_venc_width;
	int m_major_venc_height;

	int m_minor_venc_fps;
	int m_minor_venc_width;
	int m_minor_venc_height;

	int m_minor2_venc_fps;
	int m_minor2_venc_width;
	int m_minor2_venc_height;
	int m_pix_format;
	char vi_json_id;						//_vi������json������е�Ԫ��id
	vi_cfg_t m_vi_cfg;
	vpss_cfg_t m_vpss_cfg;
	vo_chn_cfg_t m_vo_cfg;
}channel_cfg_t, *pchannel_cfg_t;

typedef struct _channel_data_s
{
	channel_type_e m_channel_type;			//_ͨ������
	channel_status_e m_channel_stat;		//_ͨ��״̬
	char m_has_audio;						//_�Ƿ�����Ƶ
	char m_has_vodeo;						//_�Ƿ�����Ƶ
	//_����video��Ϣ
	int m_source_fps;
	int m_source_width;
	int m_source_height;

	int m_raw_fps;
	int m_raw_width;
	int m_raw_height;

	int m_major_venc_fps;
	int m_major_venc_width;
	int m_major_venc_height;

	int m_minor_venc_fps;
	int m_minor_venc_width;
	int m_minor_venc_height;

	int m_minor2_venc_fps;
	int m_minor2_venc_width;
	int m_minor2_venc_height;

	int m_major_vpss_chn;
	int m_minor_vpss_chn;
	int m_minor2_vpss_chn;
	int m_render_vpss_chn;
	channel_cfg_t m_cfg;
}channel_data_t, *pchannel_data_t;

int core_channel_local_setup();
int core_channel_local_desetup();
int core_channel_uninit_cfg();
int core_channel_init_cfg();
int core_channel_load_cfg();
int core_channel_save_cfg()

#ifdef __cplusplus
}
#endif

#endif