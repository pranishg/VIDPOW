#pragma once
#include <wls/chain/account_object.hpp>
#include <wls/chain/block_summary_object.hpp>
#include <wls/chain/comment_object.hpp>
#include <wls/chain/global_property_object.hpp>
#include <wls/chain/history_object.hpp>
#include <wls/chain/wls_objects.hpp>
#include <wls/chain/transaction_object.hpp>
#include <wls/chain/witness_objects.hpp>

#include <wls/tags/tags_plugin.hpp>

#include <wls/witness/witness_objects.hpp>

namespace wls { namespace app {

using namespace wls::chain;

typedef chain::block_summary_object                    block_summary_api_obj;
typedef chain::comment_vote_object                     comment_vote_api_obj;
typedef chain::withdraw_vesting_route_object           withdraw_vesting_route_api_obj;
typedef chain::witness_vote_object                     witness_vote_api_obj;
typedef chain::witness_schedule_object                 witness_schedule_api_obj;
typedef chain::reward_fund_object                      reward_fund_api_obj;
typedef witness::account_bandwidth_object              account_bandwidth_api_obj;

struct comment_api_obj
{
   comment_api_obj( const chain::comment_object& o ):
      id( o.id ),
      category( to_string( o.category ) ),
      parent_author( o.parent_author ),
      parent_permlink( to_string( o.parent_permlink ) ),
      author( o.author ),
      permlink( to_string( o.permlink ) ),
      title( to_string( o.title ) ),
      body( to_string( o.body ) ),
      json_metadata( to_string( o.json_metadata ) ),
      last_update( o.last_update ),
      created( o.created ),
      active( o.active ),
      last_payout( o.last_payout ),
      depth( o.depth ),
      children( o.children ),
      net_rshares( o.net_rshares ),
      abs_rshares( o.abs_rshares ),
      vote_rshares( o.vote_rshares ),
      children_abs_rshares( o.children_abs_rshares ),
      cashout_time( o.cashout_time ),
      max_cashout_time( o.max_cashout_time ),
      total_vote_weight( o.total_vote_weight ),
      reward_weight( o.reward_weight ),
      total_payout_value( o.total_payout_value ),
      curator_payout_value( o.curator_payout_value ),
      author_rewards( o.author_rewards ),
      net_votes( o.net_votes ),
      root_comment( o.root_comment ),
      max_accepted_payout( o.max_accepted_payout ),
      allow_replies( o.allow_replies ),
      allow_votes( o.allow_votes ),
      allow_curation_rewards( o.allow_curation_rewards )
   {
      for( auto& route : o.beneficiaries )
      {
         beneficiaries.push_back( route );
      }
   }

   comment_api_obj(){}

   comment_id_type   id;
   string            category;
   account_name_type parent_author;
   string            parent_permlink;
   account_name_type author;
   string            permlink;

   string            title;
   string            body;
   string            json_metadata;
   time_point_sec    last_update;
   time_point_sec    created;
   time_point_sec    active;
   time_point_sec    last_payout;

   uint8_t           depth = 0;
   uint32_t          children = 0;

   share_type        net_rshares;
   share_type        abs_rshares;
   share_type        vote_rshares;

   share_type        children_abs_rshares;
   time_point_sec    cashout_time;
   time_point_sec    max_cashout_time;
   uint64_t          total_vote_weight = 0;

   uint16_t          reward_weight = 0;

   asset             total_payout_value;
   asset             curator_payout_value;

   share_type        author_rewards;

   int32_t           net_votes = 0;

   comment_id_type   root_comment;

   asset             max_accepted_payout;
   bool              allow_replies = false;
   bool              allow_votes = false;
   bool              allow_curation_rewards = false;
   vector< beneficiary_route_type > beneficiaries;
};

struct tag_api_obj
{
   tag_api_obj( const tags::tag_stats_object& o ) :
      name( o.tag ),
      total_payouts(o.total_payout),
      net_votes(o.net_votes),
      top_posts(o.top_posts),
      comments(o.comments),
      trending(o.total_trending) {}

   tag_api_obj() {}

