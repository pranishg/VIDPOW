#pragma once

#include <wls/protocol/operations.hpp>
#include <wls/chain/wls_object_types.hpp>

namespace wls { namespace app {

struct applied_operation
{
   applied_operation();
   applied_operation( const wls::chain::operation_object& op_obj );

   wls::protocol::transaction_id_type trx_id;
   uint32_t                               block = 0;
   uint32_t                               trx_in_block = 0;
   uint16_t                               op_in_trx = 0;
   uint64_t                               virtual_op = 0;
   fc::time_point_sec                     timestamp;
   wls::protocol::operation           op;
};

} }

FC_REFLECT( wls::app::applied_operation,
   (trx_id)
   (block)
   (trx_in_block)
   (op_in_trx)
   (virtual_op)
   (timestamp)
   (op)
)
