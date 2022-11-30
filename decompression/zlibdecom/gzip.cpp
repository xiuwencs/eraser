#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <zlib.h>
#include <assert.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

#define SIZE_CHUNK (8 * 1024)
#define MAX_SRC_FILE_SIZE (10 * 1024 * 1024)
unsigned char *pSrcs[900];
long int FileSize[900];
unsigned char *pDsts[900];
int count;
struct timeval tv;
long t1, t2, t3, t4;
void freememory(int start, int end, unsigned char *p[])
{
    for (int i = start; i <= end; i++)
    {
        free(p[i]);
        p[i] = NULL;
    }
    return;
}

int getFileName(char *dirPath)
{
    DIR *dir = opendir(dirPath);
    if (dir == NULL)
    {
        printf("%s\n", strerror(errno));
        return -1;
    }
    chdir(dirPath);
    char t[] = "/";
    strcat(dirPath, t);
    struct dirent *ent;
    count = 0;
    while ((ent = readdir(dir)) != NULL)
    {
        char str[50];
        snprintf(str, sizeof(str), "%s", dirPath);
        // printf("%s  \n",name);
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue;
        }
        strcat(str, ent->d_name); // Stitch out the file name
        FILE *pfile = NULL;
        pfile = fopen(str, "rb");
        if (pfile == NULL)
        {
            // printf("open file fails\n");
            return -1;
        }
        // printf("19\n");
        fseek(pfile, 0, SEEK_END);
        int file_length = ftell(pfile);
        rewind(pfile);

        // request Memory space for data
        int buf_length = file_length * sizeof(unsigned char);
        // printf("26\n");
        unsigned char *fbuf = (unsigned char *)malloc(buf_length);
        memset(fbuf, 0, buf_length);

        // printf("30\n");
        // read data from file to memory
        int read_length = fread(fbuf, sizeof(unsigned char), file_length, pfile);
        // printf("33\n");
        if (read_length != file_length)
        {
            // printf("read file failed!\n");
            exit(0);
        }

        pSrcs[count] = fbuf;
        FileSize[count] = buf_length;
        long unsigned int dstLength = MAX_SRC_FILE_SIZE * sizeof(unsigned char);
        // printf("40\n");
        unsigned char *desbuf = (unsigned char *)malloc(dstLength);
        // printf("42\n");
        memset(desbuf, 0, dstLength);
        // printf("44\n");
        pDsts[count] = desbuf;

        count++;
    }
    closedir(dir);
    return 1;
}

int file_inflate(const char *fsrc)
{

    long unsigned int dstLength = MAX_SRC_FILE_SIZE * sizeof(unsigned char);
    long unsigned int tmpdstlength;
    int iRet = 0;

    gettimeofday(&tv, NULL);
    t3 = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    for (int i = 0; i < count; i++)
    {
        tmpdstlength = dstLength;
        iRet = 0;
        do
        {
            // printf("40\n");
            iRet = uncompress(pDsts[i], &tmpdstlength, pSrcs[i], FileSize[i]);
            // printf("41");
            if (0 == iRet)
            {
                // printf("%d -- ok\n",i);
                break;
            }
            else if (-5 == iRet)
            {
                printf("error -5\n");
                return -1;
            }
            else
            {
                printf("error\n");
                return -1;
            }
        } while (1);
    }
    gettimeofday(&tv, NULL);
    t4 = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    freememory(1, count, pSrcs);
    freememory(1, count, pDsts);
    return 1;
}
int main(int argc, char *argv[])
{
    int res = 0;

    if (argv[1])
    {
        gettimeofday(&tv, NULL);
        t1 = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        res = getFileName(argv[1]);
        t2 = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
    if (res == -1)
    {
        printf("98 error");
        return 0;
    }
    res = file_inflate(argv[1]);
    printf("t2 - t1 = %ld,t4 - t3 = %ld\n", t2 - t1, t4 - t3);
    return 0;
}
