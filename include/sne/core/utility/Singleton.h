#pragma once

/// 개인적으로 Singleton 사용을 반대하는 입장이지만, 사용의 편의성을 위해서는
/// 제한적으로 사용하는 것에는 찬성한다. 생성자 또는 메쏘드를 통해
/// 인스턴스를 넘기는 구조는 너무 지저분하다.
/// 또한, 여기에서 쓰이는 싱글톤은 인스턴스 소유권을 해당 클래스가 아닌
/// 다른 클래스(보통 어플리케이션)의 인스턴스에서 유지하도록 하여 문제 발생
/// 소지를 최대한 없앴다.

#define SNE_DECLARE_SINGLETON(CLASS) \
    public: \
        static void instance(CLASS* newInstance); \
        static CLASS* instance(); \
    private: \
        static CLASS* instance_;

#define SNE_DEFINE_SINGLETON(CLASS) \
    CLASS* CLASS::instance_ = nullptr; \
    void CLASS::instance(CLASS* newInstance) { \
        instance_ = newInstance; \
    } \
    CLASS* CLASS::instance() { \
        return instance_; \
    }
