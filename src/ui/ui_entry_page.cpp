#include "ui/ui_entry_page.hpp"

ui_entry_page::ui_entry_page(Evas_Object* naviframe) :
  naviframe (naviframe),
  popup (elm_popup_add(naviframe)),
  layout (elm_layout_add (popup)),
  box (elm_box_add(layout)),
  label (elm_label_add (box)),
  subui_entry_button (box, naviframe)
{
  // popup
  elm_object_style_set (popup, "circle");
  elm_object_content_set (popup, layout);

  // layout
  elm_layout_theme_set (layout, "layout", "popup", "content/circle/buttons1");
  elm_object_content_set (layout, box);

  // box
  elm_box_pack_end (box, label);
  elm_box_pack_end (box, subui_entry_button.get_button());
  evas_object_show (box);

  // label
  evas_object_show (label);
  evas_object_size_hint_weight_set (label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set (label, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_label_line_wrap_set (label, ELM_WRAP_MIXED);

  // popup button
  Evas_Object *button = subui_entry_button.get_button ();
  evas_object_show (button);
  evas_object_size_hint_weight_set (button, EVAS_HINT_EXPAND, 0.0);
  evas_object_size_hint_align_set (button, EVAS_HINT_FILL, EVAS_HINT_FILL);

  // Action button 1
  button1 = elm_button_add (popup);
  elm_object_style_set (button1, "popup/circle/bottom");
  elm_object_part_content_set (popup, "button1", button1);
}

Evas_Object *ui_entry_page::get_popup () {
  return popup;
}

Evas_Object *ui_entry_page::get_layout () {
  return layout;
}

Evas_Object *ui_entry_page::get_entry () {
  return subui_entry_button.get_entry();
}

Evas_Object *ui_entry_page::get_button1 () {
  return button1;
}

const char *ui_entry_page::get_title () const {
  return elm_object_part_text_get (layout, "elm.text.titlte");
}

void ui_entry_page::set_title (const char *title) {
  elm_object_part_text_set (layout, "elm.text.title", title);
}

const char *ui_entry_page::get_description () const {
  return elm_object_text_get (label);
}

void ui_entry_page::set_description (const char *description) {
  elm_object_text_set (label, description);
}

const char *ui_entry_page::get_text () const {
  return subui_entry_button.get_text ();
}

void ui_entry_page::set_text(const char *text) {
  subui_entry_button.set_text (text);
}

const char *ui_entry_page::get_placeholder () const {
  return subui_entry_button.get_placeholder ();
}

void ui_entry_page::set_placeholder (const char *placeholder) {
  subui_entry_button.set_placeholder (placeholder);
}

