#include <google/vcdecoder.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
using namespace std;

int LoadDelta(char *path, std::vector<std::vector<char>> &inputSet)
{
    struct stat statbuf;
    char szDir[256] = {0};
    strcpy(szDir, path);
    DIR *pDir = opendir(szDir);
    struct dirent *entry;
    char szFile[256];
    while ((entry = readdir(pDir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        sprintf(szFile, "%s/%s", szDir, entry->d_name);
        unsigned long dwRead = 0;
        stat(szFile, &statbuf);
        FILE *hFile = fopen(szFile, "rb");
        std::vector<char> input;
        input.resize(statbuf.st_size);
        fread(&input[0], input.size(), 1, hFile);
        fclose(hFile);
        inputSet.push_back(input);
    }
    closedir(pDir);
    return 0;
}

int main(int argc, char **argv)
{
    string dictionary = argv[2];
    std::vector<char> dictionary_;
    struct stat statbuf;
    FILE *f = fopen(dictionary.data(), "rb");
    stat(dictionary.c_str(), &statbuf);
    dictionary_.resize(statbuf.st_size);
    fread(&dictionary_[0], 1, statbuf.st_size, f);

    // delta
    string Dir = argv[1];
    std::vector<std::vector<char>> inputSet;
    LoadDelta(Dir.data(), inputSet);
    open_vcdiff::VCDiffStreamingDecoder decoder;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    for (int i = 0; i < inputSet.size(); i++)
    {
        string output;
        std::vector<char> input_buffer_ = inputSet[i];
        decoder.StartDecoding(&dictionary_[0], dictionary_.size());
        bool flag = decoder.DecodeChunk(&input_buffer_[0], input_buffer_.size(), &output);
        if (!flag)
        {
            cout << "error!" << endl;
            return -1;
        }
        decoder.FinishDecoding();
        // string file = "a";
        // file += to_string(i);
        // file += ".txt";
        // FILE *output_file_ = fopen(file.data(), "w");
        // fwrite(output.data(), 1, output.size(), output_file_);
    }

    gettimeofday(&tv, NULL);
    long end = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    printf("time = %d\n", (end - start));
}