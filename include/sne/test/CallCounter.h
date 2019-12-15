#pragma once

#include <sne/Common.h>
#include <sne/core/container/Containers.h>
#include <map>
#include <string>

namespace sne { namespace test {

    /**
     * @class CallCounter
     *
     * 함수 호출 횟수 기록
     */
    class CallCounter
    {
        typedef core::Map<std::string, int> CallCounts;
    public:
        int getCallCount(const std::string& funcName) const {
            const CallCounts::const_iterator pos = callCounts_.find(funcName);
            if (pos != callCounts_.end()) {
                return (*pos).second;
            }
            return 0;
        }

    protected:
        void addCallCount(const std::string& funcName, int count = 1) {
            callCounts_[funcName] += count;
        }

    private:
        CallCounts callCounts_;
    };

}} // namespace sne { namespace test {
