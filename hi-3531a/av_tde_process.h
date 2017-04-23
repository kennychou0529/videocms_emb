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
	effect_mode_t m_mode;							//_��Чģʽ
	int m_duration;									//_����ʱ����
	int m_start_delay_cnt;							//_������Чʱ����ʱһ������ȴ���Ƶ׼���ã������������Ƶ�Ļ������
	int m_end_delay_cnt;							//_�ر���Чʱ����ʱһ����������µ�ͼ��û��׼���ã�Դ�ͱ��ر���
	int m_width_max;								//_ͼ�������
	int m_height_max;								//_ͼ�����߶�
	pixel_fmt_type_t m_pix_fmt;						//_ͼ�����ظ�ʽ
}tde_effect_cfg_t, *ptde_effect_cfg_t;

typedef struct _tde_effect_data_s
{
	int m_vo_id;									//_��Чͨ��id
	effect_mode_t m_mode;							//_��Чģʽ
	int m_duration;									//_���õĳ���ʱ����
	int m_expired;									//_��Ч�Ѿ�������ʱ��
	int m_start_delay_cnt;							//_������Чʱ����ʱһ������ȴ���Ƶ׼���ã������������Ƶ�Ļ������
	int m_end_delay_cnt;							//_�ر���Чʱ����ʱһ����������µ�ͼ��û��׼���ã�Դ�ͱ��ر���
	int m_width_max;								//_ͼ�������
	int m_height_max;								//_ͼ�����߶�
	pixel_fmt_type_t m_pix_fmt;						//_ͼ�����ظ�ʽ
	VB_POOL m_pool_handle;							//_����ؾ��
	pthread_t m_thread;								//_tde�����߳�
	pthread_mutex_t m_mutex;						//_������
}tde_effect_data_t, *ptde_effect_data_t;

typedef struct _tde_cfg_s
{
	int m_cur_effect_chn;							//_��ǰ��Чͨ��
	tde_effect_cfg_t m_effect_cfg[VIR_VO_DEV_MAX];	//_��Ч����
}tde_cfg_t, *ptde_cfg_t;

typedef struct _tde_ctx_s
{
	tde_exit_cmd_e m_exit_cmd;						//_tde�߳��˳�����
	tde_effect_data_t m_effect_data[VIR_VO_DEV_MAX];//_tde��Ч����
	tde_cfg_t m_tde_cfg;							//_tde����
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