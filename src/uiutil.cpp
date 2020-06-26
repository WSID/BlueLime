#include "uiutil.hpp"

#include <map>

#include <app.h>
#include <dlog.h>

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



ui_agreement_popup::ui_agreement_popup(Evas_Object* naviframe, Eext_Circle_Surface* circle_surface)
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
  evas_object_smart_callback_add (entry_content, "anchor,clicked", &ui_agreement_popup::on_link_clicked, this);
  evas_object_show (entry_content);

  // button
  evas_object_size_hint_align_set (button, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_size_hint_weight_set (button, EVAS_HINT_EXPAND, 0);
  elm_object_text_set (button, "accept");
  elm_object_style_set (button, "bottom");
  evas_object_smart_callback_add (button, "clicked", &ui_agreement_popup::on_button_clicked, &on_agree);
  evas_object_show (button);
}

Evas_Object *ui_agreement_popup::get_popup () {
  return scroll;
}

void ui_agreement_popup::set_description (const char *description) {
  elm_object_text_set (label_descr, description);
}

void ui_agreement_popup::set_content (const char *content) {
  elm_object_text_set (entry_content, content);
}

void ui_agreement_popup::on_button_clicked (void *data, Evas_Object *object, void *event_info) {
  std::function<void ()> &callable = *(std::function<void ()>*)data;

  if (callable) {
    callable ();
  }
}

void ui_agreement_popup::on_link_clicked (void *data, Evas_Object *object, void *event_info) {
  Elm_Entry_Anchor_Info *anchor_info = (Elm_Entry_Anchor_Info*)event_info;
  dlog_print(DLOG_VERBOSE, "bluelime", "clicked anchor: %s", anchor_info->name);

  // Launch web browser.
  app_control_h app_control;
  app_control_create (&app_control);
  app_control_set_operation (app_control, APP_CONTROL_OPERATION_VIEW);
  app_control_set_uri(app_control, anchor_info->name);
  app_control_send_launch_request (app_control, nullptr, nullptr);
  app_control_destroy (app_control);
}


std::string decorate_string (const td::td_api::formattedText& formatted_text) {
  std::multimap<int, int> map_pos_idx;
  // A mapping from pos, to index
  // - position on formatted string.
  // - index to format text entry.

  // Build up break point maps.
  int n = formatted_text.entities_.size();
  for (int i = 0; i < n; ++i) {
    const td::td_api::object_ptr<td::td_api::textEntity> &entity = formatted_text.entities_[i];
    map_pos_idx.emplace(entity->offset_, i + 1);
  }

  for (int i = n - 1; 0 <= i; --i) {
    const td::td_api::object_ptr<td::td_api::textEntity> &entity = formatted_text.entities_[i];
    map_pos_idx.emplace(entity->offset_ + entity->length_, -i - 1);
  }

  // Iterate over breakage.
  std::ostringstream result_builder;
  std::multimap<int, int>::const_iterator iter;
  std::multimap<int, int>::const_iterator end;
  iter = map_pos_idx.cbegin();
  end = map_pos_idx.cend();



  int head = 0;
  while (iter != end) {
    int nhead = iter->first;

    if (head < nhead) {
      result_builder << formatted_text.text_.substr (head, nhead - head);
      head = nhead;
    }

    if (iter->second < 0) {
      int idx = (-iter->second) - 1;
      decorate_out (result_builder, *formatted_text.entities_[idx]->type_.get());
    }

    else if (0 < iter->second) {
      int idx = iter->second - 1;
      decorate_in (result_builder, *formatted_text.entities_[idx]->type_.get());
    }

    iter ++;
  }

  result_builder << formatted_text.text_.substr (head);

  // Replace newline with <br>
  std::ostringstream br_result_builder;
  std::string decorated = result_builder.str();
  std::string::size_type br_head = 0;
  std::string::size_type br_nhead = decorated.find('\n', 0);
  while (br_nhead != std::string::npos) {
    br_result_builder << decorated.substr(br_head, br_nhead - br_head) << "<br>";
    br_head = br_nhead + 1;
    br_nhead = decorated.find('\n', br_head);
  }
  br_result_builder << decorated.substr(br_head);

  return br_result_builder.str();
}

