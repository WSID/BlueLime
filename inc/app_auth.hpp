/*
 * Wissle. 2020
 *
 * Currently, no license is carried.
 */

#ifndef __APP_AUTH_H__
#define __APP_AUTH_H__

// tdlib includes
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

// Enlightenment includes
#include <Eina.h>
#include <Ecore.h>
#include <Evas.h>
#include <Elementary.h>

// local include
#include "bluelime.hpp"
#include "ui/ui_entry_page.hpp"

using namespace td;

class app;

/**
 * Authentification Part
 *
 * Manages authentification state.
 */
class app_auth {
private:
  app *ap;

public:
  app_auth (app *ap);

  void handle (td_api::object_ptr<td_api::updateAuthorizationState> state);

  void wait_tdlib_parameters ();

  void wait_encryption_key ();

  void wait_phone_number ();

  //void wait_other_device_confirmation ();

  void wait_code (td_api::object_ptr<td_api::authorizationStateWaitCode> state);

  void wait_terms_of_service (td_api::object_ptr<td_api::authorizationStateWaitRegistration> state);

  void wait_registration ();

  /*
  void wait_state_ready ();

  //
  void wait_password ();

  void logging_out ();

  void closing ();

  void closed ();
   */

private:
  void prepare_phone_number_popup();
  void prepare_code_popup();
  void prepare_register_popup();

  static void callback_phone_number_accept(void *data, Evas_Object *object, void *event_info);
  static void callback_code_accept(void *data, Evas_Object *object, void *event_info);
  static void callback_terms_of_service_agree(void *data, Evas_Object *object, void *event_info);
  static void callback_register_accept (void *data, Evas_Object *object, void *event_info);
  static void callback_ready_to_close (void *data, Evas_Object *object, void *event_info);

  ui_entry_page *phone_number_popup;
  ui_entry_page *code_popup;

  Evas_Object *terms_of_serv_cscroll;
  Evas_Object *terms_of_serv_scroll;
  Evas_Object *terms_of_serv_box;
  Evas_Object *terms_of_serv_label_title;
  Evas_Object *terms_of_serv_label_content;
  Evas_Object *terms_of_serv_button;

  Evas_Object *register_popup;
  Evas_Object *register_layout;
  Evas_Object *register_box;
  Evas_Object *register_descr;
  ui_entry_button *register_first_name;
  ui_entry_button *register_last_name;
  Evas_Object *register_accept;
};

#endif //APP_AUTH_H
