#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "ac.h"
#include "dfa.h"
#include <dirent.h>
#include <queue>
#include <map>
#include <sys/time.h>
using namespace std;
#define sSIZE 3200000

typedef union 
{
	struct
	{
		unsigned short cmd;
		unsigned short token;
	}cmd;
	unsigned int len;
	unsigned int index;
	unsigned int dist;
	
}TextInfo;

enum COMMAND
{
	Literal = 1,
	Distance = 3,
	Dictionary = 2,
	Run = 4
};

typedef unsigned char uint8_t;

typedef struct A
{
	uint8_t* pBuff;
	size_t size;
}m_buffer;

typedef struct
{
	uint8_t pat[256];
	short len;
}PatternInfo;

typedef struct _AC_INFO
{

	short state;
	short status;
	short depth;

}AC_INFO;


typedef struct
{
    unsigned char token;
	union
	{
		unsigned short dist;
		unsigned short length;
	};
}TokenInfo;

#define CHECK 1
#define UnCHECK 2
#define Match 3
#define CDepth 2


//common
void Performance();
int LoadText(const char* szDir, vector<m_buffer>& buff);
int Init(vector<m_buffer>& buff, TextInfo** Txt, int* sizeArray, int count);
int LoadPattern(vector<PatternInfo>& patList, char* strPatFile);

inline int ScanByte(short &state, unsigned char ch, DFA *dfa)
{
	state = dfa->get_next_state(state, ch);
	#ifdef slow
		printf("");
	#endif

	#ifdef ACTION
		if(!dfa->accepts(state)->empty()) //g_match++;
	#endif

	
	return state;
}

inline int ScanByte(short &state, unsigned char ch, ACSM_STRUCT *acsm)
{
	state = acsm->acsmStateTable[state].NextState[ch];
	#ifdef ACTION
	if(acsm->acsmStateTable[state].MatchList!=NULL){
		//g_match++;
	}
	#endif
	return state;
}

inline int ScanByteAC(short &state, unsigned char ch, ACSM_STATETABLE *StateTable, int *stateTodepth)
{
	state = StateTable[state].NextState[ch];
	if (StateTable[state].MatchList != NULL)
	{
		return Match;
	}
	else if (stateTodepth[state] >= CDepth)
		return CHECK;

	return UnCHECK;
}
uint8_t * LoadDictionary(const char* dictionary,int &len);
void GetDictionaryState(unsigned short* DictionaryState, DFA* dfa,int len, uint8_t* DictionaryData);
void GetDictionaryStateTwins(short* DictionaryState, ACSM_STRUCT* acsm, int len, uint8_t* DictionaryData);
void GetDictionaryStateSDCH(AC_INFO* DictionaryState, ACSM_STRUCT* acsm, int len, uint8_t* DictionaryData, int* stateTodepth);
void GetDictionaryState(short* DictionaryState, ACSM_STATETABLE* StateTable, int len, uint8_t* DictionaryData, int* stateTodepth);
void GetDictionaryState(AC_INFO* DictionaryState, DFA* dfa, int len, uint8_t* DictionaryDate);

