#pragma once
#include <iostream>

using namespace std;

class Compare
{
public:

    //function of comparison name string in case-insensitive
    //return value(positive number: str1 > str2 / 0: str1 == str2 / negative number: str1 < str2)
    int compareName(string s1, string s2)
    {
        /*
        string newS1, newS2;
        
        //copy s1
        for (int i = 0; i < s1.length(); i++)
        {
            if (s1[i] >= 'A' && s1[i] <= 'Z')   //upper case
            {
                newS1.push_back(s1[i] + ('a' - 'A'));
            }
            else
            {
                newS1.push_back(s1[i]);
            }
        }

        //copy s2
        for (int i = 0; i < s2.length(); i++)
        {
            if (s2[i] >= 'A' && s2[i] <= 'Z')   //upper case
            {
                newS2.push_back(s2[i] + ('a' - 'A'));
            }
            else
            {
                newS2.push_back(s2[i]);
            }
        }

        return newS1.compare(newS2);    //compare
        */
        return s1.compare(s2);
    }

    //From the beginning, copy letters as many as size parameter
    string partialString(string s, int size)
    {
        string partial;

        //copy from the front
        for (int k = 0; k < s.length() && size > 0; k++)
        {
            partial.push_back(s[k]);
            size--;
        }

        return partial;
    }
};