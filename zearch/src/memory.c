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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "memory.h"
#include "types.h"

void memory_init() {
	mem.blocks = (PAIR**)malloc(sizeof(PAIR*) * BLOCKS_LENGTH);
	mem.num_blocks = 0;
	mem.max_blocks = BLOCKS_LENGTH;
	mem.num_pos = MAX_PREALLOCATED - 1;
}

void memory_malloc(short *block, short *pos){
	if (mem.num_pos == MAX_PREALLOCATED - 1) {
		if (mem.num_blocks == mem.max_blocks) {
			mem.max_blocks = mem.max_blocks*2;
			mem.blocks = realloc(mem.blocks, sizeof(PAIR*) * mem.max_blocks);
		}
		mem.blocks[mem.num_blocks] = (PAIR*)calloc(MAX_PREALLOCATED,sizeof(PAIR));
		mem.num_blocks++;
		mem.num_pos = 0;
	}

	(*block) = (mem.num_blocks - 1);
	(*pos) = mem.num_pos++;
}

void memory_free() {
	int i;
	for (i = 0; i < mem.num_blocks; i++) free(mem.blocks[i]);
	free(mem.blocks);
	return;
}