#ifndef __AV_HANDLER_H__
#define __AV_HANDLER_H__

#include "av_process.h"
#include "../core/core_render.h"

#ifdef __cplusplus
extern "C"{
#endif

int av_handler_set_com_rect_by_mode(compound_cfg_t *p_set_chn, divison_mode_t division_mode); //»­ÖÐ»­Ä£Ê½
int render_display_callback(RENDER_DIS_CFG_S render_dis_cfg);

#ifdef __cplusplus
}
#endif

#endif