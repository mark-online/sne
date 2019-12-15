#include "DatabaseTestPCH.h"

#if defined(_WIN32) && defined(SNE_BUILD_DATABASE_TEST)

#include <sne/database/ado/AdoDatabase.h>
#include <sne/core/utility/Win32Util.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace sne;
using namespace sne::base;
using namespace sne::database;

#ifdef SNE_USE_ORACLE_DATABASE

/// Oracle 10g
const char connectionString[] =
    "Provider=OraOLEDB.Oracle;"
    "Data Source=sne_db_server:1521/storm;"
    "User Id=sne_test;"
    "Password=sne_test;"
    "PLSQLRSet=1;"; // support Oracle ref cursor

#else

const char connectionString[] =
    "Provider=sqloledb;"
    "Data Source=localhost\\SQLEXPRESS;"
    "Initial Catalog=sne_test;"
    "User Id=sne_test;"
    "Password=sne_test;"
    "Trusted_Connection=False;";

#endif // SNE_USE_ORACLE_DATABASE


/**
* @class AdoDatabaseTest
*
* ADO(ActiveX Data Objects) 테스트
* - doc/database/create_test_user.sql을 실행해야 한다
*/
class AdoDatabaseTest : public testing::Test
{ 
protected:
    virtual void SetUp();
    virtual void TearDown();

protected:
    std::string getErrorString(AdoDatabase& db, const std::string& msg);
    void createTestTable();
    void dropTestTable();
    int insertRecords(int row_count);
    void createStoredProcedure();
    void dropStoredProcedure();

protected:
    AdoDatabase* database_;
    core::InitOle initOle_;
};


void AdoDatabaseTest::SetUp()
{
    database_ = new AdoDatabase;
    database_->open(connectionString);

    dropTestTable();
    createTestTable();

    dropStoredProcedure();
    createStoredProcedure();
}

void AdoDatabaseTest::TearDown()
{
    dropStoredProcedure();
    dropTestTable();

    database_->close();
    delete database_;
}


std::string AdoDatabaseTest::getErrorString(AdoDatabase& db,
    const std::string& msg)
{
    std::string result(msg);
    result += "(Error:";
    result += db.getLastError();
    result += ")";
    return result.c_str();
}


void AdoDatabaseTest::createTestTable()
{
    try {
        if (! database_->isOpened()) {
            return;
        }

        database_->execute(
            "CREATE TABLE test_table "
            "(a int NOT NULL,b char (10) NULL,c SMALLDATETIME NULL,d BIGINT,"
            "e NVARCHAR(10))");
    }
    catch (const AdoException& /*e*/) {
        ASSERT_TRUE(false) << getErrorString(*database_, "createTestTable");
    }
}


void AdoDatabaseTest::dropTestTable()
{
    if (! database_->isOpened()) {
        return;
    }

    try {
        database_->execute("DROP TABLE test_table");
    }
    catch (const AdoException&) {}
}


int AdoDatabaseTest::insertRecords(int row_count)
{
    if (!database_->isOpened()) {
        return 0;
    }

    int sum_a = 0;
    for (int i = 0; i < row_count; ++i) {
        std::ostringstream sql;
        sql << "INSERT INTO test_table (a,b,c,d,e) VALUES (" << i << ",'" <<
            i << "', GETDATE()," << (i * 1000000000L) << ",N'아리랑')";
        EXPECT_NO_THROW(database_->execute(sql.str())) <<
            getErrorString(*database_, sql.str());
        sum_a += i;
    }
    return sum_a;
}


void AdoDatabaseTest::createStoredProcedure()
{
    if (!database_->isOpened()) {
        return;
    }

    const char sp1[] =
        "CREATE PROCEDURE sp_ReturnTest As return (10)";
    ASSERT_NO_THROW(database_->execute(sp1)) <<
        getErrorString(*database_, connectionString);

    const char sp2[] =
        "Create Procedure sp_OutputTest "
        "@IN1 int,"
        "@OutInt int OUTPUT,"
        "@OutChar varchar(20) OUTPUT,"
        "@OutDate smalldatetime OUTPUT,"
        "@OutNvarchar NVARCHAR(10) OUTPUT "
        "As "
        "SELECT @OutChar = 'Hello World' "
        "SELECT @OutInt = 10 * @IN1 "
        "SELECT @OutDate = cast('1970-01-02 00:00:00' as smalldatetime) "
        "SELECT @OutNvarchar = N'아리랑' "
        "return (90)";
    ASSERT_NO_THROW(database_->execute(sp2)) <<
        getErrorString(*database_, connectionString);

    const char sp3[] =
        "CREATE PROCEDURE sp_CollectTest As SELECT * FROM test_table";
    ASSERT_NO_THROW(database_->execute(sp3)) <<
        getErrorString(*database_, connectionString);
}


