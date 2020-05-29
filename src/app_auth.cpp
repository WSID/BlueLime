#include "app_auth.hpp"

// C++ std include
#include <utility>
#include <future>

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

app_auth::app_auth (app *ap) : ap(ap) {}

void
app_auth::handle (td_api::object_ptr<td_api::updateAuthorizationState> state) {
  switch (state->authorization_state_->get_id()) {
    case td_api::authorizationStateWaitTdlibParameters::ID:
      return wait_tdlib_parameters();

    case td_api::authorizationStateWaitEncryptionKey::ID:
      return wait_encryption_key();

    default:
      dlog_print (DLOG_WARN, "bluelime", "Unhandled Auth State ... \n%s", to_string(state).c_str());
  }
}

void
app_auth::wait_tdlib_parameters() {
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
    // do nothing here.
}
