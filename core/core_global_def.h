#ifndef __CORE_GLOBAL_DEF_H__
#define __CORE_GLOBAL_DEF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define RETURN_OK 0
#define RETURN_ERR_FALSE -1
#define RETURN_ERR_INVALID_PARAM -2
#define RETURN_ERR_NO_MEMORY -3
#define RETURN_ERR_OPEN_FILE_FAILED -4
#define RETURN_ERR_FILE_EMPTY -5

#ifdef __EN_DBG_
#define DBG_PRT printf 
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


#ifdef __cplusplus
}
#endif

#endif 