#include "app_client.hpp"

#include <dlog.h>

int app_client::add_update_handler (app_update_callback callback) {
  if (! callback) return 0;

  update_handler_id ++;
  update_handlers.emplace (update_handler_id, callback);
  return update_handler_id;
}

bool app_client::remove_update_handler (int id) {
  return (update_handlers.erase (id) != 0);
}

void app_client::poller_func () {
  poller_running = true;

  while (poller_running) {
    td::Client::Response response = client->receive (10.0);
    if (! response.object) continue;

    // Response
    if (response.id != 0) {
      std::map<uint64_t, app_result_callback>::const_iterator iter;

      iter = callbacks.find (response.id);
      if (iter == callbacks.cend()) {
        dlog_print (DLOG_WARN, "bluelime",
                    "Response from unknown request: %llu", response.id);
        continue;
      }

      const app_result_callback& callback = iter->second;
      if (callback) callback (std::move(response.object));
      callbacks.erase(iter);
    }

    // Update
    else {
      td::td_api::object_ptr<td::td_api::Update> update;
      update = td::td_api::move_object_as<td::td_api::Update> (response.object);

      for (const std::pair<const uint64_t, app_update_callback>& kv : update_handlers) {
        const app_update_callback& callback = kv.second;

        if (! callback) dlog_print (DLOG_WARN, "bluelime", "P!! Invalid update callback!");
        else callback (*update);
      }
    }
  }
}

bool app_client::run_poller () {
  if (poller) return false;

  poller = std::thread(&app_client::poller_func, this);
}
