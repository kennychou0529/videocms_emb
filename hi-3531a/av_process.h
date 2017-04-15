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


#define BT656    0
#define BT1120I  1
#define BT1120S  2

#define YUV420   0
#define YUV422   1
#define YUV444   2


#ifdef __cplusplus
extern "C"
{
#endif

#include "../thirtyparty/pthread_win32/pthread.h"
#include "../core/core_channel.h"

typedef struct _vo_dev_cfg_s
{
	int m_dev_id;							//_设备ID：0-DH0  1-DH1  2-SD0
	unsigned int m_out_dev_type;			//_输出设备类型
	resulotion_type_t m_resulotion_type;	//_输出分辨率类型
	frame_rate_type_t m_frame_rate_type;	//_输出设备帧率类型
	pixel_fmt_type_t m_pixel_fmt_type;		//_输出设备YUV格式类型
	unsigned int m_bg_color;				//_画面背景颜色
}av_vo_dev_cfg_t, *pav_vo_dev_cfg_t;

typedef struct __av_platform_cfg_s
{
	av_vo_dev_cfg_t m_vo_cfg_ui;			//_UI输出设备配置
	//vo_chn_cfg_t m_vo_chn_cfg_ui;			//_UI输出VO通道
	vpss_cfg_t m_vpss_cfg_ui;				//_UI输出设备VPSS参数
	av_vo_dev_cfg_t m_vo_cfg_live;			//_LIVE输出设备配置
	//vo_chn_cfg_t m_vo_chn_cfg_live;			//_LIVE输出VO通道
	vpss_cfg_t m_vpss_cfg_live;				//_LIVE输出设备VPSS参数
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
	char m_app_start_ok;					//_系统已经启动完成
	int m_vichn_cnt;						//_vi通道数
	int m_filechn_cnt;						//_file通道数
	int m_remotechn_cnt;					//_remote通道数
	pthread_mutex_t m_mtx;					//_互斥锁
	av_platform_cfg_t m_cfg;				//_av platform cfg
	channel_data_t *m_all_channel_ptr[256];		//_所有通道头指针
	channel_data_t *m_local_channel_ptr;	//_本地通道头指针
	int m_local_chn_index_offset;			//_本地通道在所有通道头指针的偏移位置
	channel_data_t *m_file_channel_ptr;		//_文件通道头指针
	int m_file_chn_index_offset;			//_文件通道在所有通道头指针的偏移位置
	channel_data_t *m_remote_channel_ptr;	//_远程通道头指针
	int m_remote_chn_index_offset;			//_远程通道在所有通道头指针的偏移位置
}av_platform_ctx_t, *pav_platform_ctx_t;

int av_startup();
int  av_shutdown();

extern av_platform_ctx_t g_av_platform_ctx;

#ifdef __cplusplus
}
#endif

#endif