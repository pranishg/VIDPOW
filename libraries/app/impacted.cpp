/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <wls/protocol/authority.hpp>

#include <wls/app/impacted.hpp>

#include <fc/utility.hpp>

namespace wls { namespace app {

using namespace fc;
using namespace wls::protocol;

// TODO:  Review all of these, especially no-ops
struct get_impacted_account_visitor
{
   flat_set<account_name_type>& _impacted;
   get_impacted_account_visitor( flat_set<account_name_type>& impact ):_impacted( impact ) {}
   typedef void result_type;

   template<typename T>
   void operator()( const T& op )
   {
      op.get_required_posting_authorities( _impacted );
      op.get_required_active_authorities( _impacted );
      op.get_required_owner_authorities( _impacted );
   }

   // ops
   void operator()( const account_create_operation& op )
   {
      _impacted.insert( op.new_account_name );
      _impacted.insert( op.creator );
   }

   void operator()( const comment_operation& op )
   {
      _impacted.insert( op.author );
      if( op.parent_author.size() )
         _impacted.insert( op.parent_author );
   }

   void operator()( const vote_operation& op )
   {
      _impacted.insert( op.voter );
      _impacted.insert( op.author );
   }

   void operator()( const transfer_operation& op )
   {
      _impacted.insert( op.from );
      _impacted.insert( op.to );
   }

   void operator()( const transfer_to_vesting_operation& op )
   {
      _impacted.insert( op.from );

      if ( op.to != account_name_type() && op.to != op.from )
      {
         _impacted.insert( op.to );
      }
   }

   void operator()( const set_withdraw_vesting_route_operation& op )
   {
      _impacted.insert( op.from_account );
      _impacted.insert( op.to_account );
   }

   void operator()( const account_witness_vote_operation& op )
   {
      _impacted.insert( op.account );
      _impacted.insert( op.witness );
   }

   void operator()( const account_witness_proxy_operation& op )
   {
      _impacted.insert( op.account );
      _impacted.insert( op.proxy );
   }


   // vops

   void operator()( const author_reward_operation& op )
   {
      _impacted.insert( op.author );
   }

   void operator()( const curation_reward_operation& op )
   {
      _impacted.insert( op.curator );
   }

   void operator()( const fill_vesting_withdraw_operation& op )
   {
      _impacted.insert( op.from_account );
      _impacted.insert( op.to_account );
   }

   void operator()( const shutdown_witness_operation& op )
   {
      _impacted.insert( op.owner );
   }

   void operator()( const comment_benefactor_reward_operation& op )
   {
      _impacted.insert( op.benefactor );
      _impacted.insert( op.author );
   }

   void operator()( const producer_reward_operation& op )
   {
      _impacted.insert( op.producer );
   }

   void operator()( const devfund_operation& op )
   {
      _impacted.insert( op.account );
   }

   //void operator()( const operation& op ){}
};

void operation_get_impacted_accounts( const operation& op, flat_set<account_name_type>& result )
{
   get_impacted_account_visitor vtor = get_impacted_account_visitor( result );
   op.visit( vtor );
}

void transaction_get_impacted_accounts( const transaction& tx, flat_set<account_name_type>& result )
{
   for( const auto& op : tx.operations )
      operation_get_impacted_accounts( op, result );
}

} }