void AdoDatabaseTest::dropStoredProcedure()
{
    if (!database_->isOpened()) {
        return;
    }

    try {
        database_->execute("DROP PROCEDURE sp_ReturnTest");
        database_->execute("DROP PROCEDURE sp_OutputTest");
        database_->execute("DROP PROCEDURE sp_CollectTest");
    }
    catch (const AdoException&) {}
}


TEST_F(AdoDatabaseTest, testOpen)
{
    ASSERT_EQ(true, database_->isOpened());

    database_->close();
    ASSERT_EQ(false, database_->isOpened());
}


TEST_F(AdoDatabaseTest, testInsert)
{
    ASSERT_NO_THROW(
        database_->execute(
            "INSERT INTO test_table (a,b,c,d,e) VALUES "
            "(1, '1', GETDATE(), 0, N'A')"));
    ASSERT_EQ(1, database_->getAffectedRecordsCount());
}


TEST_F(AdoDatabaseTest, testSelect)
{
    const std::string insert("INSERT INTO test_table (a,b,c,d,e) VALUES "
        "(2, '2', GETDATE(), 9223372036854775807, N'아리랑')");
    ASSERT_NO_THROW(database_->execute(insert)) <<
        getErrorString(*database_, insert);

    const std::string query("SELECT a,b,c,d,e FROM test_table WHERE a=2");
    int a;
    std::string b;
    time_t c;
    int64_t d;
    std::wstring e;
    AdoRecordSet rs(*database_);
    if (rs.open(query)) {
        try {
            rs.getFieldValue(0, a);
            rs.getFieldValue(1, b);
            rs.getFieldValue(2, c);
            rs.getFieldValue(3, d);
            rs.getFieldValue(4, e);
            ASSERT_EQ(2, a);
            ASSERT_EQ('2', b[0]);
            ASSERT_EQ(9223372036854775807, d);
            ASSERT_EQ(L"아리랑", e);
        }
        catch (const AdoException&) {
            ASSERT_TRUE(false) << getErrorString(*database_, query);
        }
    }
    else {
        ASSERT_TRUE(false) << getErrorString(*database_, query);
    }
}


TEST_F(AdoDatabaseTest, testSelectMultipleRow)
{
    const int row_count = 10;
    int record_count = 0;
    int total_a = 0;
    int sum_a = 0;

    sum_a = insertRecords(row_count);

    const std::string query("SELECT * FROM test_table");
    AdoRecordSet rs(*database_);
    if (rs.open(query)) {
        while (!rs.isEof()) {
            int a;
            std::string b;
            try {
                rs.getFieldValue(0, a);
                rs.getFieldValue(1, b);
                total_a += a;
                ++record_count;
                ASSERT_NO_THROW(rs.moveNext()) <<
                    getErrorString(*database_, "moveNext");
            }
            catch (const AdoException&) {
                ASSERT_TRUE(false) << getErrorString(*database_, query);
            }
        }
    }
    else {
        ASSERT_TRUE(false) << getErrorString(*database_, query);
    }

    ASSERT_EQ(row_count, record_count);
    ASSERT_EQ(sum_a, total_a);
}


TEST_F(AdoDatabaseTest, testStoredProcedureArguments)
{
    int outInt = 0;
    std::string outChar;
    time_t outDate;
    int retVal = 0;
    std::wstring woutChar;

    AdoCommand cmd(*database_, "sp_OutputTest");

    try {
        AdoIntegerParameter paramRetVal(cmd, ADODB::adParamReturnValue);
        AdoIntegerParameter paramIn(cmd);
        paramIn.setValue(2);
        AdoIntegerParameter paramOutInt(cmd, ADODB::adParamOutput);
        AdoVarCharParameter paramOutChar(cmd, 20, ADODB::adParamOutput);
        AdoDateParameter paramOutDate(cmd, ADODB::adParamOutput);
        AdoVarWCharParameter paramOutNvarchar(cmd, 10, ADODB::adParamOutput);

        AdoRecordSet rs(*database_);
        ASSERT_NO_THROW(rs.execute(cmd)) <<
            getErrorString(*database_, "sp_OutputTest");

        paramRetVal.getValue(retVal);
        ASSERT_EQ(90, retVal) << "sp_OutputTest return value";
        paramOutInt.getValue(outInt);
        ASSERT_EQ(2 * 10, outInt) << "sp_OutputTest outInt value";
        paramOutChar.getValue(outChar);
        ASSERT_EQ(std::string("Hello World"), outChar) <<
            "sp_OutputTest outChar value";
        paramOutDate.getValue(outDate);
        paramOutNvarchar.getValue(woutChar);
        ASSERT_EQ(L"아리랑", woutChar) << "sp_OutputTest woutChar value";
    }
    catch (const AdoException&) {
        ASSERT_TRUE(false) << getErrorString(*database_, "sp_OutputTest");
    }
}


