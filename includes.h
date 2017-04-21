#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#ifdef __cplusplus
extern "C"
{
#endif

/////////////////////////////////////////////////////////////////////////////
#include <sys/sem.h>
#include "hi_common.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_venc.h"
#include "hi_comm_vpss.h"
#include "hi_comm_vdec.h"
#include "hi_comm_vda.h"
#include "hi_comm_region.h"
#include "hi_comm_adec.h"
#include "hi_comm_aenc.h"
#include "hi_comm_ai.h"
#include "hi_comm_ao.h"
#include "hi_comm_aio.h"
#include "hi_comm_hdmi.h"
#include "hi_defines.h"

#include "mpi_sys.h"
#include "mpi_vb.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"
#include "mpi_vpss.h"
#include "mpi_vdec.h"
#include "mpi_vda.h"
#include "mpi_region.h"
#include "mpi_adec.h"
#include "mpi_aenc.h"
#include "mpi_ai.h"
#include "mpi_ao.h"
#include "mpi_hdmi.h"

#include "tlv320aic31.h"
#include "tp2823.h"

#include "loadbmp.h"

#include "sample_user_def.h"
/////////////////////////////////////////////////////////////////////////////

#include "hi-3531a/av_handler.h"
#include "hi-3531a/av_process.h"
#include "hi-3531a/av_tde_process.h"
#include "core/core_manager.h"
#include "core/core_global_def.h"
#include "core/core_platform.h"
#include "core/core_tools.h"
#include "core/core_parse_file.h"
#include "thirtyparty/cjson/cJSON.h"
#include "thirtyparty/strfunc/strfunc.h"

#ifdef __cplusplus
}
#endif

#endif