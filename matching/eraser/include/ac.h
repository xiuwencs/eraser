#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#define ALPHABET_SIZE    256

#define ACSM_FAIL_STATE   -1

typedef struct _acsm_userdata
{
	uint32_t ref_count;
	void* id;
}ACSM_USERDATA;

typedef struct _acsm_pattern {

	struct  _acsm_pattern* next;
	unsigned char* patrn;
	unsigned char* casepatrn;
	int      n;
	int      nocase;
	int      offset;
	int      depth;
	int      negative;
	ACSM_USERDATA* udata;
	int      iid;
	void* rule_option_tree;
	void* neg_list;

} ACSM_PATTERN;


typedef struct {

	/* Next state - based on input character */
	int      NextState[ALPHABET_SIZE];

	/* Failure state - used while building NFA & DFA  */
	int      FailState;

	/* List of patterns that end here, if any */
	ACSM_PATTERN* MatchList;

}ACSM_STATETABLE;


/*
* State machine Struct
*/
typedef struct {

	int acsmMaxStates;
	int acsmNumStates;

	ACSM_PATTERN* acsmPatterns;
	ACSM_STATETABLE* acsmStateTable;

	int   bcSize;
	short bcShift[256];

	int numPatterns;
	void (*userfree)(void* p);
	void (*optiontreefree)(void** p);
	void (*neg_list_free)(void** p);

}ACSM_STRUCT;

/*
*   Prototypes
*/
ACSM_STRUCT* acsmNew();

int acsmAddPattern(ACSM_STRUCT* p, unsigned char* pat, int n,
	int nocase, int offset, int depth, int negative);

int acsmCompile(ACSM_STRUCT* acsm);
struct _SnortConfig;
int acsmCompileWithSnortConf(struct _SnortConfig*, ACSM_STRUCT* acsm,
	int (*build_tree)(struct _SnortConfig*, void* id, void** existing_tree),
	int (*neg_list_func)(void* id, void** list));

int acsmSearch(ACSM_STRUCT* acsm, unsigned char* T, int n,
	void* data, int* current_state);

void acsmFree(ACSM_STRUCT* acsm);
int acsmPatternCount(ACSM_STRUCT* acsm);

int acsmPrintDetailInfo(ACSM_STRUCT*);

int acsmPrintSummaryInfo(void);
static void Print_DFA(ACSM_STRUCT* acsm);

