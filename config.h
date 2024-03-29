#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <memory>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <map>

class ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name,const std::string& description = "")
        :m_name(name)
        ,m_description(description){

    }
    virtual ~ConfigVarBase(){}

    const std::string& getName() const {return m_name;}
    const std::string& getDescription() const {return m_description;}

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& val) = 0;
private:
    std::string m_name;
    std::string m_description;

};

template<class T>
class ConfigVar : public ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVar> ptr;

    ConfigVar(const std::string& name,const T& default_value,const std::string& description = "")
        :ConfigVarBase(name,description)
        ,m_val(default_value){

    }

    std::string toString() override{
        try
        {
            return boost::lexical_cast<std::string>(m_val);
        }
        catch(std::exception& e)
        {
            LOG__ERROR(LOG_ROOT()) << "ConfigVar::toString exception"<<e.what()<<"convert:"<<typeid(m_val).name()<<" to string";
        }
        return "";
    }

    bool fromString(const std::string& val) override{
        try
        {
            m_val = boost::lexical_cast<T>(val);
        }
        catch(std::exception& e)
        {
            LOG__ERROR(LOG_ROOT()) << "ConfigVar::toString exception"<<e.what()<<"convert: string to"<<typeid(m_val).name();
        }
        return false;
    }

    const T getValue() const {return m_val;}
    void setValue(const T& v){m_val = v;}
private:
    T m_val;
};

class Config{
public:
    typedef std::map<std::string,ConfigVarBase::ptr> ConfigVarMap;

    template<class T>
    static typename ConfigVar<T>::ptr LookUp(const std::string& name,const T& default_value,const std::string& description = ""){
        auto tmp = LookUp<T>(name);
        if(tmp){
            LOG__INFO(LOG_ROOT()) << "LookUp name=" << name <<" exits";
            return tmp;
        }
        if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._012345678")!= std::string::npos){
            LOG__ERROR(LOG_ROOT()) << "LookUp name invalid"<<name;    
            throw std::invalid_argument(name);
        }   
        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,default_value,description));
        s_datas[name] = v;
        return v;
    }

    template<class T>
    static typename ConfigVar<T>::ptr LookUp(const std::string& name){
        auto it = s_datas.find(name);
        if(it == s_datas.end()){
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second());
    }
private:
    static ConfigVarMap s_datas;

};

#endif