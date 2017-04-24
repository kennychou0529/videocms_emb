#include "../includes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int core_tools_write_json_to_file(cJSON *json, FILE *fp)
{
	if (!json || !fp)
	{
		return AV_ERR_INVALID_PARAM;
	}
	char *p_str;
	int filesize;

	p_str = cJSON_Print(json);
	if(p_str)
	{
		filesize = strlen(p_str) + 1;
		fwrite(p_str, filesize, 1,fp);
		free(p_str);
	}
	fsync(fileno(fp));

	return AV_OK;
}

int core_tools_get_platform_json(char *pathname, cJSON **out_json)
{
	FILE *fp;
	cJSON *json;
	char platform_json[32] = "";

	if (!out_json || !pathname)
	{
		return AV_ERR_INVALID_PARAM;
	}
	*out_json = NULL;

	fp = fopen(pathname, "r+");
	if (!fp)
	{
		return AV_ERR_OPEN_FILE_FAILED;
	}

	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	if (0 == filesize)
	{
		fclose(fp);
		return AV_ERR_FILE_EMPTY;
	}

	fseek(fp, 0, SEEK_SET);
	char * filebuf = (char *)malloc(filesize);
	if (NULL == filebuf)
	{
		fclose(fp);
		return AV_ERR_NOMEMORY;
	}

	fread(filebuf, filesize, 1, fp);
	*out_json = cJSON_Parse(filebuf);
	if (NULL == *out_json)
	{
		free(filebuf);
		fclose(fp);
		return AV_ERR_PARSE_JSON_FAILED;
	}
	free(filebuf);
	fclose(fp);
	return AV_OK;
}

int core_manager_put_platform_json(char *pathname, cJSON *in_json)
{
	char platform_json[32] = "";
	FILE *fp;

	if (!in_json || !pathname)
	{
		return AV_ERR_INVALID_PARAM;
	}
	fp = fopen(pathname, "w+");
	if (!fp)
	{
		return AV_ERR_OPEN_FILE_FAILED;
	}
	core_tools_write_json_to_file(in_json, fp);
	fclose(fp);

	return AV_OK;
}