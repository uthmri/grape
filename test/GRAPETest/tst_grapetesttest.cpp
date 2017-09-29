#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "utility.h"
#include <iostream>

//--------------------------------------------------------------------------------------------------
class GRAPETestTest : public QObject
{
    Q_OBJECT

public:
    GRAPETestTest();

private:
    bool run_GRAPE_and_compare_log(QString pipeline_file_name, QString saved_log_file_name);
    void test_node_case(QString FcnName);
    void test_pipeline_case(QString pipeline_file_name, QString saved_log_file_name);
    void test_pipeline_case(QString FcnName);


private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1_data();
    void testCase1();

    // ===================================================
    // IMAGE OPERATIONS
    // ===================================================
//    void bet() {test_node_case(__FUNCTION__); }
//    void imageregistration() {test_node_case(__FUNCTION__); } //<<<<<<<<<<<
//    void imagesegmentation() {test_node_case(__FUNCTION__); }
//    void biascorrection() {test_node_case(__FUNCTION__); }
//    void curvefitting() {test_node_case(__FUNCTION__); }  //<<<<<<<<<<<<
//    void imagenormalizer() {test_node_case(__FUNCTION__); }
//    void imagearithmetic() {test_node_case(__FUNCTION__); }
//    void imageprocessing() {test_node_case(__FUNCTION__); }
//    void imageslicer() {test_node_case(__FUNCTION__); }
//    void imagestack() {test_node_case(__FUNCTION__); }
//    void imagestatistics() {test_node_case(__FUNCTION__); }
//    void logoperation() {test_node_case(__FUNCTION__); }
//    void reloperator() {test_node_case(__FUNCTION__); }

    // ===================================================
    // IO TOOLS
    // ===================================================
//    void source() {test_node_case(__FUNCTION__); }      // fails due to different random numbers generated
//    void sink() {test_node_case(__FUNCTION__); }
//    void synch() {test_node_case(__FUNCTION__); }
//    void parreader() {test_node_case(__FUNCTION__); }
//    void parwriter() {test_node_case(__FUNCTION__); }
//    void imagereader() {test_node_case(__FUNCTION__); }
//    void imagewriter() {test_node_case(__FUNCTION__); }

//    // ===================================================
//    // PIPELINE TOOLS
//    // ===================================================
//    void divider() {test_node_case(__FUNCTION__); }
//    void select() {test_node_case(__FUNCTION__); }
//    void systemcall() {test_node_case(__FUNCTION__); }
//    void develop() {test_node_case(__FUNCTION__); }
//    void branch() {test_node_case(__FUNCTION__); }
//    void synch() {test_node_case(__FUNCTION__); }
//    void delay() {test_node_case(__FUNCTION__); }
//    void end() {test_node_case(__FUNCTION__); }
//    void loop() {test_node_case(__FUNCTION__); }

    // ===================================================
    // TEST PIPELINES
    // ===================================================
//    void FLAIR2()
//    {
//        QString pipeline_file_name = QString("/test/pipelines/FLAIR2/FLAIR2.grp");
//        QString saved_log_file_name = QString("/test/pipelines/FLAIR2/output.txt.sav");
//        test_pipeline_case(pipeline_file_name,saved_log_file_name);
//    }

//    void FLAIR3() {test_pipeline_case(__FUNCTION__); }
    void FLAIR3()
    {
        QString pipeline_file_name = QString("/test/pipelines/FLAIR3/FLAIR3.grp");
        QString saved_log_file_name = QString("/test/pipelines/FLAIR3/output.txt.sav");
        test_pipeline_case(pipeline_file_name,saved_log_file_name);
    }

};
//--------------------------------------------------------------------------------------------------
GRAPETestTest::GRAPETestTest()
{
}
//--------------------------------------------------------------------------------------------------
void GRAPETestTest::initTestCase()
{
}
//--------------------------------------------------------------------------------------------------
void GRAPETestTest::cleanupTestCase()
{
}
//--------------------------------------------------------------------------------------------------
void GRAPETestTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}
//--------------------------------------------------------------------------------------------------
void GRAPETestTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}
//--------------------------------------------------------------------------------------------------
void GRAPETestTest::test_node_case(QString FcnName)
{
    if(FcnName.contains("GRAPETestTest::"))
        FcnName = FcnName.mid(15);

    QString pipeline_file_name = QString("/test/") + FcnName + QString("_test/") + FcnName + QString("_test.grp");
    QString saved_log_file_name = QString("/test/") + FcnName + QString("_test/output.txt.sav");

    cout << "Testing pipeline: " << pipeline_file_name.toStdString() << endl;

    bool identical_files = run_GRAPE_and_compare_log(pipeline_file_name, saved_log_file_name);
    QVERIFY2(identical_files, "Files not identical");
}
//--------------------------------------------------------------------------------------------------
void GRAPETestTest::test_pipeline_case(QString FcnName)
{
    FcnName = FcnName.mid(15);

    QString pipeline_file_name = QString("/test/pipelines/") + FcnName + "/" + FcnName + QString(".grp");
    QString saved_log_file_name = QString("/test/pipelines/") + FcnName + "/" + QString("output.txt.sav");

    cout << "Testing pipeline: " << pipeline_file_name.toStdString() << endl;

    bool identical_files = run_GRAPE_and_compare_log(pipeline_file_name, saved_log_file_name);
    QVERIFY2(identical_files, "Files not identical");
}
//--------------------------------------------------------------------------------------------------
void GRAPETestTest::test_pipeline_case(QString pipeline_file_name, QString saved_log_file_name)
{
//    qDebug() << "Testing pipeline: " << pipeline_file_name;
    bool identical_files = run_GRAPE_and_compare_log(pipeline_file_name, saved_log_file_name);
    QVERIFY2(identical_files, "Files not identical");
}
//--------------------------------------------------------------------------------------------------
bool GRAPETestTest::run_GRAPE_and_compare_log(QString pipeline_file_name, QString saved_log_file_name)
{
    // get path to GRAPE
    QString GrapeDirPath;

    QDir d(QCoreApplication::applicationDirPath());
    d.cdUp();
    d.cdUp();
    d.cdUp();
    d.cdUp();
    GrapeDirPath = d.absolutePath() + "/GRAPE";
    //qDebug() << " $$$$$$$$$$$$$$-1,  GrapeDirPath = "<<GrapeDirPath;

    QString GRAPE_path = GrapeDirPath + "/bin/GRAPE";
    QString log_path = GrapeDirPath + saved_log_file_name;
    log_path.replace(".sav","");


    // clear the log file.
    QFile::remove(log_path);

    QString grape_pipe = GrapeDirPath + pipeline_file_name;
    QString saved_log_path = GrapeDirPath + saved_log_file_name;

    //qDebug() << "saved_log_file_name: " << saved_log_file_name;
    //qDebug() << "grape_pipe: " << grape_pipe;

    QFileInfo tmp;
    if(!tmp.exists(grape_pipe))
    {
        cout << "Missing pipeline file. Cannot proceed." << endl;
        qDebug() << "grape_pipe="<<grape_pipe;
        return false;
    }

    if(!tmp.exists(saved_log_path))
    {
        cout << "Missing output file. Cannot proceed." << endl;
        return false;
    }

    QString GRAPE_Command = GRAPE_path + " -i " + grape_pipe + " -r -s";

    system(GRAPE_Command.toUtf8());

    bool identical_files = compare_files(log_path.toStdString(), saved_log_path.toStdString());

    return identical_files;
}
//--------------------------------------------------------------------------------------------------

QTEST_MAIN(GRAPETestTest)

#include "tst_grapetesttest.moc"
