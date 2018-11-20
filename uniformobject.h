#ifndef UNIFORMOBJECT_H
#define UNIFORMOBJECT_H

#include <glad/glad.h>
#include <type_traits>
#include <string>
#include <stdexcept>

#include "shaderprogram.h"

class UniformObjectMonitor
{
    static const unsigned int maxBindingPoints = GL_MAX_UNIFORM_BUFFER_BINDINGS;
protected:
    UniformObjectMonitor();
    virtual ~UniformObjectMonitor();
    int ownBindingPoint;

private:
    static int currentBindingPoint;
};

template <class T, typename std::enable_if<
              std::is_trivial<T>::value &&
              std::is_standard_layout<T>::value, int>::type = 0>
class UniformObject : private UniformObjectMonitor
{
public:
    UniformObject(const T & data/*, const ShaderProgram & shader, std::string name*/) : ref_data(data)
    {
//        unsigned int ub_index = glGetUniformBlockIndex(shader.getId(), name.c_str());
//        if(ub_index == GL_INVALID_INDEX)
//        {
//            throw std::runtime_error(std::string("ERROR::UBO::BINDING\n") + std::string("Can't find specidied uniform object in the shader programm.\n"));
//        }
//        else if(glGetError() == GL_INVALID_OPERATION)
//        {
//            throw std::runtime_error(std::string("ERROR::UBO::BINDING\n") + std::string("Can't find specidied shader programm.\n"));
//        }

        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

//        glUniformBlockBinding(shader.getId(), ub_index, ownBindingPoint);
//        if(glGetError() == GL_INVALID_VALUE)
//        {
//            glDeleteBuffers(1, &ubo);
//            throw std::runtime_error(std::string("ERROR::UBO::BINDING\n") + std::string("Can't bind.\n"));
//        }

//        glBindBufferBase(GL_UNIFORM_BUFFER, ownBindingPoint, ubo);
//        if(glGetError() == GL_INVALID_VALUE || glGetError() == GL_INVALID_ENUM)
//        {
//            glDeleteBuffers(1, &ubo);
//            throw std::runtime_error(std::string("ERROR::UBO::BINDING\n") + std::string("Can't bind.\n"));
//        }
    }

    void bindToShader(const ShaderProgram & shader, std::string name)
    {
        unsigned int ub_index = glGetUniformBlockIndex(shader.getId(), name.c_str());
        if(ub_index == GL_INVALID_INDEX)
        {
            throw std::runtime_error(std::string("ERROR::UBO::BINDING\n") + std::string("Can't find specidied uniform object in the shader programm.\n"));
        }
        else if(glGetError() == GL_INVALID_OPERATION)
        {
            throw std::runtime_error(std::string("ERROR::UBO::BINDING\n") + std::string("Can't find specidied shader programm.\n"));
        }

        glUniformBlockBinding(shader.getId(), ub_index, ownBindingPoint);
        if(glGetError() == GL_INVALID_VALUE)
        {
            glDeleteBuffers(1, &ubo);
            throw std::runtime_error(std::string("ERROR::UBO::BINDING\n") + std::string("Can't bind.\n"));
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, ownBindingPoint, ubo);
        if(glGetError() == GL_INVALID_VALUE || glGetError() == GL_INVALID_ENUM)
        {
            glDeleteBuffers(1, &ubo);
            throw std::runtime_error(std::string("ERROR::UBO::BINDING\n") + std::string("Can't bind.\n"));
        }
    }

    void updateAll()
    {
        // save previous binded buffer
        GLint bindedBuffer;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &bindedBuffer);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &ref_data);

        // bind buffer back
        glBindBuffer(GL_UNIFORM_BUFFER, bindedBuffer);
    }


    ///
    /// \brief The IUniformProxy class
    ///
    template<typename Q>
    class IUniformProxy
    {
    public:
        IUniformProxy() = default;
        virtual void operator=(const Q & other) = 0;
    };

    template<typename Q, unsigned int offset, unsigned int size>
    class UniformProxy/* : public IUniformProxy<Q>*/
    {
    public:
        UniformProxy(UniformObject<T> & outer) :
            ref_outer(outer)
        {}

        Q & operator=(Q & other)
        {
            value = other;
            ref_outer.updatePartial<Q, offset, size>(value);
            return value;
        }

        operator Q()
        {
            return value;
        }

    private:
        Q value;
        UniformObject<T> & ref_outer;
    };

    template<typename Q, unsigned int offset, unsigned int size>
    UniformProxy<Q, offset, size> getProxy()
    {
        return UniformProxy<Q, offset, size>(*this);
    }

private:

    template<typename Q, unsigned int offset, unsigned int size>
    void updatePartial(const Q & value)
    {
        // save previous binded buffer
        GLint bindedBuffer;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &bindedBuffer);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &value);

        // bind buffer back
        glBindBuffer(GL_UNIFORM_BUFFER, bindedBuffer);
    }

//    template<typename Q, unsigned int offset, unsigned int size>
//    class UniformProxy : public IUniformProxy<Q>
//    {
//    public:
//        UniformProxy(UniformObject<T> & outer) :
//            ref_outer(outer)
//        {}

//        virtual void operator=(const Q & other) override
//        {
//            ref_outer.updatePartial<Q, offset, size>(other);
//        }

//    private:
//        UniformObject<T> & ref_outer;
//    };

    const T & ref_data;
    unsigned int ubo;
};


template<typename T>
class IUniformProxy
{
public:
    virtual T & operator=(const T & other) = 0;
};

#endif // UNIFORMOBJECT_H
