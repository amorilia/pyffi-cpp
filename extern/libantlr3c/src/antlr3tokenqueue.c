/** \file
 *
 * Base implementation of a token queue for an antlr 3 lexer.
 */

// [The "BSD licence"]
// Copyright (c) 2005-2009 Python File Format Interface
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <antlr3tokenqueue.h>

void token_queue_reset(pTokenQueue pqueue) {
	int i;

	pqueue->pos_begin = 0;
	pqueue->pos_end = 0;
	for (i=0; i < 256; ++i) pqueue->buffer[i] = NULL;
}

void token_queue_push(pTokenQueue pqueue, pANTLR3_COMMON_TOKEN token) {
	// pos_end++ automatically wraps around 255
	pqueue->buffer[pqueue->pos_end++] = token;
	if (pqueue->pos_begin == pqueue->pos_end) {
		// revert action and report error
		pqueue->buffer[--pqueue->pos_end] = NULL;
		//throw std::runtime_error("Cannot push to full queue.");
		ANTLR3_FPRINTF(stderr, "token_queue_push: Error: Pushing to full token queue. Some tokens will be lost!\n");
		return;
	};
}

pANTLR3_COMMON_TOKEN token_queue_pop(pTokenQueue pqueue) {
	if (pqueue->pos_begin == pqueue->pos_end) {
		//throw std::runtime_error("Cannot pop from empty queue.");
		ANTLR3_FPRINTF(stderr, "token_queue_pop: Error: Popping from empty token queue.\n");
		return NULL;
	};
	// pos_begin++ automatically wraps around 255
	pANTLR3_COMMON_TOKEN token = pqueue->buffer[pqueue->pos_begin];
	pqueue->buffer[pqueue->pos_begin++] = NULL;
	return token;
}

int token_queue_is_empty(pTokenQueue pqueue) {
	if (pqueue->pos_begin == pqueue->pos_end) {
		return 1;
	} else {
		return 0;
	};
}

int token_queue_is_full(pTokenQueue pqueue) {
	pqueue->pos_end++;
	if (pqueue->pos_begin == pqueue->pos_end) {
		pqueue->pos_end--;
		return 1;
	} else {
		pqueue->pos_end--;
		return 0;
	};
}
