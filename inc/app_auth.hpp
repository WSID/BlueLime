/*
 * Wissle. 2020
 *
 * Currently, no license is carried.
 */

#ifndef __APP_AUTH_H__
#define __APP_AUTH_H__

// tdlib includes
#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

// local include
#include "bluelime.hpp"

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
  /*

  void wait_other_device_confirmation ();

  void wait_code ();

  void wait_registration ();

  void wait_state_ready ();

  //
  //
  void wait_password ();

  void logging_out ();

  void closing ();

  void closed ();
   */
};

#endif //APP_AUTH_H
