#pragma once

#include <wls/protocol/wls_operations.hpp>

#include <wls/chain/evaluator.hpp>

namespace wls{ namespace chain {

using namespace wls::protocol;

DEFINE_EVALUATOR( account_create )
DEFINE_EVALUATOR( account_update )
DEFINE_EVALUATOR( account_forsale )
DEFINE_EVALUATOR( account_buying )
DEFINE_EVALUATOR( transfer )
DEFINE_EVALUATOR( transfer_to_vesting )
DEFINE_EVALUATOR( witness_update )
DEFINE_EVALUATOR( account_witness_vote )
DEFINE_EVALUATOR( account_witness_proxy )
DEFINE_EVALUATOR( withdraw_vesting )
DEFINE_EVALUATOR( set_withdraw_vesting_route )
DEFINE_EVALUATOR( comment )
DEFINE_EVALUATOR( comment_options )
DEFINE_EVALUATOR( delete_comment )
DEFINE_EVALUATOR( vote )
DEFINE_EVALUATOR( custom )
DEFINE_EVALUATOR( custom_json )
DEFINE_EVALUATOR( custom_binary )
DEFINE_EVALUATOR( claim_reward_balance )

} } // wls::chain
