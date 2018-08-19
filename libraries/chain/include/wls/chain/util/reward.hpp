#pragma once

#include <wls/chain/util/asset.hpp>
#include <wls/chain/wls_objects.hpp>

#include <wls/protocol/asset.hpp>
#include <wls/protocol/config.hpp>
#include <wls/protocol/types.hpp>

#include <fc/reflect/reflect.hpp>

#include <fc/uint128.hpp>

namespace wls { namespace chain { namespace util {

using wls::protocol::asset;
using wls::protocol::price;
using wls::protocol::share_type;

using fc::uint128_t;

struct comment_reward_context
{
   share_type rshares;
   uint16_t   reward_weight = 0;
   asset      max_payout;
   uint128_t  total_reward_shares2;
   asset      total_reward_fund_steem;
   curve_id   reward_curve = quadratic;
   uint128_t  content_constant = WLS_CONTENT_CONSTANT;
};

uint64_t get_rshare_reward( const comment_reward_context& ctx );

inline uint128_t get_content_constant_s()
{
   return WLS_CONTENT_CONSTANT; // looking good for posters
}

uint128_t evaluate_reward_curve( const uint128_t& rshares, const curve_id& curve = quadratic, const uint128_t& content_constant = WLS_CONTENT_CONSTANT );


} } } // wls::chain::util

FC_REFLECT( wls::chain::util::comment_reward_context,
   (rshares)
   (reward_weight)
   (max_payout)
   (total_reward_shares2)
   (total_reward_fund_steem)
   (reward_curve)
   (content_constant)
   )
