/* -*- c++ -*- */
/* /* 
 *  * Copyright 2025 Joao Alves
 *  * 
 *  * This is free software; you can redistribute it and/or modify
 *  * it under the terms of the GNU General Public License as published by
 *  * the Free Software Foundation; either version 3, or (at your option)
 *  * any later version.
 *  * 
 *  * This software is distributed in the hope that it will be useful,
 *  * but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  * GNU General Public License for more details.
 *  * 
 *  * You should have received a copy of the GNU General Public License
 *  * along with this software; see the file COPYING.  If not, write to
 *  * the Free Software Foundation, Inc., 51 Franklin Street,
 *  * Boston, MA 02110-1301, USA.
 *  */

#include <gnuradio/io_signature.h>
#include "decrypt_public_impl.h"

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
    decrypt_public::sptr
    decrypt_public::make(const std::string &filename_pk,
                         const std::string &filename_sk)
    {
      return gnuradio::get_initial_sptr(
        new decrypt_public_impl(filename_pk, filename_sk)
      );
    }

    // Constructor
    decrypt_public_impl::decrypt_public_impl(const std::string &filename_pk,
                                             const std::string &filename_sk)
      : gr::block("decrypt_public",
                  gr::io_signature::make(0, 0, 0),
                  gr::io_signature::make(0, 0, 0)),
        d_pk_file(filename_pk),
        d_sk_file(filename_sk)
    {
      d_port_id_in = pmt::mp("in");
      register_port(d_port_id_in);

      // CHANGE: use std::bind instead of boost::bind and std::placeholders::_1
      set_msg_handler(d_port_id_in,
        std::bind(&decrypt_public_impl::handle_msg,
                  this,
                  std::placeholders::_1)
      );

      d_port_id_out = pmt::mp("out");
      message_port_register_out(d_port_id_out);
    }

    // Destructor
    decrypt_public_impl::~decrypt_public_impl() = default;

    // Message handler
    void
    decrypt_public_impl::handle_msg(pmt::pmt_t msg)
    {
      // Extract payload
      pmt::pmt_t payload = pmt::cdr(msg);

      // CHANGE: replace __GR_VLA with std::vector for input data
      size_t data_size = pmt::blob_length(payload);
      const unsigned char *raw_data =
        reinterpret_cast<const unsigned char*>(pmt::blob_data(payload));
      std::vector<unsigned char> data_char(data_size);
      std::memcpy(data_char.data(), raw_data, data_size);

      // Extract nonce (if your protocol uses one)
      pmt::pmt_t nonce_pmt = pmt::dict_ref(msg, pmt::mp("nonce"), pmt::PMT_NIL);
      size_t nonce_size = pmt::blob_length(nonce_pmt);
      const unsigned char *raw_nonce =
        reinterpret_cast<const unsigned char*>(pmt::blob_data(nonce_pmt));
      std::vector<unsigned char> nonce_char(nonce_size);
      std::memcpy(nonce_char.data(), raw_nonce, nonce_size);

      // … perform decryption into a std::vector<unsigned char> …
      // std::vector<unsigned char> decrypted(data_char.size() - MAC_BYTES);
      // decrypt_fn(data_char.data(), data_char.size(),
      //            nonce_char.data(), nonce_char.size(),
      //            decrypted.data());

      // CHANGE: prepare output via std::vector
      std::vector<unsigned char> decrypted;  // fill this with your decrypted bytes

      // Build output blob and publish
      pmt::pmt_t out_blob = pmt::make_blob(decrypted.data(), decrypted.size());
      message_port_pub(d_port_id_out, out_blob);
    }

    // Streaming worker (unused for a pure message block)
    int
    decrypt_public_impl::work(int noutput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items)
    {
      return 0;
    }

  } /* namespace nacl */
} /* namespace gr */
