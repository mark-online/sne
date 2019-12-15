#include "SecurityTestPCH.h"
#include <sne/security/hash/AuthlogicSha512Hash.h>

using namespace sne;

/**
 * @class AuthlogicSha512HashTest
 *
 * SHA512 hash 테스트.
 */
class AuthlogicSha512HashTest : public testing::Test
{
};


// Rails Device gem의 AuthlogicSha512 시뮬레이션
//def self.digest(password, stretches, salt, pepper)
//    digest = [password, salt].flatten.join('')
//    stretches.times { digest = Digest::SHA512.hexdigest(digest) }
//    digest
//end
TEST_F(AuthlogicSha512HashTest, testAuthlogicSha512Hash)
{
    const std::string salt = "pjzpWoaW1TBMAUi7JNvg";
    const std::string devisePassword =
        "2f1f38aec3808cb52a23fad96a42806e8cfbdf08660c05cfb8d3db8b4b1c936d78382ec"
        "6025a92c176d7a5afe9bede049420f271b0aa6e17e4b04335df0bab53";

    const std::string password = "mark_admin";
    security::AuthlogicSha512Hash hasher;
    const std::string digest = hasher.digest(password, salt);

    ASSERT_EQ(digest, devisePassword);
}
