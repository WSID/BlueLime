#ifndef __APP_CLIENT_H__
#define __APP_CLIENT_H__

// stdlib includes
#include <map>
#include <memory>
#include <functional>
#include <thread>

// tdlib includes
#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

/**
 * app_result_callback:
 *
 * A Callback for request.
 *
 * @1: Telegram function's result value, or nullptr, if failed.
 */
typedef
  std::function<void(td::td_api::object_ptr<td::td_api::Object>)>
  app_result_callback;

/**
 * app_update_callback:
 *
 * A update notify callback, for td::td_api::Update.
 */
typedef std::function<void(td::td_api::Update&)> app_update_callback;

/**
 * app_client:
 *
 * A part of application that communicates with Telegram.
 *
 * This not only manages query id, but also runs polling thread.
 */
class app_client {
  public:
    app_client();

    /**
     * send:
     *
     * Send a telegram function without callback.
     *
     * @TF: A type of Telegram function.
     * @td_func: A Telegram function.
     */
    template <typename TF>
    void send (td::td_api::object_ptr<TF> td_func);

    /**
     * send:
     *
     * Send a telegram function with callback.
     *
     * Expected callback type is (...) (TF::ResultType). But TF::ResultType is
     * generally td::td_api::object_ptr<...>, for most of TF type.
     *
     * So F becomes (...) (td::td_api::object_ptr<...>)
     *
     * If it fails, nullptr is passed to callback.
     *
     * @TF: A type of Telegram function.
     * @F: A callback type of (...) (TF::ResultType)
     * @td_func: A type of Telegram function.
     * @callback: A callback for operation.
     */
    template <typename TF, typename F>
    void send (td::td_api::object_ptr<TF> td_func, F callback);

    /**
     * add_update_handler:
     *
     * Register callback for update.
     *
     * @callback: A callback.
     * @returns: An id of handler, or 0 if callback is invalid.
     */
    int add_update_handler (app_update_callback callback);

    /**
     * remove_update_handler:
     *
     * Unregister callback for update.
     *
     * @id: An id of handler.
     * @returns: Whether the handler is removed.
     */
    bool remove_update_handler (int id);


    //////// Pollers ///////////////////////////////////////////////////////////

    /**
     * run_poller:
     */
    bool run_poller ();

  private:
    /*
     * poller_func
     *
     * A function that run by poller.
     */
    void poller_func ();

    std::unique_ptr<td::Client> client;
    std::map<uint64_t, app_result_callback> callbacks;
    uint64_t query_id;

    std::map<int, app_update_callback> update_handlers;
    int update_handler_id;

    std::thread poller;
    bool poller_running;
};


//////// Function templace implementations /////////////////////////////////////

template <typename TF>
void app_client::send (td::td_api::object_ptr<TF> td_func) {
  query_id ++;
  callbacks.emplace (query_id, app_result_callback());
  client->send ({query_id, std::move(td_func)});
}

template <typename TF, typename F>
void app_client::send (td::td_api::object_ptr<TF> td_func, F callback) {
  using R = typename TF::ReturnType;

  query_id ++;
  callbacks.emplace (query_id,
    [c {std::move(callback)} ]
    (td::td_api::object_ptr<td::td_api::Object> result) {
      c(td::td_api::move_object_as<typename R::element_type>(result));
    } );
  client->send ({query_id, std::move(td_func)});
}

#endif //APP_CLIENT_H
