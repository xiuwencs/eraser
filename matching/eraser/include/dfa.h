#ifndef __DFA_H               
#define __DFA_H

#include <map>
#include <set>
#include <list>
#include <set>
#include <stdio.h>
/*
* This is a DFA engine, used as an interface. 
  More details can be found at http://regex.wustl.edu
  You can replace this engine with another one.
*/

typedef unsigned short state_t;
typedef unsigned char symbol_t;

class DFA
{
public:
	DFA(unsigned = 50);
	~DFA();

	void get(FILE* file);

	state_t lookup(state_t s, symbol_t c);
	state_t get_next_state(state_t s, symbol_t c);
};

#endif /*__DFA_H*/
