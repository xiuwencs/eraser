#include "utils.h"
extern int g_scan;

extern int g_match;
extern unsigned int compress;
extern clock_t spend;
extern unsigned int total;
extern int literal_num;
extern int real_index;
extern int pointer_num;
extern int pointer_len;

int LoadText(const char *path, vector<m_buffer> &buff)
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
		unsigned char *pBuf = (unsigned char *)malloc(statbuf.st_size);
		FILE *hFile = fopen(szFile, "r");
		if (hFile == NULL)
		{
			printf("open file %s error\n", entry->d_name);
			continue;
		}
		fread(pBuf, statbuf.st_size, 1, hFile);
		fclose(hFile);
		buff.push_back({pBuf, static_cast<size_t>(statbuf.st_size)});
	}
	closedir(pDir);
	return buff.size();
}

int Init(vector<m_buffer> &buff, TextInfo **Txt, int *sizeArray, int count)
{
	for (int i = 0; i < count; i++)
	{
		Txt[i] = (TextInfo *)buff[i].pBuff;
		sizeArray[i] = buff[i].size / sizeof(TextInfo);
	}
	return 0;
}



uint8_t *LoadDictionary(const char *path, int &len) // read dictionary
{
	unsigned long dwRead = 0;
	struct stat statbuf;
	char fineName[256] = {0};
	strcpy(fineName, path);
	stat(fineName, &statbuf);
	unsigned char *pBuf = new unsigned char[statbuf.st_size];
	FILE *hFile = fopen(fineName, "r");
	fread(pBuf, statbuf.st_size, 1, hFile);
	len = statbuf.st_size;
	fclose(hFile);
	return pBuf;
}

void GetDictionaryState(unsigned short *DictionaryState, DFA *dfa, int len, uint8_t *DictionaryDate)
{
	uint8_t *byte = DictionaryDate;
	state_t start_state = 0;
	for (int i = 0; i < len; i++)
	{
		state_t cur_state = dfa->get_next_state(start_state, *byte);
		*DictionaryState = cur_state;
		start_state = cur_state;
		byte++;
		DictionaryState++;
	}
}
void GetDictionaryState(short *DictionaryState, ACSM_STATETABLE *StateTable, int len, uint8_t *DictionaryData, int *stateTodepth)
{
	uint8_t *byte = DictionaryData;
	short start_state = 0;
	for (int i = 0; i < len; i++)
	{
		short cur_state = ScanByteAC(start_state, *byte, StateTable, stateTodepth);
		*DictionaryState = cur_state;
		start_state = cur_state;
		byte++;
		DictionaryState++;
	}
}

int LoadPattern(vector<PatternInfo> &patList, char *strPatFile)
{
	FILE *pFile = fopen(strPatFile, "r");
	if (pFile == NULL)
	{
		printf("could not open pattern file\n");
		exit(0);
	}

	while (true)
	{
		PatternInfo pi = {0};
		if (fscanf(pFile, "%[^\n]\n", pi.pat) == -1)
			break;
		pi.len = strlen((const char *)pi.pat);
		patList.push_back(pi);
	}

	return patList.size();
}

/*void GetDictionaryState(AC_INFO* DictionaryState, DFA* dfa, int len, uint8_t* DictionaryDate) {

	uint8_t* byte = DictionaryDate;
	state_t start_state = 0;
	for (int i = 0; i < len; i++) {
		state_t cur_state = dfa->get_next_state(start_state, *byte);
		DictionaryState->state = cur_state;
		DictionaryState->depth = Depth(dfa, cur_state, DictionaryState, i);
		if (!dfa->accepts(cur_state)->empty())
		{
			DictionaryState->status = Match;
		}
		else
		{
			if (DictionaryState->depth >= CDepth)
				DictionaryState->status = CHECK;
			else
				DictionaryState->status = UnCHECK;
		}
		start_state = cur_state;
		byte++;
		DictionaryState++;
	}
}*/

void Performance()
{
	printf("time:%d ms, matched:%d, scan:%d, total:%d, skip-ratio: %.2f%, throughput %.2f mbps,literal-num:%d, pointer-ratio:%.2f\n",
		   spend, g_match, g_scan, total, 100 * (1.0 - (g_scan * 1.0 / total)), (compress * 8) / (double)(spend) / 1000, literal_num, 100 * (1.0 - (literal_num * 1.0 / total)));
	printf("pointer average len = %.2f\n", ((pointer_len * 1.0 / pointer_num)));
}


void GetDictionaryStateTwins(short *DictionaryState, ACSM_STRUCT *acsm, int len, uint8_t *DictionaryData)
{
	uint8_t *byte = DictionaryData;
	short start_state = 0;
	for (int i = 0; i < len; i++)
	{
		short cur_state = ScanByte(start_state, *byte, acsm);
		*DictionaryState = cur_state;
		start_state = cur_state;
		byte++;
		DictionaryState++;
	}
}