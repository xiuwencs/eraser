#include "twins.h"
int g_scan = 0;
int g_match = 0;
unsigned int compress = 480787746; // 153608528;;//26872098; //153,608,528  //478998647
unsigned int total = 228422347;	   // 228422347;;//73385030; //228,422,347  //73385030 //1241229032
clock_t spend = 0;
int literal_num = 0;
int real_index = 0;
int pointer_num = 0;
int pointer_len = 0;
int main(int argc, char **argv)
{
    char *input = argv[1];
    char *rule = argv[2];

    vector<m_buffer> buff;
	
    int size = LoadText(input, buff);
    int *sizeArray = new int[size];
    TokenInfo **Txt = new TokenInfo *[size];
    for (int i = 0; i < buff.size(); i++)
    {
        sizeArray[i] = buff[i].size / sizeof(TokenInfo);
        Txt[i] = (TokenInfo *)buff[i].pBuff;
    }
	printf("get DFA\n");
    DFA *dfa = new DFA;
    FILE *Pattern = fopen(rule, "r");
    dfa->get(Pattern);
    TwinsMatching(
        Txt, dfa, sizeArray, size);
}