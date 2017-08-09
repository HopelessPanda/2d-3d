/*
***********************************************************************************
** INTEL CORPORATION                                                              *
** Copyright 2008-2010 Intel Corporation All Rights Reserved.                     *
**                                                                                *
** The source code contained or described herein and all documents related to     *
** the source code ("Material") are owned by Intel Corporation or its suppliers   *
** or licensors. Title to the Material remains with Intel Corporation or its      *
** suppliers and licensors. The Material contains trade secrets and proprietary   *
** and confidential information of Intel or its suppliers and licensors. The      *
** Material is protected by worldwide copyright and trade secret laws and         *
** treaty provisions. No part of the Material may be used, copied, reproduced,    *
** modified, published, uploaded, posted, transmitted, distributed, or            *
** disclosed in any way without Intel's prior express written permission.         *
**                                                                                *
** No license under any patent, copyright, trade secret or other intellectual     *
** property right is granted to or conferred upon you by disclosure or delivery   *
** of the Materials, either expressly, by implication, inducement, estoppel or    *
** otherwise. Any license under such intellectual property rights must be         *
** express and approved by Intel in writing.                                      *
***********************************************************************************
*/

#ifndef _ASYNC_STD_STREAMS
#define _ASYNC_STD_STREAMS

namespace async {

    extern "C"  void           _init_control(void**);
    extern "C"  void           _destroy_control(void*);
    extern "C"  int            _wait_queue(void*, unsigned int);
 
    extern "C"  void*          _create_operation_object(size_t, void*);
    extern "C"  void           _delete_operation_object(void*);
    extern "C"  void           _init_operation_object(void*, void*);
    extern "C"  unsigned int   _get_last_operation_id(void*);

    extern "C"  int            _get_status(unsigned int, void*);

    extern "C"  unsigned int   _get_last_error(void*);
    extern "C"  unsigned int   _get_error_operation_id(void*);

    extern "C"  int _stop_queue(void*);
    extern "C"  int _resume_queue(void*);
    extern "C"  int _clear_queue(void*);


    enum STATUS {
        STATUS_WAIT =0,
        STATUS_COMPLETED,
        STATUS_ERROR,
        STATUS_IS_EXECUTING,
        STATUS_BLOCKED
    };

    // Base class for any operations
    class base_operation{
    public:
        virtual void execute_operation()=0;
    };

    // Base class for thread which controls async execution
    class thread_control{
        void* control_id;
    public:
        void* get_control_id() {return control_id;}
        void push_back_operation(class base_operation* new_stmt){
            _init_operation_object(new_stmt, control_id);
        }

// get Id of last added operation
        unsigned int get_last_operation_id()
        {
            return _get_last_operation_id(control_id);
        }

// Wait operations
        int wait()
        {
            return _wait_queue(control_id, 0);
        }
        int wait(unsigned int id)
        {
            return _wait_queue(control_id, id);
        }

// get STATUS of control_thread or operation ID
        int get_status(unsigned int operation_id)
        {
            return _get_status(operation_id, control_id);
        }

// get error status
        unsigned int get_last_error()
        {
            return _get_last_error(control_id);
        }
        unsigned int get_error_operation_id()
        {
            return _get_error_operation_id(control_id);
        }

// asynchronous operations queue control functions
        int stop_queue()   //stop queue
        {
            return _stop_queue(control_id);
        }
        int resume_queue() //resume queue
        {
            return _resume_queue(control_id);
        }
        int clear_queue()  //clear queue
        {
            return _clear_queue(control_id);
        }


        ~thread_control() { wait(); _destroy_control(control_id);}
    
    protected:
        thread_control():control_id(0) {
                _init_control(&(control_id));
        }
    };

    typedef class thread_control thread_control;


#define ASYNC_MEMORY_OPERATIONS                                      \
    void* operator new (size_t object_size, void* thread_control)    \
    { return async::_create_operation_object(object_size, thread_control);} 



    // Main template class

template<class A>
class async_class: public A, public thread_control  {
public:

    // Constructors
    async_class():A(){
    }
    template <class T>
    async_class(T val):A(val){
    }
    template <class T, class T2>
    async_class(T val, T2 val2):A(val, val2){
    }
    template <class T, class T2, class T3>
    async_class(T val, T2 val2, T3 val3):A(val, val2, val3){
    }


    // Destructor
//  ~async_class()
//  {
//  }
};


};


#if defined(WIN32) || defined(_WIN32)
#pragma comment (lib, "libicaio.lib")
#endif


#include <iostream>

// Output operator << 
    template<class Stream, class V>
    class output_operator:public async::base_operation{
        V data;
        Stream& str;
    public:
        output_operator(V val, Stream& strio):data(val),str(strio){}

        ASYNC_MEMORY_OPERATIONS
    
        void execute_operation()
        {
           str << (data);
        }
    };

    // Input operator >> 
    template<class Stream, class V>
    class input_operator:public async::base_operation{
        V data;
        Stream& str;
    public:
        input_operator(V val, Stream& strio):data(val),str(strio){}

        ASYNC_MEMORY_OPERATIONS
    
        void execute_operation()
        {
            str >> (data);
        }
    };





