#pragma once
#include "utils.h"

//REGEX
void EraserCompressMatching(TextInfo** text, DFA* dfa, int ProcessFileSize[], unsigned short* DictionaryState, int TxtLen);
void NaiveMatching(TextInfo** text, DFA* dfa, int ProcessFileSize[], unsigned short* DictionaryState, int TxtLen);
int SkipDistance(short state, short* idxArray, TextInfo* txtArray, int dist, int tlen, DFA* dfa);
int DistanceNaive(short state, short* idxArray, TextInfo* txtArray, int dist, int tlen, DFA* dfa);
int SkipDictionary(short state, short* idxArray, TextInfo* txtArray, int index, int tlen, DFA* dfa, unsigned short* DictionaryState);
int DictionaryNaive(short state, short* idxArray, TextInfo* txtArray, int index, int tlen, DFA* dfa, unsigned short* dictionaryState);

//String
void EraserCompressMatching(TextInfo** text, ACSM_STRUCT* acsm, int ProcessFileSize[], short* DictionaryState, int TxtLen);
void NaiveMatching(TextInfo** text, ACSM_STRUCT* acsm, int ProcessFileSize[], short* DictionaryState, int TxtLen);
int SkipDistance(short state, short* idxArray, TextInfo* txtArray, int dist, int tlen, ACSM_STRUCT* acsm);
int DistanceNaive(short state, short* idxArray, TextInfo* txtArray, int dist, int tlen, ACSM_STRUCT* acsm);
int SkipDictionary(short state, short* idxArray, TextInfo* txtArray, int index, int tlen, ACSM_STRUCT* acsm, short* dictionaryState);
int DictionaryNaive(short state, short* idxArray, TextInfo* txtArray, int index, int tlen, ACSM_STRUCT* acsm, short* dictionaryState);