#ifndef __UI_AGREEMENT_PAGE_H__
#define __UI_AGREEMENT_PAGE_H__

// C++ stdlib include
#include <functional>
#include <string>
#include <sstream>

// Enlightenment includes
#include <Elementary.h>

// EFL Extension includes
#include <efl_extension.h>

/**
 * ui_agreement_page:
 *
 * A UI fragment for a page, to provide contents for agreement.
 */
class ui_agreement_page {
  public:
  /**
   * def_anchor_clicked_webbrowser
   *
   * Tries to launch web page for given anchor name.
   * This function can be fed as 'on_anchor_clicked'.
   */
  static void def_anchor_clicked_webbrowser(const char *href);

  ui_agreement_page (Evas_Object* naviframe, Eext_Circle_Surface* circle_surface);

  Evas_Object *get_popup();

  void set_description (const char *description);

  void set_content (const char *content);

  std::function<void()> on_agree;

  std::function<void(const char*)> on_anchor_clicked;

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

#endif //UI_AGREEMENT_PAGE_H
