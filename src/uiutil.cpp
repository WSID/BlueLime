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
