#include "stringutils.h"
#include <algorithm>

namespace StringUtils
{

std::string repeated(const std::string& str, unsigned int times)
{
    using namespace std;

    if(!times)
    {
        return string();
    }

    size_t total_size = str.size() * times;
    size_t half_size = (total_size + 1) >> 1;
    string ret(str);
    ret.reserve(total_size);

    while(ret.size() < half_size)
    {
        ret.append(ret);
    }

    ret.append(ret, 0, total_size - ret.size());
    return ret;
}

std::string trimmed(const std::string& str)
{
    using namespace std;
    string cleared = str;
    string whitespaces(" \t\f\v\n\r");
    size_t firstNotWhiteSpace = cleared.find_first_not_of(whitespaces);

    if(firstNotWhiteSpace != string::npos)
    {
        cleared.erase(0, firstNotWhiteSpace);
        size_t lastNotWhiteSpace = cleared.find_last_not_of(whitespaces);
        cleared.erase(lastNotWhiteSpace + 1);
    }
    else
    {
        cleared.clear();
    }

    return cleared;
}

std::string simplified(const std::string& str)
{
    using namespace std;
    string cleared = trimmed(str);
    string whitespaces("\t\f\v\n\r");
    size_t pos = cleared.find_first_of(whitespaces);

    while(pos != string::npos)
    {
        cleared.replace(pos, 1, " ");
        pos = cleared.find_first_of(whitespaces, pos);
    }

    string doubleWhitespace = "  ";
    pos = cleared.find(doubleWhitespace);

    while(pos != string::npos)
    {
        cleared.replace(pos, 2, " ");
        pos = cleared.find(doubleWhitespace, pos);
    }

    return cleared;
}

std::string fromBase64(const std::string& str)
{
    using namespace std;
    auto f = [](char ch)->char
    {
        //[a-z]
        if(ch >= 'a')
        {
            //"71 = a - base64(a)"
            return ch - 71;
        }

        //[A-Z]
        else if(ch >= 'A')
        {
            //"65 = A - base64(A)"
            return ch - 65;
        }

        //[/0-9]
        else if(ch >= '/')
        {
            return ch + 4;
        }

        //+
        else
        {
            return 62;
        }
    };
    size_t noEqualsSize = str.size();

    if(str.rfind('=') != str.npos)
    {
        --noEqualsSize;

        if(str.rfind('=', str.size() - 2) != str.npos)
        {
            --noEqualsSize;
        }
    }

    const size_t targetSize = (noEqualsSize * 3) / 4;
    const size_t turns = noEqualsSize / 4;
    const size_t rest = noEqualsSize % 4;
    const char* ori_it = str.c_str();
    string target;
    target.resize(targetSize);
    string::iterator target_it = target.begin();

    for(size_t i = 0; i < turns; ++i)
    {
        unsigned int acc = 0;
        acc |= f(*ori_it++) << 18;
        acc |= f(*ori_it++) << 12;
        acc |= f(*ori_it++) << 6;
        acc |= f(*ori_it++);
        *target_it++ = static_cast<char>((acc & 0x00FF0000) >> 16);
        *target_it++ = static_cast<char>((acc & 0x0000FF00) >> 8);
        *target_it++ = static_cast<char>((acc & 0x000000FF));
    }

    if(rest)
    {
        unsigned int acc = 0;
        acc |= f(*ori_it++) << 18;
        acc |= f(*ori_it++) << 12;

        if(rest > 2)
        {
            acc |= f(*ori_it++) << 6;
        }

        *target_it++ = static_cast<char>((acc & 0x00FF0000) >> 16);

        if(rest > 2)
        {
            *target_it = static_cast<char>((acc & 0x0000FF00) >> 8);
        }
    }

    return target;
}

std::string toBase64(const std::string& str)
{
    using namespace std;
    const char charTable[] =
    {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+' , '/'
    };
    size_t turns = str.size() / 3;
    const size_t rest = str.size() % 3;
    const size_t targetSize = ((str.size() + 2) / 3) << 2;
    const char* ori_it = str.c_str();
    string target;
    target.resize(targetSize);
    string::iterator target_it = target.begin();

    while(turns--)
    {
        unsigned int acc = 0;
        acc |= *ori_it++ << 16;
        acc |= *ori_it++ << 8;
        acc |= *ori_it++;
        *target_it++ = charTable[(acc & 0x00FC0000) >> 18];
        *target_it++ = charTable[(acc & 0x0003F000) >> 12];
        *target_it++ = charTable[(acc & 0x00000FC0) >> 6];
        *target_it++ = charTable[(acc & 0x0000003F)];
    }

    if(rest)
    {
        unsigned int acc = 0;
        acc |= *ori_it++ << 16;

        if(rest == 2)
        {
            acc |= *ori_it << 8;
        }

        *target_it++ = charTable[(acc & 0x00FC0000) >> 18];
        *target_it++ = charTable[(acc & 0x0003F000) >> 12];

        if(rest == 2)
        {
            *target_it++ = charTable[(acc & 0x00000FC0) >> 6];
        }
        else
        {
            *target_it++ = '=';
        }

        *target_it++ = '=';
    }

    return target;
}

std::string fromHex(const std::string& str)
{
    using namespace std;
    auto f = [](char ch)->char
    {
        //[0-9]
        if(ch <= '9')
        {
            return ch - 48;
        }

        //[A-Z]
        else if(ch <= 'Z')
        {
            //55 = A - hex(A)
            return ch - 55;
        }
        else
        {
            //87 = a - hex(a)
            return ch - 87;
        }

    };
    const size_t targetSize = str.size() >> 1;
    const char* ori_it = str.c_str();
    string target;
    target.resize(targetSize);
    for_each(target.begin(), target.end(), [&ori_it, &f](char& ch)
    {
        ch |= f(*ori_it++) << 4;
        ch |= f(*ori_it++);
    });
    return target;
}

std::string toHex(const std::string& str)
{
    using namespace std;
    const char charTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    const size_t targetSize = str.size() << 1;
    string target;
    target.resize(targetSize);
    string::iterator target_it = target.begin();
    for_each(str.begin(), str.end(), [&target_it, &charTable](char ch)
    {
        *target_it++ = charTable[(ch & 0xF0) >> 4];
        *target_it++ = charTable[(ch & 0x0F)];
    });
    return target;
}
};

