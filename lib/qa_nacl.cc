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

#include <cppunit/extensions/HelperMacros.h>
#include <gnuradio/testing.h>

#include <nacl/crypt_tagged_stream.h>
#include <nacl/generate_key.h>
#include <nacl/generate_keypair.h>

namespace gr {
namespace nacl {

class qa_nacl : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(qa_nacl);
    CPPUNIT_TEST(test_crypt_tagged_stream_make);
    CPPUNIT_TEST(test_generate_key_make);
    CPPUNIT_TEST(test_generate_keypair_make);
  CPPUNIT_TEST_SUITE_END();

public:
  void test_crypt_tagged_stream_make() {
    auto b = crypt_tagged_stream::make("keyfile","noncefile",false,"packet_len");
    CPPUNIT_ASSERT(b);
  }
  void test_generate_key_make() {
    auto k = generate_key::make("dummyfile");
    CPPUNIT_ASSERT(k);
  }
  void test_generate_keypair_make() {
    auto kp = generate_keypair::make("skfile","pkfile");
    CPPUNIT_ASSERT(kp);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(gr::nacl::qa_nacl);

}  // namespace nacl
}  // namespace gr

int main(int argc, char **argv) {
    return gr::testing::run_tests(argc, argv);
}
