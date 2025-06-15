/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * This class gathers together all the test cases for the gr-filter
 * directory into a single test suite.  As you create new test cases,
 * add them here.
 */

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <nacl/crypt_tagged_stream.h>

class qa_nacl : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(qa_nacl);
    CPPUNIT_TEST(test_crypt_tagged_stream);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()    {}
    void tearDown() {}

    void test_crypt_tagged_stream()
    {
        auto blk = gr::nacl::crypt_tagged_stream::make(
            "mykey",             // encryption key
            "mynonce",           // nonce
            false,               // rotate nonce
            "packet_len"         // length-tag key
        );
        CPPUNIT_ASSERT(blk);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(qa_nacl);
