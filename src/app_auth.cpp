#include "app_auth.hpp"

// C++ std include
#include <algorithm>
#include <iterator>
#include <utility>
#include <future>
#include <sstream>

// Tizen includes
#include <dlog.h>
#include <system_info.h>
#include <system_settings.h>

#define MACRO_STRINGIFY(A) #A

#ifndef BLUELIME_TDAPI_ID
#error BLUELIME_TDAPI_ID should be setted through environment variable.
#endif

#ifndef BLUELIME_TDAPI_HASH
#error BLUELIME_TDAPI_HASH should be setted through environment variable.
#endif

#ifndef BLUELIME_TDAPI_USE_TEST
#warning BLUELIME_TDAPI_USE_TEST is suggested to setted through environment variable.
#define BLUELIME_TDAPI_USE_TEST (1)
#endif

#if BLUELIME_TDAPI_USE_TEST
#define BLUELIME_TDAPI_USE_TEST_B (EINA_TRUE)
#elif
#define BLUELIME_TDAPI_USE_TEST_B (EINA_FALSE)
#endif

#define BLUELIME_TDAPI_HASH_STR ( MACRO_STRINGIFY (BLUELIME_TDAPI_HASH) )

app_auth::app_auth (app *ap) : ap(ap) {
  prepare_phone_number_popup ();
  prepare_code_popup ();
  prepare_register_popup();
}

void app_auth::handle (typename td_api::object_ptr<td_api::updateAuthorizationState> state) {
  std::int32_t tid = state->authorization_state_->get_id();

  switch (tid) {
    case td_api::authorizationStateWaitTdlibParameters::ID:
      wait_tdlib_parameters();
      break;

    case td_api::authorizationStateWaitEncryptionKey::ID:
      wait_encryption_key();
      break;

    case td_api::authorizationStateWaitPhoneNumber::ID:
      wait_phone_number();
      break;

    case td_api::authorizationStateWaitCode::ID:
      wait_code(td_api::move_object_as <td_api::authorizationStateWaitCode> (state->authorization_state_));
      break;

    case td_api::authorizationStateWaitRegistration::ID:
      wait_terms_of_service (td_api::move_object_as <td_api::authorizationStateWaitRegistration> (state->authorization_state_));
      break;

    default:
      dlog_print (DLOG_WARN, "bluelime", "Unhandled Auth State ... \n%s", to_string(state).c_str());
  }
}

void app_auth::wait_tdlib_parameters() {
    char *system_language;
    char *system_model_name;
    char *system_version;
    system_settings_get_value_string (SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE,
                                      &system_language);

    system_info_get_platform_string("http://tizen.org/system/model_name",
                                    &system_model_name);

    system_info_get_platform_string("http://tizen.org/feature/platform.version",
                                    &system_version);

    td_api::object_ptr<td_api::tdlibParameters> param;

    param = td_api::make_object<td_api::tdlibParameters>();
    param->use_test_dc_ = BLUELIME_TDAPI_USE_TEST_B;
    param->database_directory_ = app_get_data_path();
    param->files_directory_ = app_get_data_path();
    param->use_file_database_ = EINA_TRUE;
    param->use_chat_info_database_ = EINA_TRUE;
    param->use_message_database_ = EINA_TRUE;
    param->use_secret_chats_ = EINA_FALSE; // Would you like to do secret chat on watch? I guess no.
    param->api_id_ = BLUELIME_TDAPI_ID;
    param->api_hash_ = BLUELIME_TDAPI_HASH_STR;
    param->system_language_code_ = system_language;
    param->device_model_ = system_model_name;
    param->system_version_ = system_version;
    param->application_version_ = "0.0.1";
    param->enable_storage_optimizer_ = EINA_TRUE;
    param->ignore_file_names_ = EINA_FALSE;

    free (system_language);
    free (system_model_name);
    free (system_version);

    td_api::object_ptr<td_api::setTdlibParameters> tf;
    tf = td_api::make_object<td_api::setTdlibParameters>(std::move(param));
    ap->send(std::move(tf));
}

void
app_auth::wait_encryption_key () {
    ap->send(td_api::make_object<td_api::checkDatabaseEncryptionKey>());
}

void
app_auth::wait_phone_number () {
  elm_naviframe_item_push(ap->naviframe, NULL, NULL, NULL, phone_number_popup->get_popup(), "empty");
  evas_object_show (phone_number_popup->get_popup ());
}

