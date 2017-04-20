#include "../includes.h"
#include <string.h>

int av_handler_set_com_rect_by_mode(compound_cfg_t *p_set_chn, divison_mode_t division_mode) //画中画模式
{
	int i = 0;
	p_set_chn->m_division_mode = division_mode;
	switch(division_mode)
	{
	case DIVISON_MODE_0:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 1;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		break;
	case DIVISON_MODE_1:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 2;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height*2/3;
		break;
	case DIVISON_MODE_2:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 2;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height*2/3;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height*2/3;
		break;
	case DIVISON_MODE_3:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 2;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = p_set_chn->m_height/4;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[1].m_x = p_set_chn->m_width/2;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/4;
		break;
	case DIVISON_MODE_4:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 3;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height;
		p_set_chn->m_rect[0].m_x = p_set_chn->m_width/3;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = 0;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/2-p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_x = 0;
		p_set_chn->m_rect[2].m_y = p_set_chn->m_height/2;
		break;
	case DIVISON_MODE_5:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 3;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/2-p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[2].m_y = p_set_chn->m_height/2;
		break;
	case DIVISON_MODE_6:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 3;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[0].m_x = p_set_chn->m_width/4;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[1].m_x = 0;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/2;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[2].m_x = p_set_chn->m_width/2;
		p_set_chn->m_rect[2].m_y = p_set_chn->m_height/2;
		break;
	case DIVISON_MODE_7:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 4;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[1].m_x = 0;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height/2;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[2].m_x = p_set_chn->m_width/2;
		p_set_chn->m_rect[2].m_y = 0;
		p_set_chn->m_rect[3].m_width = p_set_chn->m_width/2;
		p_set_chn->m_rect[3].m_height = p_set_chn->m_height/2;
		p_set_chn->m_rect[3].m_x = p_set_chn->m_width/2;
		p_set_chn->m_rect[3].m_y = p_set_chn->m_height/2;
		break;
	case DIVISON_MODE_8:
		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		p_set_chn->m_count = 6;
		p_set_chn->m_rect[0].m_width = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[0].m_height = p_set_chn->m_height*2/3;
		p_set_chn->m_rect[0].m_x = 0;
		p_set_chn->m_rect[0].m_y = 0;
		p_set_chn->m_rect[1].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[1].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[1].m_x = 0;
		p_set_chn->m_rect[1].m_y = p_set_chn->m_height*2/3;
		p_set_chn->m_rect[2].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[2].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[2].m_x = p_set_chn->m_width/3;
		p_set_chn->m_rect[2].m_y = p_set_chn->m_height*2/3;
		p_set_chn->m_rect[3].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[3].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[3].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[3].m_y = 0;
		p_set_chn->m_rect[4].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[4].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[4].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[4].m_y = p_set_chn->m_height/3;
		p_set_chn->m_rect[5].m_width = p_set_chn->m_width/3;
		p_set_chn->m_rect[5].m_height = p_set_chn->m_height/3;
		p_set_chn->m_rect[5].m_x = p_set_chn->m_width*2/3;
		p_set_chn->m_rect[5].m_y = p_set_chn->m_height*2/3;
		break;
	case DIVISON_MODE_USER1:
		// 		p_set_chn->m_show_mode = SHOW_MODE_SCALE;
		// 		p_set_chn->m_count = 1;
		// 		//for(i = 0;i < p_set_chn->m_count;i++)
		// 		{
		// 			p_set_chn->m_rect[i].m_width = 1920;
		// 			p_set_chn->m_rect[i].m_height = 1080;
		// 			p_set_chn->m_rect[i].m_x = 0;
		// 			p_set_chn->m_rect[i].m_y = 0;
		// 		}
		break;
	default:
		break;
	}
	return AV_OK;
}

//===========_参数说明===========
//compound_chn:_合成通道ID
//index:_合成通道中子通道的索引号
//chn:_视频源通道号
//===============================
int av_handler_set_com_chn(compound_chn_t compound_chn, int index, int chn)
{
	if (compound_chn < 0 || compound_chn > COMPOUND_CHN_VP || index >= COMPOUND_SUB_CHN_MAX \
		|| chn < 0 || chn > g_av_platform_ctx.m_vichn_cnt + g_av_platform_ctx.m_filechn_cnt \
		+ g_av_platform_ctx.m_remotechn_cnt + VI_CHN_START)
	{
		return AV_ERR_INVALID_PARAM;
	}
	compound_cfg_t *p_compound_cfg = &g_av_platform_ctx.m_cfg.m_compound_cfg[compound_chn];
	compound_cfg_t tmp_compound_cfg;
	
	memcpy(&tmp_compound_cfg, p_compound_cfg, sizeof(tmp_compound_cfg));
	tmp_compound_cfg.m_chn[index] = chn;
	av_stop_compound_effect(compound_chn);
	//_设备资源有限，所以只有DIVISON_MODE_4以下的模式才支持特效
	if (p_compound_cfg->m_division_mode < DIVISON_MODE_4 && g_tde_ctx.m_effect_data->m_mode > EFFECT_MODE_NONE)
	{
		av_start_compound_effect(compound_chn);
	}
	else
	{
		av_set_compound_vo_chn(compound_chn, &tmp_compound_cfg);
	}
	av_save_cfg();

	return AV_OK;
}