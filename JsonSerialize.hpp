﻿/**
 * @file JsonSerialize.hpp
 * @author myhql (https://github.com/bianc2018)
 * @brief json 序列化与反序列化库
 * 
 * 
 * 
 * @version 0.1
 * @date 2020-10-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef JSON_SERIALIZE_HPP_
#define JSON_SERIALIZE_HPP_

#include <string.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//依赖cjson
#include "cJSON.h"

#define JSON_PP_STRINGIZE(text) JSON_PP_STRINGIZE_I(text)
#define JSON_PP_STRINGIZE_I(...) #__VA_ARGS__

//序列化函数命名
//void Serialize(archive& ar, class& t)
//void class.Serialize(archive& ar)
#define SERIALIZE_FUNC Serialize

//根据不同类型获取序列化的值
//从json中获取值
//bool GetJsonValue(cJSON**obj, class &t) 全局的
#define SERIALIZE_GET_JSON_VALUE_FUNC GetJsonValue

//向json中设置值
//bool SetJsonObject(cJSON*obj, class &t) 全局的
#define SERIALIZE_SET_JSON_OBJECT_FUNC SetJsonObject



/**
 * @brief JsonSerialize的命名空间防止重名
 * 
 */
namespace JsonSerialize
{
    //序列化
    template<typename archive,typename T>
    void SERIALIZE_FUNC(archive& ar, T& t)
    {
        t.SERIALIZE_FUNC(ar);
    }

    template<typename T>
	struct KVPair :
		public std::pair<const char *, T &>
	{
		explicit KVPair(const char * name_, T & t,bool bmust) :
			std::pair<const char *, T &>(name_, t),bmust_(bmust)
		{}

		const char* name() const {
			return this->first;
		}

		T & value()  {
			return this->second;
		}

		const T & const_value() const {
			return this->second;
		}
        const bool& be_must() const {
            return bmust_;
        }
    private:
        //是否是必须的？
        bool bmust_;
	};

