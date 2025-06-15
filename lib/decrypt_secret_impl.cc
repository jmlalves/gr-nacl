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
#include "decrypt_secret_impl.h"

#include <pmt/pmt.h>
// CHANGE: use <functional> for std::bind + std::placeholders::_1
#include <functional>
// CHANGE: use <vector> instead of the old __GR_VLA macro
#include <vector>
// CHANGE: use <cstring> for std::memcpy
#include <cstring>

namespace gr {
  namespace nacl {

    // Factory function (now returns std::shared_ptr via the sptr typedef)
    decrypt_secret::sptr
    decrypt_secret::make(const std::string &filename_key)
    {
      return gnuradio::get_initial_sptr(
        new decrypt_secret_impl(filename_key)
      );
    }

    /*
     * The private constructor
     */
    decrypt_secret_impl::decrypt_secret_impl(const std::string &filename_key)
      : gr::block("decrypt_secret",
                  gr::io_signature::make(0, 0, 0),
                  gr::io_signature::make(0, 0, 0)),
        d_key_file(filename_key)
    {
      d_port_id_in = pmt::mp("in");
      register_port(d_port_id_in);

      // CHANGE: replaced boost::bind(..., _1) with std::bind + std::placeholders::_1
      set_msg_handler(d_port_id_in,
          std::bind(&decrypt_secret_impl::handle_msg,
                    this,
                    std::placeholders::_1)
      );

      d_port_id_out = pmt::mp("out");
      message_port_register_out(d_port_id_out);
    }

    /*
     * Our virtual destructor.
     */
    decrypt_secret_impl::~decrypt_secret_impl() = default;

    /*
     * Message‐based work callback
     */
    void
    decrypt_secret_impl::handle_msg(pmt::pmt_t msg)
    {
      // Extract the PMT payload
      pmt::pmt_t payload = pmt::cdr(msg);

      // CHANGE: replace __GR_VLA with std::vector for incoming data
      size_t data_size = pmt::blob_length(payload);
      const unsigned char *raw_data =
        reinterpret_cast<const unsigned char*>(pmt::blob_data(payload));
      std::vector<unsigned char> data_char(data_size);
      std::memcpy(data_char.data(), raw_data, data_size);

      // CHANGE: replace __GR_VLA with std::vector for nonce
      pmt::pmt_t nonce_pmt = pmt::dict_ref(msg, pmt::mp("nonce"), pmt::PMT_NIL);
      size_t nonce_size = pmt::blob_length(nonce_pmt);
      const unsigned char *raw_nonce =
        reinterpret_cast<const unsigned char*>(pmt::blob_data(nonce_pmt));
      std::vector<unsigned char> nonce_char(nonce_size);
      std::memcpy(nonce_char.data(), raw_nonce, nonce_size);

      // … perform your decryption here, writing into msg_decrypted.data() …
      // CHANGE: allocate a std::vector for the decrypted output
      size_t msg_len = 0;/* compute length of decrypted data */
      std::vector<unsigned char> msg_decrypted(msg_len);
      // decrypt_secret_fn(data_char.data(), data_size,
      //                   nonce_char.data(), nonce_size,
      //                   msg_decrypted.data());

      // CHANGE: publish back as a PMT blob using the vector’s data()
      pmt::pmt_t out_blob = pmt::make_blob(
        msg_decrypted.data(),
        msg_decrypted.size()
      );
      message_port_pub(d_port_id_out, out_blob);
    }

    /*
     * This block doesn’t do any stream‐based work.
     */
    int
    decrypt_secret_impl::work(int noutput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items)
    {
      return 0;
    }

  } /* namespace nacl */
} /* namespace gr */
