#ifndef __CORE_PLATFORM_H__
#define __CORE_PLATFORM_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _sys_info_s 
{
	unsigned long long m_lldevice_major_id;				//<!--设备主id
	unsigned long long m_lldevice_minor_id;				//<!--设备子id
	int m_idevice_type_id;								//<!--设备类型ID：通畅通过除以某个固定的值确定设备类型，这样就可以将不同设备设于不同区域的ID
	int m_idevice_hardware_id;							//<!--设备硬件ID：同款设备类型，可能存在多个机型
	char m_software_version[64];						//<!--软件版本
}sys_info_t, *psys_info_t;

#ifdef __cplusplus
}
#endif

#endif