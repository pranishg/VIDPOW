#pragma once
#include <fc/fixed_string.hpp>

#include <wls/protocol/authority.hpp>
#include <wls/protocol/wls_operations.hpp>

#include <wls/chain/wls_object_types.hpp>
#include <wls/chain/witness_objects.hpp>
#include <wls/chain/shared_authority.hpp>

#include <boost/multi_index/composite_key.hpp>

#include <numeric>

namespace wls { namespace chain {

   using wls::protocol::authority;

   class account_object : public object< account_object_type, account_object >
   {
      account_object() = delete;

      public:
         template<typename Constructor, typename Allocator>
         account_object( Constructor&& c, allocator< Allocator > a )
            :json_metadata( a )
         {
            c(*this);
         };

         id_type           id;

         account_name_type name;
         public_key_type   memo_key;
         shared_string     json_metadata;
         account_name_type proxy;

         time_point_sec    last_account_update;

         time_point_sec    created;
         account_name_type recovery_account; // WLS_NULL_ACCOUNT
         uint32_t          comment_count = 0;
         uint32_t          lifetime_vote_count = 0;
         uint32_t          post_count = 0;

         uint16_t          voting_power = WLS_100_PERCENT;   ///< current voting power of this account, it falls after every vote
         time_point_sec    last_vote_time; ///< used to increase the voting power of this account the longer it goes without voting.

         asset             balance = asset( 0, WLS_SYMBOL );  ///< total liquid shares held by this account
         asset             reward_steem_balance = asset( 0, WLS_SYMBOL );
         asset             reward_vesting_balance = asset( 0, VESTS_SYMBOL );
         asset             reward_vesting_steem = asset( 0, WLS_SYMBOL );

         share_type        curation_rewards = 0;
         share_type        posting_rewards = 0;

         asset             vesting_shares = asset( 0, VESTS_SYMBOL ); ///< total vesting shares held by this account, controls its voting power

         asset             vesting_withdraw_rate = asset( 0, VESTS_SYMBOL ); ///< at the time this is updated it can be at most vesting_shares/104
         time_point_sec    next_vesting_withdrawal = fc::time_point_sec::maximum(); ///< after every withdrawal this is incremented by 1 week
         share_type        withdrawn = 0; /// Track how many shares have been withdrawn
         share_type        to_withdraw = 0; /// Might be able to look this up with operation history.
         uint16_t          withdraw_routes = 0;

         fc::array<share_type, WLS_MAX_PROXY_RECURSION_DEPTH> proxied_vsf_votes;// = std::vector<share_type>( WLS_MAX_PROXY_RECURSION_DEPTH, 0 ); ///< the total VFS votes proxied to this account

         uint16_t          witnesses_voted_for = 0;

         time_point_sec    last_post;
         time_point_sec    last_root_post = fc::time_point_sec::min();
         uint32_t          post_bandwidth = 0;


         /// Account selling/buying feature
         bool              asb_for_sale = false;
         account_name_type asb_to; // account receives payment
         asset             asb_price = asset( 0, WLS_SYMBOL );


         /// This function should be used only when the account votes for a witness directly
         share_type        witness_vote_weight()const {
            return std::accumulate( proxied_vsf_votes.begin(),
                                    proxied_vsf_votes.end(),
                                    vesting_shares.amount );
         }
         share_type        proxied_vsf_votes_total()const {
            return std::accumulate( proxied_vsf_votes.begin(),
                                    proxied_vsf_votes.end(),
                                    share_type() );
         }
   };

   class account_authority_object : public object< account_authority_object_type, account_authority_object >
   {
      account_authority_object() = delete;

      public:
         template< typename Constructor, typename Allocator >
         account_authority_object( Constructor&& c, allocator< Allocator > a )
            : owner( a ), active( a ), posting( a )
         {
            c( *this );
         }

         id_type           id;

         account_name_type account;

         shared_authority  owner;   ///< used for backup control, can set owner or active
         shared_authority  active;  ///< used for all monetary operations, can set active or posting
         shared_authority  posting; ///< used for voting and posting

         time_point_sec    last_owner_update;
   };


   struct by_name;
   struct by_proxy;
   struct by_last_post;
   struct by_next_vesting_withdrawal;
   struct by_steem_balance;
   struct by_smp_balance;
   struct by_smd_balance;
   struct by_post_count;
   struct by_vote_count;