void
app_auth::wait_code (td_api::object_ptr<td_api::authorizationStateWaitCode> state) {
  td_api::object_ptr<td_api::authenticationCodeInfo> &auth_info = state->code_info_;
  td_api::object_ptr<td_api::AuthenticationCodeType> &auth_type = auth_info->type_;

  std::int32_t timeout_seconds = auth_info->timeout_;
  std::ostringstream des_stream;

  switch (auth_type->get_id()) {
    case td_api::authenticationCodeTypeSms::ID:
      des_stream << "Input code from SMS";
      break;

    case td_api::authenticationCodeTypeTelegramMessage::ID:
      des_stream << "Input code from other telegram client";
      break;

    case td_api::authenticationCodeTypeCall::ID:
      des_stream << "Input code from ARS Call";
      break;

    default:
      des_stream << "Input Authentication Code";
      break;
  }

  des_stream << "\n" << "It exprires in " << timeout_seconds << "secs";

  code_popup->set_description (des_stream.str().c_str());

  elm_naviframe_item_simple_push (ap->naviframe, code_popup->get_popup ());
  evas_object_show (code_popup->get_popup ());

  // TODO: Add expire timer.
  // TODO: Show information of next tries.
}

void
app_auth::wait_terms_of_service(td_api::object_ptr<td_api::authorizationStateWaitRegistration> state) {
  td_api::object_ptr<td_api::termsOfService> &terms_of_service = state->terms_of_service_;
  td_api::object_ptr<td_api::formattedText> &text = terms_of_service->text_;

  std::string &tstr = text->text_;
  std::ostringstream ntext;
  std::ostream_iterator<std::string> ntext_iter (ntext, "<br>");

  std::string::size_type head = 0;
  std::string::size_type nhead = 0;

  for (nhead = tstr.find ('\n', head); nhead != std::string::npos; nhead = tstr.find ('\n', head)) {
    *ntext_iter = tstr.substr (head, nhead - head);
    head = nhead + 1;
    ntext_iter++;
  }

  elm_object_text_set (terms_of_serv_label_content, ntext.str().c_str());
  // TODO: Apply format on terms of service.

  elm_naviframe_item_simple_push (ap->naviframe, terms_of_serv_scroll);
  evas_object_show (terms_of_serv_scroll);
}

void
app_auth::wait_registration () {
  elm_naviframe_item_simple_push (ap->naviframe, register_popup);
  evas_object_show (register_popup);
}

void
app_auth::prepare_phone_number_popup() {
  phone_number_popup = new ui_entry_page (ap->naviframe);

  Evas_Object *entry = phone_number_popup->get_entry ();
  elm_entry_single_line_set (entry, EINA_TRUE);
  elm_entry_input_panel_layout_set (entry, ELM_INPUT_PANEL_LAYOUT_PHONENUMBER);

  Evas_Object *button1 = phone_number_popup->get_button1 ();
  elm_object_text_set (button1, "Ok");
  evas_object_smart_callback_add (button1, "clicked", &app_auth::callback_phone_number_accept, this);

  phone_number_popup->set_title ("Login");
  phone_number_popup->set_description ("Input Phone Number to login");
  phone_number_popup->set_placeholder ("Phone Number");
}

void
app_auth::prepare_code_popup() {
  code_popup = new ui_entry_page (ap->naviframe);

  Evas_Object *entry = code_popup->get_entry ();
  elm_entry_single_line_set (entry, EINA_TRUE);
  // Allow input alphabet for now.

  Evas_Object *button1 = code_popup->get_button1 ();
  elm_object_text_set (button1, "Ok");
  evas_object_smart_callback_add (button1, "clicked", &app_auth::callback_code_accept, this);

  code_popup->set_title ("Code");
  code_popup->set_description ("Input Authentication Code");
  code_popup->set_placeholder ("Code");
}

