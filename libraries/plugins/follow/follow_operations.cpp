#include <wls/follow/follow_operations.hpp>

#include <wls/protocol/operation_util_impl.hpp>

namespace wls { namespace follow {

void follow_operation::validate()const
{
   FC_ASSERT( follower != following, "You cannot follow yourself" );
}

void reblog_operation::validate()const
{
   FC_ASSERT( account != author, "You cannot reblog your own content" );
}

} } //wls::follow

DEFINE_OPERATION_TYPE( wls::follow::follow_plugin_operation )
