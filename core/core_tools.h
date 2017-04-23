#ifndef __CORE_TOOLS_H__
#define __CORE_TOOLS_H__

#include "../thirtyparty/cjson/cJSON.h"
#ifdef __cplusplus
extern "C"
{
#endif

int core_tools_get_platform_json(char *pathname, cJSON **out_json);
int core_manager_put_platform_json(char *pathname, cJSON *in_json);


#ifdef __cplusplus
}
#endif

#endif