#ifndef __UI_ENTRY_BUTTON_H__
#define __UI_ENTRY_BUTTON_H__

// C++ stdlib include
#include <functional>
#include <string>
#include <sstream>

// Enlightenment includes
#include <Elementary.h>

// EFL Extension includes
#include <efl_extension.h>

/**
 * ui_entry_button:
 *
 * An UI fragment for a button, which shows input button for text.
 */
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

#endif //UI_ENTRY_BUTTON_H
