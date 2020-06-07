#include "uiutil.hpp"

ui_entry_button::ui_entry_button(Evas_Object* parent, Evas_Object* naviframe) :
  naviframe (naviframe),
  button (elm_button_add(parent)),
  box (elm_box_add(naviframe)),
  entry (elm_entry_add(box))
{
  // Button
  evas_object_smart_callback_add (button, "clicked", &callback_button_clicked, this);

  // box
  elm_box_pack_start (box, entry);

  // Entry
  evas_object_size_hint_padding_set (entry, 80, 80, 40, 40);
  evas_object_size_hint_weight_set (entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set (entry, EVAS_HINT_FILL, 0);
  evas_object_smart_callback_add (entry, "changed", &callback_entry_changed, button);
  evas_object_show (entry);
}

const char *ui_entry_button::get_text () const {
  return elm_entry_entry_get (entry);
}

void ui_entry_button::set_text (const char *text) {
  elm_entry_entry_set(entry, text);
}

const char *ui_entry_button::get_placeholder () const {
  return elm_object_part_text_get (entry, "guide");
}

void ui_entry_button::set_placeholder (const char *text) {
  elm_object_part_text_set (entry, "guide", text);

  if (elm_entry_is_empty (entry))
    elm_object_text_set (button, text);
}


Evas_Object *ui_entry_button::get_button () {
  return button;
}

Evas_Object *ui_entry_button::get_entry () {
  return entry;
}

void ui_entry_button::callback_button_clicked (void *data, Evas_Object *object, void *event_info) {
  ui_entry_button *self = (ui_entry_button*)data;
  Elm_Naviframe_Item *item = elm_naviframe_item_push(self->naviframe, NULL, NULL, NULL, self->box, "empty");

  evas_object_show(self->box);
}

void ui_entry_button::callback_entry_changed (void *data, Evas_Object *object, void *event_info) {
  Evas_Object *button = (Evas_Object*)data;
  elm_object_text_set (button,
      elm_entry_is_empty (object) ?
      elm_object_part_text_get (object, "guide") :
      elm_entry_entry_get (object));
}



ui_entry_popup::ui_entry_popup(Evas_Object* naviframe) :
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

Evas_Object *ui_entry_popup::get_popup () {
  return popup;
}

Evas_Object *ui_entry_popup::get_layout () {
  return layout;
}

Evas_Object *ui_entry_popup::get_entry () {
  return subui_entry_button.get_entry();
}

Evas_Object *ui_entry_popup::get_button1 () {
  return button1;
}

const char *ui_entry_popup::get_title () const {
  return elm_object_part_text_get (layout, "elm.text.titlte");
}

void ui_entry_popup::set_title (const char *title) {
  elm_object_part_text_set (layout, "elm.text.title", title);
}

const char *ui_entry_popup::get_description () const {
  return elm_object_text_get (label);
}

void ui_entry_popup::set_description (const char *description) {
  elm_object_text_set (label, description);
}

const char *ui_entry_popup::get_text () const {
  return subui_entry_button.get_text ();
}

void ui_entry_popup::set_text(const char *text) {
  subui_entry_button.set_text (text);
}

const char *ui_entry_popup::get_placeholder () const {
  return subui_entry_button.get_placeholder ();
}

void ui_entry_popup::set_placeholder (const char *placeholder) {
  subui_entry_button.set_placeholder (placeholder);
}

