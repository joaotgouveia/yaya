#pragma once

#include <map>
#include <string>
#include <vector>

namespace yaya {

template <typename T> class SymbolTable {
    std::vector<std::map<std::string, T>> _table;

  public:
    SymbolTable() = default;

    void push() { _table.emplace_back(); }
    void pop() { _table.pop_back(); }
    const std::map<std::string, T>& top() { return _table.back(); }

    bool insert(const std::string& name, T value) {
        return _table.back().emplace(name, value).second;
    }

    bool defined(const std::string& name) {
        auto entry = _table.back().find(name);
        return entry != _table.back().end();
    }

    T find(const std::string& name) {
        for (auto ctx = _table.crbegin(); ctx != _table.crend(); ++ctx) {
            auto entry = ctx->find(name);
            if (entry != ctx->end()) {
                return entry->second;
            }
        }
        return nullptr;
    }
};

} // namespace yaya
