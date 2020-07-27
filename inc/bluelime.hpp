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

// local include
#include "app_client.hpp"
#include "chat_list_page.hpp"

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "bluelime"

#if !defined(PACKAGE)
#define PACKAGE "standalone.bluelime"
#endif

class app_auth;

class app {
  public:
    Evas_Object *win;
    Evas_Object *conform;
    Evas_Object *naviframe;
    Eext_Circle_Surface *circle_surface;
    std::shared_ptr<ChatListPage> chat_list_page;

  private:
    app_client part_client;
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

  private:
    void create_base_gui ();

    bool handle_update (td::td_api::object_ptr<td::td_api::Object> &object);

    // Callback functions for windows
    void on_win_delete_request(Evas_Object *obj);

    void on_win_back (Evas_Object *obj);
};

#endif /* __bluelime_H__ */
