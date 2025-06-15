/* -*- c++ -*- */
/* Copyright 2025 Joao Alves
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <gnuradio/io_signature.h>
#include "generate_key_impl.h"

#include <pmt/pmt.h>
// CHANGE: include <functional> to use std::bind + std::placeholders::_1
#include <functional>
// CHANGE: include <vector> to replace __GR_VLA with std::vector
#include <vector>
// CHANGE: include <cstring> to use std::memcpy (if needed)
#include <cstring>

namespace gr {
  namespace nacl {

    // Factory function (now returns std::shared_ptr via the sptr typedef)
    generate_key::sptr
    generate_key::make(const std::string &filename_key)
    {
      return gnuradio::get_initial_sptr(
        new generate_key_impl(filename_key)
      );
    }

    /*
     * The private constructor
     */
    generate_key_impl::generate_key_impl(const std::string &filename_key)
      : gr::block("generate_key",
                  gr::io_signature::make(0, 0, 0),
                  gr::io_signature::make(0, 0, 0)),
        d_key_file(filename_key)
    {
      // Register an input message port to trigger key generation
      d_port_id_in = pmt::mp("in");
      register_port(d_port_id_in);

      // CHANGE: use std::bind + std::placeholders instead of boost::bind
      set_msg_handler(d_port_id_in,
          std::bind(&generate_key_impl::handle_msg,
                    this,
                    std::placeholders::_1)
      );

      // Register the output port for the generated key blob
      d_port_id_out = pmt::mp("out");
      message_port_register_out(d_port_id_out);
    }

    /*
     * Our virtual destructor.
     */
    generate_key_impl::~generate_key_impl() = default;

    /*
     * Message handler: called when a PMT arrives on the “in” port.
     */
    void
    generate_key_impl::handle_msg(pmt::pmt_t msg)
    {
      // (Optionally inspect msg for parameters…)

      // CHANGE: generate your key bytes into a std::vector
      size_t key_len = /* your key length in bytes */;
      std::vector<unsigned char> key_bytes(key_len);
      // e.g. fill via libsodium or std::random_device:
      // std::random_device rd;
      // std::generate(key_bytes.begin(), key_bytes.end(), std::ref(rd));

      // CHANGE: package the vector into a PMT blob and publish
      pmt::pmt_t out_blob = pmt::make_blob(key_bytes.data(),
                                           key_bytes.size());
      message_port_pub(d_port_id_out, out_blob);
    }

    /*
     * No streaming work for this block.
     */
    int
    generate_key_impl::work(int /*noutput_items*/,
                             gr_vector_const_void_star & /*input_items*/,
                             gr_vector_void_star & /*output_items*/)
    {
      return 0;
    }

  } /* namespace nacl */
} /* namespace gr */