   /**
    * @ingroup object_index
    */
   typedef multi_index_container<
      account_object,
      indexed_by<
         ordered_unique< tag< by_id >,
            member< account_object, account_id_type, &account_object::id > >,
         ordered_unique< tag< by_name >,
            member< account_object, account_name_type, &account_object::name > >,
         ordered_unique< tag< by_proxy >,
            composite_key< account_object,
               member< account_object, account_name_type, &account_object::proxy >,
               member< account_object, account_id_type, &account_object::id >
            > /// composite key by proxy
         >,
         ordered_unique< tag< by_next_vesting_withdrawal >,
            composite_key< account_object,
               member< account_object, time_point_sec, &account_object::next_vesting_withdrawal >,
               member< account_object, account_id_type, &account_object::id >
            > /// composite key by_next_vesting_withdrawal
         >,
         ordered_unique< tag< by_last_post >,
            composite_key< account_object,
               member< account_object, time_point_sec, &account_object::last_post >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< time_point_sec >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_steem_balance >,
            composite_key< account_object,
               member< account_object, asset, &account_object::balance >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< asset >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_smp_balance >,
            composite_key< account_object,
               member< account_object, asset, &account_object::vesting_shares >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< asset >, std::less< account_id_type > >
         >,
//         ordered_unique< tag< by_smd_balance >,
//            composite_key< account_object,
//               member< account_object, asset, &account_object::sbd_balance >,
//               member< account_object, account_id_type, &account_object::id >
//            >,
//            composite_key_compare< std::greater< asset >, std::less< account_id_type > >
//         >,
         ordered_unique< tag< by_post_count >,
            composite_key< account_object,
               member< account_object, uint32_t, &account_object::post_count >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< uint32_t >, std::less< account_id_type > >
         >,
         ordered_unique< tag< by_vote_count >,
            composite_key< account_object,
               member< account_object, uint32_t, &account_object::lifetime_vote_count >,
               member< account_object, account_id_type, &account_object::id >
            >,
            composite_key_compare< std::greater< uint32_t >, std::less< account_id_type > >
         >
      >,
      allocator< account_object >
   > account_index;


   struct by_account;
   struct by_last_valid;
   struct by_last_owner_update;

   typedef multi_index_container <
      account_authority_object,
      indexed_by <
         ordered_unique< tag< by_id >,
            member< account_authority_object, account_authority_id_type, &account_authority_object::id > >,
         ordered_unique< tag< by_account >,
            composite_key< account_authority_object,
               member< account_authority_object, account_name_type, &account_authority_object::account >,
               member< account_authority_object, account_authority_id_type, &account_authority_object::id >
            >,
            composite_key_compare< std::less< account_name_type >, std::less< account_authority_id_type > >
         >,
         ordered_unique< tag< by_last_owner_update >,
            composite_key< account_authority_object,
               member< account_authority_object, time_point_sec, &account_authority_object::last_owner_update >,
               member< account_authority_object, account_authority_id_type, &account_authority_object::id >
            >,
            composite_key_compare< std::greater< time_point_sec >, std::less< account_authority_id_type > >
         >
      >,
      allocator< account_authority_object >
   > account_authority_index;

} }

FC_REFLECT( wls::chain::account_object,
             (id)(name)(memo_key)(json_metadata)(proxy)(last_account_update)
             (created)
             (recovery_account)
             (comment_count)(lifetime_vote_count)(post_count)(voting_power)(last_vote_time)
             (balance)
             (reward_steem_balance)(reward_vesting_balance)(reward_vesting_steem)
             (vesting_shares)
             (vesting_withdraw_rate)(next_vesting_withdrawal)(withdrawn)(to_withdraw)(withdraw_routes)
             (curation_rewards)
             (posting_rewards)
             (proxied_vsf_votes)(witnesses_voted_for)
             (last_post)(last_root_post)(post_bandwidth)
             (asb_for_sale)(asb_to)(asb_price)
          )
CHAINBASE_SET_INDEX_TYPE( wls::chain::account_object, wls::chain::account_index )

FC_REFLECT( wls::chain::account_authority_object,
             (id)(account)(owner)(active)(posting)(last_owner_update)
)
CHAINBASE_SET_INDEX_TYPE( wls::chain::account_authority_object, wls::chain::account_authority_index )