	template<typename T>
	inline KVPair< T > make_kv(const char * name, T & t,bool must) {
		return KVPair< T >(name, t, must);
	}

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, bool& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, char& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, unsigned char& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, int& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, unsigned int& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, long& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, long long& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, unsigned long long& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, double& t);
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, std::string& t);
    template<typename T>
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* arr, std::vector<T>& ts);
    template<unsigned int len>
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, char(&ts)[len]);
    template<typename T, unsigned int len>
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* arr, char(&ts)[len]);

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, bool& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, char& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, unsigned char& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, int& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, unsigned int& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, double& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, long& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, long long& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, unsigned long long& t);
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, std::string& t);
    template<typename T>
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, std::vector<T>& ts);
    //长度为len的数组
    template<unsigned int len>
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, char(&ts)[len]);
    //长度为len的数组
    template<typename T,unsigned int len>
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, T(&ts)[len]);
    //获取参数
    template<typename T>
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON*obj, T &t);
    //设置参数
    template<typename T>
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, T& t);

    //序列化基类
    class BaseSerialize
    {
    public:
        BaseSerialize(const std::string& json) :json_root_(nullptr)
        {
            parse(json);
        }

        BaseSerialize(cJSON* json_root) :json_root_(json_root)
        {

        }

        BaseSerialize() :json_root_(nullptr)
        {

        }

        virtual ~BaseSerialize()
        {
            free_root();
        }

        virtual  operator bool()const
        {
            return is_vaild();
        }

        //是否有效
        virtual  bool is_vaild()const
        {
            return nullptr != json_root_;
        }
    public:
        //json 操作接口

        //获取
        cJSON* get_root()
        {
            return json_root_;
        }

        //移动
        cJSON* move_root()
        {
            auto p = json_root_;
            json_root_ = nullptr;
            return p;
        }

        //释放
        bool free_root()
        {
            if (json_root_)
            {
                cJSON_Delete(json_root_);
                json_root_ = nullptr;
            }
            return true;
        }

        bool  parse(const std::string& json)
        {
            json_root_ = cJSON_Parse(json.c_str());
            return *this;
        }

        std::string print()
        {
            std::string str;
            if (json_root_)
            {
                auto cp = cJSON_Print(json_root_);
                if (cp)
                {
                    str = cp;
                    cJSON_free(cp);
                }
            }
            return str;
        }
        //禁止拷贝
    private:
        BaseSerialize(const BaseSerialize& that) = delete;
        BaseSerialize& operator=(const BaseSerialize& that) = delete;
    protected:
        cJSON* json_root_;
    };

    //序列化类  struct->std::string
    class DoSerialize :public BaseSerialize
    {
    public:
        using BaseSerialize::BaseSerialize;

        template<typename T>
        DoSerialize& operator<<(KVPair<T> pair)
        {
            //std::cout << "<<" << pair.name() << std::endl;

            if (json_root_ == nullptr)
            {
                json_root_ = cJSON_CreateObject();
                if (json_root_ == nullptr)
                {
                    if (pair.be_must())
                    {
                        //错误 考虑用throw代替
                        free_root();
                    }
                    return *this;
                }
            }

            cJSON* obj = cJSON_GetObjectItem(json_root_, pair.name());

            auto ret = SERIALIZE_SET_JSON_OBJECT_FUNC(&obj, (pair.value()));
            if (obj)
            {
                cJSON_AddItemToObject(json_root_, pair.name(), obj);
            }
            if (pair.be_must() && (!ret || nullptr == obj))
            {
                //错误
                free_root();
            }
            return *this;
        }

        template<typename T>
        DoSerialize& operator<<(T& t)
        {
            //std::cout << " <<T" << std::endl;
            Serialize(*this, t);
            return *this;
        }

        template<typename T>
        DoSerialize& operator&(KVPair<T> pair)
        {
            return operator<<(pair);
        }
    };

    //反序列化
    class DeSerialize :public BaseSerialize
    {
    public:
        using BaseSerialize::BaseSerialize;

        template<typename T>
        DeSerialize& operator>>(KVPair<T> pair)
        {
            //std::cout << ">>" << pair.name() << std::endl;
            auto obj = cJSON_GetObjectItem(json_root_, pair.name());
            auto ret = SERIALIZE_GET_JSON_VALUE_FUNC(obj, pair.value());
            if (pair.be_must() && (!ret))
            {
                free_root();
            }
            return *this;
        }

        template<typename T>
        DeSerialize& operator>>(T& t)
        {
            //std::cout << " >>T"<< std::endl;
            Serialize(*this, t);
            return *this;
        }

        template<typename T>
        DeSerialize& operator&(KVPair<T> pair)
        {
            return operator>>(pair);
        }
    };



    ///实现
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, bool& t)
    {
        // std::cout << "set int" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_True)
            {
                t = true;
                return true;
            }
            else if (obj->type == cJSON_False)
            {
                t = false;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, char& t)
    {
        // std::cout << "set int" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_Number)
            {
                t = (char)obj->valuedouble;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, unsigned char& t)
    {
        // std::cout << "set int" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_Number)
            {
                if (obj->valuedouble < 0)
                    return false;
                t = (unsigned char)obj->valuedouble;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, int& t)
    {
        // std::cout << "set int" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_Number)
            {

                t = (int)obj->valuedouble;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, unsigned int& t)
    {
        // std::cout << "set int" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_Number)
            {
                if (obj->valuedouble < 0)
                    return false;
                t = (unsigned int)obj->valuedouble;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, long& t)
    {
        // std::cout << "set int" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_Number)
            {
                t = (long)obj->valuedouble;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, long long& t)
    {
        // std::cout << "set int" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_Number)
            {
                t = (long long)obj->valuedouble;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, unsigned long long& t)
    {
        // std::cout << "set int" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_Number)
            {
                if (obj->valuedouble < 0)
                    return false;
                t = (unsigned long long)obj->valuedouble;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, double& t)
    {
        //std::cout << "set double" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_Number)
            {
                t = obj->valuedouble;
                return true;
            }

        }
        return false;
    }

    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, std::string& t)
    {
        if (obj)
        {
            if (obj->type == cJSON_String)
            {
                if (obj->valuestring)
                    t = obj->valuestring;
                return true;
            }

        }
        return false;
    }

    template<typename T>
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* arr, std::vector<T>& ts)
    {
        //std::cout << "set vector" << std::endl;
        if (arr)
        {
            //数组
            if (arr->type == cJSON_Array)
            {
                auto size = cJSON_GetArraySize(arr);

                for (auto i = 0; i < size; ++i)
                {
                    auto obj = cJSON_GetArrayItem(arr, i);
                    T t;
                    if (true == SERIALIZE_GET_JSON_VALUE_FUNC(obj, t))
                    {
                        ts.push_back(t);
                    }
                    else
                    {
                        //错误
                    }
                }
                return true;
            }

        }
        return false;
    }

    template<unsigned int len>
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, char(&ts)[len])
    {
        if (obj)
        {
            if (obj->type == cJSON_String)
            {
                
                if (obj->valuestring)
                {
                    //长度不符合
                    auto valuestringlen = ::strlen(obj->valuestring);
                    if (valuestringlen > len)
                        return false;
                    memcpy(ts, obj->valuestring, len);
                    return true;
                }
                return true;
            }

        }
        return false;
    }

    template<typename T, unsigned int len>
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* arr, char(&ts)[len])
    {
        //std::cout << "set vector" << std::endl;
        if (arr)
        {
            //数组
            if (arr->type == cJSON_Array)
            {
                auto size = cJSON_GetArraySize(arr);
                if (size > len)
                    return false;

                for (auto i = 0; i < size; ++i)
                {
                    auto obj = cJSON_GetArrayItem(arr, i);
                    T t;
                    if (true == SERIALIZE_GET_JSON_VALUE_FUNC(obj, t))
                    {
                        ts[i]=t;
                    }
                    else
                    {
                        //错误
                    }
                }
                return true;
            }

        }
        return false;
    }
    
    
    //获取
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, bool& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_True || (*obj)->type == cJSON_False)
            {
                (*obj)->type = (t) ? cJSON_True : cJSON_False;
                return true;
            }
        }
        *obj = cJSON_CreateBool(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, char& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_Number)
            {
                cJSON_SetNumberValue(*obj, t);
                return true;
            }
        }
        *obj = cJSON_CreateNumber(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, unsigned char& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_Number)
            {
                cJSON_SetNumberValue(*obj, t);
                return true;
            }
        }
        *obj = cJSON_CreateNumber(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, int& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_Number)
            {
                cJSON_SetNumberValue(*obj, t);
                return true;
            }
        }
        *obj = cJSON_CreateNumber(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, unsigned int& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_Number)
            {
                cJSON_SetNumberValue(*obj, t);
                return true;
            }
        }
        *obj = cJSON_CreateNumber(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, double& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_Number)
            {
                cJSON_SetNumberValue(*obj, t);
                return true;
            }
        }
        *obj = cJSON_CreateNumber(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, long& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_Number)
            {
                cJSON_SetNumberValue(*obj, t);
                return true;
            }
        }
        *obj = cJSON_CreateNumber(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, long long& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_Number)
            {
                cJSON_SetNumberValue(*obj, t);
                return true;
            }
        }
        *obj = cJSON_CreateNumber(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, unsigned long long& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_Number)
            {
                cJSON_SetNumberValue(*obj, t);
                return true;
            }
        }
        *obj = cJSON_CreateNumber(t);
        return true;
    }

    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, std::string& t)
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_String)//cJSON_Raw
            {
                cJSON_SetValuestring(*obj, t.c_str());
                return true;
            }
        }
        *obj = cJSON_CreateString(t.c_str());
        return true;
    }

    template<typename T>
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, std::vector<T>& ts)
    {
        if (*obj)
        {
            if ((*obj)->type != cJSON_Array)
            {
                //重新生成
                *obj = nullptr;
            }
        }
        if (*obj == nullptr)
        {
            *obj = cJSON_CreateArray();
            if (*obj == nullptr)
                return false;
        }
        //
        for (T& t : ts)
        {
            cJSON* it = nullptr;
            SERIALIZE_SET_JSON_OBJECT_FUNC(&it, t);
            if (it)
                cJSON_AddItemToArray(*obj, it);
        }
        return true;
    }

    //长度为len的数组
    template<unsigned int len>
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, char(&ts)[len])
    {
        if (*obj)
        {
            if ((*obj)->type == cJSON_String)//cJSON_Raw
            {
                cJSON_SetValuestring(*obj, ts);
                return true;
            }
        }
        *obj = cJSON_CreateString(ts);
        return true;
    }
    
    //长度为len的数组
    template<typename T,unsigned int len>
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, T(&ts)[len])
    {
        if (*obj)
        {
            if ((*obj)->type != cJSON_Array)
            {
                //重新生成
                *obj = nullptr;
            }
        }
        if (*obj == nullptr)
        {
            *obj = cJSON_CreateArray();
            if (*obj == nullptr)
                return false;
        }
        //
        for (int i=0;i< len;++i)
        {
            cJSON* it = nullptr;
            SERIALIZE_SET_JSON_OBJECT_FUNC(&it, ts[i]);
            if (it)
                cJSON_AddItemToArray(*obj, it);
        }
        return true;
    }

    //获取参数
    template<typename T>
    inline bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON*obj, T &t)
    {
        if (obj)
        {
            DeSerialize ds(obj);
            SERIALIZE_FUNC(ds, t);
            ds.move_root();//释放
            return true;
        }
        return false;
    }

    //设置参数
    template<typename T>
    inline bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, T& t)
    {
       
        DoSerialize ds(*obj);
        SERIALIZE_FUNC(ds, t);
        *obj = ds.move_root();//释放
        return true;
    }


}

