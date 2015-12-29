#include "cnc/stdafx.h"
#include "cnc/evaluator.h"
#include "cnc/error.h"

namespace cnc {

static std::vector<std::string> Tokens(const std::string& expr, const std::string& ops) {
  std::vector<std::string> tokens;
  std::string s;
  for (const auto& c : expr) {
    if (std::isspace(c)) {
      if (!s.empty()) {
        tokens.emplace_back(s);
      }
      s.clear();
    } else if (ops.find(c) != std::string::npos) {
      if (!s.empty()) {
        tokens.emplace_back(s);
      }
      s.clear();
      tokens.emplace_back(1, c);
    } else {
      s += c;
    }
  }
  if (!s.empty()) {
    tokens.emplace_back(s);
  }
  return tokens;
}

static std::vector<std::string> ToPostfix(const std::vector<std::string>& tokens,
                                          const std::map<std::string, int32_t>& syms) {
  std::vector<std::string> postfix;
  std::stack<std::string> s;
  for (const auto& t : tokens) {
    if (t[0] == '(') {
      s.push(t);
    } else if (t[0] == ')') {
      std::string temp;
      while (true) {
        temp = s.top();
        s.pop();
        if (temp == "(") {
          break;
        }
        postfix.emplace_back(temp);
      }
    } else if (std::isdigit(t[0])) {
      postfix.emplace_back(t);
    } else if (std::isalpha(t[0])) {
      if (syms.find(t) == syms.end()) {
        throw Error(MSG(std::string("Substitution '") + t + "' undefined"));
      }
      postfix.emplace_back(std::to_string(syms.at(t)));
    } else {
      static const std::map<std::string, int32_t> Prec = {
        { "+", 0 },
        { "-", 0 },
        { "*", 1 },
        { "/", 1 },
        { "(", -1 }
      };
      while (!s.empty() && Prec.at(t) <= Prec.at(s.top())) {
        postfix.emplace_back(s.top());
        s.pop();
      }
      s.push(t);
    }
  }
  while (!s.empty()) {
    postfix.emplace_back(s.top());
    s.pop();
  }
  return postfix;
}

static void ApplyBinop(std::stack<int32_t>&s, const std::function<int32_t(int32_t, int32_t)>& f) {
  auto x = s.top();
  s.pop();
  auto y = s.top();
  s.pop();
  s.push(f(x, y));
}

int32_t Evaluator::Evaluate(const std::string& expr, const std::map<std::string, int32_t>& syms) {
  auto tokens = Tokens(expr, "+-*/()");
  auto postfix = ToPostfix(tokens, syms);

  std::stack<int32_t> s;

  for (const auto& t : postfix) {
    switch (t[0]) {
    case '+':
      ApplyBinop(s, [](const auto& x, const auto& y) { return y + x; });
      break;
    case '-':
      ApplyBinop(s, [](const auto& x, const auto& y) { return y - x; });
      break;
    case '*':
      ApplyBinop(s, [](const auto& x, const auto& y) { return y * x; });
      break;
    case '/':
      ApplyBinop(s, [](const auto& x, const auto& y) { return y / x; });
      break;
    default:
      s.push(std::stoi(t));
      break;
    }
  }

  return s.top();
}

}