   string               name;
   asset                total_payouts;
   int32_t              net_votes = 0;
   uint32_t             top_posts = 0;
   uint32_t             comments = 0;
   fc::uint128          trending = 0;
};

struct account_api_obj
{
   account_api_obj( const chain::account_object& a, const chain::database& db ) :
      id( a.id ),
      name( a.name ),
      memo_key( a.memo_key ),
      json_metadata( to_string( a.json_metadata ) ),
      proxy( a.proxy ),
      last_account_update( a.last_account_update ),
      created( a.created ),
      recovery_account( a.recovery_account ),
      comment_count( a.comment_count ),
      lifetime_vote_count( a.lifetime_vote_count ),
      post_count( a.post_count ),
      voting_power( a.voting_power ),
      last_vote_time( a.last_vote_time ),
      balance( a.balance ),
      reward_steem_balance( a.reward_steem_balance ),
      reward_vesting_balance( a.reward_vesting_balance ),
      reward_vesting_steem( a.reward_vesting_steem ),
      curation_rewards( a.curation_rewards ),
      posting_rewards( a.posting_rewards ),
      vesting_shares( a.vesting_shares ),
      vesting_withdraw_rate( a.vesting_withdraw_rate ),
      next_vesting_withdrawal( a.next_vesting_withdrawal ),
      withdrawn( a.withdrawn ),
      to_withdraw( a.to_withdraw ),
      withdraw_routes( a.withdraw_routes ),
      witnesses_voted_for( a.witnesses_voted_for ),
      last_post( a.last_post ),
      last_root_post( a.last_root_post ),
      asb_for_sale( a.asb_for_sale ),
      asb_to( a.asb_to ),
      asb_price( a.asb_price )
   {
      size_t n = a.proxied_vsf_votes.size();
      proxied_vsf_votes.reserve( n );
      for( size_t i=0; i<n; i++ )
         proxied_vsf_votes.push_back( a.proxied_vsf_votes[i] );

      const auto& auth = db.get< account_authority_object, by_account >( name );
      owner = authority( auth.owner );
      active = authority( auth.active );
      posting = authority( auth.posting );
      last_owner_update = auth.last_owner_update;

      if( db.has_index< witness::account_bandwidth_index >() )
      {
         auto forum_bandwidth = db.find< witness::account_bandwidth_object, witness::by_account_bandwidth_type >( boost::make_tuple( name, witness::bandwidth_type::forum ) );

         if( forum_bandwidth != nullptr )
         {
            average_bandwidth = forum_bandwidth->average_bandwidth;
            lifetime_bandwidth = forum_bandwidth->lifetime_bandwidth;
            last_bandwidth_update = forum_bandwidth->last_bandwidth_update;
         }
      }
   }


   account_api_obj(){}

   account_id_type   id;

   account_name_type name;
   authority         owner;
   authority         active;
   authority         posting;
   public_key_type   memo_key;
   string            json_metadata;
   account_name_type proxy;

   time_point_sec    last_owner_update;
   time_point_sec    last_account_update;

   time_point_sec    created;
   account_name_type recovery_account;
   uint32_t          comment_count = 0;
   uint32_t          lifetime_vote_count = 0;
   uint32_t          post_count = 0;

   uint16_t          voting_power = 0;
   time_point_sec    last_vote_time;

   asset             balance;

   asset             reward_steem_balance;
   asset             reward_vesting_balance;
   asset             reward_vesting_steem;

   share_type        curation_rewards;
   share_type        posting_rewards;

   asset             vesting_shares;
   asset             vesting_withdraw_rate;
   time_point_sec    next_vesting_withdrawal;
   share_type        withdrawn;
   share_type        to_withdraw;
   uint16_t          withdraw_routes = 0;

   vector< share_type > proxied_vsf_votes;

   uint16_t          witnesses_voted_for;

   share_type        average_bandwidth = 0;
   share_type        lifetime_bandwidth = 0;
   time_point_sec    last_bandwidth_update;

   time_point_sec    last_post;
   time_point_sec    last_root_post;

   /// Account selling/buying feature
   bool              asb_for_sale = false;
   account_name_type asb_to; // account receives payment
   asset             asb_price;
};

struct account_history_api_obj
{

};

struct witness_api_obj
{
   witness_api_obj( const chain::witness_object& w ) :
      id( w.id ),
      owner( w.owner ),
      created( w.created ),
      url( to_string( w.url ) ),
      total_missed( w.total_missed ),
      last_aslot( w.last_aslot ),
      last_confirmed_block_num( w.last_confirmed_block_num ),
      signing_key( w.signing_key ),
      props( w.props ),
      votes( w.votes ),
      virtual_last_update( w.virtual_last_update ),
      virtual_position( w.virtual_position ),
      virtual_scheduled_time( w.virtual_scheduled_time ),
      last_work( w.last_work ),
      running_version( w.running_version ),
      hardfork_version_vote( w.hardfork_version_vote ),
      hardfork_time_vote( w.hardfork_time_vote )
   {}

   witness_api_obj() {}

