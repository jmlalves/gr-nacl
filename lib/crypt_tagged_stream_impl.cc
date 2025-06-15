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
#include "crypt_tagged_stream_impl.h"

#include <pmt/pmt.h>
// CHANGE: include <functional> to use std::bind and std::placeholders::_1
#include <functional>
// CHANGE: include <vector> to replace __GR_VLA with std::vector
#include <vector>
#include <cstring>  // for std::memcpy

namespace gr {
  namespace nacl {

    // Factory function (unchanged; now returns std::shared_ptr via sptr typedef)
    crypt_tagged_stream::sptr
    crypt_tagged_stream::make(const std::string &key,
                              const std::string &nonce,
                              bool rotate_nonce,
                              const std::string &len_key)
    {
      return gnuradio::get_initial_sptr(
        new crypt_tagged_stream_impl(key, nonce, rotate_nonce, len_key)
      );
    }

    /*
     * The private constructor
     */
    crypt_tagged_stream_impl::crypt_tagged_stream_impl(const std::string &key,
                                                       const std::string &nonce,
                                                       bool rotate_nonce,
                                                       const std::string &len_key)
      : gr::tagged_stream_block("crypt_tagged_stream",
                                gr::io_signature::make(1, 1, sizeof(char)),
                                gr::io_signature::make(1, 1, sizeof(char)),
                                len_key),
        d_key(key),
        d_nonce(nonce),
        d_rotate_nonce(rotate_nonce)
    {
      d_port_id_in = pmt::mp("in");
      register_port(d_port_id_in);

      // CHANGE: replaced boost::bind(..., _1) with std::bind and std::placeholders::_1
      set_msg_handler(d_port_id_in,
          std::bind(&crypt_tagged_stream_impl::handle_msg,
                    this,
                    std::placeholders::_1)
      );
    }

    /*
     * Our virtual destructor.
     */
    crypt_tagged_stream_impl::~crypt_tagged_stream_impl() = default;

    int
    crypt_tagged_stream_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      return ninput_items[0];
    }

    void
    crypt_tagged_stream_impl::handle_msg(pmt::pmt_t msg)
    {
      pmt::pmt_t payload = pmt::cdr(msg);

      // CHANGE: replace __GR_VLA for input data with std::vector
      size_t data_size = pmt::blob_length(payload);
      const unsigned char *raw_data =
        reinterpret_cast<const unsigned char*>(pmt::blob_data(payload));
      std::vector<unsigned char> data_in(data_size);
      std::memcpy(data_in.data(), raw_data, data_size);

      //
      // … YOUR ENCRYPTION/DECRYPTION LOGIC HERE …
      // e.g. compute ciphertext_out and out_size
      //
      size_t out_size = /* compute output size */;
      std::vector<unsigned char> data_out(out_size);
      // std::memcpy(data_out.data(), ciphertext_ptr, out_size);

      // CHANGE: use std::vector payload for output
      pmt::pmt_t blob = pmt::make_blob(data_out.data(), data_out.size());
      message_port_pub(pmt::mp("out"), blob);
    }

    int
    crypt_tagged_stream_impl::work(int noutput_items,
                                   gr_vector_const_void_star &input_items,
                                   gr_vector_void_star &output_items)
    {
      // … your existing work() implementation …
      return noutput_items;
    }

  } /* namespace nacl */
} /* namespace gr */
