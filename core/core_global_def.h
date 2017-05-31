#ifndef __CORE_GLOBAL_DEF_H__
#define __CORE_GLOBAL_DEF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define TRUE 1
#define FALSE 0

#define UPDATE_FILE	"/mnt/mtd/update.ini"
#define PLATFORM_FILE_PATH "/mnt/mtd/platform"
#define CFG_FILE	"/mnt/mtd/app/config"

#define VI_CHN_INFO "PhyChannel"//"VI_CHN_INFO"
#define VPSS_INFO "VPSS_CHN_INFO"

#define RETURN_OK 0
#define RETURN_ERR_FALSE -1
#define RETURN_ERR_INVALID_PARAM -2
#define RETURN_ERR_NO_MEMORY -3
#define RETURN_ERR_OPEN_FILE_FAILED -4
#define RETURN_ERR_FILE_EMPTY -5


#define BLACK_PRT "\033[30m"
#define RED_PRT "\033[31m"
#define GREEN_PRT "\033[32m"
#define YELLOW_PRT "\033[33m"
#define BLUE_PRT "\033[34m"
#define END_PRT "\033[0m"

#define __EN_DBG_
#ifdef __EN_DBG_
//#define DBG_PRT printf 
#define DBG_PRT(format,...) printf(RED_PRT "%s:%s:%d--》" format END_PRT,  __FILE__, __func__, __LINE__, ##__VA_ARGS__) 
#else
#define DBG_PRT 
#endif

#define CHECK_CHN_RET(express,Chn,name)\
	do{\
	HI_S32 Ret;\
	Ret = express;\
	if (HI_SUCCESS != Ret)\
	{\
	printf("\033[0;31m%s chn %d failed at %s: LINE: %d with %#x!\033[0;39m\n", name, Chn, __FUNCTION__, __LINE__, Ret);\
	fflush(stdout);\
	return Ret;\
}\
	}while(0)

#define CHECK_RET(express,name)\
	do{\
	HI_S32 Ret;\
	Ret = express;\
	if (HI_SUCCESS != Ret)\
	{\
	printf("\033[0;31m%s failed at %s: LINE: %d with %#x!\033[0;39m\n", name, __FUNCTION__, __LINE__, Ret);\
	return Ret;\
}\
	}while(0)

#define ALIGN_TWO(num) ((num)&(~0x1))

	//#define SAMPLE_GLOBAL_NORM	    VIDEO_ENCODING_MODE_PAL
#define SAMPLE_PIXEL_FORMAT         PIXEL_FORMAT_YUV_SEMIPLANAR_422

#define TW2865_FILE "/dev/tw2865dev"
#define TP2823_FILE "/dev/tp2823dev"
#define TLV320_FILE "/dev/tlv320aic31"

#define SAMPLE_VO_DEV_DHD0 0
#define SAMPLE_VO_DEV_DHD1 1
#define SAMPLE_VO_DEV_DSD0 2
#define SAMPLE_VO_DEV_VIRT0 3
#define SAMPLE_VO_DEV_VIRT1 4
#define SAMPLE_VO_DEV_VIRT2 5
#define SAMPLE_VO_DEV_VIRT3 6
#define SAMPLE_VO_DEV_DSD1 -1

typedef enum _vo_dev_id_e
{
	VO_DEV_DHD0 = 0,
	VO_DEV_DHD1 = 1,
	VO_DEV_DSD0 = 2,
	VO_DEV_VIRT0 = 3,
	VO_DEV_VIRT1 = 4,
	VO_DEV_VIRT2 = 5,
	VO_DEV_VIRT3 = 6,
}vo_dev_id_s;

#define SAMPLE_VO_LAYER_VHD0 0
#define SAMPLE_VO_LAYER_VHD1 1
#define SAMPLE_VO_LAYER_VPIP 2
#define SAMPLE_VO_LAYER_VSD0 3
#define SAMPLE_VO_LAYER_VIRT0 4
#define SAMPLE_VO_LAYER_VIRT1 5
#define SAMPLE_VO_LAYER_VIRT2 6
#define SAMPLE_VO_LAYER_VIRT3 7

#define GET_VO_LAYER(VO_LAYER, VO_DEV)	do {if(VO_DEV < 2) VO_LAYER = VO_DEV;else VO_LAYER = VO_DEV + 1;} while (0);



#define VO_LAYER_PIP      2
#define VO_LAYER_PIP_STA	2
#define VO_LAYER_PIP_END	2
#define VO_DEV_HD_END	2


#define SAMPLE_VO_WBC_BASE  0
#define SAMPLE_VO_LAYER_PRIORITY_BASE 0
#define SAMPLE_VO_LAYER_PRIORITY_PIP 1
	//#define GRAPHICS_LAYER_HC0 3

#define SAMPLE_AUDIO_PTNUMPERFRM   320
#define SAMPLE_AUDIO_TLV320_AI_DEV 2
#define SAMPLE_AUDIO_TLV320_AO_DEV 0


#define SAMPLE_AUDIO_INNER_HDMI_AO_DEV 2
#define SAMPLE_AUDIO_HDMI_AO_DEV 1
#define SAMPLE_AUDIO_AI_DEV 1
#define SAMPLE_AUDIO_AO_DEV 0

