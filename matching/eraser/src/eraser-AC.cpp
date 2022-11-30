#include "eraser.h"
extern int g_scan;
extern int g_match;
extern int literal_num;
extern clock_t spend;

void EraserCompressMatching(TextInfo **Txt, ACSM_STRUCT *acsm, int ProcessFileSize[], short *DictionaryState, int TxtLen)
{
	short *State = new short[sSIZE];
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long begin = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	short state = 0;
	for (int i = 0; i < TxtLen; i++)
	{
		int count = 0;
		int process = ProcessFileSize[i];
		TextInfo *tokenList = Txt[i];
		short *token_state = State;
		TextInfo *endPtr = tokenList + process;
		state = 0;
		int index = 0;
		while (tokenList < endPtr)
		{
			int cmd = tokenList->cmd.cmd;
			switch (cmd)
			{
			case Literal:
			{
				*token_state = ScanByte(state, tokenList->cmd.token, acsm);
				tokenList++;
				token_state++;
				g_scan++;
				literal_num++;
				count++;
			}
			break;
			case Distance:
			{
				tokenList++;
				int dist = tokenList->dist;
				tokenList++;
				int tlen = tokenList->len;
				tokenList++;
				state = SkipDistance(state, token_state, tokenList, dist, tlen, acsm);
				token_state += tlen;
				tokenList += tlen;
			}
			break;

			case Dictionary:
			{
				tokenList++;
				unsigned int index = tokenList->index;
				tokenList++;
				unsigned int tlen = tokenList->len;
				tokenList++;
				state = SkipDictionary(state, token_state, tokenList, index, tlen, acsm, DictionaryState);
				token_state += tlen;
				tokenList += tlen;
			}
			break;

			default:
				printf("error!");
				return;
			}
		}
	}
	gettimeofday(&tv, NULL);
	long end = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	spend = end - begin;
	printf("state = %d\n", state);
	delete[] State;
}

void NaiveMatching(TextInfo **Txt, ACSM_STRUCT *acsm, int ProcessFileSize[], short *DictionaryState, int TxtLen)
{
	short *State = new short[sSIZE];
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long begin = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	for (int i = 0; i < TxtLen; i++)
	{
		int count = 0;
		int process = ProcessFileSize[i];
		TextInfo *tokenList = Txt[i];
		short *token_state = State;
		TextInfo *endPtr = tokenList + process;
		short state = 0;
		int index = 0;
		while (tokenList < endPtr)
		{
			int cmd = tokenList->cmd.cmd;
			switch (cmd)
			{
			case Literal:
			{
				*token_state = ScanByte(state, tokenList->cmd.token, acsm);
				tokenList++;
				token_state++;
				literal_num++;
				g_scan++;
				count++;
			}
			break;
			case Distance:
			{
				tokenList++;
				int dist = tokenList->dist;
				tokenList++;
				int tlen = tokenList->len;
				tokenList++;
				state = DistanceNaive(state, token_state, tokenList, dist, tlen, acsm);
				token_state += tlen;
				tokenList += tlen;
			}
			break;

			case Dictionary:
			{
				tokenList++;
				unsigned int index = tokenList->index;
				tokenList++;
				unsigned int tlen = tokenList->len;
				tokenList++;
				state = DictionaryNaive(state, token_state, tokenList, index, tlen, acsm, DictionaryState);
				token_state += tlen;
				tokenList += tlen;
			}
			break;

			default:
				printf("error!");
				return;
			}
		}
	}
	gettimeofday(&tv, NULL);
	long end = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	spend = end - begin;
	delete[] State;
}

int SkipDistance(short state, short *infoArray, TextInfo *txtArray, int dist, int tlen, ACSM_STRUCT *acsm)
{
	short *offset = infoArray - dist - 1;
	for (int pos = 0; pos < tlen; pos++, txtArray++, infoArray++, offset++)
	{
		if (state == *offset)
		{
			if (dist >= tlen)
			{
				memcpy(infoArray, offset + 1, sizeof(short) * (tlen - pos));
			}
			else
			{
				for (int i = 0; i < tlen - pos; i++)
				{
					infoArray[i] = offset[i + 1];
				}
			}

#ifdef ACTION
			for (int i = 0; i < tlen - pos; i++)
			{
				if (acsm->acsmStateTable[infoArray[i]].MatchList != NULL)
				{
					g_match++;
				}
			}
#endif
			return offset[tlen - pos];
		}
		else
		{
			*infoArray = ScanByte(state, txtArray->cmd.token, acsm);
			g_scan++;
		}
	}
	return state;
}
int DistanceNaive(short state, short *infoArray, TextInfo *txtArray, int dist, int tlen, ACSM_STRUCT *acsm)
{
	for (int pos = 0; pos < tlen; pos++, infoArray++, txtArray++)
	{
		*infoArray = ScanByte(state, txtArray->cmd.token, acsm);
		g_scan++;
	}
	return state;
}
int SkipDictionary(short state, short *infoArray, TextInfo *txtArray, int index, int tlen, ACSM_STRUCT *acsm, short *dictionaryState)
{
	short *offset = dictionaryState + index - 1;
	int idx = 0;
	for (int pos = 0; pos < tlen; pos++, txtArray++, infoArray++, offset++, index++)
	{
		if (state == *offset)
		{
			memcpy(infoArray, offset + 1, sizeof(short) * (tlen - pos));

#ifdef ACTION
			for (int i = 0; i < tlen - pos; i++)
			{
				if (acsm->acsmStateTable[infoArray[i]].MatchList != NULL)
				{
					g_match++;
				}
			}
#endif
			return offset[tlen - pos];
		}
		else
		{
			*infoArray = ScanByte(state, txtArray->cmd.token, acsm);
			g_scan++;
		}
	}
	return state;
}
int DictionaryNaive(short state, short *infoArray, TextInfo *txtArray, int index, int tlen, ACSM_STRUCT *acsm, short *dictionaryState)
{
	for (int pos = 0; pos < tlen; pos++, infoArray++, txtArray++)
	{
		*infoArray = ScanByte(state, txtArray->cmd.token, acsm);
		g_scan++;
	}
	return state;
}
