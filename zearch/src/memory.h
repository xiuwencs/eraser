/*
	Description: Implementation of a memory.

	Properties:
		* Each element is a group of NUM_PAIRS_PER_STRUCT pairs (short, short).
		* Memory indexed by two short values, which saves space compared to storing a pointer.

	Author: Pedro Valero

	LICENSE: -zearch- Regular Expression Search on Compressed Text.
    Copyright (C) 2018 Pedro Valero & Pierre Ganty

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MEMORY_H_
#define MEMORY_H_

#include "types.h"

#define MAX_PREALLOCATED 32768
#define BLOCKS_LENGTH 1

typedef struct {
	short num_blocks; // Number of blocks used.
	short max_blocks; // Number of blocks allocated.
	int num_pos; // Number of positions in a block used.
	PAIR **blocks;
} MEMORY;

extern MEMORY mem; // Global variable, keeps track of allocated memory

/*
	Description: Initializes memory.
	Arguments:
		- None
	Return: Nothing.
*/
void memory_init();

/*
	Description: Allocates a new block, accessible at mem[block][pos].
	Arguments:
		- *block, block in memory of the new block.
		- *pos, position of the new PAIR  within block.
	Return: Nothing. Updates values block and pos.
*/
void memory_malloc(short *block, short *pos);

/*
	Description: Gets pointer to PAIR located at mem[block][pos].
	Indeed, translates pointer (short, short) to conventional pointer.
	Arguments:
		- block, block to be accessed.
		- pos, position to be accessed
	Return: Nothing.
*/
#define memory_get(block, index) &(mem.blocks[block][index])

/*
	Description: Free memory.
	Arguments:
		- None
	Return: Nothing.
*/
void memory_free();

#endif
