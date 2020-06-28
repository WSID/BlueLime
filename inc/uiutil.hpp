#ifndef __UI_UTIL_H__
#define __UI_UTIL_H__

// C++ stdlib include
#include <functional>
#include <string>
#include <sstream>

// Telegram includes
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

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
