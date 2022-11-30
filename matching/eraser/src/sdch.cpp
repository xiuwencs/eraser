#include "sdch.h"
extern int g_scan;
extern int g_match;
extern clock_t spend;
extern int literal_num;
void StateToDepth(ACSM_STATETABLE *StateTable, int *stateTodepth)
{
	std::queue<short> q;
	short state = 0;
	q.push(state);
	stateTodepth[0] = 0;
	int Depth = 0;
	while (!q.empty())
	{
		int count = q.size();
		for (int j = 0; j < count; j++)
		{
			short state = q.front();
			q.pop();
			for (int i = 0; i < 256; i++)
			{
				short nextState = StateTable[state].NextState[i];
				if (stateTodepth[nextState] == -1)
				{
					q.push(nextState);
					stateTodepth[nextState] = stateTodepth[state] + 1;
				}
			}
		}
		Depth++;
	}
}
void GetDictionaryStateSDCH(AC_INFO *DictionaryState, ACSM_STRUCT *acsm, int len, uint8_t *DictionaryData, int *stateTodepth)
{
	uint8_t *byte = DictionaryData;
	short state = 0;
	for (int index = 0; index < len; index++)
	{
		state = acsm->acsmStateTable[state].NextState[*byte];
		DictionaryState[index].depth = stateTodepth[state];
		DictionaryState[index].state = state;
		if (acsm->acsmStateTable[state].MatchList != NULL)
		{
			DictionaryState[index].status = Match;
		}
		else
		{
			if (DictionaryState[index].depth >= CDepth)
				DictionaryState[index].status = CHECK;
			else
				DictionaryState[index].status = UnCHECK;
		}
		byte++;
	}
}
int ScanLeftSDCH(ACSM_STRUCT *acsm, state_t &state, AC_INFO *infoArray, TextInfo *txtArray, int index, int tlen, int *stateTodepth, int txt_index)
{
	int curPos = 0;

	while (stateTodepth[state] > curPos && curPos < tlen)
	{
		state = SdchScanByte(acsm, state, txtArray[txt_index + curPos].cmd.token, infoArray, stateTodepth, index + curPos);
		curPos++;
	}
	return curPos;
}
inline state_t SdchScanByte(ACSM_STRUCT *acsm, state_t state, unsigned char token, AC_INFO *infoArray, int *stateTodepth, int index)
{

	g_scan++;
	state = acsm->acsmStateTable[state].NextState[token];

	infoArray[index].depth = stateTodepth[state];
	infoArray[index].state = state;

	if (acsm->acsmStateTable[state].MatchList != NULL)
	{
		infoArray[index].status = Match;
		g_match++;
	}
	else
	{
		if (infoArray[index].depth >= CDepth)
			infoArray[index].status = CHECK;
		else
			infoArray[index].status = UnCHECK;
	}
	return state;
}
int ScanSegmentSDCH(ACSM_STRUCT *acsm, state_t &state, AC_INFO *infoArray, TextInfo *txtArray, int refer, int tlen, int index, int nStart, int nEnd, int *stateTodepth, AC_INFO *DictionaryState, int txt_index)
{

	int unCheckPos = nStart;
	for (int i = nStart; i <= nEnd; i++)
	{
		if (infoArray[refer + i].status == UnCHECK)
			unCheckPos = i;
	}

	int curPos = 0;
	if (nStart < (unCheckPos - CDepth + 2))
	{
		for (int j = nStart; j <= (unCheckPos - CDepth + 2); j++)
		{
			infoArray[j + index].status = infoArray[refer + j].status;
#ifdef ACTION
			if (infoArray[j + index].status == Match)
			{
				g_match++;
			}
#endif
		}
		state = 0;
		for (curPos = unCheckPos - CDepth + 2; curPos < unCheckPos; curPos++)
		{
			state = SdchScanByte(acsm, state, txtArray[txt_index + curPos].cmd.token, infoArray, stateTodepth, index + curPos);
		}
	}
	for (curPos = unCheckPos; curPos <= nEnd; curPos++)
	{
		state = SdchScanByte(acsm, state, txtArray[txt_index + curPos].cmd.token, infoArray, stateTodepth, index + curPos);
	}
	return curPos;
}
int ScanSegmentSDCHDict(ACSM_STRUCT *acsm, state_t &state, AC_INFO *infoArray, TextInfo *txtArray, int refer, int tlen, int index, int nStart, int nEnd, int *stateTodepth, AC_INFO *DictionaryState, int txt_index)
{

	int unCheckPos = nStart;
	for (int i = nStart; i <= nEnd; i++)
	{
		if (DictionaryState[refer + i].status == UnCHECK)
			unCheckPos = i;
	}
	int curPos = 0;
	if (nStart < (unCheckPos - CDepth + 2))
	{
		for (int j = nStart; j <= (unCheckPos - CDepth + 2); j++)
		{
			infoArray[j + index].status = DictionaryState[refer + j].status;
#ifdef ACTION
			if (infoArray[j + index].status == Match)
			{
				g_match++;
			}
#endif
		}
		state = 0;
		for (curPos = unCheckPos - CDepth + 2; curPos < unCheckPos; curPos++)
		{
			state = SdchScanByte(acsm, state, txtArray[txt_index + curPos].cmd.token, infoArray, stateTodepth, index + curPos);
		}
	}
	for (curPos = unCheckPos; curPos <= nEnd; curPos++)
	{
		state = SdchScanByte(acsm, state, txtArray[txt_index + curPos].cmd.token, infoArray, stateTodepth, index + curPos);
	}
	return curPos;
}

