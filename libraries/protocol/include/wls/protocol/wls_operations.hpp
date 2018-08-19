#pragma once
#include <wls/protocol/base.hpp>
#include <wls/protocol/block_header.hpp>
#include <wls/protocol/asset.hpp>

#include <fc/utf8.hpp>
#include <fc/crypto/equihash.hpp>

namespace wls { namespace protocol {

   inline void validate_account_name( const string& name )
   {
      FC_ASSERT( is_valid_account_name( name ), "Account name ${n} is invalid", ("n", name) );
   }

   inline void validate_permlink( const string& permlink )
   {
      FC_ASSERT( permlink.size() < WLS_MAX_PERMLINK_LENGTH, "permlink is too long" );
      FC_ASSERT( fc::is_utf8( permlink ), "permlink not formatted in UTF8" );
   }

   struct account_create_operation : public base_operation
   {
      asset             fee;
      account_name_type creator;
      account_name_type new_account_name;
      authority         owner;
      authority         active;
      authority         posting;
      public_key_type   memo_key;
      string            json_metadata;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(creator); }
   };

   struct account_update_operation : public base_operation
   {
      account_name_type             account;
      optional< authority >         owner;
      optional< authority >         active;
      optional< authority >         posting;
      public_key_type               memo_key;
      string                        json_metadata;

      void validate()const;

      void get_required_owner_authorities( flat_set<account_name_type>& a )const
      { if( owner ) a.insert( account ); }

      void get_required_active_authorities( flat_set<account_name_type>& a )const
      { if( !owner ) a.insert( account ); }
   };

   struct account_forsale_operation : public base_operation
   {
      account_name_type             account;
      bool                          for_sale = false;
      account_name_type             to; // account receives payment
      asset                         price = asset( 0, WLS_SYMBOL );

      void validate()const;
      void get_required_owner_authorities( flat_set<account_name_type>& a )const{ a.insert( account ); }
   };