void
app_auth::prepare_register_popup() {
  terms_of_serv_scroll = elm_scroller_add (ap->naviframe);
  terms_of_serv_cscroll = eext_circle_object_scroller_add (terms_of_serv_scroll, ap->circle_surface);
  elm_scroller_policy_set (terms_of_serv_scroll, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
  eext_circle_object_scroller_policy_set (terms_of_serv_scroll, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);

  terms_of_serv_box = elm_box_add (terms_of_serv_scroll);
  elm_object_content_set (terms_of_serv_scroll, terms_of_serv_box);
  evas_object_show (terms_of_serv_box);

  terms_of_serv_label_title = elm_label_add (terms_of_serv_box);
  evas_object_size_hint_weight_set (terms_of_serv_label_title, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set (terms_of_serv_label_title, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_padding_set (terms_of_serv_label_title, 0, 0, 120, 40);
  elm_object_text_set (terms_of_serv_label_title, "Terms of Service<br>from Telegram");
  elm_object_style_set (terms_of_serv_label_title, "marker");
  elm_box_pack_end (terms_of_serv_box, terms_of_serv_label_title);
  evas_object_show (terms_of_serv_label_title);

  terms_of_serv_label_content = elm_label_add (terms_of_serv_box);
  evas_object_size_hint_weight_set (terms_of_serv_label_content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set (terms_of_serv_label_content, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_padding_set (terms_of_serv_label_content, 40, 40, 0, 0);
  elm_label_line_wrap_set (terms_of_serv_label_content, ELM_WRAP_MIXED);
  elm_box_pack_end (terms_of_serv_box, terms_of_serv_label_content);
  evas_object_show (terms_of_serv_label_content);

  terms_of_serv_button = elm_button_add (terms_of_serv_box);
  evas_object_size_hint_weight_set (terms_of_serv_button, EVAS_HINT_EXPAND, 0);
  evas_object_size_hint_align_set (terms_of_serv_button, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_object_text_set (terms_of_serv_button, "Agree");
  elm_object_style_set (terms_of_serv_button, "bottom");
  evas_object_smart_callback_add (terms_of_serv_button, "clicked", &app_auth::callback_terms_of_service_agree, this);
  elm_box_pack_end (terms_of_serv_box, terms_of_serv_button);
  evas_object_show (terms_of_serv_button);


  register_popup = elm_popup_add (ap->naviframe);
  elm_object_style_set (register_popup, "circle");

  register_layout = elm_layout_add (register_popup);
  elm_layout_theme_set (register_layout, "layout", "popup", "circle/popup/button1");
  elm_object_part_text_set (register_layout, "elm.text.title", "Registration");
  elm_object_content_set (register_popup, register_layout);

  register_box = elm_box_add (register_layout);
  elm_box_padding_set (register_box, 10, 10);
  elm_object_content_set (register_layout, register_box);

  register_descr = elm_label_add (register_box);
  evas_object_size_hint_align_set (register_descr, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set (register_descr, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  elm_object_text_set (register_descr, "Input your name.");
  elm_label_line_wrap_set (register_descr, ELM_WRAP_MIXED);
  elm_box_pack_end (register_box, register_descr);
  evas_object_show (register_descr);

  register_first_name = new ui_entry_button (register_box, ap->naviframe);
  register_first_name->set_placeholder("First Name");

  Evas_Object *button_first_name = register_first_name->get_button();
  evas_object_size_hint_align_set (button_first_name, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set (button_first_name, EVAS_HINT_EXPAND, 0);
  elm_box_pack_end (register_box, button_first_name);
  evas_object_show (button_first_name);

  Evas_Object *entry_first_name = register_first_name->get_entry();
  elm_entry_single_line_set (entry_first_name, EINA_TRUE);

  register_last_name = new ui_entry_button (register_box, ap->naviframe);
  register_last_name->set_placeholder("Last Name");
  Evas_Object *button_last_name = register_last_name->get_button();
  evas_object_size_hint_align_set (button_last_name, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set (button_last_name, EVAS_HINT_EXPAND, 0);
  elm_box_pack_end (register_box, button_last_name);
  evas_object_show (button_last_name);

  Evas_Object *entry_last_name = register_last_name->get_entry();
  elm_entry_single_line_set (entry_last_name, EINA_TRUE);

  register_accept = elm_button_add (register_popup);
  elm_object_style_set (register_accept, "popup/circle/bottom");
  elm_object_text_set (register_accept, "Register");
  evas_object_smart_callback_add (register_accept, "clicked",
      &app_auth::callback_register_accept, this);
  elm_object_part_content_set (register_popup, "button1", register_accept);
}


void
app_auth::callback_phone_number_accept (void *data, Evas_Object *object, void *event_info) {
  app_auth *self = (app_auth*) data;

  td_api::object_ptr<td_api::phoneNumberAuthenticationSettings> settings;
  settings = td_api::make_object<td_api::phoneNumberAuthenticationSettings> ();

  td_api::object_ptr<td_api::setAuthenticationPhoneNumber> tdfunc;
  tdfunc = td_api::make_object<td_api::setAuthenticationPhoneNumber> (
      self->phone_number_popup->get_text(),  // phone number
      std::move(settings));                  // authentification settings

  self->ap->send (std::move(tdfunc));
}

void
app_auth::callback_code_accept (void *data, Evas_Object *object, void *event_info) {
  app_auth *self = (app_auth*) data;

  td_api::object_ptr<td_api::checkAuthenticationCode> tdfunc;
  tdfunc = td_api::make_object<td_api::checkAuthenticationCode> (
      self->code_popup->get_text());  // code

  self->ap->send (std::move(tdfunc));
}

void
app_auth::callback_terms_of_service_agree (void *data, Evas_Object *object, void *event_info) {
  app_auth *self = (app_auth*) data;

  td_api::object_ptr<td_api::acceptTermsOfService> tdfunc;
  tdfunc = td_api::make_object<td_api::acceptTermsOfService> ();

  self->ap->send (std::move(tdfunc),
    [self] (td_api::object_ptr<td_api::ok> result) {
      // TODO: Handle error case.
      self->wait_registration();
    });
}

void
app_auth::callback_register_accept (void *data, Evas_Object *object, void *event_info) {
  app_auth *self = (app_auth*) data;

  td_api::object_ptr<td_api::registerUser> tdfunc;
  tdfunc = td_api::make_object<td_api::registerUser> (
      self->register_first_name->get_text(),
      self->register_last_name->get_text());

  self->ap->send (std::move (tdfunc));
}
