#pragma once

#include <wls/chain/evaluator.hpp>

#include <wls/private_message/private_message_operations.hpp>
#include <wls/private_message/private_message_plugin.hpp>

namespace wls { namespace private_message {

DEFINE_PLUGIN_EVALUATOR( private_message_plugin, wls::private_message::private_message_plugin_operation, private_message )

} }
