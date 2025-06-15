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
#include "generate_keypair_impl.h"

#include <pmt/pmt.h>
// CHANGE: include <functional> to use std::bind + std::placeholders::_1
#include <functional>
// CHANGE: include <vector> to collect keypair bytes
#include <vector>
// CHANGE: include <cstring> to use std::memcpy when packaging blobs
#include <cstring>

namespace gr {
  namespace nacl {

    // Factory function (now returns std::shared_ptr via the sptr typedef)
    generate_keypair::sptr
    generate_keypair::make(const std::string &filename_sk,
                           const std::string &filename_pk)
    {
      return gnuradio::get_initial_sptr(
        new generate_keypair_impl(filename_sk, filename_pk)
      );
    }

    /*
     * The private constructor
     */
    generate_keypair_impl::generate_keypair_impl(const std::string &filename_sk,
                                                 const std::string &filename_pk)
      : gr::block("generate_keypair",
                  gr::io_signature::make(0, 0, 0),
                  gr::io_signature::make(0, 0, 0)),
        d_sk_file(filename_sk),
        d_pk_file(filename_pk)
    {
      // Register message ports
      d_port_id_in  = pmt::mp("in");
      register_port(d_port_id_in);

      // CHANGE: use std::bind + std::placeholders::_1 instead of boost::bind
      set_msg_handler(d_port_id_in,
          std::bind(&generate_keypair_impl::handle_msg,
                    this,
                    std::placeholders::_1)
      );

      d_port_id_sk = pmt::mp("sk_out");
      message_port_register_out(d_port_id_sk);

      d_port_id_pk = pmt::mp("pk_out");
      message_port_register_out(d_port_id_pk);
    }

    /*
     * Our virtual destructor.
     */
    generate_keypair_impl::~generate_keypair_impl() = default;

    /*
     * Message handler: generates a keypair and sends out two blobs
     */
    void
    generate_keypair_impl::handle_msg(pmt::pmt_t msg)
    {
      // Optionally inspect msg for parameters…

      // CHANGE: define lengths for secret and public keys
      constexpr size_t SK_LEN = 0;/* your secret-key length */
      constexpr size_t PK_LEN = 0;/* your public-key length */

      // CHANGE: generate keypair bytes into std::vector containers
      std::vector<unsigned char> sk_bytes(SK_LEN);
      std::vector<unsigned char> pk_bytes(PK_LEN);

      // TODO: fill sk_bytes and pk_bytes, e.g. via libsodium:
      crypto_box_keypair(pk_bytes.data(), sk_bytes.data());

      // CHANGE: package each vector into a PMT blob and publish
      pmt::pmt_t sk_blob = pmt::make_blob(sk_bytes.data(), sk_bytes.size());
      pmt::pmt_t pk_blob = pmt::make_blob(pk_bytes.data(), pk_bytes.size());

      message_port_pub(d_port_id_sk, sk_blob);
      message_port_pub(d_port_id_pk, pk_blob);
    }

    /*
     * No stream‐based work for this block.
     */
    int
    generate_keypair_impl::work(int /*noutput_items*/,
                                gr_vector_const_void_star &,
                                gr_vector_void_star &)
    {
      return 0;  // all done via message handler
    }

  } /* namespace nacl */
} /* namespace gr */
