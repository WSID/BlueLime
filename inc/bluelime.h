#ifndef __bluelime_H__
#define __bluelime_H__

// stdlib includes
#include <memory>

// Tizen includes
#include <app.h>
#include <system_settings.h>
#include <dlog.h>

// Enlightenment / Tizen Extension includes
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
using namespace td::td_api;

class app {
  public:
    Evas_Object *win;
    Evas_Object *conform;
    Evas_Object *naviframe;
    Eext_Circle_Surface *circle_surface;
    std::shared_ptr<ChatListPage> chat_list_page;

  private:
    std::unique_ptr<td::Client> td_client;
    uint64_t td_query_id;

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

  private:
    void create_base_gui ();

    // Callback functions for windows
    void on_win_delete_request(Evas_Object *obj);

    void on_win_back (Evas_Object *obj);
};

#endif /* __bluelime_H__ */
