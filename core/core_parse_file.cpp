#include "../includes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int PareseFile_Read_ini_int(const char *FilePath, const char *Node ,const char *Name, int *Dest)
{
	if (!FilePath || !Node || !Name  || !Dest)
	{
		DBG_PRT("param is invalid\n");
		return AV_ERR_INVALID_PARAM;
	}
	
	FILE *fp = NULL;
	char str[256];

	fp = fopen(FilePath, "rb");
	if (fp == NULL)
	{
		return AV_ERR_OPEN_FILE_FAILED;
	}
	memset(str, 0, sizeof(str));
	while( fgets(str, sizeof(str), fp) != NULL)
	{
		if ( strncmp(str, Node, strlen(Node)) == 0 )
		{
			memset(str, 0, sizeof(str));
			while( fgets(str, sizeof(str), fp) != NULL )
			{
				if ( strncmp(str, Name, strlen(Name)) == 0)
				{
					char *p;
					p = str;
					//DBG_PRT("p %s", p);
					while(*p != '\0')
					{
						//DBG_PRT("p %s", p);
						if (*p == '=')
						{
							p++;
							*Dest = atoi(p);
							//DBG_PRT("p %s", p);
							fclose(fp);
							return AV_OK;
						}
						p++;
					}
				}
			}
		}
	}
	fclose(fp);
	return AV_FALSE;
}

int PareseFile_Read_ini_str(const char *FilePath, const char *Node ,const char *Name, char * Dst, int Dst_size)
{
	if (!FilePath || !Node || *Name || !Dst || !Dst_size)
	{
		DBG_PRT("param is invalid\n");
		return AV_ERR_INVALID_PARAM;
	}
	
	FILE *fp = NULL;
	char str[256];
	int rom_version;
	fp  = fopen(FilePath, "rb");
	if (fp == NULL)
	{
		return -1;
	}
	memset(str, 0, sizeof(str));
	while( fgets(str, sizeof(str), fp) != NULL)
	{
		if ( strncmp(str, Node, strlen(Node)) == 0 )
		{
			memset(str, 0, sizeof(str));
			while( fgets(str, sizeof(str), fp) != NULL )
			{
				if ( strncmp(str, Name, strlen(Name)) == 0)
				{
					char *p;
					p = str;
					while(*p != '\0')
					{
						if (*p == '=')
						{
							p++;
							snprintf(Dst, Dst_size, "%s", p);
							fclose(fp);
							return 1;
						}
						p++;
					}
				}
			}
		}
	}
	fclose(fp);
	return AV_OK;
}