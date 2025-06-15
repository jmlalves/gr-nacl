#ifndef INCLUDED_NACL_CRYPT_TAGGED_STREAM_IMPL_H
#define INCLUDED_NACL_CRYPT_TAGGED_STREAM_IMPL_H

#include <nacl/crypt_tagged_stream.h>
#include <pmt/pmt.h>

namespace gr {
  namespace nacl {

    class crypt_tagged_stream_impl : public crypt_tagged_stream
    {
     private:
      std::string d_key;
      std::string d_nonce;
      bool        d_rotate_nonce;
      pmt::pmt_t  d_port_id_in;
      pmt::pmt_t  d_port_id_out;

     public:
      /**
       * Constructor
       * @param key the encryption key
       * @param nonce the nonce to use
       * @param rotate_nonce whether to rotate nonce per-packet
       * @param len_key the name of the length tag key
       */
      crypt_tagged_stream_impl(const std::string &key,
                               const std::string &nonce,
                               bool               rotate_nonce = false,
                               const std::string &len_key       = "packet_len");

      ~crypt_tagged_stream_impl();

      /**
       * PMT message handler
       * Called when a PMT blob message arrives on the "in" port
       */
      void handle_msg(pmt::pmt_t msg);

      /**
       * Calculate how many output items are produced for given input counts
       * @param ninput_items vector of input stream counts
       * @return number of output items to allocate
       */
      int calculate_output_stream_length(const gr_vector_int &ninput_items) override;

      /**
       * Tagged-stream work function
       * @param noutput_items number of output items to produce
       * @param ninput_items (in/out) vector of available input items
       * @param input_items array of input pointers
       * @param output_items array of output pointers
       * @return number of output items actually produced
       */
      int work(int                        noutput_items,
               gr_vector_int            &ninput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star       &output_items) override;
    };

  } // namespace nacl
} // namespace gr

#endif /* INCLUDED_NACL_CRYPT_TAGGED_STREAM_IMPL_H */
