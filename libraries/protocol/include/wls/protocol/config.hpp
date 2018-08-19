#pragma once

#define WLS_BLOCKCHAIN_VERSION                 ( version(0, 0, 0) )
#define WLS_BLOCKCHAIN_HARDFORK_VERSION        ( hardfork_version( WLS_BLOCKCHAIN_VERSION ) )

#ifdef IS_TEST_NET
   #define WLS_INIT_PRIVATE_KEY                 (fc::ecc::private_key::regenerate(fc::sha256::hash(std::string("init_key"))))
   #define WLS_INIT_PUBLIC_KEY_STR              (std::string( wls::protocol::public_key_type(WLS_INIT_PRIVATE_KEY.get_public_key()) ))
   #define WLS_CHAIN_ID                         (fc::sha256::hash("testnet"))

   #define VESTS_SYMBOL  (uint64_t(6) | (uint64_t('V') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< VESTS with 6 digits of precision
   #define WLS_SYMBOL    (uint64_t(3) | (uint64_t('T') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< TESTS with 3 digits of precision

   #define WLS_SYMBOL_STR                       "TESTS"
   #define WLS_ADDRESS_PREFIX                   "TST"

   #define WLS_GENESIS_TIME                     (fc::time_point_sec(1515765200))
   #define WLS_CASHOUT_WINDOW_SECONDS           (60*60) /// 1 hr
   #define WLS_UPVOTE_LOCKOUT                   (fc::minutes(5))

   #define WLS_MIN_ACCOUNT_CREATION_FEE         0

   #define WLS_OWNER_UPDATE_LIMIT               fc::seconds(0)
#else // IS LIVE NETWORK
   #define WLS_INIT_PUBLIC_KEY_STR              "WLS5dVkwQMJMmqgKAdt3GUeMoFQyo82qe3hc2SoMNHwgFsVWC5d6L"
   #define WLS_CHAIN_ID                         (fc::sha256::hash("whaleshares")) // de999ada2ff7ed3d3d580381f229b40b5a0261aec48eb830e540080817b72866

   #define VESTS_SYMBOL  (uint64_t(6) | (uint64_t('V') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< VESTS with 6 digits of precision
   #define WLS_SYMBOL    (uint64_t(3) | (uint64_t('W') << 8) | (uint64_t('L') << 16) | (uint64_t('S') << 24)) ///< WLS with 3 digits of precision

   #define WLS_SYMBOL_STR                       "WLS"
   #define WLS_ADDRESS_PREFIX                   "WLS"

   #define WLS_GENESIS_TIME                     (fc::time_point_sec(1534070000))
   #define WLS_CASHOUT_WINDOW_SECONDS           (60*60*24*7)  /// 7 days
   #define WLS_UPVOTE_LOCKOUT                   (fc::hours(12))

   #define WLS_MIN_ACCOUNT_CREATION_FEE         10

   #define WLS_OWNER_UPDATE_LIMIT               fc::minutes(60)
#endif

#define WLS_BLOCK_INTERVAL                      3
#define WLS_BLOCKS_PER_YEAR                     (365*24*60*60/WLS_BLOCK_INTERVAL)
#define WLS_BLOCKS_PER_DAY                      (24*60*60/WLS_BLOCK_INTERVAL)
#define WLS_START_MINER_VOTING_BLOCK            (1 * 30) /// (WLS_BLOCKS_PER_DAY * 30)

#define WLS_INIT_MINER_NAME                     "initminer"
#define WLS_MAX_WITNESSES                       21

#define WLS_MAX_VOTED_WITNESSES                 20
#define WLS_MAX_RUNNER_WITNESSES                1

#define WLS_HARDFORK_REQUIRED_WITNESSES         17 // 17 of the 21 dpos witnesses (20 elected and 1 virtual time) required for hardfork. This guarantees 75% participation on all subsequent rounds.
#define WLS_MAX_TIME_UNTIL_EXPIRATION          (60*60) // seconds,  aka: 1 hour
#define WLS_MAX_MEMO_SIZE                       2048
#define WLS_MAX_PROXY_RECURSION_DEPTH           4
#define WLS_VESTING_WITHDRAW_INTERVALS          13
#define WLS_VESTING_WITHDRAW_INTERVAL_SECONDS   (60*60*24*7) /// 1 week per interval
#define WLS_MAX_WITHDRAW_ROUTES                 10
#define WLS_VOTE_REGENERATION_SECONDS           (5*60*60*24) // 5 day
#define WLS_MAX_VOTE_CHANGES                    5
#define WLS_REVERSE_AUCTION_WINDOW_SECONDS      (60*30) /// 30 minutes
#define WLS_MIN_VOTE_INTERVAL_SEC               3
#define WLS_VOTE_DUST_THRESHOLD                 (50)

#define WLS_MIN_ROOT_COMMENT_INTERVAL           (fc::seconds(60*5)) // 5 minutes
#define WLS_MIN_REPLY_INTERVAL                  (fc::seconds(20)) // 20 seconds
#define WLS_POST_AVERAGE_WINDOW                 (60*60*24u) // 1 day
#define WLS_POST_MAX_BANDWIDTH                  (4*WLS_100_PERCENT) // 2 posts per 1 days, average 1 every 12 hours
#define WLS_POST_WEIGHT_CONSTANT                (uint64_t(WLS_POST_MAX_BANDWIDTH) * WLS_POST_MAX_BANDWIDTH)

