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

#define SPECIAL_LIVE_CHN  -1
#define SPECIAL_VIR0_CHN  -2
#define SPECIAL_VIR1_CHN  -3
#define SPECIAL_VIR2_CHN  -4
#define SPECIAL_VIR3_CHN  -5


#ifdef __cplusplus
extern "C"
{
#endif

#include "../thirtyparty/pthread_win32/pthread.h"
#include "../core/core_channel.h"
#include "../core/core_global_def.h"

typedef struct _vo_dev_cfg_s
{
	vo_dev_id_s m_dev_id;					//_�豸ID��0-DH0  1-DH1  2-SD0 3-VirVo0 4-VirVo1  5-VirVo2  6-VirVo3
	unsigned int m_out_dev_type;			//_����豸����
	resulotion_type_t m_resulotion_type;	//_����ֱ�������
	frame_rate_type_t m_frame_rate_type;	//_����豸֡������
	pixel_fmt_type_t m_pixel_fmt_type;		//_����豸YUV��ʽ����
	unsigned int m_bg_color;				//_���汳����ɫ
}av_vo_dev_cfg_t, *pav_vo_dev_cfg_t;

//_�ϳ�vo����
typedef struct _compound_cfg_s
{
	int m_width;							//_�ϳ��������
	int m_height;							//_�ϳ��������
	divison_mode_t m_division_mode;			//_����ģʽ
	show_mode_t m_show_mode;				//_��Чģʽ
	int m_count;							//_��������
	int m_chn[COMPOUND_SUB_CHN_MAX];		//_�����ӦԴͨ������������
	CHN_RECT m_rect[COMPOUND_SUB_CHN_MAX];	//_������������
	av_vo_dev_cfg_t m_vo_dev_cfg;			//_�����vo�豸
	vpss_cfg_t m_vpss_cfg;					//_��vo�豸�󶨵�vpss����
}compound_cfg_t, *pcompound_cfg_t;

typedef struct __av_platform_cfg_s
{
	av_vo_dev_cfg_t m_vo_cfg_ui;						//_UI����豸����
	vpss_cfg_t m_vpss_cfg_ui;							//_UI����豸VPSS����
	av_vo_dev_cfg_t m_vo_cfg_live;						//_LIVE����豸����
	vpss_cfg_t m_vpss_cfg_live;							//_LIVE����豸VPSS����
	compound_cfg_t m_compound_cfg[VIR_VO_DEV_MAX];		//_�ϳɻ������ò���
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
	int m_vichn_cnt;						//_viͨ����
	int m_filechn_cnt;						//_fileͨ����
	int m_remotechn_cnt;					//_remoteͨ����
	pthread_mutex_t m_mtx;					//_������
	av_platform_cfg_t m_cfg;				//_av platform cfg
	channel_data_t *m_all_channel_ptr[256];		//_����ͨ��ͷָ��
	channel_data_t *m_local_channel_ptr;	//_����ͨ��ͷָ��
	int m_local_chn_index_offset;			//_����ͨ��������ͨ��ͷָ���ƫ��λ��
	channel_data_t *m_file_channel_ptr;		//_�ļ�ͨ��ͷָ��
	int m_file_chn_index_offset;			//_�ļ�ͨ��������ͨ��ͷָ���ƫ��λ��
	channel_data_t *m_remote_channel_ptr;	//_Զ��ͨ��ͷָ��
	int m_remote_chn_index_offset;			//_Զ��ͨ��������ͨ��ͷָ���ƫ��λ��
}av_platform_ctx_t, *pav_platform_ctx_t;

int av_startup();
int  av_shutdown();
int av_save_cfg();
int av_set_compound_vo_rect(compound_chn_t compound_chn, compound_cfg_t *pset_compound_cfg);
int av_set_compound_vo_chn(compound_chn_t compound_chn, compound_cfg_t *pset_compound_cfg);

extern av_platform_ctx_t g_av_platform_ctx;

#ifdef __cplusplus
}
#endif

#endif