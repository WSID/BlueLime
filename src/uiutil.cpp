#include "uiutil.hpp"

#include <map>

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
