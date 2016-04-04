//
//  runtimeDemo.h
//  TipsDemo
//
//  Created by 洪敬军 on 16/4/4.
//  Copyright © 2016年 洪敬军. All rights reserved.
//

#ifndef runtimeDemo_h
#define runtimeDemo_h


/**
 *  objective c语言具有动态类型语言的特征，但它不是通过动态语言的解释器方式工作，而是本地二进制运行，
 *  因此，兼具动态特征与本地性能的折中。
 *  实现原理：通过本地c语言抽象出对象、多态的概念，封装成运行库，在编译器侧建立面向对象语言的语法，编译器将语法预编译为c语言
 *  ，在编译到本地目标文件。
 *
 *  在objective c中，有对象和类，还有元类的概念，类一般保存实例方法，元类保存静态方法
 *
 *  通过实现，可以看出在实现侧对象、类、元类都是内存中运行时的对象，没有本质区别，唯一区别仅仅是职责分工不同，也正因为都是
 *  内存中的对象，所以在运行时可以做到动态修改。
 *
 *  既然所有内容均在内存中，意味着一切都可运行时修改，包括对象、类、元类、继承关系、方法、属性等等。
 *
 *  封装：通过结构体封装属性和方法实现
 *  继承：通过superClass指向父类实现
 *  多态：通过动态方法查找实现
 *
 *  objective c语言被人诟病为类型不安全，这其实是取舍问题，想要这么强大的动态特性，必然要在其他方面做些牺牲
 */


//属性描述
typedef struct tag_method {
    
    char *name;
    void *function;
}TMethod;

//方法描述
typedef struct tag_property {
    
    char *name;
    void *value;
}TProperty;


//最基本的对象描述
typedef struct tag_class {
    
    struct tag_class *isa;   //指向元类
    struct tag_class *superClass;  //指向父类
    
    TProperty *propertys;    //属性列表
    TMethod *methods;        //方法列表
    
    //...                    //other
    
} TClass;

typedef TClass TObject;  //类和对象，都是实例


//=======test==========

//这里我们只是模拟了实例方法的调用，主要是了解对象的几个特性：封装、继承、多态

//这里是入口调用
void entryMain();

#endif /* runtimeDemo_h */
