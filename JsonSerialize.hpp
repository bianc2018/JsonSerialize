/**
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

#include <string>
#include <iostream>
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
		explicit KVPair(const char * name_, T & t) :
			std::pair<const char *, T &>(name_, t)
		{}

		const char* name() const {
			return this->first;
		}

		T & value() const {
			return this->second;
		}

		const T & const_value() const {
			return this->second;
		}
	};

	template<typename T>
	inline const KVPair< T > make_kv(const char * name, T & t) {
		return KVPair< T >(name, t);
	}

    //获取参数
    template<typename T>
    bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON*obj, T &t)
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
    
    bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, int& t)
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
    
    bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, double& t)
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

    bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* obj, std::string& t)
    {
        //std::cout << "set string" << std::endl;
        if (obj)
        {
            if (obj->type == cJSON_String)
            {
                if(obj->valuestring)
                    t = obj->valuestring;
                return true;
            }

        }
        return false;
    }

    template<typename T>
    bool SERIALIZE_GET_JSON_VALUE_FUNC(cJSON* arr, std::vector<T>& ts)
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
                    auto obj = cJSON_GetArrayItem(arr,i);
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
    
    //设置参数
    template<typename T>
    bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, T& t)
    {
       
        DoSerialize ds(*obj);
        SERIALIZE_FUNC(ds, t);
        *obj = ds.move_root();//释放
        return true;
    }

    bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, int& t)
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

    bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, double& t)
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

    bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, std::string& t)
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
    bool SERIALIZE_SET_JSON_OBJECT_FUNC(cJSON** obj, std::vector<T>& ts)
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

    //序列化基类
    class BaseSerialize
    {
    public:
        BaseSerialize(const std::string& json) :json_root_(nullptr)
        {
            json_root_ = cJSON_Parse(json.c_str());
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

        bool  parse(const std::string &json)
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
        BaseSerialize& operator=(const BaseSerialize& that)=delete;
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

            if (json_root_==nullptr)
            {
                json_root_ = cJSON_CreateObject();
                if (json_root_ == nullptr)
                {
                    return *this;
                }
            }

            cJSON*  obj = cJSON_GetObjectItem(json_root_, pair.name());

            SERIALIZE_SET_JSON_OBJECT_FUNC(&obj, (pair.value()));
            if (obj)
            {
                cJSON_AddItemToObject(json_root_, pair.name(),obj);
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
            SERIALIZE_GET_JSON_VALUE_FUNC(obj,pair.value());
            return *this;
        }
        
        template<typename T>
        DeSerialize& operator>>(T &t)
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
}

// 侵入式, 指定key.
#define JSON_SERIALIZATION_KEY_KV(key, name)	\
    JsonSerialize::make_kv(key, name)

// 用于serialize成员函数中声明要序列化的成员.
#define JSON_SERIALIZATION_KV(name)	\
    JSON_SERIALIZATION_KEY_KV(JSON_PP_STRINGIZE(name), name)

// 非侵入式，避免类成员名字生成不正确.
#define JSON_NI_SERIALIZATION_KV(classname, name)	\
    JSON_SERIALIZATION_KEY_KV(JSON_PP_STRINGIZE(name), classname . name)

//#define JSON_SERIALIZATION_BASE_OBJECT_KV(name) \
//	JsonSerialize::make_kv(JSON_PP_STRINGIZE(name), base_object<name>(*this))



template<typename T>
bool js_form_json(const std::string& json, T& t)
{
    JsonSerialize::DeSerialize ar(json);
    ar >> t;
    auto ret =  ar.is_vaild();
    return ret;
}

template<typename T>
bool js_to_json(std::string& json, T& t)
{
    JsonSerialize::DoSerialize ar(json);
    ar << t;
    json = ar.print();
    return ar;
}