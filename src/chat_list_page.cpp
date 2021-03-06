#include "chat_list_page.hpp"

#include <string.h>

#include <string>

#include <dlog.h>

#include "bluelime.hpp"
#include "uiutil.hpp"

/* ---- Callbacks: Chat Title getters for Genlist */
static Evas_Object *_title_genlist (void *data, Evas_Object *genlist, const char *part);

static char *_chat_title_genlist (void *data, Evas_Object *genlist, const char *part);

ChatListPage::ChatListPage (app_client& client,
                            Evas_Object *naviframe,
                            Eext_Circle_Surface *circle_surface)
  : client(client),
    naviframe(naviframe),
    update_terms_of_serv_page (naviframe, circle_surface)
{
	chat_genlist = elm_genlist_add(naviframe);
	chat_cgenlist = eext_circle_object_genlist_add(chat_genlist, circle_surface);

	eext_circle_object_genlist_scroller_policy_set(chat_cgenlist,
			ELM_SCROLLER_POLICY_OFF,
			ELM_SCROLLER_POLICY_AUTO);
	eext_rotary_object_event_activated_set(chat_cgenlist, EINA_TRUE);

	Elm_Genlist_Item_Class *ic_padding = elm_genlist_item_class_new();
	ic_padding->item_style = "padding";

	Elm_Genlist_Item_Class *ic_title = elm_genlist_item_class_new();
	ic_title->item_style = "full";
	ic_title->func.content_get = _title_genlist;

	Elm_Genlist_Item_Class *ic_chat = elm_genlist_item_class_new();
	ic_chat->item_style = "1text";
	ic_chat->func.text_get = _chat_title_genlist;
	// Fill dummy data for now.
	elm_genlist_item_append (chat_genlist, ic_padding, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_append (chat_genlist, ic_title, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_append (chat_genlist, ic_chat, (void *)"Asdf", NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_append (chat_genlist, ic_chat, (void *)"Quwer", NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_append (chat_genlist, ic_chat, (void *)"Zxcv", NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_append (chat_genlist, ic_chat, (void *)"VBNM", NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_append (chat_genlist, ic_padding, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);

	elm_genlist_item_class_unref (ic_chat);

	update_terms_of_serv_page.set_description ("<title>Terms of Service is updated</title>");
}

ChatListPage::~ChatListPage() {
	evas_object_unref (chat_genlist);
	evas_object_unref (chat_cgenlist);
}

void ChatListPage::update_terms_of_service (td::td_api::object_ptr<td::td_api::updateTermsOfService> res) {
  std::string terms_of_serv_id = res->terms_of_service_id_;
  std::ostringstream content;
  content << "<font_size=24>";
  content << decorate_string(*res->terms_of_service_->text_.get());
  content << "</font_size>";


  update_terms_of_serv_page.set_content(content.str().c_str());
  update_terms_of_serv_page.on_agree = [terms_of_serv_id, this] () {
    td::td_api::object_ptr<td::td_api::acceptTermsOfService> accept;
    accept = td::td_api::make_object<td::td_api::acceptTermsOfService>(terms_of_serv_id);

    client.send(std::move(accept), [this](auto anything) {
      elm_naviframe_item_pop (naviframe);
    });
  };

  elm_naviframe_item_simple_push(naviframe, update_terms_of_serv_page.get_popup());
  evas_object_show (update_terms_of_serv_page.get_popup());

  dlog_print(DLOG_VERBOSE, "bluelime", "Decorated text:...\n%s", content.str().c_str());
}

static Evas_Object *_title_genlist (void *data, Evas_Object *genlist, const char *part) {
	Evas_Object *v_box = elm_box_add(genlist);
	elm_box_horizontal_set(v_box, EINA_FALSE);

	Evas_Object *h_box = elm_box_add(v_box);
	elm_box_horizontal_set(h_box, EINA_TRUE);

	Evas_Object *button = elm_button_add (h_box);
	Evas_Object *title = elm_label_add(v_box);
	elm_object_text_set (title, "Blue Lime");

	elm_box_pack_start (v_box, h_box);
	elm_box_pack_end (v_box, title);

	elm_box_pack_start (h_box, button);
	evas_object_show (v_box);
	evas_object_show (button);
	evas_object_show (title);

	return v_box;
}

static char *_chat_title_genlist(void *data, Evas_Object *genlist, const char *part) {
	char *title = (char *)data;
	return strndup(title, 128);
}
