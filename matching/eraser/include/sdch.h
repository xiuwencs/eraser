#include "utils.h"

//SDCH
state_t SdchDistance(ACSM_STRUCT* acsm, state_t curState, AC_INFO* infoArray, TextInfo* tokenList, int* stateTodepth, int index, int dist, int length,int);
state_t SdchDistanceNaive(ACSM_STRUCT* acsm, state_t curState, AC_INFO* infoArray, TextInfo* tokenList, int* stateTodepth, int index, int dist, int length, int);
void StateToDepth(ACSM_STATETABLE* StateTable, int* stateTodepth);
inline int ScanByteAC(short& state, unsigned char ch, ACSM_STATETABLE* StateTable, int* stateTodepth);
state_t SdchDictionary(ACSM_STRUCT* acsm, state_t curState, AC_INFO* infoArray, TextInfo* tokenList, AC_INFO* DictionaryState, int* stateTodepth, int index, int dictIndex, int length,int);
state_t SdchDictionaryNaive(ACSM_STRUCT* acsm, state_t curState, AC_INFO* infoArray, TextInfo* tokenList, AC_INFO* DictionaryState, int* stateTodepth, int index, int dictIndex, int length, int);
int ScanLeftSDCH(ACSM_STRUCT* acsm, state_t& state, AC_INFO* infoArray, TextInfo* txtArray, int index, int tlen, int* stateTodepth,int);
inline state_t SdchScanByte(ACSM_STRUCT* acsm, state_t state, unsigned char token, AC_INFO* infoArray, int* stateTodepth, int index);
int ScanSegmentSDCH(ACSM_STRUCT* acsm, state_t& state, AC_INFO* infoArray, TextInfo* txtArray, int refer, int tlen, int index, int nStart, int nEnd, int* stateTodepth, AC_INFO* DictionaryState,int);
int ScanSegmentSDCHDict(ACSM_STRUCT* acsm, state_t& state, AC_INFO* infoArray, TextInfo* txtArray, int refer, int tlen, int index, int nStart, int nEnd, int* stateTodepth, AC_INFO* DictionaryState,int);
void ACCHMatching(TextInfo** Txt, ACSM_STRUCT* acsm, int ProcessFileSize[], AC_INFO* DictionaryState, int TxtLen, int* stateTodepth);

void CompressedMatching(DFA* dfa, TextInfo** infoArray, int* sizeArray, int count, int loop,AC_INFO*);
int ScanSegment(DFA* dfa, state_t& state, TextInfo* tokenInfo, AC_INFO* slideWin, int nStart, int nEnd, int dist, int index, int length, int txt_index);
inline state_t ArchScanByte(DFA* dfa, state_t state, unsigned char token, AC_INFO* slideWin, int index);
int ScanLeft(DFA* dfa, state_t& state, TextInfo* tokenInfo, AC_INFO* slideWin, int index, int length, int txt_index);
state_t MethodArchDistance(DFA* dfa, state_t curState, AC_INFO* slideWin, TextInfo* tokenList, int index, int dist, int length, int txt_index);
state_t MethodArchDict(DFA* dfa, state_t curState, AC_INFO* slideWin, TextInfo* tokenList, int index, int dist, int length,AC_INFO *Dict, int txt_index);
int ScanSegmentDict(DFA* dfa, state_t& state, TextInfo* tokenInfo, AC_INFO* slideWin, int nStart, int nEnd, int dist, int index, int length, AC_INFO* DictionaryState, int txt_index);