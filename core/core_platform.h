#ifndef __CORE_PLATFORM_H__
#define __CORE_PLATFORM_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _sys_info_s 
{
	unsigned long long m_lldevice_major_id;				//<!--�豸��id
	unsigned long long m_lldevice_minor_id;				//<!--�豸��id
	int m_idevice_type_id;								//<!--�豸����ID��ͨ��ͨ������ĳ���̶���ֵȷ���豸���ͣ������Ϳ��Խ���ͬ�豸���ڲ�ͬ�����ID
	int m_idevice_hardware_id;							//<!--�豸Ӳ��ID��ͬ���豸���ͣ����ܴ��ڶ������
	char m_software_version[64];						//<!--����汾
}sys_info_t, *psys_info_t;

#ifdef __cplusplus
}
#endif

#endif