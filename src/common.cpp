#include <QApplication>
#include <QDebug>
#include <QtCore>

#include "common.h"


//--------------------------------------------------------------------------------------------------
int writeStrToLog(const QString& LogString)
{
    QString logFilename = QCoreApplication::applicationDirPath() + "/log.txt";
    QFile logFile(logFilename);
    if(!logFile.exists())
    {
        if(!logFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Failed to open the log file.";
            return -1;
        }
    }

    QDateTime now =  QDateTime::currentDateTime();
    QFileInfo info1("log.txt");
    //  qDebug()  << "LOG FILE size" << info1.size();
    if(info1.size() > 1e6)               // returns 56201
    {
        qDebug() << "Log file reached size limit, creating a new log file...";

        QDate date = QDate::currentDate();
        QString datestring = date.toString("yyyy-MM-dd");
        QTime time = QTime::currentTime();
        QString timestring = time.toString("hh-mm-ss");

        QString newName = info1.absolutePath() + "/" + info1.baseName() + "-" + datestring + "-" + timestring;

        if (!info1.completeSuffix().isEmpty())
            newName += "." + info1.completeSuffix();

        QString oldName = info1.absoluteFilePath();

        QFile ff(oldName);
        bool rename_success = ff.rename(newName);
        if(rename_success)
            qDebug()  << "log file backed up successfully!";
        else
            qDebug()  << "could not back up log file.";
    }

    logFile.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append);

    /* Check it opened OK */
    if(!logFile.isOpen()){
        qDebug()  << "- Error: unable to open" << logFilename << "for output";
    }

    /* Point a QTextStream object at the file */
    QTextStream outStream(&logFile);

    /* Write the line to the file */
    outStream << now.toString() << ": " << LogString << "\r\n";

    /* Close the file */
    logFile.close();

    return 0;
}
//--------------------------------------------------------------------------------------------------
bool checkExecutableFileExists(const QString &commandPath)
{
    // check the file exists at the supplied path
    bool found = false;

    QFileInfo checkFile(commandPath);
    if (checkFile.exists() && checkFile.isFile() && checkFile.isExecutable())
        found = true;

    return found;
}
//--------------------------------------------------------------------------------------------------
bool checkFileExists(const QString &commandPath)
{
    // check the file exists at the supplied path
    bool found = false;

    QFileInfo checkFile(commandPath);
    if (checkFile.exists() && checkFile.isFile())
        found = true;

    return found;
}
//--------------------------------------------------------------------------------------------------
bool checkPathExists(const QString &commandPath)
{
    // check if the supplied path exist
    bool found = false;

    QFileInfo checkFile(commandPath);
    if (checkFile.exists())
        found = true;

    return found;
}
//--------------------------------------------------------------------------------------------------
