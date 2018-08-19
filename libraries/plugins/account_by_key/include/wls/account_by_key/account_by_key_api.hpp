#pragma once

#include <wls/app/application.hpp>

#include <wls/account_by_key/account_by_key_objects.hpp>

#include <fc/api.hpp>

namespace wls { namespace account_by_key {

namespace detail
{
   class account_by_key_api_impl;
}

class account_by_key_api
{
   public:
      account_by_key_api( const app::api_context& ctx );

      void on_api_startup();

      vector< vector< account_name_type > > get_key_references( vector< public_key_type > keys )const;

   private:
      std::shared_ptr< detail::account_by_key_api_impl > my;
};

} } // wls::account_by_key

FC_API( wls::account_by_key::account_by_key_api, (get_key_references) )
