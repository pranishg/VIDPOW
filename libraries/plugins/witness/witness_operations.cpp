#include <wls/witness/witness_operations.hpp>

#include <wls/protocol/operation_util_impl.hpp>

namespace wls { namespace witness {

void enable_content_editing_operation::validate()const
{
   chain::validate_account_name( account );
}

} } // wls::witness

DEFINE_OPERATION_TYPE( wls::witness::witness_plugin_operation )
