/*
 * Wissle (c) 2020.
 *
 * Currently no license is carried.
 */

#ifndef CHAT_LIST_PAGE_H_
#define CHAT_LIST_PAGE_H_

#include <memory>

#include <app.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <dlog.h>

#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include "uiutil.hpp"

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "bluelime"

#if !defined(PACKAGE)
#define PACKAGE "standalone.bluelime"
#endif


class app;

/**
 * Chat List Pages
 *
 * * What End-user expected to do.
 *
 *  * Create new chat / channel
 *  * Enter already opened chat / channel.
 */

class ChatListPage {
public:
  ChatListPage (app* ap, Evas_Object *naviframe, Eext_Circle_Surface *circle_surface);
  ~ChatListPage ();

  void update_terms_of_service(td::td_api::object_ptr<td::td_api::updateTermsOfService> res);

	Evas_Object *chat_genlist;
	Evas_Object *chat_cgenlist;


private:
  app* ap;

  Evas_Object *naviframe;
  ui_agreement_popup update_terms_of_serv_popup;
};
#endif //CHAT_LIST_PAGE_H_
