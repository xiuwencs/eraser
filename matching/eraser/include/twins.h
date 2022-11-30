#include "utils.h"
short SkipPointer(TokenInfo *token, int length, int dist, short *state_array, DFA *dfa, short state);
void TwinsMatching(TokenInfo **Txt, DFA *dfa, int sizArray[], int TxtLen);