#define WLS_MAX_ACCOUNT_WITNESS_VOTES           30

#define WLS_100_PERCENT                         10000
#define WLS_1_PERCENT                           (WLS_100_PERCENT/100)
#define WLS_1_TENTH_PERCENT                     (WLS_100_PERCENT/1000)

#define WLS_INFLATION_RATE_START_PERCENT        (10000) // reduce to ~ WLS_INFLATION_RATE_STOP_PERCENT
#define WLS_INFLATION_RATE_STOP_PERCENT         (488) // ~5%
#define WLS_CONTENT_REWARD_PERCENT              (85*WLS_1_PERCENT) //75% of inflation
#define WLS_CONTENT_CURATE_REWARD_PERCENT       (25*WLS_1_PERCENT) // 25% of WLS_CONTENT_REWARD_PERCENT only
#define WLS_DEV_FUND_PERCENT                    (5*WLS_1_PERCENT) //5% of inflation

#define WLS_MIN_RATION                          100000
#define WLS_MAX_RATION_DECAY_RATE               (1000000)
#define WLS_FREE_TRANSACTIONS_WITH_NEW_ACCOUNT  100

#define WLS_BANDWIDTH_AVERAGE_WINDOW_SECONDS    (60*60*24*7) ///< 1 week
#define WLS_BANDWIDTH_PRECISION                 (uint64_t(1000000)) ///< 1 million
#define WLS_MAX_COMMENT_DEPTH                   0xffff // 64k
#define WLS_SOFT_MAX_COMMENT_DEPTH              0xff // 255

#define WLS_MAX_RESERVE_RATIO                   (20000)

#define WLS_POST_REWARD_FUND_NAME               ("post")
#define WLS_RECENT_RSHARES_DECAY_RATE           (fc::days(15))
#define WLS_CONTENT_CONSTANT                    (uint128_t(uint64_t(2000000000000ll)))

#define WLS_MIN_PAYOUT_STEEM                    20

#define WLS_MIN_ACCOUNT_NAME_LENGTH             3
#define WLS_MAX_ACCOUNT_NAME_LENGTH             16

#define WLS_MIN_PERMLINK_LENGTH                 0
#define WLS_MAX_PERMLINK_LENGTH                 256
#define WLS_MAX_WITNESS_URL_LENGTH              2048

#define WLS_INIT_SUPPLY                         int64_t(21000000000ll)
#define WLS_MAX_SHARE_SUPPLY                    int64_t(1000000000000000ll)
#define WLS_MAX_SIG_CHECK_DEPTH                 2

#define WLS_MIN_TRANSACTION_SIZE_LIMIT          1024
#define WLS_SECONDS_PER_YEAR                    (uint64_t(60*60*24*365ll))

#define WLS_MAX_TRANSACTION_SIZE                (1024*64)
#define WLS_MIN_BLOCK_SIZE_LIMIT                (WLS_MAX_TRANSACTION_SIZE)
#define WLS_MAX_BLOCK_SIZE                      (WLS_MAX_TRANSACTION_SIZE*WLS_BLOCK_INTERVAL*2000)
#define WLS_SOFT_MAX_BLOCK_SIZE                 (2*1024*1024)
#define WLS_MIN_BLOCK_SIZE                      115
#define WLS_BLOCKS_PER_HOUR                     (60*60/WLS_BLOCK_INTERVAL)

#define WLS_MIN_UNDO_HISTORY                    10
#define WLS_MAX_UNDO_HISTORY                    10000

#define WLS_MIN_TRANSACTION_EXPIRATION_LIMIT    (WLS_BLOCK_INTERVAL * 5) // 5 transactions per block
#define WLS_BLOCKCHAIN_PRECISION                uint64_t( 1000 )

#define WLS_BLOCKCHAIN_PRECISION_DIGITS         3
#define WLS_MAX_URL_LENGTH                      127

#define WLS_IRREVERSIBLE_THRESHOLD              (75 * WLS_1_PERCENT)

#define VIRTUAL_SCHEDULE_LAP_LENGTH             ( fc::uint128(uint64_t(-1)) )
#define VIRTUAL_SCHEDULE_LAP_LENGTH2            ( fc::uint128::max_value() )

/**
 *  Reserved Account IDs with special meaning
 */
///@{
/// Represents the current witnesses
#define WLS_MINER_ACCOUNT                       "miners"
/// Represents the canonical account with NO authority (nobody can access funds in null account)
#define WLS_NULL_ACCOUNT                        "null"
/// Represents the canonical account with WILDCARD authority (anybody can access funds in temp account)
#define WLS_TEMP_ACCOUNT                        "temp"
/// Represents the canonical account for specifying you will vote for directly (as opposed to a proxy)
#define WLS_PROXY_TO_SELF_ACCOUNT               ""
/// Represents the canonical root post parent account
#define WLS_ROOT_POST_PARENT                    (account_name_type())
///@}

#define WLS_DEV_FUND_ACC_NAME                   "devfund"
#define WLS_WHALESHARE_ACC_NAME                 "wls"
