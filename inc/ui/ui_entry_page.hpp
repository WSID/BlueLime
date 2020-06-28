#ifndef __UI_ENTRY_PAGE_H__
#define __UI_ENTRY_PAGE_H__

// C++ stdlib include
#include <functional>
#include <string>
#include <sstream>

// Enlightenment includes
#include <Elementary.h>

// EFL Extension includes
#include <efl_extension.h>

// Internal includes
#include "ui/ui_entry_button.hpp"

/**
 * ui_entry_page:
 *
 * A UI fragment for a page, to receive text input with entry button.
 */
class ui_entry_page {
  public:
  ui_entry_page (Evas_Object* naviframe);

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

#endif //UI_ENTRY_PAGE_H
