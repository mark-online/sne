#pragma once

/// ���������� Singleton ����� �ݴ��ϴ� ����������, ����� ���Ǽ��� ���ؼ���
/// ���������� ����ϴ� �Ϳ��� �����Ѵ�. ������ �Ǵ� �޽�带 ����
/// �ν��Ͻ��� �ѱ�� ������ �ʹ� �������ϴ�.
/// ����, ���⿡�� ���̴� �̱����� �ν��Ͻ� �������� �ش� Ŭ������ �ƴ�
/// �ٸ� Ŭ����(���� ���ø����̼�)�� �ν��Ͻ����� �����ϵ��� �Ͽ� ���� �߻�
/// ������ �ִ��� ���ݴ�.

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
