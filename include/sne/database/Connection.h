#pragma once

#include <boost/noncopyable.hpp>

namespace sne { namespace database {

/**
* @class Connection
*
* Database Connection root class
*/
class Connection : public boost::noncopyable
{
public:
    virtual ~Connection() {}

    /**
     * 데이터베이스에 연결한다
     * @param connectionString DB 연결을 위한 정보.
     * @param connectionTimeout 연결 시간 제한 (초 단위)
     */
    virtual bool open(const std::string & connectionString,
        uint8_t connectionTimeout) = 0;

    /// 데이터베이스 연결을 해제한다.
    virtual void close() = 0;

    /// 데이터베이스에 연결되었는가?
    virtual bool isOpened() = 0;

    /// 최근 발생한 에러를 문자열 형식으로 얻는다.
    virtual std::string  getLastError() const = 0;

    /// 최근 발생한 에러 코드를 얻는다.
    virtual unsigned int getLastResult() const = 0;
};

}} // namespace sne { namespace database {
