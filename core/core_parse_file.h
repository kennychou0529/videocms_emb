#ifndef __CORE_PARSE_FILE_H__
#define __CORE_PARSE_FILE_H__

#ifdef __cplusplus
extern "C"
{
#endif

int PareseFile_Read_ini_int(const char *FilePath, const char *Node ,const char *Name, int *Dest);
int PareseFile_Read_ini_str(const char *FilePath, const char *Node ,const char *Name, char * Dst, int Dst_size);

#ifdef __cplusplus
}
#endif

#endif