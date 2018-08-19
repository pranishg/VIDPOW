
#pragma once

#include <wls/app/plugin.hpp>
#include <boost/any.hpp>

namespace wls {
    namespace plugin {
        namespace changelog {

            namespace detail { class changelog_impl; }

            typedef std::vector<std::string> changelog_block;

            using app::application;

            class changelog_plugin : public wls::app::plugin {
                std::shared_ptr<detail::changelog_impl> my;
            public:
                /**
                * The plugin requires a constructor which takes app.  This is called regardless of whether the plugin is loaded.
                * The app parameter should be passed up to the superclass constructor.
                */
                changelog_plugin(application *app);

                /**
                * Plugin is destroyed via base class pointer, so a virtual destructor must be provided.
                */
                virtual ~changelog_plugin();

                /**
                * Every plugin needs a name.
                */
                virtual std::string plugin_name() const override;

                /**
                * Called when the plugin is enabled, but before the database has been created.
                */
                virtual void plugin_initialize(const boost::program_options::variables_map &options) override;

                /**
                * Called when the plugin is enabled.
                */
                virtual void plugin_startup() override;

                /**
                * Called when the plugin is shutdown.
                */
                virtual void plugin_shutdown() override;

                ////////////////////////////////////////////////////////////////////////////////
                DB *get_changelog_db();

            private:
            };

        }
    }
}