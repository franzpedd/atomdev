#pragma once

#include "Core/Core.h"

#include <string>

namespace Atom
{
    // returns if a given string can be converted to number
    bool API IsNumber(const std::string& s)
    {
        for (char const& ch : s)
        {
            if (std::isdigit(ch) == 0)
            {
                return false;
            }
        }
        return true;
    }

    // return index if substring is present in the string
    int API Find(std::string str1, std::string str2)
    {
        int i, j;
        size_t len1 = str1.length();
        size_t len2 = str2.length();

        for (i = 0; i <= (len2 - len1); i++)
        {
            for (j = 0; j < len1; j++)
            {
                if (str2[i + j] != str1[j])
                    break;
            }
            
            if (j == len1)
            {
                return i;
            }
        }

        return -1;
    }
}