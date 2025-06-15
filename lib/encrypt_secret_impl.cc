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
#include "encrypt_secret_impl.h"

#include <pmt/pmt.h>
// CHANGE: include <functional> to use std::bind + std::placeholders::_1
#include <functional>
// CHANGE: include <vector> to replace __GR_VLA with std::vector
#include <vector>
// CHANGE: include <cstring> to use std::memcpy
#include <cstring>

namespace gr {
  namespace nacl {

    // Factory function (now returns std::shared_ptr via sptr typedef)
    encrypt_secret::sptr
    encrypt_secret::make(const std::string &filename_key)
    {
      return gnuradio::get_initial_sptr(
        new encrypt_secret_impl(filename_key)
      );
    }

    /*
     * The private constructor
     */
    encrypt_secret_impl::encrypt_secret_impl(const std::string &filename_key)
      : gr::block("encrypt_secret",
                  gr::io_signature::make(0, 0, 0),
                  gr::io_signature::make(0, 0, 0)),
        d_key_file(filename_key)
    {
      d_port_id_in = pmt::mp("in");
      register_port(d_port_id_in);

      // CHANGE: replaced boost::bind(..., _1) with std::bind + std::placeholders::_1
      set_msg_handler(d_port_id_in,
          std::bind(&encrypt_secret_impl::handle_msg,
                    this,
                    std::placeholders::_1)
      );

      d_port_id_out = pmt::mp("out");
      message_port_register_out(d_port_id_out);
    }

    /*
     * Our virtual destructor.
     */
    encrypt_secret_impl::~encrypt_secret_impl() = default;

    /*
     * Message‐based work callback
     */
    void
    encrypt_secret_impl::handle_msg(pmt::pmt_t msg)
    {
      // Extract the PMT payload
      pmt::pmt_t payload = pmt::cdr(msg);

      // CHANGE: replace __GR_VLA with std::vector for incoming data
      size_t data_size = pmt::blob_length(payload);
      const unsigned char *raw_data =
        reinterpret_cast<const unsigned char*>(pmt::blob_data(payload));
      std::vector<unsigned char> data_in(data_size);
      std::memcpy(data_in.data(), raw_data, data_size);

      // … perform your secret‐key encryption here into data_out …
      // CHANGE: allocate std::vector for encrypted output
      size_t out_size = /* compute encrypted length */;
      std::vector<unsigned char> data_out(out_size);
      // secret_encrypt_fn(data_in.data(), data_size,
      //                   d_key_file.c_str(),
      //                   data_out.data());

      // CHANGE: publish back as a PMT blob using the vector’s data()
      pmt::pmt_t out_blob = pmt::make_blob(
        data_out.data(),
        data_out.size()
      );
      message_port_pub(d_port_id_out, out_blob);
    }

    /*
     * This block doesn’t do any stream‐based work.
     */
    int
    encrypt_secret_impl::work(int /*noutput_items*/,
                              gr_vector_const_void_star &,
                              gr_vector_void_star &)
    {
      return 0;
    }

  } /* namespace nacl */
} /* namespace gr */