void ACCHMatching(TextInfo **Txt, ACSM_STRUCT *acsm, int ProcessFileSize[], AC_INFO *DictionaryState, int TxtLen, int *stateTodepth)
{
	int s = 0;
	AC_INFO *State = new AC_INFO[sSIZE];
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long begin = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	for (int i = 0; i < TxtLen; i++)
	{
		int process = ProcessFileSize[i];
		TextInfo *tokenList = Txt[i];
		AC_INFO *token_state = State;
		memset(State, 0, sSIZE);
		int index = 0;
		state_t state = 0;
		int real = 0;
		while (real < process)
		{
			int cmd = tokenList[real].cmd.cmd;
			switch (cmd)
			{
			case Literal:
			{
				state = SdchScanByte(acsm, state, tokenList[real].cmd.token, token_state, stateTodepth, index);
				index++;
				real++;
				literal_num++;
			}
			break;
			case Distance:
			{
				real++;
				int dist = tokenList[real].dist;
				real++;
				int length = tokenList[real].len;
				real++;
				state = SdchDistance(acsm, state, token_state, tokenList, stateTodepth, index, dist, length, real);
				index += length;
				real += length;
			}
			break;

			case Dictionary:
			{
				real++;
				int dictIndex = tokenList[real].index;
				real++;
				int length = tokenList[real].len;
				real++;
				state = SdchDictionary(acsm, state, token_state, tokenList, DictionaryState, stateTodepth, index, dictIndex, length, real);
				index += length;
				real += length;
			}
			break;

			default:
				printf("error!");
				return;
			}
		}

		s += index;
	}
	gettimeofday(&tv, NULL);
	long end = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	spend = end - begin;

	delete[] State;
}

state_t SdchDistance(ACSM_STRUCT *acsm, state_t curState, AC_INFO *infoArray, TextInfo *tokenList, int *stateTodepth, int index, int dist, int length, int txt_index)
{
	int offset = index - dist;
	for (int k = 0; k < length; k++)
	{
		infoArray[index + k].depth = infoArray[offset + k].depth;
	}
	int curPos = ScanLeftSDCH(acsm, curState, infoArray, tokenList, index, length, stateTodepth, txt_index);

	while (curPos < length)
	{
		int matchPos = -1;
		for (int i = curPos; i < length; i++)
		{
			if (infoArray[offset + i].status == Match)
			{
				matchPos = i;
				break;
			}
		}
		if (matchPos == -1) // no such matchP os exist; Case of Right Boundary Segment
			curPos = ScanSegmentSDCH(acsm, curState, infoArray, tokenList, offset, length, index, curPos, length - 1, stateTodepth, NULL, txt_index);

		else // Case of Match Segment
			curPos = ScanSegmentSDCH(acsm, curState, infoArray, tokenList, offset, length, index, curPos, matchPos, stateTodepth, NULL, txt_index);
	}
	return curState;
}

state_t SdchDistanceNaive(ACSM_STRUCT *acsm, state_t curState, AC_INFO *infoArray, TextInfo *tokenList, int *stateTodepth, int index, int dist, int length, int txt_index)
{
	for (int i = 0; i < length; i++)
	{

		curState = SdchScanByte(acsm, curState, tokenList[txt_index + i].cmd.token, infoArray, stateTodepth, index);
		index++;
	}
	return curState;
}

state_t SdchDictionary(ACSM_STRUCT *acsm, state_t curState, AC_INFO *infoArray, TextInfo *tokenList, AC_INFO *DictionaryState, int *stateTodepth, int index, int dictIndex, int length, int txt_index)
{

	for (int k = 0; k < length; k++)
	{
		infoArray[index + k].depth = DictionaryState[dictIndex + k].depth;
		// infoArray[index + k].status = DictionaryState[dictIndex + k].status;
	}

	int curPos = ScanLeftSDCH(acsm, curState, infoArray, tokenList, index, length, stateTodepth, txt_index);

	while (curPos < length)
	{
		int matchPos = -1;
		for (int i = curPos; i < length; i++)
		{
			if (DictionaryState[dictIndex + i].status == Match)
			{
				matchPos = i;
				break;
			}
		}
		if (matchPos == -1) // no such matchP os exist; Case of Right Boundary Segment
			curPos = ScanSegmentSDCHDict(acsm, curState, infoArray, tokenList, dictIndex, length, index, curPos, length - 1, stateTodepth, DictionaryState, txt_index);

		else // Case of Match Segment

			curPos = ScanSegmentSDCHDict(acsm, curState, infoArray, tokenList, dictIndex, length, index, curPos, matchPos, stateTodepth, DictionaryState, txt_index);
	}

	return curState;
}

state_t SdchDictionaryNaive(ACSM_STRUCT *acsm, state_t curState, AC_INFO *infoArray, TextInfo *tokenList, AC_INFO *DictionaryState, int *stateTodepth, int index, int dictIndex, int length, int txt_index)
{
	for (int i = 0; i < length; i++)
	{
		curState = SdchScanByte(acsm, curState, tokenList[txt_index + i].cmd.token, infoArray, stateTodepth, index);
		index++;
	}
	return curState;
}
