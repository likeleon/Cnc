#pragma once

#include "cnc/string.h"

namespace cnc {

template <typename TSource, typename TKey, typename TElement>
std::map<TKey, TElement> ToMapWithConflictLog(
  const std::vector<TSource>& source,
  const std::function<TKey (const TSource&)>& key_selector,
  const std::function<TElement (const TSource&)>& element_selector,
  const std::string& debug_name,
  const std::function<std::string(const TKey&)>& log_key,
  const std::function<std::string(const TSource&)>& log_value) {
  std::map<TKey, std::vector<std::string>> dup_keys;
  std::map<TKey, TElement> map;
  for (const auto& item : source) {
    auto key = key_selector(item);
    auto element = element_selector(item);

    if (map.find(key) != map.end()) {
      if (dup_keys.find(key) == dup_keys.end()) {
        dup_keys.emplace(key, { log_value(map.at(key)) });
      }
      dup_keys.at(key).emplace_back(log_value(element));
      continue;
    }

    map.emplace(key, element);
  }

  if (!dup_keys.empty()) {
    std::vector<std::string> msgs;
    for (const auto& kvp : dup_keys) {
      std::ostringstream oss;
      oss << log_key(kvp.first) << ": [" + String::Join(kvp.second, ",") + "]";
      msgs.emplace_back(oss.str());
    }

    auto bad_keys_formatted = String::Join(msgs, ", ");
    std::ostringstream oss;
    oss << debug_name << ", duplicate values found for the following keys: " << bad_keys_formatted;
    throw Error(MSG(oss.str()));
  }

  return map;
}

template <typename TSource, typename TKey>
std::map<TKey, TSource> ToMapWithConflictLog(
  const std::vector<TSource>& source,
  const std::function<TKey(const TSource&)>& key_selector,
  const std::string& debug_name,
  const std::function<std::string(const TKey&)>& log_key,
  const std::function<std::string(const TSource&)>& log_value) {
  auto source_identity = [](const TSource& x) { return x; };
  return ToMapWithConflictLog<TSource, TKey, TSource>(source, key_selector, source_identity, debug_name, log_key, log_value);
}

}