// 侵入式, 指定key.
#define JSON_SERIALIZATION_KEY_KV(key, name,m)	\
    JsonSerialize::make_kv(key, name,m)

// 用于serialize成员函数中声明要序列化的成员.
#define JSON_SERIALIZATION_KV(name,m)	\
    JSON_SERIALIZATION_KEY_KV(JSON_PP_STRINGIZE(name), name,m)

// 用于serialize成员函数中声明要序列化的成员. 非必填项
#define JSON_SERIALIZATION_KV_N(name)	\
    JSON_SERIALIZATION_KV(name,false)


// 非侵入式，避免类成员名字生成不正确.
#define JSON_NI_SERIALIZATION_KV(classname, name, m)	\
    JSON_SERIALIZATION_KEY_KV(JSON_PP_STRINGIZE(name), classname . name,m)

// 非侵入式，避免类成员名字生成不正确. 非必填项
#define JSON_NI_SERIALIZATION_KV_N(classname, name)	\
    JSON_NI_SERIALIZATION_KV(classname , name, false)

//#define JSON_SERIALIZATION_BASE_OBJECT_KV(name) \
//	JsonSerialize::make_kv(JSON_PP_STRINGIZE(name), base_object<name>(*this))


//json数据序列化到结构体
template<typename T>
bool JsonSerializeFormJson(const std::string& json, T& t)
{
    JsonSerialize::DeSerialize ar(json);
    ar >> t;
    auto ret =  ar.is_vaild();
    return ret;
}

