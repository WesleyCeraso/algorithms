#include "numberutils.h"
#include <regex>
#include <exception>
#include <cassert>
#include "stringutils.h"

namespace NumberUtils
{

std::string toRoman(unsigned short number)
{
    if(!number || number >= 5000)
    {
        std::range_error exp("Number is not in roman number range");
        throw exp;
    }

    unsigned int M = number / 1000;
    number = number % 1000;
    unsigned int CM = number / 900;
    number = number % 900;
    unsigned int D = number / 500;
    number = number % 500;
    unsigned int CD = number / 400;
    number = number % 400;
    unsigned int C = number / 100;
    number = number % 100;
    unsigned int XC = number / 90;
    number = number % 90;
    unsigned int L = number / 50;
    number = number % 50;
    unsigned int XL = number / 40;
    number = number % 40;
    unsigned int X = number / 10;
    number = number % 10;
    unsigned int IX = number / 9;
    number = number % 9;
    unsigned int V = number / 5;
    number = number % 5;
    unsigned int IV = number / 4;
    number = number % 4;
    unsigned int I = number;
    std::string result;
    result += std::string(M, 'M');
    result += CM ? "CM" : std::string();
    result += std::string(D, 'D');
    result += CD ? "CD" : std::string();
    result += std::string(C, 'C');
    result += XC ? "XC" : std::string();
    result += std::string(L, 'L');
    result += XL ? "XL" : std::string();
    result += std::string(X, 'X');
    result += IX ? "IX" : std::string();
    result += std::string(V, 'V');
    result += IV ? "IV" : std::string();
    result += std::string(I, 'I');
    return result;
}

unsigned short int fromRoman(const std::string& str)
{
    if(str.empty() || !std::regex_match(str, std::regex("^M{0,4}(?:CM|CD|D?C{0,3})(?:XC|XL|L?X{0,3})(?:IX|IV|V?I{0,3})$")))
    {
        std::domain_error exp("String doesn't contain a valid Roman number (" + str + ")");
        throw exp;
    }

    std::string aux(str);
    aux = std::regex_replace(aux, std::regex("CM"), std::string("DCCCC"));
    aux = std::regex_replace(aux, std::regex("CD"), std::string("CCCC"));
    aux = std::regex_replace(aux, std::regex("XC"), std::string("LXXXX"));
    aux = std::regex_replace(aux, std::regex("XL"), std::string("XXXX"));
    aux = std::regex_replace(aux, std::regex("IX"), std::string("VIIII"));
    aux = std::regex_replace(aux, std::regex("IV"), std::string("IIII"));
    unsigned short int result = 0;

    for(size_t i = 0; i < aux.size(); ++i)
    {
        switch(aux[i])
        {
            case 'M':
                result += 1000;
                break;

            case 'D':
                result += 500;
                break;

            case 'C':
                result += 100;
                break;

            case 'L':
                result += 50;
                break;

            case 'X':
                result += 10;
                break;

            case 'V':
                result += 5;
                break;

            case 'I':
                result += 1;
                break;

            default:
                assert(false);
        }
    }

    return result;
}

std::string toSpeech(int number)
{
    if(!number)
    {
        return "zero";
    }

    std::string units[] = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen"};
    std::string decimals[] = {"", "teen", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
    std::string result;

    if(number < 0)
    {
        result = "minus ";
        number = std::abs(number);
    }

    if(unsigned int B = number / 1000000000)
    {
        result += toSpeech(B) + (B == 1 ? " billion " : " billions ");
        number %= 1000000000;
    }

    if(unsigned int M = number / 1000000)
    {
        result += toSpeech(M) + (M == 1 ? " million " : " millions ");
        number %= 1000000;
    }

    if(unsigned int T = number / 1000)
    {
        result += toSpeech(T) + (T == 1 ? " thousand " : " thousands ");
        number %= 1000;
    }

    if(unsigned int H = number / 100)
    {
        result += toSpeech(H) + (H == 1 ? " hundred " : " hundreds ");
        number %= 100;
    }

    if(number <= 15)
    {
        result += units[number];
    }
    else if(number < 20)
    {
        result += units[number % 10] + decimals[number / 10];
    }
    else
    {
        result += decimals[number / 10] + " " + units[number % 10];
    }

    return StringUtils::trimmed(result);
}

};

