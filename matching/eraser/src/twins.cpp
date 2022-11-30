#include "twins.h"
extern int g_scan;
extern int g_match;
short SkipPointer(TokenInfo *token, int length, int dist, short *state_array, DFA *dfa, short state)
{
    short *offset = state_array - dist - 1;
    for (int pos = 0; pos < length; pos++, token++, state_array++, offset++)
    {
        if (state == *offset)
        {
            for (int i = 0; i < length - pos; ++i)
            {
                state_array[i] = offset[i + 1];
            }
#ifdef ACTION
            for (int i = 0; i < length - pos; ++i)
            {
               if (!dfa->accepts(state)->empty())
                    g_match++;
            }
#endif
            return offset[length - pos];
        }
        else
        {
            state = ScanByte(state, token->token, dfa);
            *state_array = state;
            g_scan++;
#ifdef ACTION
            if (!dfa->accepts(state)->empty())
                    g_match++;
#endif
        }
    }
    return state;
}

void TwinsMatching(TokenInfo **Txt, DFA *dfa, int sizArray[], int TxtLen)
{
    short *State = new short[sSIZE];
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long begin = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    short state = 0;
    for (int i = 0; i < TxtLen; i++)
    {
        TokenInfo *token = Txt[i];
        int len = sizArray[i];
        short *states = State;
        state = 0;
        TokenInfo *end = token + len;
        while (token < end)
        {
            if (token->length == 0) // literal
            {
                state = ScanByte(state, token->token, dfa);
                *states = state;
#ifdef ACTION
                if (!dfa->accepts(state)->empty())
                    g_match++;
#endif
                g_scan++;
                states++;
                token++;
            }
            else // pointer
            {
                int length = token->length;
                int dist = (token + 1)->dist;
                state = SkipPointer(token, length, dist, states, dfa, state);
                states += length;
                token += length;
            }
        }
    }
    gettimeofday(&tv, NULL);
    long end = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    printf("state = %d\n", state);
    printf("time = %d ms\n", end - begin);
    printf("scan = %d\n", g_scan);
    printf("match = %d\n", g_match);
    delete[] Txt;
}