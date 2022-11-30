#include "sdch.h"

int main(int argc,char **argv){

    DFA *dfa = new DFA;

    char *file = argv[1];

    FILE *f =  fopen(file,"r");

    dfa->get(f);

    return 0;
}