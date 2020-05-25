
#include <cstdint>
#include <utility>

#include "app_auth.hpp"

#include "bluelime.hpp"


// ChatListPage -> ChatPage -> StickerPage
// ^- AuthPage

template <typename T, void (T::*function)(Evas_Object*)>
class mf_2_cb {
    public:
    static void callback (void *data, Evas_Object *obj, void *einfo) {
        T* self = (T*)data;
        (self->*function)(obj);
    }
};

app::app () :
    td_client (std::make_unique<td::Client>()),
    td_query_id(1LU)
{
    create_base_gui ();

    part_auth = std::make_unique<app_auth> (this);

    td_poller = ecore_poller_add (ECORE_POLLER_CORE, 1,
        [](void *data) {return static_cast<app*>(data)->poll_td_client(); }, this);
}

app::~ app () {
    // Doing nothing here!
    // td_client is auto-deleted.
}


std::future<td_api::object_ptr<td_api::Object>>
app::send (td_api::object_ptr<td_api::Function> td_func) {
  td_query_id ++;
  td_client->send ({td_query_id, std::move(td_func)});
  return td_promises_res[td_query_id].get_future();
}

// Callback for application lifecycle
void app::control(app_control_h app_control) {}

void app::pause () {}

void app::resume () {}

// Callback for UI application events
void app::ui_lang_changed (app_event_info_h event_info) {
    /*APP_EVENT_LANGUAGE_CHANGED*/
    char *locale = NULL;
    system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
    elm_language_set(locale);
    free(locale);
    return;
}

void app::ui_orient_changed (app_event_info_h event_info) {}

void app::ui_region_changed (app_event_info_h event_info) {}

void app::ui_low_battery (app_event_info_h event_info) {}

void app::ui_low_memory (app_event_info_h event_info) {}

void app::create_base_gui () {
    win = elm_win_util_standard_add(PACKAGE, PACKAGE);
    elm_win_autodel_set(win, true);

    evas_object_smart_callback_add (win, "delete,request",
        &mf_2_cb<app, &app::on_win_delete_request>::callback, this);

    eext_object_event_callback_add (win, EEXT_CALLBACK_BACK,
        &mf_2_cb<app, &app::on_win_back>::callback, this);

    /* Conformant */
    /* Create and initialize elm_conformant.
       elm_conformant is mandatory for base gui to have proper size
       when indicator or virtual keypad is visible. */
    conform = elm_conformant_add(win);
    elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_SHOW);
    elm_win_indicator_opacity_set(win, ELM_WIN_INDICATOR_OPAQUE);
    evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win, conform);
    evas_object_show(conform);

    /* Naviframe */
    naviframe = elm_naviframe_add(conform);
    elm_object_content_set(conform, naviframe);
    circle_surface = eext_circle_surface_naviframe_add (naviframe);

    /* Page 1 */
    chat_list_page = std::make_shared <ChatListPage> (naviframe, circle_surface);

    elm_naviframe_item_push(naviframe, NULL, NULL, NULL, chat_list_page->chat_genlist, "empty");

    /* Show window after base gui is set up */
    evas_object_show(win);
}


// Poller

Eina_Bool app::poll_td_client () {
    bool received = false;
    do {
        td::Client::Response response = td_client->receive(0.0);

        received = (response.object != nullptr);
        if (received) {
            if (response.id == 0) {
                std::int32_t id = response.object->get_id();

                if (id == td_api::updateAuthorizationState::ID) {
                    part_auth->handle (td_api::move_object_as<td_api::updateAuthorizationState> (response.object));
                }
                else {
                    dlog_print (DLOG_WARN, "bluelime", "Unhandled Mesasge ... \n%s",
                        to_string(response.object).c_str());
                }
            }
            else {
                dlog_print (DLOG_VERBOSE, "bluelime", "Responsed Query: %llu", response.id);
                auto iter = td_promises_res.find(response.id);
                if (iter == td_promises_res.end()) {
                    dlog_print (DLOG_WARN, "bluelime", "Unknown Query!!");
                    continue;
                }

                iter->second.set_value(std::move(response.object));
                td_promises_res.erase(iter);
            }
        }
    }
    while (received);
    return EINA_TRUE;
}


// Callback functions for windows
void app::on_win_delete_request(Evas_Object *obj) {
    ui_app_exit();
}

void app::on_win_back (Evas_Object *obj) {
    elm_win_lower (win);
}

int
main(int argc, char *argv[])
{
    app *ap = nullptr;
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

    event_callback.create = [] (void *data) {
        data = (void*) new app();
        return true;
    };

    event_callback.terminate = [] (void *data) {
        if (data != nullptr) {
            delete (app*)data;
            data = nullptr;
        }
    };

    event_callback.pause = [] (void *data) {
        static_cast<app*>(data)->pause();
    };

    event_callback.resume = [] (void *data) {
        static_cast<app*>(data)->resume();
    };

    event_callback.app_control = [] (app_control_h control, void *data) {
        static_cast<app*>(data)->control (control);
    };

    ui_app_add_event_handler(
        &handlers[APP_EVENT_LOW_BATTERY],
        APP_EVENT_LOW_BATTERY,
        [] (app_event_info_h event_info, void *data) {
            static_cast<app*>(data)->ui_low_battery(event_info);
        }, &ap);

    ui_app_add_event_handler(
        &handlers[APP_EVENT_LOW_MEMORY],
        APP_EVENT_LOW_MEMORY,
        [] (app_event_info_h event_info, void *data) {
            static_cast<app*>(data)->ui_low_memory(event_info);
        }, &ap);

    ui_app_add_event_handler(
        &handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED],
        APP_EVENT_DEVICE_ORIENTATION_CHANGED,
        [] (app_event_info_h event_info, void *data) {
            static_cast<app*>(data)->ui_orient_changed(event_info);
        }, &ap);

    ui_app_add_event_handler(
        &handlers[APP_EVENT_LANGUAGE_CHANGED],
        APP_EVENT_LANGUAGE_CHANGED,
        [] (app_event_info_h event_info, void *data) {
            static_cast<app*>(data)->ui_lang_changed(event_info);
        }, &ap);

    ui_app_add_event_handler(
        &handlers[APP_EVENT_REGION_FORMAT_CHANGED],
        APP_EVENT_REGION_FORMAT_CHANGED,
        [] (app_event_info_h event_info, void *data) {
            static_cast<app*>(data)->ui_region_changed(event_info);
        }, &ap);

	ret = ui_app_main(argc, argv, &event_callback, &ap);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