std::ostream& decorate_in (
    std::ostream& os,
    const td::td_api::TextEntityType& entity_type)
{
  // TODO: We can add custom tag for EFL to process, through theme set.
  // TODO: Some attribute may be used to make links!
  switch (entity_type.get_id()) {
  case td::td_api::textEntityTypeBold::ID:
    return os << "<font_weight=bold>";

  case td::td_api::textEntityTypeBotCommand::ID:
    return os << "<font=Monospace>";

  case td::td_api::textEntityTypeCashtag::ID:
    return os << "<font_width=expanded>"; // <cashtag>

  case td::td_api::textEntityTypeCode::ID:
    return os << "<font=Monospace>";

  case td::td_api::textEntityTypeEmailAddress::ID:
    return os << "<font_style=Italic>"; // <a href="mailto://...">

  case td::td_api::textEntityTypeHashtag::ID:
    return os << "<link>"; // <hashtag>

  case td::td_api::textEntityTypeItalic::ID:
    return os << "<em>";

  case td::td_api::textEntityTypeMention::ID:
    return os << "<keyword>"; // <mention>

  case td::td_api::textEntityTypeMentionName::ID:
    return os << "<keyword>"; // <mentionname>

  case td::td_api::textEntityTypePhoneNumber::ID:
    return os << "<number>"; // <phonenumber>

  case td::td_api::textEntityTypePre::ID:
    return os << "<code>"; // <pre>

  case td::td_api::textEntityTypePreCode::ID:
    return os << "<code>"; // <pre> code

  case td::td_api::textEntityTypeStrikethrough::ID:
    return os << "<link>"; // strikethrough

  case td::td_api::textEntityTypeTextUrl::ID:
    {
      const td::td_api::textEntityTypeTextUrl& entity_text_url =
        static_cast<const td::td_api::textEntityTypeTextUrl&>(entity_type);
      return os << "<underline=on><color=#8CF><a href=" << entity_text_url.url_ << ">";
    }

  case td::td_api::textEntityTypeUnderline::ID:
    return os << "<link>"; // Underline

  case td::td_api::textEntityTypeUrl::ID:
    return os << "<link>"; // Link
  }

  return os;
}

std::ostream& decorate_out (
    std::ostream& os,
    const td::td_api::TextEntityType& entity_type)
{
  switch (entity_type.get_id()) {
  case td::td_api::textEntityTypeBold::ID:
    return os << "</font_weight>";

  case td::td_api::textEntityTypeBotCommand::ID:
    return os << "</font>";

  case td::td_api::textEntityTypeCashtag::ID:
    return os << "</font_width>"; // <cashtag>

  case td::td_api::textEntityTypeCode::ID:
    return os << "</font>";

  case td::td_api::textEntityTypeEmailAddress::ID:
    return os << "</font_style>"; // <a href="mailto://...">

  case td::td_api::textEntityTypeHashtag::ID:
    return os << "</link>"; // <hashtag>

  case td::td_api::textEntityTypeItalic::ID:
    return os << "</em>";

  case td::td_api::textEntityTypeMention::ID:
    return os << "</keyword>"; // <mention>

  case td::td_api::textEntityTypeMentionName::ID:
    return os << "</keyword>"; // <mentionname>

  case td::td_api::textEntityTypePhoneNumber::ID:
    return os << "</number>"; // <phonenumber>

  case td::td_api::textEntityTypePre::ID:
    return os << "</code>"; // <pre>

  case td::td_api::textEntityTypePreCode::ID:
    return os << "</code>"; // <pre> code

  case td::td_api::textEntityTypeStrikethrough::ID:
    return os << "</link>"; // strikethrough

  case td::td_api::textEntityTypeTextUrl::ID:
    return os << "</a></color></underline>";

  case td::td_api::textEntityTypeUnderline::ID:
    return os << "</link>"; // Underline

  case td::td_api::textEntityTypeUrl::ID:
    return os << "</link>"; // Link
  }

  return os;
}
