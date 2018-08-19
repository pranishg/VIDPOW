
#pragma once

#include <memory>
#include <string>
#include <vector>

namespace wls { namespace app {

class abstract_plugin;
class application;

} }

namespace wls { namespace plugin {

void initialize_plugin_factories();
std::shared_ptr< steemit::app::abstract_plugin > create_plugin( const std::string& name, steemit::app::application* app );
std::vector< std::string > get_available_plugins();

} }
