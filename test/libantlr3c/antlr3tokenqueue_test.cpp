/*

Copyright (c) 2007-2008, Python File Format Interface
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   * Neither the name of the Python File Format Interface
     project nor the names of its contributors may be used to endorse
     or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <iostream>

#include "antlr3tokenqueue.h"

BOOST_AUTO_TEST_SUITE(tokenqueue_test_suite)

BOOST_AUTO_TEST_CASE(declare_test) {
	// check that declaration does not throw
	BOOST_CHECK_NO_THROW(TokenQueue tq);
}

BOOST_AUTO_TEST_CASE(reset_test) {
	TokenQueue tq;
	// check that reset does not throw
	BOOST_CHECK_NO_THROW(token_queue_reset(&tq));
}

BOOST_AUTO_TEST_CASE(is_empty_test) {
	TokenQueue tq;
	token_queue_reset(&tq);
	// check that queue is empty after resetting
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
	// check that queue is not empty after push
	token_queue_push(&tq, NULL);
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 0);
	// check that queue is empty after pop
	token_queue_pop(&tq);
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
}

BOOST_AUTO_TEST_CASE(fifo_test1) {
	TokenQueue tq;
	token_queue_reset(&tq);
	// create two tokens
	ANTLR3_COMMON_TOKEN tok1;
	ANTLR3_COMMON_TOKEN tok2;
	// check that push does not throw
	BOOST_CHECK_NO_THROW(token_queue_push(&tq, &tok1));
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 0);
	BOOST_CHECK_NO_THROW(token_queue_push(&tq, &tok2));
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 0);
	// check that pop returns what we have pushed, first in first out
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), &tok1);
	// check that queue is still non-empty after first pop
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 0);
	// second pop...
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), &tok2);
	// check that queue is empty after second pop
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
}

BOOST_AUTO_TEST_CASE(fifo_test2) {
	TokenQueue tq;
	token_queue_reset(&tq);
	ANTLR3_COMMON_TOKEN tok1;
	ANTLR3_COMMON_TOKEN tok2;
	ANTLR3_COMMON_TOKEN tok3;

	// check a more complicated fifo sequence
	token_queue_push(&tq, &tok1);
	token_queue_push(&tq, &tok3);
	token_queue_push(&tq, &tok2);
	// t1 t3 t2
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), &tok1);
	// t3 t2
	token_queue_push(&tq, &tok1);
	// t3 t2 t1
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), &tok3);
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), &tok2);
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), &tok1);
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
}

BOOST_AUTO_TEST_CASE(fifo_test3) {
	TokenQueue tq;
	token_queue_reset(&tq);
	ANTLR3_COMMON_TOKEN toklist[100];

	// stress test! push and pop 100 tokens 1000 times
	for (int i = 0; i < 1000; i++) {
		BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
		for (int j = 0; j < 100; j++)
			token_queue_push(&tq, &toklist[j]);
		for (int j = 0; j < 100; j++)
			BOOST_CHECK_EQUAL(token_queue_pop(&tq), &toklist[j]);
	};
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
}

BOOST_AUTO_TEST_CASE(pop_empty_test) {
	TokenQueue tq;
	token_queue_reset(&tq);
	ANTLR3_COMMON_TOKEN tok1;

	// pop from an empty buffer, returns NULL
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), (void*)NULL);
	// still empty
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
	// push something
	token_queue_push(&tq, &tok1);
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 0);
	// pop
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), &tok1);
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
	// pop again (once more from empty queue)
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), (void*)NULL);
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
}

BOOST_AUTO_TEST_CASE(push_full_test) {
	TokenQueue tq;
	token_queue_reset(&tq);
	ANTLR3_COMMON_TOKEN tok1;
	ANTLR3_COMMON_TOKEN tok2;

	// push buffer until full (note: implementation allows 255 entries)
	int i;
	for (i = 0; i < 10000; i++) {
		token_queue_push(&tq, &tok1);
		if (token_queue_is_full(&tq))
			break;
	};
	// check maximal length of buffer
	BOOST_CHECK_EQUAL(i, 254); // 255 entries: 0, 1, ..., 254
	// try pushing; this should leave the queue unaffected
	token_queue_push(&tq, &tok2);
	// check that previous action had no effect by popping and checking
	// all tokens
	for (; i>=0; i--) {
		BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 0);
		BOOST_CHECK_EQUAL(token_queue_pop(&tq), &tok1);
	};
	// check that no more tokens can be popped
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
	BOOST_CHECK_EQUAL(token_queue_pop(&tq), (void*)NULL);
	// remains empty after failed pop
	BOOST_CHECK_EQUAL(token_queue_is_empty(&tq), 1);
}

BOOST_AUTO_TEST_SUITE_END()

