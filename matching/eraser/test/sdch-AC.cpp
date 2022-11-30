#include "sdch.h"
int g_scan = 0;
int g_match = 0;
unsigned int compress = 480787746; // 153608528;;//26872098; //153,608,528  //478998647
unsigned int total = 228422347;    // 228422347;;//73385030; //228,422,347  //73385030 //1241229032
clock_t spend = 0;
int literal_num = 0;
int real_index = 0;

int run = 0;
int add = 0;
int dict = 0;
int refer = 0;
int pointer_num = 0;
int pointer_len = 0;

int main(int argc, char **argv)
{
    const char *input = argv[1];
    const char *dict = argv[2];
    const char *rule = argv[3];

    int set = stoi(argv[4]);
    int group = stoi(argv[5]);
    vector<m_buffer> mBuff;
    vector<PatternInfo> patList;
    int count = LoadText(input, mBuff);
    ACSM_STRUCT *acsm = acsmNew();
    int patternSize = LoadPattern(patList, (char *)rule);
    for (int i = 0; i < patternSize; i++)
    {
        acsmAddPattern(acsm, patList[i].pat, patList[i].len, 0, 0, 0, 0);
    }
    acsmCompile(acsm);

    int *stateTodepth = new int[acsm->acsmMaxStates];
    for (int i = 1; i < acsm->acsmMaxStates; i++)
    {
        stateTodepth[i] = -1;
    }
    StateToDepth(acsm->acsmStateTable, stateTodepth);

    int len = 0;
    uint8_t *DictionaryData = LoadDictionary(dict, len);
    AC_INFO* DictionaryState = new AC_INFO[len];

    GetDictionaryStateSDCH(DictionaryState, acsm, len, DictionaryData,stateTodepth);

    TextInfo **Txt = new TextInfo *[count];
    int *ProcessSizeArray = new int[count];
    Init(mBuff, Txt, ProcessSizeArray, count);
    g_scan = 0;
    g_match = 0;

    ACCHMatching(Txt,acsm,ProcessSizeArray,DictionaryState,count,stateTodepth);
    
    Performance();
    delete[] Txt;
    delete[] ProcessSizeArray;
}