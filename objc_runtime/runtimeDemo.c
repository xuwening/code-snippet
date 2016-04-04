//
//  runtimeDemo.c
//  TipsDemo
//
//  Created by 洪敬军 on 16/4/4.
//  Copyright © 2016年 洪敬军. All rights reserved.
//

#include "runtimeDemo.h"

//这里只依赖c库，不依赖objective c任何资源
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//c语言中没有bool类型，自己定义
#ifndef bool
#define bool int
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

//模拟编译动作

//对象创建、释放、属性方法操作
//====================================
TObject *createObject() {
    
    int classSize = sizeof(TClass);
    
    TObject *obj = malloc(classSize);
    
    if (obj == NULL) {
        return NULL;
    }
    
    obj->isa = NULL;
    obj->superClass = NULL;
    obj->propertys = NULL;
    obj->methods = NULL;
    
    return obj;
}

void releaseObject(TObject *obj) {
    
    if (obj != NULL) {
        
        free(obj->methods);
        free(obj->propertys);
        free(obj->isa);
        
        obj->superClass = NULL;
        
        free(obj);
    }
}

void addProperty(TObject *obj, TProperty *property) {
    obj->propertys = property;
}

void addMethod(TObject *obj, TMethod *method) {
    obj->methods = method;
}

void setMetaClass(TObject *obj, TClass *metaClass) {
    obj->isa = metaClass;
}

void setSuperClass(TObject *obj, TClass *superClass) {
    obj->superClass = superClass;
}

//为简单起见，这里没有做列表，只实现了一个属性
TProperty *createProperty(char *name, void *value) {
    
    int size = sizeof(TProperty);
    TProperty *property = malloc(size);
    if (property == NULL) {
        return NULL;
    }
    
    property->name = name;
    property->value = value;
    return property;
}

//为简单起见，这里没有做列表，只实现了一个属性
TMethod *createMethod(char *name, void *func) {
    
    int size = sizeof(TMethod);
    TMethod *method = malloc(size);
    if (method == NULL) {
        return NULL;
    }
    
    method->name = name;
    method->function = func;
    return method;
}

//为简单起见，这里只实现了无参函数
typedef void (*NoArgFunction)();

//发送消息函数
bool sendMessage(TObject *obj, char *name) {
    
    if (obj == NULL) {
        return false;
    }
    
    //从实例对应的类中寻找方法
    TObject *tmpObj = obj->isa;
    
    while (tmpObj != NULL) {
        
        if (strcmp(tmpObj->methods->name, name) == 0) {
            
            NoArgFunction func = (NoArgFunction) tmpObj->methods->function;
            func();
            return true;
        }
        
        //这里只处理了实例方法，没有处理类方法（因为我们没有做类和元类的区分，有兴趣自己实现）
        tmpObj = tmpObj->superClass;
    }
    
    return false;
}

//===========================================

void load() {
    printf("load method ...\n");
}

void isEqual() {
    printf("isEqual method ...\n");
}

//创建NSObject对象
static TObject *g_NSObject = NULL;
static TClass *g_NSObjectMataClass = NULL;
void createNSObject() {
    
    if (g_NSObjectMataClass == NULL) {
        g_NSObjectMataClass = createObject();
    }
    
    if (g_NSObject == NULL) {
        g_NSObject = createObject();
    }
    
    g_NSObject->isa = g_NSObjectMataClass;
    g_NSObjectMataClass->superClass = g_NSObject;
    
    //初始化实例属性和实例方法
    TMethod *method = createMethod("isEqual", isEqual);
    g_NSObject->methods = method;
    
    //初始化静态属性和静态方法
    method = createMethod("load", load);
    g_NSObjectMataClass->methods = method;
}

void sayHi_person() {
    
    printf("I'm a person.\n");
}

