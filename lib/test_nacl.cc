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

#include <cppunit/extensions/HelperMacros.h>
#include <gnuradio/testing.h>   // GNURadio’s CppUnit runner

#include <nacl/crypt_tagged_stream.h>
#include <nacl/encrypt_public.h>
#include <nacl/decrypt_public.h>
#include <nacl/encrypt_secret.h>
#include <nacl/decrypt_secret.h>
#include <nacl/generate_key.h>
#include <nacl/generate_keypair.h>

namespace gr {
namespace nacl {

class test_nacl : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(test_nacl);
    CPPUNIT_TEST(test_crypt_tagged_stream);
    CPPUNIT_TEST(test_encrypt_public);
    CPPUNIT_TEST(test_decrypt_public);
    CPPUNIT_TEST(test_encrypt_secret);
    CPPUNIT_TEST(test_decrypt_secret);
    CPPUNIT_TEST(test_generate_key);
    CPPUNIT_TEST(test_generate_keypair);
  CPPUNIT_TEST_SUITE_END();

public:
  void test_crypt_tagged_stream() {
    auto b = crypt_tagged_stream::make("keyfile", "noncefile", false, "packet_len");
    CPPUNIT_ASSERT(b);
  }
  void test_encrypt_public() {
    auto b = encrypt_public::make("pkfile","skfile");
    CPPUNIT_ASSERT(b);
  }
  void test_decrypt_public() {
    auto b = decrypt_public::make("pkfile","skfile");
    CPPUNIT_ASSERT(b);
  }
  void test_encrypt_secret() {
    auto b = encrypt_secret::make("keyfile");
    CPPUNIT_ASSERT(b);
  }
  void test_decrypt_secret() {
    auto b = decrypt_secret::make("keyfile");
    CPPUNIT_ASSERT(b);
  }
  void test_generate_key() {
    auto b = generate_key::make("dummyfile");
    CPPUNIT_ASSERT(b);
  }
  void test_generate_keypair() {
    auto b = generate_keypair::make("skfile","pkfile");
    CPPUNIT_ASSERT(b);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(gr::nacl::test_nacl);

}  // namespace nacl
}  // namespace gr

// this main comes from <gnuradio/testing.h>
int main(int argc, char **argv) {
    return gr::testing::run_tests(argc, argv);
}