#define outputOperatorForBaseTypes(_type)   \
    template <class A> \
    async::async_class<A>& operator << ( async::async_class<A>&str, _type val)\
    { \
    class async::base_operation* new_op= new(str.get_control_id()) output_operator<A, _type>(val,(A&)(str));\
        str.push_back_operation(new_op);\
        return str;\
    }\
\


//  =====================================================================
//  Operator <<

// Operator <<  for base types
    outputOperatorForBaseTypes(bool)
    outputOperatorForBaseTypes(char)
    outputOperatorForBaseTypes(wchar_t)
    outputOperatorForBaseTypes(unsigned char)
    outputOperatorForBaseTypes(signed char)
    outputOperatorForBaseTypes(short)
//  outputOperatorForBaseTypes(unsigned short)
    outputOperatorForBaseTypes(int)
    outputOperatorForBaseTypes(unsigned int)
    outputOperatorForBaseTypes(long)
    outputOperatorForBaseTypes(unsigned long)
    outputOperatorForBaseTypes(long long)
    outputOperatorForBaseTypes(unsigned long long)
    outputOperatorForBaseTypes(float)
    outputOperatorForBaseTypes(double)
    outputOperatorForBaseTypes(long double)

// Operator <<  for references
    template <class A, class V>
    async::async_class<A>& operator << ( async::async_class<A>&str, V& val)
    {
        class async::base_operation* new_op= new(str.get_control_id()) output_operator<A, V&>(val,(A&)(str));
        str.push_back_operation(new_op);
        return str;
    }

// Operator <<  for pointers
    template <class A, class V>
    async::async_class<A>& operator << ( async::async_class<A>&str, V* val)
    {
        class async::base_operation* new_op= new(str.get_control_id()) output_operator<A, V*>(val,(A&)(str));
        str.push_back_operation(new_op);
        return str;
    }


#if defined(WIN32)|| defined(_WIN32)
// Operator << for stream manipulators
    template <class A, class B>
    async::async_class<A>& operator << ( async::async_class<A>&str, typename B& (*val) (typename B&))
    {
        typedef typename B&(*manip)(typename B&);
        class async::base_operation* new_op= new(str.get_control_id()) output_operator<A, manip>(val,(A&)(str));
        str.push_back_operation(new_op);
        return str;
    }
#endif
// operator  << is necessary for ofstream objects to use standard manipulators
    template <class A>
    async::async_class<A>& operator << ( async::async_class<A>&str, class std::basic_ostream<char,struct std::char_traits<char> >&(*val)(class std::basic_ostream<char,struct std::char_traits<char> >&))
    {
        typedef class std::basic_ostream<char,struct std::char_traits<char> >&(*manip_basic)(class std::basic_ostream<char,struct std::char_traits<char> >&);
        class async::base_operation* new_op= new(str.get_control_id()) output_operator<A, manip_basic>(val,(A&)(str));
        str.push_back_operation(new_op);
        return str;
    }
//  Operator <<
//  =====================================================================


//  =====================================================================
//  Operator >>

    // Operator >>  for base types

#define inputOperatorForBaseTypes(_type)   \
    template <class A> \
    async::async_class<A>& operator >> ( async::async_class<A>&str, _type val)\
    { \
        class async::base_operation* new_op= new(str.get_control_id()) input_operator<A, _type>(val,(A&)(str));\
        str.push_back_operation(new_op);\
        return str;\
    }\
\


    inputOperatorForBaseTypes(bool&)
    inputOperatorForBaseTypes(char&)
    inputOperatorForBaseTypes(wchar_t&)
    inputOperatorForBaseTypes(unsigned char&)
    inputOperatorForBaseTypes(signed char&)
    inputOperatorForBaseTypes(short&)
//  inputOperatorForBaseTypes(unsigned short&)
    inputOperatorForBaseTypes(int&)
    inputOperatorForBaseTypes(unsigned int&)
    inputOperatorForBaseTypes(long&)
    inputOperatorForBaseTypes(unsigned long&)
    inputOperatorForBaseTypes(long long&)
    inputOperatorForBaseTypes(unsigned long long&)
    inputOperatorForBaseTypes(float&)
    inputOperatorForBaseTypes(double&)
    inputOperatorForBaseTypes(long double&)


// Operator >>  for references
    template <class A, class V>
    async::async_class<A>& operator >> ( async::async_class<A>&str, V& val)
    {
        class async::base_operation* new_op= new(str.get_control_id()) input_operator<A, V&>(val,(A&)(str));
        str.push_back_operation(new_op);
        return str;
    }


//  Operator >>
//  =====================================================================
#endif
