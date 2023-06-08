#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// c 实现面向对象 https://www.zhihu.com/question/48785886/answer/112651728
// https://zhuanlan.zhihu.com/p/338267632
/**************************************************/
struct _MyObject;

typedef struct _MyClass{
    struct _MyObject* (*_new)();
    void (*_delete)(struct _MyObject*);
    void (*init)(struct _MyObject*);
    void (*destruct)(struct _MyObject*);
    void (*set_data)(struct _MyObject*,int size,void* data);
    void* (*get_data)(struct _MyObject*);
    int (*work)(struct _MyObject*);
    void (*release)(struct _MyObject*);
}MyClass;


typedef struct _MyObject{
    MyClass*classptr;
}MyObject;

typedef struct _SubClass_Object
{
    MyObject _myobject;
    void* data;
}SubClass_Object;




/**************************************************/
static MyObject* subclass_new();
static void subclass_delete(MyObject* obj);
static void subclass_init(MyObject* ptr);
static void subclass_destruct(MyObject* ptr);
static void subclass_set_data(MyObject* ptr,int size,void* data);
static void* subclass_get_data(MyObject* ptr);
static void subclass_release(MyObject* ptr);

MyClass subClass={subclass_new,subclass_delete,subclass_init,subclass_destruct,subclass_set_data,subclass_get_data,NULL,subclass_release};

/**************************************************/
static MyObject* subclass_new()
{
    return (MyObject*)malloc(sizeof(SubClass_Object));
}
static void subclass_delete(MyObject* obj)
{
    free(obj);
}
/**************************************************/
static void subclass_init(MyObject* ptr)
{
    SubClass_Object* _this=(SubClass_Object*)ptr;
    _this->_myobject.classptr=&subClass;
    _this->data=NULL;
}
static void subclass_destruct(MyObject* ptr)
{
    SubClass_Object* _this=(SubClass_Object*)ptr;
    if(_this->data!=NULL)
    {
        _this->_myobject.classptr->release(ptr);
    }
}
/**************************************************/
static void subclass_set_data(MyObject* ptr,int size,void* data)
{
    SubClass_Object* _this=(SubClass_Object*)ptr;
    if(_this->data!=NULL)
    {
        _this->_myobject.classptr->release(ptr);
    }
    if(size>0)
    {
        _this->data=malloc(size);
        memcpy(_this->data,data,size);
    }
}
static void* subclass_get_data(MyObject* ptr)
{
    SubClass_Object* _this=(SubClass_Object*)ptr;
    return _this->data;
}
static void subclass_release(MyObject* ptr)
{
    SubClass_Object* _this=(SubClass_Object*)ptr;
    if(_this->data!=NULL)
    {
        free(_this->data);
        _this->data=NULL;
    }
}
/**************************************************/
MyObject* create_object(MyClass* _class)
{
    MyObject* newobj=_class->_new();
    _class->init(newobj);
    return newobj;
}
void delete_object(MyObject* obj)
{
    MyClass*class = obj->classptr;
    class->destruct(obj);
    class->_delete(obj);
}

int main()
{
    
    MyObject* obj;
    char data[]="fgjyhuj";
    obj=create_object(&subClass);
    obj->classptr->set_data(obj,sizeof(data),data);

    printf("%s\n",(char*)(obj->classptr->get_data(obj))); 

    return 0;
}