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

// lib/encrypt_secret_impl.cc
#include "encrypt_secret_impl.h"
#include <gnuradio/io_signature.h>
#include <cstring>
#include <functional>

namespace gr {
namespace nacl {

encrypt_secret::sptr
encrypt_secret::make(const std::string &filename_key)
{
    return encrypt_secret::sptr(
        new encrypt_secret_impl(filename_key));
}

encrypt_secret_impl::encrypt_secret_impl(const std::string &filename_key)
    : d_key_file(filename_key)
{
    d_port_id_in = pmt::mp("in");
    message_port_register_in(d_port_id_in);
    set_msg_handler(d_port_id_in,
        std::bind(&encrypt_secret_impl::handle_msg, this, std::placeholders::_1));
    d_port_id_out = pmt::mp("out");
    message_port_register_out(d_port_id_out);
}

encrypt_secret_impl::~encrypt_secret_impl() = default;

void
encrypt_secret_impl::handle_msg(pmt::pmt_t msg)
{
    if (pmt::is_blob(msg)) {
        size_t len = pmt::blob_length(msg);
        const unsigned char *data = static_cast<const unsigned char*>(pmt::blob_data(msg));
        pmt::pmt_t out = pmt::init_u8vector(len, data);
        message_port_pub(d_port_id_out, out);
    }
}

int
encrypt_secret_impl::work(int                       noutput_items,
                         gr_vector_int            &ninput_items,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star      &output_items)
{
    return 0;
}

} // namespace nacl
} // namespace gr
