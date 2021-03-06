#ifndef WATER_BASE_TOOLS_HPP
#define WATER_BASE_TOOLS_HPP

#include <string>
#include <memory>

#include "format.h"

namespace water{
namespace componet{

template<typename T>
std::string toString(const T& obj)
{
    std::string ret;
    appendToString(&ret, obj);
    return ret;
}

template <typename T>
T fromString(const std::string& str)
{
    T ret = T();
    std::stringstream ss(str);
    ss >> ret;
    return ret;
}


//字符串分割， 不保留空串
template <typename StrContiner>
void splitString(StrContiner* result, const std::string& str, const std::string& delimiter)
{
    if(result == nullptr)
        return;
    result->clear();
    std::string::size_type subBegin = 0;
    std::string::size_type subEnd = 0;
    while(true)
    {
        subEnd = str.find(delimiter, subBegin); //subEnd此时指向分隔符

        if(subEnd == std::string::npos)//没有剩余的分隔符了, 即切分完毕
        {
            if(subBegin != subEnd)// 最后一个分隔符不在串尾
                result->insert(result->end(), str.substr(subBegin, subEnd - subBegin));
            break;
        }

        if(subBegin != subEnd)// 当前位置的字符， 不是刚找到的分隔符
            result->insert(result->end(), str.substr(subBegin, subEnd - subBegin));

        subBegin = subEnd + delimiter.length(); //将begin指向下个子串的起点
    }
}

//随机访问容器的尾删除
template <typename RandomContiner>
void tailEraseInRandomAccessContiner(RandomContiner* continer, typename RandomContiner::size_type index)
{
    typename RandomContiner::value_type tmp = continer->back();
    continer->back() = continer->at(index);
    continer->at(index) = continer->back();
}

}}

#endif //ifndef WATER_NET_BASE_HPP


