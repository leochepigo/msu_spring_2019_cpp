#pragma once
#include <string>
#include <iostream>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{

    static constexpr char Separator = ' ';

    std::ostream& out;

    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args)
    {
        if (process(std::forward<T>(value)) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);

        return Error::CorruptedArchive;
    }

    Error process(bool value)
    {
        const char* string_v = value ? "true" : "false";
        if (value) out << "true" << Separator;
        else out << "false" << Separator;

        return Error::NoError;
    }

    Error process(uint64_t value)
    {
        if (out << value << Separator) return Error::NoError;

        return Error::CorruptedArchive;
    }

public:
    explicit Serializer(std::ostream& _out): out(_out) { }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
};


class Deserializer
{
    std::istream& in;

    template <class T, class... ArgsT>
    Error process(T&& value, ArgsT&&... args)
    {
        if (process(std::forward<T>(value)) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);

        return Error::CorruptedArchive;
    }

    Error process(bool& value)
    {
        std::string str;
        in >> str;

        if (str == "true") value = true;
        else if (str == "false") value = false;
        else return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error process(uint64_t& value)
    {
        std::string str;
        in >> str;

        for (auto c: str) {
            if (c >= '0' && c <= '9')
                value = value*10 + c - '0';
            else
                return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

public:
    explicit Deserializer(std::istream& _in): in(_in) { }

    template <class T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }
};