   witness_id_type   id;
   account_name_type owner;
   time_point_sec    created;
   string            url;
   uint32_t          total_missed = 0;
   uint64_t          last_aslot = 0;
   uint64_t          last_confirmed_block_num = 0;
   public_key_type   signing_key;
   chain_properties  props;
   share_type        votes;
   fc::uint128       virtual_last_update;
   fc::uint128       virtual_position;
   fc::uint128       virtual_scheduled_time;
   digest_type       last_work;
   version           running_version;
   hardfork_version  hardfork_version_vote;
   time_point_sec    hardfork_time_vote;
};

struct signed_block_api_obj : public signed_block
{
   signed_block_api_obj( const signed_block& block ) : signed_block( block )
   {
      block_id = id();
      signing_key = signee();
      transaction_ids.reserve( transactions.size() );
      for( const signed_transaction& tx : transactions )
         transaction_ids.push_back( tx.id() );
   }
   signed_block_api_obj() {}

   block_id_type                 block_id;
   public_key_type               signing_key;
   vector< transaction_id_type > transaction_ids;
};

struct dynamic_global_property_api_obj : public dynamic_global_property_object
{
   dynamic_global_property_api_obj( const dynamic_global_property_object& gpo, const chain::database& db ) :
      dynamic_global_property_object( gpo )
   {
      if( db.has_index< witness::reserve_ratio_index >() )
      {
         const auto& r = db.find( witness::reserve_ratio_id_type() );

         if( BOOST_LIKELY( r != nullptr ) )
         {
            current_reserve_ratio = r->current_reserve_ratio;
            average_block_size = r->average_block_size;
            max_virtual_bandwidth = r->max_virtual_bandwidth;
         }
      }
   }

   dynamic_global_property_api_obj( const dynamic_global_property_object& gpo ) :
      dynamic_global_property_object( gpo ) {}

   dynamic_global_property_api_obj() {}

   uint32_t    current_reserve_ratio = 0;
   uint64_t    average_block_size = 0;
   uint128_t   max_virtual_bandwidth = 0;
};

} } // wls::app

FC_REFLECT( wls::app::comment_api_obj,
             (id)(author)(permlink)
             (category)(parent_author)(parent_permlink)
             (title)(body)(json_metadata)(last_update)(created)(active)(last_payout)
             (depth)(children)
             (net_rshares)(abs_rshares)(vote_rshares)
             (children_abs_rshares)(cashout_time)(max_cashout_time)
             (total_vote_weight)(reward_weight)(total_payout_value)(curator_payout_value)(author_rewards)(net_votes)(root_comment)
             (max_accepted_payout)(allow_replies)(allow_votes)(allow_curation_rewards)
             (beneficiaries)
          )

FC_REFLECT( wls::app::account_api_obj,
             (id)(name)(owner)(active)(posting)(memo_key)(json_metadata)(proxy)(last_owner_update)(last_account_update)
             (created)
             (recovery_account)
             (comment_count)(lifetime_vote_count)(post_count)(voting_power)(last_vote_time)
             (balance)
             (reward_steem_balance)(reward_vesting_balance)(reward_vesting_steem)
             (vesting_shares)(vesting_withdraw_rate)(next_vesting_withdrawal)(withdrawn)(to_withdraw)(withdraw_routes)
             (curation_rewards)
             (posting_rewards)
             (proxied_vsf_votes)(witnesses_voted_for)
             (average_bandwidth)(lifetime_bandwidth)(last_bandwidth_update)
             (last_post)(last_root_post)
             (asb_for_sale)(asb_to)(asb_price)
          )

FC_REFLECT( wls::app::tag_api_obj,
            (name)
            (total_payouts)
            (net_votes)
            (top_posts)
            (comments)
            (trending)
          )

FC_REFLECT( wls::app::witness_api_obj,
             (id)
             (owner)
             (created)
             (url)(votes)(virtual_last_update)(virtual_position)(virtual_scheduled_time)(total_missed)
             (last_aslot)(last_confirmed_block_num)(signing_key)
             (props)
             (last_work)
             (running_version)
             (hardfork_version_vote)(hardfork_time_vote)
          )

FC_REFLECT_DERIVED( wls::app::signed_block_api_obj, (wls::protocol::signed_block),
                     (block_id)
                     (signing_key)
                     (transaction_ids)
                  )

FC_REFLECT_DERIVED( wls::app::dynamic_global_property_api_obj, (wls::chain::dynamic_global_property_object),
                     (current_reserve_ratio)
                     (average_block_size)
                     (max_virtual_bandwidth)
                  )