TEST_F(AdoDatabaseTest, testStoredProcedureReturnValue)
{
    int retVal = 0;

    AdoCommand cmd(*database_, "sp_ReturnTest");
    AdoIntegerParameter paramRetVal(cmd, ADODB::adParamReturnValue);

    AdoRecordSet rs(*database_);
    ASSERT_NO_THROW(rs.execute(cmd)) <<
        getErrorString(*database_, "sp_ReturnTest");

    try {
        paramRetVal.getValue(retVal);
        ASSERT_EQ(10, retVal);
    }
    catch (const AdoException&) {
        ASSERT_TRUE(false) << getErrorString(*database_, "sp_ReturnTest");
    }
}


TEST_F(AdoDatabaseTest, testStoredProcedureCollection)
{
    const int row_count = 10;
    int sum_a = insertRecords(row_count);

    AdoCommand cmd(*database_, "sp_CollectTest");

    AdoRecordSet rs(*database_);
    ASSERT_NO_THROW(rs.execute(cmd)) <<
        getErrorString(*database_, "sp_CollectTest");

    int record_count = 0;
    int total_a = 0;
    while (!rs.isEof()) {
        int a = 0;
        std::string b;
        try {
            rs.getColumnValue("a", a);
            rs.getColumnValue("b", b);
        }
        catch (const AdoException&) {
            ASSERT_TRUE(false) << getErrorString(*database_, "sp_CollectTest");
        }

        total_a += a;
        ++record_count;
        ASSERT_NO_THROW(rs.moveNext()) <<
            getErrorString(*database_, "moveNext");
    }

    ASSERT_EQ(row_count, record_count);
    ASSERT_EQ(sum_a, total_a);
}


TEST_F(AdoDatabaseTest, testConnectionPerformance)
{
    const int count = 100;

    const auto startTime = std::chrono::high_resolution_clock::now();

    AdoDatabase database;
    for (int i = 0; i < count; ++i) {
        ASSERT_EQ(true, database.open(connectionString));
        database.close();
    }

    const auto stopTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsedSecs = (stopTime - startTime);
    ASSERT_LT(elapsedSecs.count(), 1.0f);
}


TEST_F(AdoDatabaseTest, testFetchFloat)
{
    const char query[] = "SELECT CAST(1000000.12 AS FLOAT)";

    AdoRecordSet rs(*database_);
    if (rs.open(query)) {
        try {
            double d;
            rs.getFieldValue(0, d);
            ASSERT_DOUBLE_EQ(1000000.12, d);
        }
        catch (const AdoException&) {
            ASSERT_TRUE(false) << getErrorString(*database_, query);
        }
    }
    else {
        ASSERT_TRUE(false) << getErrorString(*database_, query);
    }
}


TEST_F(AdoDatabaseTest, testFetchDouble)
{
    const char query[] = "SELECT CAST(12345678901.1234 AS FLOAT(53))";

    AdoRecordSet rs(*database_);
    if (rs.open(query)) {
        try {
            double d;
            rs.getFieldValue(0, d);
            ASSERT_DOUBLE_EQ(12345678901.1234, d);
        }
        catch (const AdoException&) {
            ASSERT_TRUE(false) << getErrorString(*database_, query);
        }
    }
    else {
        ASSERT_TRUE(false) << getErrorString(*database_, query);
    }
}


TEST_F(AdoDatabaseTest, testFetchDecimal)
{
    const char query[] = "SELECT CAST(12345678901.1234 AS DECIMAL(19,4))";

    AdoRecordSet rs(*database_);
    if (rs.open(query)) {
        try {
            double d;
            rs.getFieldValue(0, d);
            ASSERT_NEAR(12345678901.1234, d, 0.001);
        }
        catch (const AdoException&) {
            ASSERT_TRUE(false) << getErrorString(*database_, query);
        }
    }
    else {
        ASSERT_TRUE(false) << getErrorString(*database_, query);
    }
}

#endif // #if defined(_WIN32) && defined(SNE_BUILD_DATABASE_TEST)
