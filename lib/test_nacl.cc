/* -*- c++ -*- */
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

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <nacl/generate_key.h>
#include <nacl/generate_keypair.h>
#include <nacl/encrypt_public.h>
#include <nacl/decrypt_public.h>
#include <nacl/encrypt_secret.h>
#include <nacl/decrypt_secret.h>

class test_nacl : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(test_nacl);
    CPPUNIT_TEST(test_generate_key);
    CPPUNIT_TEST(test_generate_keypair);
    CPPUNIT_TEST(test_encrypt_decrypt_public);
    CPPUNIT_TEST(test_encrypt_decrypt_secret);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()    {}
    void tearDown() {}

    void test_generate_key()
    {
        auto blk = gr::nacl::generate_key::make("test_key.bin");
        CPPUNIT_ASSERT(blk);
    }

    void test_generate_keypair()
    {
        auto blk = gr::nacl::generate_keypair::make("sk.bin","pk.bin");
        CPPUNIT_ASSERT(blk);
    }

    void test_encrypt_decrypt_public()
    {
        auto enc = gr::nacl::encrypt_public::make("pk.bin","sk.bin");
        CPPUNIT_ASSERT(enc);
        auto dec = gr::nacl::decrypt_public::make("pk.bin","sk.bin");
        CPPUNIT_ASSERT(dec);
    }

    void test_encrypt_decrypt_secret()
    {
        auto enc = gr::nacl::encrypt_secret::make("secret_key.bin");
        CPPUNIT_ASSERT(enc);
        auto dec = gr::nacl::decrypt_secret::make("secret_key.bin");
        CPPUNIT_ASSERT(dec);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_nacl);