//创建用户自定义对象Person
TClass *createPerson() {
    
    TObject *person = createObject();
    TClass *personClass = createObject();
    
    person->isa = personClass;
    person->superClass = g_NSObject;
    
    personClass->isa = g_NSObjectMataClass;
    personClass->superClass = g_NSObjectMataClass;
    
    //初始化实例属性和实例方法
    TMethod *method = createMethod("sayHi", sayHi_person);
    person->methods = method;
    
    //初始化静态属性和静态方法
    //...
    
    return person;
}

void sayHi_xiaoming() {
    
    printf("I'm xiaoming.\n");
}

//创建用户自定义对象xiaoming
TClass *createPersonXiaoming(TClass *superClass) {
    
    TObject *xiaoming = createObject();
    TClass *xiaomingClass = createObject();
    
    xiaoming->isa = xiaomingClass;
    xiaoming->superClass = superClass;
    
    xiaomingClass->isa = g_NSObjectMataClass;
    xiaomingClass->superClass = superClass->superClass;
    
    //初始化实例属性和实例方法
    TMethod *method = createMethod("sayHi", sayHi_xiaoming);
    xiaoming->methods = method;
    
    //初始化静态属性和静态方法
    //...
    
    return xiaoming;
}

void sayHi_xiaoqiang() {
    
    printf("I'm xiaoqiang.\n");
}

//创建用户自定义对象xiaoqiang
TClass *createPersonXiaoqiang(TClass *superClass) {
    
    TObject *xiaoqiang = createObject();
    TClass *xiaoqiangClass = createObject();
    
    xiaoqiang->isa = xiaoqiangClass;
    xiaoqiang->superClass = superClass;
    
    xiaoqiangClass->isa = g_NSObjectMataClass;
    xiaoqiangClass->superClass = superClass->superClass;
    
    //初始化实例属性和实例方法
    TMethod *method = createMethod("sayHi", sayHi_xiaoqiang);
    xiaoqiang->methods = method;
    
    //初始化静态属性和静态方法
    //...
    
    return xiaoqiang;
}


TObject *alloc_Object(TClass *class) {
    
    if (class == NULL) {
        return NULL;
    }
    
    TObject *object = malloc(sizeof(TObject));
    memcpy(object, class, sizeof(TObject));
    
    //对象指向实例化它的类
    object->isa = class;
    //实例对象没有父类
    object->superClass = NULL;
    return object;
}

//依据对象类型执行对应的函数
void performSayHI(TObject *obj) {
    
    //通过发送消息实现函数执行，这里的sayHi字符串可以看做是消息，和真实要执行的函数名可以不同
    //objective c中的属性访问，也是通过发送setter getter消息实现，这也是可以通过KVC可以访问属性的原因（这里未实现）
    sendMessage(obj, "sayHi");
}

//===========================================

/**
 *  模拟objective c示例：  personClass继承根类，另外两个类继承personClass

 @interface personClass : NSObject
 
 -(void) sayHi;
 
 @end
 
 @interface xiaomingClass : personClass
 
 -(void) sayHi;
 
 @end
 
 @interface xiaoqiangClass : personClass
 
 -(void) sayHi;
 
 @end

 */

void entryMain() {
    
    //初始化根类
    createNSObject();
    
    //生成用户自定义类：编译器实现
    TClass *personClass = createPerson();
    TClass *xiaomingClass = createPersonXiaoming(personClass);
    TClass *xiaoqiangClass = createPersonXiaoqiang(personClass);
    
    //根据用户自定义类生成实例: 类似执行 [[Persion alloc] init];
    TObject *person = alloc_Object(personClass);
    TObject *xiaoming = alloc_Object(xiaomingClass);
    TObject *xiaoqiang = alloc_Object(xiaoqiangClass);
    
    //执行多态
    performSayHI(person);       //打印I'm a person.
    performSayHI(xiaoming);     //打印I'm xiaoming.
    performSayHI(xiaoqiang);    //打印I'm xiaoqiang.
}


