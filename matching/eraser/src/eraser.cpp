#include "eraser.h"
extern int g_scan;
extern int g_match;
extern unsigned int compress;
extern clock_t spend;
extern unsigned int total;
extern int literal_num;
extern int pointer_num;
extern int pointer_len;

void EraserCompressMatching(TextInfo **Txt, DFA *P_DFA, int ProcessFileSize[], unsigned short *DictionaryState, int TxtLen)
{
    short *State = new short[sSIZE];
    struct timeval tv;
    gettimeofday(&tv, NULL);
    short state = 0;
    long begin = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    for (int i = 0; i < TxtLen; i++)
    {
        int process = ProcessFileSize[i];
        TextInfo *tokenList = Txt[i];
        short *token_state = State;

        TextInfo *endPtr = tokenList + process;

        state = 0;
        while (tokenList < endPtr)
        {
            int cmd = tokenList->cmd.cmd;
            switch (cmd)
            {
            case Literal:
            {
                *token_state = ScanByte(state, tokenList->cmd.token, P_DFA);//Get the next state according to the input token and active state
                g_scan++;
                tokenList++;
                token_state++;
                literal_num++;
            }
            break;
            case Distance:
            {
                tokenList++;
                int dist = tokenList->dist;
                tokenList++;
                int tlen = tokenList->len;
                tokenList++;
                state = SkipDistance(state, token_state, tokenList, dist, tlen, P_DFA);
                token_state += tlen;
                tokenList += tlen;
                pointer_len += tlen;
                pointer_num++;
            }
            break;

            case Dictionary:
            {
                tokenList++;
                unsigned int index = tokenList->index;
                tokenList++;
                unsigned int tlen = tokenList->len;
                tokenList++;
                state = SkipDictionary(state, token_state, tokenList, index, tlen, P_DFA, DictionaryState);
                token_state += tlen;
                tokenList += tlen;
                pointer_len += tlen;
                pointer_num++;
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

void NaiveMatching(TextInfo **Txt, DFA *P_DFA, int ProcessFileSize[], unsigned short *DictionaryState, int TxtLen)
{
    short *State = new short[sSIZE];
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long begin = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    for (int i = 0; i < TxtLen; i++)
    {
        int process = ProcessFileSize[i];
        TextInfo *tokenList = Txt[i];
        short *token_state = State;

        TextInfo *endPtr = tokenList + process;
        short state = 0;

        while (tokenList < endPtr)
        {
            int cmd = tokenList->cmd.cmd;
            switch (cmd)
            {
            case Literal:
            {
                *token_state = ScanByte(state, tokenList->cmd.token, P_DFA);
                g_scan++;
                tokenList++;
                token_state++;
                literal_num++;
            }
            break;
            case Distance:
            {
                tokenList++;
                int dist = tokenList->dist;
                tokenList++;
                int tlen = tokenList->len;
                tokenList++;
                state = DistanceNaive(state, token_state, tokenList, dist, tlen, P_DFA);
                token_state += tlen;
                tokenList += tlen;
                pointer_len += tlen;
                pointer_num++;
            }
            break;

            case Dictionary:
            {
                tokenList++;
                unsigned int index = tokenList->index;
                tokenList++;
                unsigned int tlen = tokenList->len;
                tokenList++;
                state = DictionaryNaive(state, token_state, tokenList, index, tlen, P_DFA, DictionaryState);
                token_state += tlen;
                tokenList += tlen;
                pointer_len += tlen;
                pointer_num++;
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

int SkipDistance(short state, short *infoArray, TextInfo *txtArray, int dist, int tlen, DFA *dfa)
{
    short *offset = infoArray - dist - 1;

    for (int pos = 0; pos < tlen; pos++, txtArray++, infoArray++, offset++)
    {
        if (state == *offset)
        {
            if (tlen <= dist)
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
                if(!dfa->accepts(infoArray[i])->empty()) g_match++;
            }
#endif
            return offset[tlen - pos];
        }
        else
        {
            *infoArray = ScanByte(state, txtArray->cmd.token, dfa);
            g_scan++;
        }
    }
    return state;
}

int DictionaryNaive(short state, short *infoArray, TextInfo *txtArray, int index, int tlen, DFA *dfa, unsigned short *dictionaryState)
{
    for (int pos = 0; pos < tlen; pos++, infoArray++, txtArray++)
    {
        *infoArray = ScanByte(state, txtArray->cmd.token, dfa);
        g_scan++;
    }
    return state;
}

int DistanceNaive(short state, short *infoArray, TextInfo *txtArray, int dist, int tlen, DFA *dfa)
{
    for (int pos = 0; pos < tlen; pos++, infoArray++, txtArray++)
    {
        *infoArray = ScanByte(state, txtArray->cmd.token, dfa);
        g_scan++;
    }
    return state;
}
int SkipDictionary(short state, short *infoArray, TextInfo *txtArray, int index, int tlen, DFA *dfa, unsigned short *dictionaryState)
{
    unsigned short *offset = dictionaryState + index - 1;
    for (int pos = 0; pos < tlen; pos++, txtArray++, infoArray++, offset++)
    {
        if (state == *offset)
        {
            memcpy(infoArray, offset + 1, sizeof(short) * (tlen - pos));
            return offset[tlen - pos];
        }
        else
        {
            *infoArray = ScanByte(state, txtArray->cmd.token, dfa);
            g_scan++;
        }
    }
    return state;
}
