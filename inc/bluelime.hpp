#ifndef __bluelime_H__
#define __bluelime_H__

// stdlib includes
#include <map>
#include <memory>
#include <type_traits>

// Tizen includes
#include <app.h>
#include <system_settings.h>
#include <dlog.h>

// Enlightenment / Tizen Extension includes
#include <Eina.h>
#include <Ecore.h>
#include <Elementary.h>
#include <efl_extension.h>

// tdlib includes
#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

// local include
#include "chat_list_page.hpp"

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "bluelime"

#if !defined(PACKAGE)
#define PACKAGE "standalone.bluelime"
#endif

// using usage - this will reduce verbosity of telegram library
using namespace td;

class app_auth;

typedef std::function<void(td_api::object_ptr<td_api::Object>)> app_callback;

class app {
  public:
    Evas_Object *win;
    Evas_Object *conform;
    Evas_Object *naviframe;
    Eext_Circle_Surface *circle_surface;
    std::shared_ptr<ChatListPage> chat_list_page;

  private:
    std::unique_ptr<td::Client> td_client;
    std::map<uint64_t, app_callback> td_callback;
    uint64_t td_query_id;

    Ecore_Poller *td_poller;

    std::unique_ptr<app_auth> part_auth;

  public:
    app ();

    ~ app ();

    // Callback for application lifecycle
    void control(app_control_h app_control);

    void pause ();

    void resume ();

    // Callback for UI application events
    void ui_lang_changed (app_event_info_h event_info);

    void ui_orient_changed (app_event_info_h event_info);

    void ui_region_changed (app_event_info_h event_info);

    void ui_low_battery (app_event_info_h event_info);

    void ui_low_memory (app_event_info_h event_info);

    // Interaction with telegram.

    /**
     * send:
     *
     * Sends telegram function.
     *
     * @td_func: A Telegram function.
     * @callback: (optional): A Callback to call.
     */
    void send (td_api::object_ptr<td_api::Function> td_func,
               const app_callback& callback = app_callback());


    /**
     * send:
     *
     * Sends telegram function, and let a @callback to run when get result.
     *
     * @F: type of td_func
     * @C: type of callback, for (anything (F::ReturnType))
     * @td_func: A Telegram function.
     * @callback: A Callback to run.
     */
    template <typename F, typename C>
    void send(td_api::object_ptr<F> td_func, C callback = C()) {
      td_query_id ++;
      td_callback[td_query_id] =
        [callback](td_api::object_ptr<td_api::Object> ret)
      {
        // (ret->get_id() == F::ReturnType::element_type::ID);
        callback(td_api::move_object_as<typename F::ReturnType::element_type>(ret));
      };
      td_client->send ({td_query_id, std::move(td_func)});
    }


  private:
    void create_base_gui ();

    // Poller
    Eina_Bool poll_td_client ();

    // Callback functions for windows
    void on_win_delete_request(Evas_Object *obj);

    void on_win_back (Evas_Object *obj);
};

#endif /* __bluelime_H__ */
