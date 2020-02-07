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

typedef struct _ChatListPage {
	Evas_Object *chat_genlist;
	Evas_Object *chat_cgenlist;
} ChatListPage;

/* ---- Chat List Page initialization / finalization */

void chat_list_page_init (ChatListPage *self, Evas_Object *parent, Eext_Circle_Surface *circle_surface);

void chat_list_page_fini (ChatListPage *self);

#endif //CHAT_LIST_PAGE_H_
