#ifndef __UI_UTIL_H__
#define __UI_UTIL_H__

// C++ stdlib include
#include <functional>
#include <string>
#include <sstream>

// Enlightenment includes
#include <Elementary.h>

// EFL Extension includes
#include <efl_extension.h>

// Telegram includes
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>


class ui_entry_button {
  public:
  ui_entry_button(Evas_Object* parent, Evas_Object* naviframe);

  Evas_Object *get_button();
  Evas_Object *get_entry();

  const char *get_text () const;
  void set_text(const char *text);

  const char *get_placeholder() const;
  void set_placeholder(const char *text);

  private:
  static void callback_button_clicked(void *data, Evas_Object* object, void *event_info);
  static void callback_entry_changed(void *data, Evas_Object* object, void *event_info);

  Evas_Object *naviframe;
  Evas_Object *button;
  Evas_Object *box;
  Evas_Object *entry;
};

class ui_entry_popup {
  public:
  ui_entry_popup(Evas_Object* naviframe);

  Evas_Object *get_popup();
  Evas_Object *get_layout();
  Evas_Object *get_entry();
  Evas_Object *get_button1();

  const char *get_title () const;
  void set_title (const char *title);

  const char *get_description () const;
  void set_description (const char *description);

  const char *get_text () const;
  void set_text(const char *text);

  const char *get_placeholder() const;
  void set_placeholder(const char *text);

  private:
  Evas_Object *naviframe;
  Evas_Object *popup;
  Evas_Object *layout;
  Evas_Object *box;
  Evas_Object *label;
  ui_entry_button subui_entry_button;
  Evas_Object *button1;
};

class ui_agreement_popup {
  public:
  ui_agreement_popup (Evas_Object* naviframe, Eext_Circle_Surface* circle_surface);

  Evas_Object *get_popup();

  void set_description (const char *description);

  void set_content (const char *content);

  std::function<void()> on_agree;

  private:
  static void on_button_clicked (void *data, Evas_Object *object, void *event_info);

  static void on_link_clicked (void *data, Evas_Object *object, void *event_info);

  Evas_Object *naviframe;
  Evas_Object *scroll;
  Evas_Object *cscroll;
  Evas_Object *box;
  Evas_Object *label_descr;
  Evas_Object *entry_content;
  Evas_Object *button;
};

/**
 * decorate_string:
 *
 * Decorates string for EFL formatted string, from telegram's formatted text.
 *
 * @formatted_text: (in): A Telegram's formatted text.
 *
 * @returns: A decorated string with tags.
 */
std::string decorate_string (const td::td_api::formattedText& formatted_text);

std::ostream& decorate_in (
    std::ostream& os,
    const td::td_api::TextEntityType& entity_type);

std::ostream& decorate_out (
    std::ostream& os,
    const td::td_api::TextEntityType& entity_type);
#endif //UI_UTIL_H
