/* -*- c++ -*- */
/* 
 * Copyright 2025 Joao Alves
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
#include "encrypt_public_impl.h"

#include <pmt/pmt.h>
// CHANGE: include <functional> for std::bind + std::placeholders::_1
#include <functional>
// CHANGE: include <vector> to replace __GR_VLA with std::vector
#include <vector>
// CHANGE: include <cstring> for std::memcpy
#include <cstring>

namespace gr {
  namespace nacl {

    // Factory function (now returns std::shared_ptr via the sptr typedef)
    encrypt_public::sptr
    encrypt_public::make(const std::string &filename_pk,
                         const std::string &filename_sk)
    {
      return gnuradio::get_initial_sptr(
        new encrypt_public_impl(filename_pk, filename_sk)
      );
    }

    // Constructor: registers message ports
    encrypt_public_impl::encrypt_public_impl(const std::string &filename_pk,
                                             const std::string &filename_sk)
      : gr::block("encrypt_public",
                  gr::io_signature::make(0, 0, 0),
                  gr::io_signature::make(0, 0, 0)),
        d_pk_file(filename_pk),
        d_sk_file(filename_sk)
    {
      d_port_id_in = pmt::mp("in");
      register_port(d_port_id_in);

      // CHANGE: switch from boost::bind(_1) to std::bind + std::placeholders
      set_msg_handler(d_port_id_in,
        std::bind(&encrypt_public_impl::handle_msg,
                  this,
                  std::placeholders::_1)
      );

      d_port_id_out = pmt::mp("out");
      message_port_register_out(d_port_id_out);
    }

    // Destructor (nothing special needed)
    encrypt_public_impl::~encrypt_public_impl() = default;

    // Message handler: encrypts incoming blob and publishes out
    void
    encrypt_public_impl::handle_msg(pmt::pmt_t msg)
    {
      // Pull the blob out of the PMT
      pmt::pmt_t payload = pmt::cdr(msg);

      // CHANGE: replace __GR_VLA for input data with std::vector
      size_t data_size = pmt::blob_length(payload);
      const unsigned char *raw_data =
        reinterpret_cast<const unsigned char*>(pmt::blob_data(payload));
      std::vector<unsigned char> data_in(data_size);
      std::memcpy(data_in.data(), raw_data, data_size);

      // … perform your public-key encryption here …
      // For example, compute ciphertext of length out_size:
      size_t out_size = 0;/* compute encrypted size */
      std::vector<unsigned char> data_out(out_size);
      encrypt_public_fn(
        data_in.data(), data_size,
        d_pk_file.c_str(), /* or however you load your key */,
        data_out.data()
      );

      // CHANGE: package vector into a PMT blob and send it out
      pmt::pmt_t out_blob = pmt::make_blob(data_out.data(), data_out.size());
      message_port_pub(d_port_id_out, out_blob);
    }

    // This block does no streaming work; everything is via messages
    int
    encrypt_public_impl::work(int /*noutput_items*/,
                              gr_vector_const_void_star &,
                              gr_vector_void_star &)
    {
      return 0;
    }

  } /* namespace nacl */
} /* namespace gr */