   struct account_buying_operation : public base_operation
   {
      account_name_type             account;                         // buyer
      account_name_type             account_buy;                     // which account being bought
      asset                         price = asset( 0, WLS_SYMBOL );  // https://gitlab.com/beyondbitcoin/whaleshares-chain/issues/8

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert( account ); }
   };

   struct comment_operation : public base_operation
   {
      account_name_type parent_author;
      string            parent_permlink;

      account_name_type author;
      string            permlink;

      string            title;
      string            body;
      string            json_metadata;

      void validate()const;
      void get_required_posting_authorities( flat_set<account_name_type>& a )const{ a.insert(author); }
   };

   struct beneficiary_route_type
   {
      beneficiary_route_type() {}
      beneficiary_route_type( const account_name_type& a, const uint16_t& w ) : account( a ), weight( w ){}

      account_name_type account;
      uint16_t          weight;

      // For use by std::sort such that the route is sorted first by name (ascending)
      bool operator < ( const beneficiary_route_type& o )const { return account < o.account; }
   };

   struct comment_payout_beneficiaries
   {
      vector< beneficiary_route_type > beneficiaries;

      void validate()const;
   };

   typedef static_variant<
            comment_payout_beneficiaries
           > comment_options_extension;

   typedef flat_set< comment_options_extension > comment_options_extensions_type;

   /**
    *  Authors of posts may not want all of the benefits that come from creating a post. This
    *  operation allows authors to update properties associated with their post.
    *
    *  The max_accepted_payout may be decreased, but never increased.
    */
   struct comment_options_operation : public base_operation
   {
      account_name_type author;
      string            permlink;

      asset             max_accepted_payout    = asset( 1000000000, WLS_SYMBOL );       /// WLS_SYMBOL value of the maximum payout this post will receive
      bool              allow_votes            = true;      /// allows a post to receive votes;
      bool              allow_curation_rewards = true; /// allows voters to recieve curation rewards. Rewards return to reward fund.
      comment_options_extensions_type extensions;

      void validate()const;
      void get_required_posting_authorities( flat_set<account_name_type>& a )const{ a.insert(author); }
   };

   struct delete_comment_operation : public base_operation
   {
      account_name_type author;
      string            permlink;

      void validate()const;
      void get_required_posting_authorities( flat_set<account_name_type>& a )const{ a.insert(author); }
   };


   struct vote_operation : public base_operation
   {
      account_name_type    voter;
      account_name_type    author;
      string               permlink;
      int16_t              weight = 0;

      void validate()const;
      void get_required_posting_authorities( flat_set<account_name_type>& a )const{ a.insert(voter); }
   };


   /**
    * @ingroup operations
    *
    * @brief Transfers STEEM from one account to another.
    */
   struct transfer_operation : public base_operation
   {
      account_name_type from;
      /// Account to transfer asset to
      account_name_type to;
      /// The amount of asset to transfer from @ref from to @ref to
      asset             amount;

      /// The memo is plain-text, any encryption on the memo is up to
      /// a higher level protocol.
      string            memo;

      void              validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ if(amount.symbol != VESTS_SYMBOL) a.insert(from); }
      void get_required_owner_authorities( flat_set<account_name_type>& a )const { if(amount.symbol == VESTS_SYMBOL) a.insert(from); }
   };


   /**
    *  This operation converts WLS into VFS (Vesting Fund Shares) at
    *  the current exchange rate. With this operation it is possible to
    *  give another account vesting shares so that faucets can
    *  pre-fund new accounts with vesting shares.
    */
   struct transfer_to_vesting_operation : public base_operation
   {
      account_name_type from;
      account_name_type to; ///< if null, then same as from
      asset             amount; ///< must be WLS

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(from); }
   };


   /**
    * At any given point in time an account can be withdrawing from their
    * vesting shares. A user may change the number of shares they wish to
    * cash out at any time between 0 and their total vesting stake.
    *
    * After applying this operation, vesting_shares will be withdrawn
    * at a rate of vesting_shares/104 per week for two years starting
    * one week after this operation is included in the blockchain.
    *
    * This operation is not valid if the user has no vesting shares.
    */
   struct withdraw_vesting_operation : public base_operation
   {
      account_name_type account;
      asset             vesting_shares;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(account); }
   };


   /**
    * Allows an account to setup a vesting withdraw but with the additional
    * request for the funds to be transferred directly to another account's
    * balance rather than the withdrawing account. In addition, those funds
    * can be immediately vested again, circumventing the conversion from
    * vests to steem and back, guaranteeing they maintain their value.
    */
   struct set_withdraw_vesting_route_operation : public base_operation
   {
      account_name_type from_account;
      account_name_type to_account;
      uint16_t          percent = 0;
      bool              auto_vest = false;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const { a.insert( from_account ); }
   };


   /**
    * Witnesses must vote on how to set certain chain properties to ensure a smooth
    * and well functioning network.  Any time @owner is in the active set of witnesses these
    * properties will be used to control the blockchain configuration.
    */
   struct chain_properties
   {
      /**
       *  This fee, paid in WLS, is converted into VESTING SHARES for the new account. Accounts
       *  without vesting shares cannot earn usage rations and therefore are powerless. This minimum
       *  fee requires all accounts to have some kind of commitment to the network that includes the
       *  ability to vote and make transactions.
       */
      asset             account_creation_fee = asset( WLS_MIN_ACCOUNT_CREATION_FEE, WLS_SYMBOL );

      /**
       *  This witnesses vote for the maximum_block_size which is used by the network
       *  to tune rate limiting and capacity
       */
      uint32_t          maximum_block_size = WLS_MIN_BLOCK_SIZE_LIMIT * 2;

      void validate()const
      {
         FC_ASSERT( account_creation_fee.amount >= WLS_MIN_ACCOUNT_CREATION_FEE);
         FC_ASSERT( maximum_block_size >= WLS_MIN_BLOCK_SIZE_LIMIT);
      }
   };


   /**
    *  Users who wish to become a witness must pay a fee acceptable to
    *  the current witnesses to apply for the position and allow voting
    *  to begin.
    *
    *  If the owner isn't a witness they will become a witness.  Witnesses
    *  are charged a fee equal to 1 weeks worth of witness pay which in
    *  turn is derived from the current share supply.  The fee is
    *  only applied if the owner is not already a witness.
    *
    *  If the block_signing_key is null then the witness is removed from
    *  contention.  The network will pick the top 21 witnesses for
    *  producing blocks.
    */
   struct witness_update_operation : public base_operation
   {
      account_name_type owner;
      string            url;
      public_key_type   block_signing_key;
      chain_properties  props;
      asset             fee; ///< the fee paid to register a new witness, should be 10x current block production pay

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(owner); }
   };


   /**
    * All accounts with a VFS can vote for or against any witness.
    *
    * If a proxy is specified then all existing votes are removed.
    */
   struct account_witness_vote_operation : public base_operation
   {
      account_name_type account;
      account_name_type witness;
      bool              approve = true;

      void validate() const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(account); }
   };


   struct account_witness_proxy_operation : public base_operation
   {
      account_name_type account;
      account_name_type proxy;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(account); }
   };


   /**
    * @brief provides a generic way to add higher level protocols on top of witness consensus
    * @ingroup operations
    *
    * There is no validation for this operation other than that required auths are valid
    */
   struct custom_operation : public base_operation
   {
      flat_set< account_name_type > required_auths;
      uint16_t                      id = 0;
      vector< char >                data;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ for( const auto& i : required_auths ) a.insert(i); }
   };


   /** serves the same purpose as custom_operation but also supports required posting authorities. Unlike custom_operation,
    * this operation is designed to be human readable/developer friendly.
    **/
   struct custom_json_operation : public base_operation
   {
      flat_set< account_name_type > required_auths;
      flat_set< account_name_type > required_posting_auths;
      string                        id; ///< must be less than 32 characters long
      string                        json; ///< must be proper utf8 / JSON string.

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ for( const auto& i : required_auths ) a.insert(i); }
      void get_required_posting_authorities( flat_set<account_name_type>& a )const{ for( const auto& i : required_posting_auths ) a.insert(i); }
   };


   struct custom_binary_operation : public base_operation
   {
      flat_set< account_name_type > required_owner_auths;
      flat_set< account_name_type > required_active_auths;
      flat_set< account_name_type > required_posting_auths;
      vector< authority >           required_auths;

      string                        id; ///< must be less than 32 characters long
      vector< char >                data;

      void validate()const;
      void get_required_owner_authorities( flat_set<account_name_type>& a )const{ for( const auto& i : required_owner_auths ) a.insert(i); }
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ for( const auto& i : required_active_auths ) a.insert(i); }
      void get_required_posting_authorities( flat_set<account_name_type>& a )const{ for( const auto& i : required_posting_auths ) a.insert(i); }
      void get_required_authorities( vector< authority >& a )const{ for( const auto& i : required_auths ) a.push_back( i ); }
   };

   struct claim_reward_balance_operation : public base_operation
   {
      account_name_type account;
      asset             reward_steem;
      asset             reward_vests;

      void get_required_posting_authorities( flat_set< account_name_type >& a )const{ a.insert( account ); }
      void validate() const;
   };

} } // wls::protocol


