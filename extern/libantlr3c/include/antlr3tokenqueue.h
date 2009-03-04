/** \file
 *
 * Base header of a token queue for an antlr 3 lexer.
 */

#ifndef	_ANTLR3_TOKENQUEUE
#define	_ANTLR3_TOKENQUEUE

// [The "BSD licence"]
// Copyright (c) 2009 Python File Format Interface
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

#include <antlr3defs.h>
#include <antlr3commontoken.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TokenQueue_struct TokenQueue, * pTokenQueue;

// a token queue (first in first out)
// implemented as a circular buffer of size 256
// http://en.wikipedia.org/wiki/Circular_buffer
struct TokenQueue_struct {
	ANTLR3_UINT8 pos_begin;
	ANTLR3_UINT8 pos_end;
	pANTLR3_COMMON_TOKEN buffer[256];
};

void token_queue_reset(pTokenQueue pqueue);
void token_queue_push(pTokenQueue pqueue, pANTLR3_COMMON_TOKEN token);
pANTLR3_COMMON_TOKEN token_queue_pop(pTokenQueue pqueue);
int token_queue_is_empty(pTokenQueue pqueue);
int token_queue_is_full(pTokenQueue pqueue);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
