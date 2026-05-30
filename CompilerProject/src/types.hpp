#ifndef TYPES_HPP
#define TYPES_HPP

enum ValueType
{
    VT_INT,
    VT_FLOAT,
    VT_CHAR,
    VT_STRING,
    VT_UNKNOWN
};

inline const char *valueTypeName(ValueType t)
{
    switch (t)
    {
    case VT_INT:
        return "int";
    case VT_FLOAT:
        return "float";
    case VT_CHAR:
        return "char";
    case VT_STRING:
        return "string";
    default:
        return "unknown";
    }
}

#endif
