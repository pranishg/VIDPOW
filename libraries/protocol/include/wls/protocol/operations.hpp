#pragma once

#include <wls/protocol/operation_util.hpp>
#include <wls/protocol/wls_operations.hpp>
#include <wls/protocol/wls_virtual_operations.hpp>

namespace wls { namespace protocol {

   /** NOTE: do not change the order of any operations prior to the virtual operations
    * or it will trigger a hardfork.
    */
   typedef fc::static_variant<
            vote_operation,
            comment_operation,

            transfer_operation,
            transfer_to_vesting_operation,
            withdraw_vesting_operation,

            account_create_operation,
            account_update_operation,
            account_forsale_operation,
            account_buying_operation,

            witness_update_operation,
            account_witness_vote_operation,
            account_witness_proxy_operation,

            custom_operation,

            delete_comment_operation,
            custom_json_operation,
            comment_options_operation,
            set_withdraw_vesting_route_operation,
            custom_binary_operation,
            claim_reward_balance_operation,

            /// virtual operations below this point
            author_reward_operation,
            curation_reward_operation,
            comment_reward_operation,
            fill_vesting_withdraw_operation,
            shutdown_witness_operation,
            hardfork_operation,
            comment_payout_update_operation,
            comment_benefactor_reward_operation,
            producer_reward_operation,
            devfund_operation
         > operation;

   /*void operation_get_required_authorities( const operation& op,
                                            flat_set<string>& active,
                                            flat_set<string>& owner,
                                            flat_set<string>& posting,
                                            vector<authority>&  other );

   void operation_validate( const operation& op );*/

   bool is_market_operation( const operation& op );

   bool is_virtual_operation( const operation& op );

} } // wls::protocol

/*namespace fc {
    void to_variant( const wls::protocol::operation& var,  fc::variant& vo );
    void from_variant( const fc::variant& var,  wls::protocol::operation& vo );
}*/

DECLARE_OPERATION_TYPE( wls::protocol::operation )
FC_REFLECT_TYPENAME( wls::protocol::operation )
