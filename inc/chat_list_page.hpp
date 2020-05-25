/*
 * Wissle (c) 2020.
 *
 * Currently no license is carried.
 */

#ifndef CHAT_LIST_PAGE_H_
#define CHAT_LIST_PAGE_H_

#include <app.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "bluelime"

#if !defined(PACKAGE)
#define PACKAGE "standalone.bluelime"
#endif


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
  ChatListPage (Evas_Object *parent, Eext_Circle_Surface *circle_surface);
  ~ChatListPage ();

	Evas_Object *chat_genlist;
	Evas_Object *chat_cgenlist;

};
#endif //CHAT_LIST_PAGE_H_
