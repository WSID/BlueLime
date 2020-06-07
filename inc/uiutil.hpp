#ifndef __UI_UTIL_H__
#define __UI_UTIL_H__

// C++ stdlib include
#include <string>

// Enlightenment includes
#include <Elementary.h>

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

#endif //UI_UTIL_H
