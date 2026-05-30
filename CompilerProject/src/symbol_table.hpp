#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "types.hpp"

// Structure to hold information about a symbol (e.g., type, scope, value)
struct SymbolInfo
{
    ValueType type; // type of symbol
    // Add more fields as needed for semantic analysis (e.g., value, scope level)
};

class SymbolTable
{
private:
    // Stack of scopes; back() is innermost
    std::vector<std::unordered_map<std::string, SymbolInfo>> scopes;

public:
    SymbolTable()
    {
        // start with global scope
        scopes.emplace_back();
    }

    // Check if identifier exists in any visible scope
    bool exists(const std::string &name) const
    {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        {
            if (it->count(name))
                return true;
        }
        return false;
    }

    // Check if identifier exists in current scope only
    bool existsCurrent(const std::string &name) const
    {
        if (scopes.empty())
            return false;
        return scopes.back().count(name) > 0;
    }

    // Insert into current scope; returns false on redeclaration in same scope
    bool insertCurrent(const std::string &name, const SymbolInfo &info)
    {
        if (scopes.empty())
            scopes.emplace_back();
        auto &cur = scopes.back();
        if (cur.count(name))
            return false;
        cur[name] = info;
        return true;
    }

    // Lookup identifier through scopes from inner to outer
    SymbolInfo lookup(const std::string &name) const
    {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        {
            auto found = it->find(name);
            if (found != it->end())
                return found->second;
        }
        return {VT_UNKNOWN};
    }

    // Scope management
    void pushScope() { scopes.emplace_back(); }

    void popScope()
    {
        if (scopes.size() > 1)
        {
            scopes.pop_back();
        }
    }

    // Debug print
    void print() const
    {
        std::cout << "--- Symbol Table Scopes (inner to outer) ---" << std::endl;
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        {
            std::cout << "  Scope" << std::endl;
            for (const auto &pair : *it)
            {
                std::cout << "    " << pair.first << " : Type = " << valueTypeName(pair.second.type) << std::endl;
            }
        }
        std::cout << "-------------------------------------------" << std::endl;
    }
};

#endif // SYMBOL_TABLE_HPP
