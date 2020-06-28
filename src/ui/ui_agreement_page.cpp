#include "ui/ui_agreement_page.hpp"

#include <dlog.h>
#include <app_control.h>

void ui_agreement_page::def_anchor_clicked_webbrowser (const char *href) {
  app_control_h app_control;
  app_control_create (&app_control);
  app_control_set_operation (app_control, APP_CONTROL_OPERATION_VIEW);
  app_control_set_uri(app_control, href);
  if (app_control_send_launch_request (app_control, nullptr, nullptr) != APP_CONTROL_ERROR_NONE) {
    dlog_print(DLOG_INFO, "bluelime", "Failed to request a view app-control.");
  }
  app_control_destroy (app_control);
}

ui_agreement_page::ui_agreement_page(Evas_Object* naviframe, Eext_Circle_Surface* circle_surface)
  : naviframe (naviframe),
    scroll (elm_scroller_add (naviframe)),
    cscroll (eext_circle_object_scroller_add (scroll, circle_surface)),
    box (elm_box_add (scroll)),
    label_descr (elm_label_add(box)),
    entry_content (elm_entry_add(box)),
    button (elm_button_add (box))
{
  // scroll
  elm_scroller_policy_set (scroll,
      ELM_SCROLLER_POLICY_OFF,
      ELM_SCROLLER_POLICY_OFF);
  elm_object_content_set (scroll, box);

  // cscroll
  eext_circle_object_scroller_policy_set (cscroll,
      ELM_SCROLLER_POLICY_OFF,
      ELM_SCROLLER_POLICY_AUTO);
  evas_object_show (cscroll);

  // box
  elm_box_padding_set (box, 20, 20);
  elm_box_pack_end (box, label_descr);
  elm_box_pack_end (box, entry_content);
  elm_box_pack_end (box, button);
  evas_object_show (box);

  // label_descr
  evas_object_size_hint_padding_set (label_descr, 40, 40, 160, 0);
  evas_object_size_hint_align_set (label_descr, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set (label_descr, EVAS_HINT_EXPAND, 0);
  elm_label_line_wrap_set (label_descr, ELM_WRAP_MIXED);
  evas_object_show (label_descr);

  // entry_content
  evas_object_size_hint_padding_set (entry_content, 40, 40, 0, 0);
  evas_object_size_hint_align_set (entry_content, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set (entry_content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  elm_entry_line_wrap_set (entry_content, ELM_WRAP_MIXED);
  elm_entry_editable_set(entry_content, EINA_FALSE);
  evas_object_smart_callback_add (entry_content, "anchor,clicked", &ui_agreement_page::on_link_clicked, this);
  evas_object_show (entry_content);

  // button
  evas_object_size_hint_align_set (button, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set (button, EVAS_HINT_EXPAND, 0);
  elm_object_text_set (button, "accept");
  elm_object_style_set (button, "bottom");
  evas_object_smart_callback_add (button, "clicked", &ui_agreement_page::on_button_clicked, &on_agree);
  evas_object_show (button);
}

Evas_Object *ui_agreement_page::get_popup () {
  return scroll;
}

void ui_agreement_page::set_description (const char *description) {
  elm_object_text_set (label_descr, description);
}

void ui_agreement_page::set_content (const char *content) {
  elm_object_text_set (entry_content, content);
}

void ui_agreement_page::on_button_clicked (void *data, Evas_Object *object, void *event_info) {
  std::function<void ()> &callable = *(std::function<void ()>*)data;

  if (callable) {
    callable ();
  }
}

void ui_agreement_page::on_link_clicked (void *data, Evas_Object *object, void *event_info) {
  ui_agreement_page* self = (ui_agreement_page*)data;
  Elm_Entry_Anchor_Info *anchor_info = (Elm_Entry_Anchor_Info*)event_info;

  self->on_anchor_clicked(anchor_info->name);
}

