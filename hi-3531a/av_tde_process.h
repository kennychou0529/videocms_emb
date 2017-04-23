#ifndef __AV_TDE_PROCESS_H__
#define __AV_TDE_PROCESS_H__

#include "../core/core_global_def.h"
#include "../thirtyparty/pthread_win32/pthread.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _tde_exit_cmd_e
{
	TDE_EXIT_CMD_START = 0,
	TDE_EXIT_CMD_STOP,
}tde_exit_cmd_e;

typedef struct _tde_effect_cfg_s
{
	effect_mode_t m_mode;							//_特效模式
	int m_duration;									//_持续时长秒
	int m_start_delay_cnt;							//_启动特效时先延时一会儿，等待视频准备好，避免出现无视频的画面参与
	int m_end_delay_cnt;							//_关闭特效时，延时一会儿，避免新的图像还没有准备好，源就被关闭了
	int m_width_max;								//_图像最大宽度
	int m_height_max;								//_图像最大高度
	pixel_fmt_type_t m_pix_fmt;						//_图像像素格式
}tde_effect_cfg_t, *ptde_effect_cfg_t;

typedef struct _tde_effect_data_s
{
	int m_vo_id;									//_特效通道id
	effect_mode_t m_mode;							//_特效模式
	int m_duration;									//_设置的持续时长秒
	int m_expired;									//_特效已经持续的时间
	int m_start_delay_cnt;							//_启动特效时先延时一会儿，等待视频准备好，避免出现无视频的画面参与
	int m_end_delay_cnt;							//_关闭特效时，延时一会儿，避免新的图像还没有准备好，源就被关闭了
	int m_width_max;								//_图像最大宽度
	int m_height_max;								//_图像最大高度
	pixel_fmt_type_t m_pix_fmt;						//_图像像素格式
	VB_POOL m_pool_handle;							//_缓存池句柄
	pthread_t m_thread;								//_tde处理线程
	pthread_mutex_t m_mutex;						//_保护锁
}tde_effect_data_t, *ptde_effect_data_t;

typedef struct _tde_cfg_s
{
	int m_cur_effect_chn;							//_当前特效通道
	tde_effect_cfg_t m_effect_cfg[VIR_VO_DEV_MAX];	//_特效配置
}tde_cfg_t, *ptde_cfg_t;

typedef struct _tde_ctx_s
{
	tde_exit_cmd_e m_exit_cmd;						//_tde线程退出命令
	tde_effect_data_t m_effect_data[VIR_VO_DEV_MAX];//_tde特效数据
	tde_cfg_t m_tde_cfg;							//_tde配置
}tde_ctx_t, *ptde_ctx_t;

int av_tde_startup();
int av_tde_shutdown();
int av_start_compound_effect(compound_chn_t compound_chn);
int av_stop_compound_effect(compound_chn_t compound_chn);

extern tde_ctx_t g_tde_ctx;

#ifdef __cplusplus
}
#endif


#endif