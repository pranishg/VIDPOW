#pragma once

#include <wls/protocol/base.hpp>

#include <wls/follow/follow_plugin.hpp>

namespace wls { namespace follow {

using namespace std;
using wls::protocol::base_operation;

struct follow_operation : base_operation
{
    account_name_type follower;
    account_name_type following;
    set< string >     what; /// blog, mute

    void validate()const;

    void get_required_posting_authorities( flat_set<account_name_type>& a )const { a.insert( follower ); }
};

struct reblog_operation : base_operation
{
   account_name_type account;
   account_name_type author;
   string            permlink;

   void validate()const;

   void get_required_posting_authorities( flat_set<account_name_type>& a )const { a.insert( account ); }
};

typedef fc::static_variant<
         follow_operation,
         reblog_operation
      > follow_plugin_operation;

DEFINE_PLUGIN_EVALUATOR( follow_plugin, follow_plugin_operation, follow );
DEFINE_PLUGIN_EVALUATOR( follow_plugin, follow_plugin_operation, reblog );

} } // wls::follow

FC_REFLECT( wls::follow::follow_operation, (follower)(following)(what) )
FC_REFLECT( wls::follow::reblog_operation, (account)(author)(permlink) )

DECLARE_OPERATION_TYPE( wls::follow::follow_plugin_operation )

FC_REFLECT_TYPENAME( wls::follow::follow_plugin_operation )