FC_REFLECT( wls::protocol::chain_properties, (account_creation_fee)(maximum_block_size) );

FC_REFLECT( wls::protocol::account_create_operation,
            (fee)
            (creator)
            (new_account_name)
            (owner)
            (active)
            (posting)
            (memo_key)
            (json_metadata) )

FC_REFLECT( wls::protocol::account_update_operation,
            (account)
            (owner)
            (active)
            (posting)
            (memo_key)
            (json_metadata) )

FC_REFLECT( wls::protocol::account_forsale_operation, (account)(for_sale)(to)(price) )
FC_REFLECT( wls::protocol::account_buying_operation, (account)(account_buy)(price) )
FC_REFLECT( wls::protocol::transfer_operation, (from)(to)(amount)(memo) )
FC_REFLECT( wls::protocol::transfer_to_vesting_operation, (from)(to)(amount) )
FC_REFLECT( wls::protocol::withdraw_vesting_operation, (account)(vesting_shares) )
FC_REFLECT( wls::protocol::set_withdraw_vesting_route_operation, (from_account)(to_account)(percent)(auto_vest) )
FC_REFLECT( wls::protocol::witness_update_operation, (owner)(url)(block_signing_key)(props)(fee) )
FC_REFLECT( wls::protocol::account_witness_vote_operation, (account)(witness)(approve) )
FC_REFLECT( wls::protocol::account_witness_proxy_operation, (account)(proxy) )
FC_REFLECT( wls::protocol::comment_operation, (parent_author)(parent_permlink)(author)(permlink)(title)(body)(json_metadata) )
FC_REFLECT( wls::protocol::vote_operation, (voter)(author)(permlink)(weight) )
FC_REFLECT( wls::protocol::custom_operation, (required_auths)(id)(data) )
FC_REFLECT( wls::protocol::custom_json_operation, (required_auths)(required_posting_auths)(id)(json) )
FC_REFLECT( wls::protocol::custom_binary_operation, (required_owner_auths)(required_active_auths)(required_posting_auths)(required_auths)(id)(data) )
FC_REFLECT( wls::protocol::delete_comment_operation, (author)(permlink) );
FC_REFLECT( wls::protocol::beneficiary_route_type, (account)(weight) )
FC_REFLECT( wls::protocol::comment_payout_beneficiaries, (beneficiaries) )

FC_REFLECT_TYPENAME( wls::protocol::comment_options_extension )

FC_REFLECT( wls::protocol::comment_options_operation, (author)(permlink)(max_accepted_payout)(allow_votes)(allow_curation_rewards)(extensions) )
FC_REFLECT( wls::protocol::claim_reward_balance_operation, (account)(reward_steem)(reward_vests) )
