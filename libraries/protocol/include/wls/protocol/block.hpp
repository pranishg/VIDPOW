#pragma once
#include <wls/protocol/block_header.hpp>
#include <wls/protocol/transaction.hpp>

namespace wls { namespace protocol {

   struct signed_block : public signed_block_header
   {
      checksum_type calculate_merkle_root()const;
      vector<signed_transaction> transactions;
   };

} } // wls::protocol

FC_REFLECT_DERIVED( wls::protocol::signed_block, (wls::protocol::signed_block_header), (transactions) )
