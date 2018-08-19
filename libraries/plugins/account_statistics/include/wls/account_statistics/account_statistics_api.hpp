#pragma once

#include <wls/account_statistics/account_statistics_plugin.hpp>

#include <fc/api.hpp>

namespace wls{ namespace app {
   struct api_context;
} }

namespace wls { namespace account_statistics {

namespace detail
{
   class account_statistics_api_impl;
}

class account_statistics_api
{
   public:
      account_statistics_api( const wls::app::api_context& ctx );

      void on_api_startup();

   private:
      std::shared_ptr< detail::account_statistics_api_impl > _my;
};

} } // wls::account_statistics

FC_API( wls::account_statistics::account_statistics_api, )