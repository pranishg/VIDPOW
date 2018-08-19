
#pragma once

#include <wls/chain/wls_object_types.hpp>

#include <fc/api.hpp>

namespace wls {
    namespace app {
        struct api_context;
    }
}

namespace wls {
    namespace plugin {
        namespace changelog {

            namespace detail {
                class changelog_api_impl;
            }

            struct get_changelog_result {
                uint32_t block_num;
                std::string data;
            };

            class changelog_api {
            public:
                changelog_api(const wls::app::api_context &ctx);
                void on_api_startup();
                get_changelog_result get_changelog(uint32_t block_num);

            private:
                std::shared_ptr<detail::changelog_api_impl> my;
            };

        }
    }
}

FC_REFLECT(wls::plugin::changelog::get_changelog_result, (block_num)(data))
FC_API(wls::plugin::changelog::changelog_api, (get_changelog))
