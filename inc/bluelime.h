#ifndef __bluelime_H__
#define __bluelime_H__

// stdlib includes
#include <future>
#include <map>
#include <memory>

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
#include "chat_list_page.h"

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

class app {
  public:
    Evas_Object *win;
    Evas_Object *conform;
    Evas_Object *naviframe;
    Eext_Circle_Surface *circle_surface;
    std::shared_ptr<ChatListPage> chat_list_page;

  private:
    std::unique_ptr<td::Client> td_client;
    std::map<uint64_t, std::promise<td_api::object_ptr<td_api::Object>>> td_promises_res;
    uint64_t td_query_id;

    Ecore_Poller *td_poller;

    std::unique_ptr<app_auth> part_auth;

  public:
    app ();

    ~ app ();


    std::future<td_api::object_ptr<td_api::Object>>
    send (td_api::object_ptr<td_api::Function> td_func);

    template <typename T, typename ... ARGS>
    inline std::future<td_api::object_ptr<td_api::Object>>
    send_make (ARGS ... args) {
      return send (
          td_api::move_object_as <td_api::Function> (
              td_api::make_object<T, ARGS...> (std::move(args...))));
    }


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

  private:
    void create_base_gui ();

    // Poller
    Eina_Bool poll_td_client ();

    // Callback functions for windows
    void on_win_delete_request(Evas_Object *obj);

    void on_win_back (Evas_Object *obj);
};

#endif /* __bluelime_H__ */
