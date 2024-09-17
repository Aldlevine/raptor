#pragma once

#include <cstring>
#include <string>
#include <vector>

namespace rptr::repr
{

    template <typename Type, typename Repr>
    inline Repr &to_repr(const Type &p_object, Repr &p_repr)
    {
        return p_repr.push_value(std::to_string(p_object));
    }

    template <typename Type, typename Repr>
    inline Repr &to_repr(const Type &p_object, Repr &p_repr)
        requires std::is_constructible_v<std::string, Type>
    {
        return p_repr.push_value(std::string(p_object));
    }

    enum CommandType
    {
        PUSH_SCOPE,
        POP_SCOPE,
        PUSH_FIELD,
        PUSH_VALUE,
    };

    struct Command
    {
        CommandType type{};
        std::string value{};
    };

    class Repr
    {
    public:
        bool indent = false;

        explicit Repr(bool p_indent = false) :
            indent(p_indent)
        {
        }

        Repr &push_scope(const std::string &p_name = "")
        {
            m_commands.emplace_back(Command{ PUSH_SCOPE, p_name });
            return *this;
        }

        Repr &pop_scope()
        {
            m_commands.emplace_back(Command{ POP_SCOPE });
            return *this;
        }

        Repr &push_field(const std::string &p_name)
        {
            m_commands.emplace_back(Command{ PUSH_FIELD, p_name });
            return *this;
        }

        Repr &push_value(const std::string &p_value)
        {
            m_commands.emplace_back(Command{ PUSH_VALUE, p_value });
            return *this;
        }

        template <typename Type>
        Repr &push_repr(const Type &p_object)
        {
            to_repr(p_object, *this);
            return *this;
        }

        Repr &push_field(const std::string &p_name, const std::string &p_value)
        {
            push_field(p_name);
            push_value(p_value);
            return *this;
        }

        template <typename Type>
        Repr &push_field(const std::string &p_name, const Type &p_object)
        {
            push_field(p_name);
            push_repr(p_object);
            return *this;
        }

        operator std::string() const
        {
            std::string result{};
            size_t current_depth{ 0 };

            for (auto it = m_commands.begin(); it != m_commands.end(); ++it)
            {
                switch (it->type)
                {
                    case PUSH_SCOPE:
                        result += it->value;
                        result += "(";

                        if (indent)
                        {
                            current_depth += 4;
                            result += "\n" + std::string(current_depth, ' ');
                        }

                        break;

                    case POP_SCOPE:
                        if (indent)
                        {
                            current_depth -= 4;
                        }

                        result += ")";

                        if (needs_comma(it, m_commands.end()))
                        {
                            result += ", ";

                            if (indent)
                            {
                                result += "\n" + std::string(current_depth, ' ');
                            }
                        }

                        break;

                    case PUSH_FIELD:
                        result += it->value;
                        result += ": ";
                        break;

                    case PUSH_VALUE:
                        result += it->value;
                        if (needs_comma(it, m_commands.end()))
                        {
                            result += ", ";

                            if (indent)
                            {
                                result += "\n" + std::string(current_depth, ' ');
                            }
                        }
                        break;
                }
            }

            return result;
        }

    protected:
    private:
        std::vector<Command> m_commands{};

        bool needs_comma(
            const std::vector<Command>::const_iterator &p_cur,
            const std::vector<Command>::const_iterator &p_end) const
        {
            const auto &next = std::next(p_cur);
            return (next != p_end && next->type != POP_SCOPE);
        }
    };

    template <typename Type>
    inline Repr to_repr(const Type &p_object, Repr &&p_repr)
    {
        return to_repr(p_object, p_repr);
    }

    template <typename Type>
    inline Repr to_repr(const Type &p_object)
    {
        return to_repr(p_object, Repr{});
    }

    inline std::ostream &operator<<(std::ostream &p_os, const Repr &p_repr)
    {
        return p_os << std::string(p_repr);
    }

} // namespace rptr::repr
