#include "eraser.h"
int g_scan = 0;
int g_match = 0;
unsigned int compress = 480787746; // 153608528;;//26872098; //153,608,528  //478998647
unsigned int total = 228422347;	   // 228422347;;//73385030; //228,422,347  //73385030 //1241229032
clock_t spend = 0;
int literal_num = 0;
int pointer_num = 0;
int pointer_len = 0;

int main(int argc,char **argv)
{
	const char *input = argv[1];
	const char *dict = argv[2];
	const char *rule = argv[3];
	vector<m_buffer> mBuff;
	vector<PatternInfo> patList;
	int count = LoadText(input, mBuff);
	int len = 0;
	uint8_t *DictionaryData = LoadDictionary(dict, len);
	unsigned short *DictionaryState = new unsigned short[len];
	DFA *dfa = new DFA;
	FILE *Pattern = fopen(rule, "r");
	dfa->get(Pattern);
	GetDictionaryState(DictionaryState, dfa, len, DictionaryData);
	TextInfo **Txt = new TextInfo *[count];
	int *ProcessSizeArray = new int[count];
	Init(mBuff, Txt, ProcessSizeArray, count);

	NaiveMatching(Txt, dfa, ProcessSizeArray, DictionaryState, count);
	
	printf("pointer average len = %.2f\n", ((pointer_len * 1.0 / pointer_num)));
	Performance();

	delete[] ProcessSizeArray;
	delete[] Txt;
	delete[] DictionaryState;
	return 0;
}