#ifndef SNE_REQUESTMETHOD_H
#define SNE_REQUESTMETHOD_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <sne/database/proxy/DatabaseRequest.h>
#include <sne/core/memory/MemoryPoolMixin.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace database {
class DatabaseManager;
}} // namespace sne { namespace database {

namespace sne { namespace database { namespace proxy {

/**
 * @enum RequestMethodType
 */
enum RequestMethodType
{
    rmtModify
};


/**
 * @class RequestMethod
 *
 * DB  요청 메쏘드
 * - release() 함수 호출을 통해 메모리를 해제해야 한다(메모리풀 때문에)
 * - command pattern
 */
class RequestMethod : public boost::noncopyable
{
protected:
    ~RequestMethod() {}

public:
    RequestMethod(DatabaseManager& proxyDatabaseManager) :
        proxyDatabaseManager_(proxyDatabaseManager),
        id_(invalidRequestId),
        priority_(rpNormal) {}

    void set(RequestId id, RequestPriority priority) {
        id_ = id;
        priority_ = priority;
    }

    bool less(const RequestMethod& rhs) const {
        return priority_ < rhs.priority_;
    }

public:
    virtual bool call() = 0;

    virtual RequestMethodType getRequestMethodType() const = 0;

    /// 메모리를 해제한다(메모리풀 사용시 필요)
    virtual void deleteRequestMethod() = 0;

protected:
    DatabaseManager& getDatabaseManager() {
        return proxyDatabaseManager_;
    }

    RequestId getRequestId() const {
        return id_;
    }

private:
    DatabaseManager& proxyDatabaseManager_;
    RequestId id_;
    RequestPriority priority_;
};


/**
 * @class ModifyRequestMethod
 *
 * DB 변경(insert, update, delete) 요청 메쏘드
 * - command pattern
 */
class ModifyRequestMethod : public RequestMethod,
    public core::ThreadSafeMemoryPoolMixin<ModifyRequestMethod>
{
public:
    ModifyRequestMethod(DatabaseManager& proxyDatabaseManager) :
        RequestMethod(proxyDatabaseManager) {}

    using RequestMethod::set;

    void set(const ModifyRequest& request) {
        request_ = request;
    }

private:
    virtual bool call();
    virtual RequestMethodType getRequestMethodType() const {
        return rmtModify;
    }

    virtual void deleteRequestMethod() {
        delete this;
    }

private:
    ModifyRequest request_;
};


}}} // namespace sne { namespace database { namespace proxy {


template <>
struct std::less<sne::database::proxy::RequestMethod*> :
    public std::binary_function<sne::database::proxy::RequestMethod*,
        sne::database::proxy::RequestMethod*, bool>
{
    bool operator()(const sne::database::proxy::RequestMethod* left,
        const sne::database::proxy::RequestMethod* right) const {
        return left->less(*right);
    }
};

#endif // SNE_REQUESTMETHOD_H
