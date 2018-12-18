/*
 * Copyright (c) 2017, Mellanox Technologies and Contributors.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of Mellanox nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>

/*
 * Get how many bits are set in the given int. Sadly if we use the builtin
 * __popcount the linker would complain that __popcountdi2() is not found,
 * so we might as well implement our own here.
 */
int popcount(unsigned int num)
{
	int result = 0;

	while (num != 0) {
		num &= num - 1;
		result++;
	}
	return result;
}

/*
 * xorshift64* random number generator (see https://arxiv.org/abs/1402.6246v2).
 * The state value must be nonzero.
 * If better random values are desired, please use the other random function at
 * random.c in the same directory; it has better randomization but at a cost of
 * beng more computationally expensive.
 * Return a random number each time this function is called.
 */
uint64_t rand_gen(uint64_t *rand_state)
{
	if (*rand_state == 0)
		*rand_state = 5652546455764565454ULL;

	*rand_state ^= *rand_state >> 12;
	*rand_state ^= *rand_state << 25;
	*rand_state ^= *rand_state >> 27;

	return *rand_state ^= 2685821657736338717ULL;
}

