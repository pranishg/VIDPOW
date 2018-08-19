
#include <wls/app/api_context.hpp>
#include <wls/app/application.hpp>

#include <wls/plugins/changelog/changelog_api.hpp>
#include <wls/plugins/changelog/changelog_plugin.hpp>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace rocksdb;

namespace wls {
    namespace plugin {
        namespace changelog {

            namespace detail {

                class changelog_api_impl {
                public:
                    changelog_api_impl(wls::app::application &_app);
                    std::shared_ptr<wls::plugin::changelog::changelog_plugin> get_plugin();
                    wls::app::application &app;
                };

                changelog_api_impl::changelog_api_impl(wls::app::application &_app) : app(_app) {}

                std::shared_ptr<wls::plugin::changelog::changelog_plugin> changelog_api_impl::get_plugin() {
                   return app.get_plugin<changelog_plugin>("changelog");
                }

            } // detail

            changelog_api::changelog_api(const wls::app::api_context &ctx) {
               my = std::make_shared<detail::changelog_api_impl>(ctx.app);
            }

            get_changelog_result changelog_api::get_changelog(uint32_t block_num) {
               get_changelog_result result;

               result.block_num = block_num;
               my->get_plugin()->get_changelog_db()->Get(ReadOptions(), std::to_string(block_num), &result.data);

               return result;
            }

            void changelog_api::on_api_startup() {}

        }
    }
} // wls::plugin::changelog
