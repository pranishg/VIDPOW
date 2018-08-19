#include <wls/plugins/changelog/changelog_api.hpp>
#include <wls/plugins/changelog/changelog_plugin.hpp>

#include <wls/app/application.hpp>
#include <wls/app/plugin.hpp>

#include <fc/io/buffered_iostream.hpp>
#include <fc/io/fstream.hpp>
#include <fc/io/json.hpp>

#include <fc/thread/future.hpp>
#include <fc/thread/mutex.hpp>
#include <fc/thread/scoped_lock.hpp>

#include <graphene/utilities/key_conversion.hpp>

#include <sstream>
#include <string>

#include <wls/chain/comment_object.hpp>

#include <boost/filesystem.hpp>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace rocksdb;

namespace wls {
    namespace plugin {
        namespace changelog {
            namespace detail {

                typedef std::map<uint32_t, changelog_block> BufferSetMap;

                class changelog_impl {
                public:
                    changelog_impl(changelog_plugin *self);

                    wls::chain::database &database() {
                       return _self->database();
                    }

                    void on_change(boost::any obj);

                    void on_applied_block(const chain::signed_block &b);

                    ///////
                    changelog_plugin *_self;
                    boost::signals2::scoped_connection _on_change_conn, _applied_block_conn;
                    uint32_t last_saved_block_num = 0;
                    BufferSetMap buffer_map;
                    DB *_changelog_db; // rocksdb
                };

                changelog_impl::changelog_impl(changelog_plugin *self) : _self(self) {}

                void changelog_impl::on_change(boost::any obj) {
                   chain::database &db = database();

                   if (obj.type() == typeid(wls::chain::comment_object)) {
                      chain::comment_object obj_comment_object = boost::any_cast<wls::chain::comment_object>(obj);
                      string content = fc::json::to_string(std::make_pair("comment_object", obj_comment_object));
                      uint32_t height = db.head_block_num();

                      if (buffer_map.find(height) == buffer_map.end()) {
                         // not found, insert new one here
                         changelog_block buffer_new = changelog_block();
                         buffer_map[height] = buffer_new;
                      }

                      changelog_block &buffer = buffer_map[height];
                      buffer.push_back(content);
                   }

                }

                void changelog_impl::on_applied_block(const chain::signed_block &b) {
                   chain::database &db = database();

                   try {
                      uint32_t last_irreversible_block_num = db.last_non_undoable_block_num();
                      if ((last_saved_block_num == 0) && (last_irreversible_block_num > 0)) {
                         last_saved_block_num = last_irreversible_block_num - 1;
                      }

                      for (uint32_t i = last_saved_block_num + 1; i <= last_irreversible_block_num; i++) {
                         if (buffer_map.find(i) != buffer_map.end()) {
                            Status s = _changelog_db->Put(WriteOptions(), std::to_string(i),
                                                          fc::json::to_string(buffer_map[i]));
                            assert(s.ok());

                            buffer_map.erase(i); // erasing by key
                         }

                         last_saved_block_num = i;
                      }
                   }
                   FC_LOG_AND_RETHROW()
                }

            }

            changelog_plugin::changelog_plugin(application *app) : plugin(app) {
               my = std::make_shared<detail::changelog_impl>(this);
            }

            changelog_plugin::~changelog_plugin() {}

            std::string changelog_plugin::plugin_name() const {
               return "changelog";
            }

            void changelog_plugin::plugin_initialize(const boost::program_options::variables_map &options) {
               ilog("changelog_plugin::plugin_initialize");

               ///////////////////////////////////////////////////////////////
               // get data_dir
               fc::path data_dir;
               if (options.count("data-dir")) {
                  data_dir = options["data-dir"].as<boost::filesystem::path>();
                  if (data_dir.is_relative())
                     data_dir = fc::current_path() / data_dir;
               }

               ///////////////////////////////////////////////////////////////

               // open DB
               // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
               // create the DB if it's not already present
               Options optionsRocksDB;
               optionsRocksDB.IncreaseParallelism();
               optionsRocksDB.OptimizeLevelStyleCompaction();
               optionsRocksDB.create_if_missing = true;
               Status s = DB::Open(optionsRocksDB, data_dir.string() + "/changelog", &my->_changelog_db);
               assert(s.ok());

               ///////////////////////////////////////////////////////////////
               chain::database &db = database();

               // connect needed signals
               my->_applied_block_conn = db.applied_block.connect(
                       [this](const chain::signed_block &b) { my->on_applied_block(b); });
               my->_on_change_conn = db.on_change.connect([this](boost::any obj) { my->on_change(obj); });
            }

            void changelog_plugin::plugin_startup() {
               ilog("changelog_plugin::plugin_startup");
               app().register_api_factory< changelog_api >( "changelog_api" );
            }

            void changelog_plugin::plugin_shutdown() {
               ilog("changelog_plugin::plugin_shutdown");
               delete my->_changelog_db;
            }

            ////////
            DB *changelog_plugin::get_changelog_db() {
               return my->_changelog_db;
            }

        }
    }
}

WLS_DEFINE_PLUGIN(changelog, wls::plugin::changelog::changelog_plugin)