//反序列化结构体数据到json
template<typename T>
bool JsonSerializeToJson(std::string& json, T& t)
{
    JsonSerialize::DoSerialize ar(json);
    ar << t;
    json = ar.print();
    return ar;
}

//json文件数据序列化到结构体
template<typename T>
bool JsonSerializeFormJsonFile(const std::string& jsonfilepath, T& t)
{
    //打开文件读数据
    std::fstream jsonfile(jsonfilepath, std::ios::in);
    if (!jsonfile)
    {
        //文件打开失败
        return false;
    }

    //读数据
    const unsigned int buff_size = 4 * 1024;
    char buff[buff_size] = { 0 };
    std::string json_content;
    while (true)
    {
        jsonfile.read(buff, buff_size);
        unsigned int rlen = jsonfile.gcount();
        if (0 == rlen)//读完了
            break;
        json_content += std::string(buff, rlen);
    }
    return JsonSerializeFormJson(json_content,t);
}

//反序列化结构体数据到json文件
template<typename T>
bool JsonSerializeToJsonFile(const std::string& jsonfilepath, T& t)
{
    //打开文件读数据
    std::fstream jsonfile(jsonfilepath, std::ios::out);
    if (!jsonfile)
    {
        //文件打开失败
        return false;
    }
    std::string json_content;
    if (JsonSerializeToJson(json_content, t))
    {
        jsonfile.write(json_content.c_str(), json_content.size());
        jsonfile.close();
        return true;
    }
    jsonfile.close();
    //转换失败
    return false;
}
#endif // !JSON_SERIALIZE_HPP_