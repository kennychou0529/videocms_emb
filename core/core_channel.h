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

typedef enum _vi_chn_set_e
{
	VI_CHN_SET_NORMAL = 0, /* mirror, filp close */ 
	VI_CHN_SET_MIRROR = 1,      /* open MIRROR */   
	VI_CHN_SET_FILP  = 2      /* open filp */
}vi_chn_set_t;

typedef struct _vo_chn_cfg_s
{
	int m_chn_id;							//_chn id
	char m_layer_id;						//_显示层次
	int m_x;								//_起始点x坐标
	int m_y;								//_起始点y坐标
	int m_width;							//_区域宽
	int m_height;							//_区域高
	char m_deflicker;						//_抗闪烁使能
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
	vi_chn_set_t m_mirrorflip;
	char m_interlaced;
	int m_src_fps;
	int m_dst_fps;
}vi_cfg_t, *pvi_cfg_t;

typedef struct _vpss_cfg_s
{
	unsigned int m_group_number;			//_vpss的group号
	unsigned int m_max_width;				//_vpss最大图像宽度
	unsigned int m_max_height;				//_vpss最大图像高度
	pixel_fmt_type_t m_pixel_fmt_type;		//_输出设备YUV格式类型
	char m_en_die_mode;						//_启用die模式
	char m_en_ie;							//_启用ie功能
	char m_en_dci;							//_启用dci功能
	char m_en_nr;							//_启用NR
	char m_en_hist;							//_启用hist
	char m_en_es;							//_启用es
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
	av_vo_dev_cfg_t m_vo_cfg_ui;			//_UI输出设备配置
	//vo_chn_cfg_t m_vo_chn_cfg_ui;			//_UI输出VO通道
	vpss_cfg_t m_vpss_cfg_ui;				//_UI输出设备VPSS参数
	av_vo_dev_cfg_t m_vo_cfg_live;			//_LIVE输出设备配置
	//vo_chn_cfg_t m_vo_chn_cfg_live;			//_LIVE输出VO通道
	vpss_cfg_t m_vpss_cfg_live;				//_LIVE输出设备VPSS参数
}av_platform_cfg_t;

typedef struct _channel_cfg_s
{
	vi_cfg_t m_vi_cfg;
	vpss_cfg_t m_vpss_cfg;
	vo_chn_cfg_t m_vo_cfg;
}channel_cfg_t, *pchannel_cfg_t;

typedef struct _channel_data_s
{
	channel_cfg_t m_cfg;
}channel_data_t, *pchannel_data_t;

#ifdef __cplusplus
}
#endif

#endif