#define SAMPLE_CIF_H264_PATH "../common/CIF.h264"
#define SAMPLE_1080P_H264_PATH "../common/1080P.h264"
#define SAMPLE_1080P_H265_PATH "../common/1080P.h265"
#define SAMPLE_4K_H264_PATH "../common/tmp1"
#define SAMPLE_4K_H265_PATH "../common/tmp2"
#define SAMPLE_1080P_MPEG4_PATH "../common/1080P.mpeg4"
#define SAMPLE_FIELD_H264_PATH "../common/D1_field.h264"
#define SAMPLE_1080P_JPEG_PATH "../common/1080P.jpg"
#define SAMPLE_4K_JPEG_PATH "../common/tmp3"

#define SAMPLE_MAX_VDEC_CHN_CNT 8


#define ALIGN_UP(x, a)              ((x+a-1)&(~(a-1)))
#define ALIGN_BACK(x, a)              ((a) * (((x) / (a))))

#define SAMPLE_SYS_ALIGN_WIDTH  16		
#define VO_BKGRD_BLUE           0x0000FF

#define HD_WIDTH                1920
#define HD_HEIGHT               1080
#define _4K_WIDTH                3840
#define _4K_HEIGHT               2160

#define D1_WIDTH                720
#define D1_HEIGHT               576

#define _720P_WIDTH              1280
#define _720P_HEIGHT             720

#define DEFAULT_MAJOR_FPS		 25
#define DEFAULT_MINOR_FPS		 25

#define VO_FPS_ACCEL (2)

#define VIR_VO_DEV_MAX			4
#define VI_CHN_START	3
#define COMPOUND_SUB_CHN_MAX     6

typedef enum _compound_chn_e
{
	COMPOUND_CHN_MOVIE = 0,					//_电影画面
	COMPOUND_CHN_PVW = 1,					//_预览画面
	COMPOUND_CHN_VP	= 2,					//_视频会议
	COMPOUND_CHN_EFF = 3,					//_视频会议
}compound_chn_t;

typedef enum _show_mode_e
{
	SHOW_MODE_SCALE	= 0,
	SHOW_MODE_STRETCH = 1,
	SHOW_MODE_CROP	= 2,
}show_mode_t;

typedef enum _divison_mode_e
{
	DIVISON_MODE_0	= 0,
	DIVISON_MODE_1	= 1,
	DIVISON_MODE_2	= 2,
	DIVISON_MODE_3	= 3,
	DIVISON_MODE_4	= 4,
	DIVISON_MODE_5	= 5,
	DIVISON_MODE_6	= 6,
	DIVISON_MODE_7	= 7,
	DIVISON_MODE_8	= 8,
	DIVISON_MODE_USER1	= 9
}divison_mode_t;

typedef enum _effect_mode_e
{
	EFFECT_MODE_NONE = 0,
	
	EFFECT_MODE_WIPE_LEFT = 1,
	EFFECT_MODE_WIPE_RIGHT = 2,
	EFFECT_MODE_WIPE_UP = 3,
	EFFECT_MODE_WIPE_DOWN = 4,
	EFFECT_MODE_WIPE_LEFTUP = 1,
	EFFECT_MODE_WIPE_RIGHTUP = 2,
	EFFECT_MODE_WIPE_LEFTDOWN = 3,
	EFFECT_MODE_WIPE_RIHGHTDOWN = 4,

	EFFECT_MODE_COVER_LEFT = 5,
	EFFECT_MODE_COVER_RIGHT = 6,
	EFFECT_MODE_COVER_UP = 7,
	EFFECT_MODE_COVER_DOWN = 8,
	EFFECT_MODE_COVER_LEFTUP = 9,
	EFFECT_MODE_COVER_RIGHTUP = 10,
	EFFECT_MODE_COVER_LEFTDOWN = 11,
	EFFECT_MODE_COVER_RIHGHTDOWN = 12,

	EFFECT_MODE_PUSH_LEFT = 13,
	EFFECT_MODE_PUSH_RIGHT = 14,
	EFFECT_MODE_PUSH_UP = 15,
	EFFECT_MODE_PUSH_DOWN = 16,
	EFFECT_MODE_PUSH_LEFTUP = 17,
	EFFECT_MODE_PUSH_RIGHTUP = 18,
	EFFECT_MODE_PUSH_LEFTDOWN = 19,
	EFFECT_MODE_PUSH_RIHGHTDOWN = 20,

	EFFECT_MODE_HCENTER = 21,
	EFFECT_MODE_VCENTER = 22,
	EFFECT_MODE_MIDDLE	= 23,
}effect_mode_t;

typedef enum _pixel_fmt_type_e
{
	PIXEL_FMT_TYPE_YUV422 = 0,
	PIXEL_FMT_TYPE_YUV420 = 1,
}pixel_fmt_type_t;

typedef enum _vpss_chn_type_e
{
	VPSS_CHN_TYPE_MAJOR = 0,
	VPSS_CHN_TYPE_MINOR,
	VPSS_CHN_TYPE_MINOR2,
	VPSS_CHN_TYPE_RENDER,
}vpss_chn_type_e;

typedef enum _channel_type_e
{
	CHANNEL_TYPE_LOCAL_CHANNEL = 0,
	CHANNEL_TYPE_FILE_CHANNEL,
	CHANNEL_TYPE_REMOTE_CHANNEL,
	CHANNEL_TYPE_NULL,
}channel_type_e;

typedef enum _channel_status_e
{
	CHANNEL_STATUS_STOP = 0,
	CHANNEL_STATUS_START,
}channel_status_e;

#ifdef __cplusplus
}
#endif

#endif 