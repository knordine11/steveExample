#include "fileloader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString imgpath;
QString soundpath;
QString currentlesson;
QList<QString> gNote;
QList<QString> gKey;
QList<QString> gTestGroup;

FileLoader::FileLoader(QObject *parent)
    : QObject{parent}
{

}

void FileLoader::ReadConfig()
{
    QFile file(":/scripts/config.txt");

    if(!file.exists())
    {
        qInfo() << "file not found";
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical("Open failed");
    }
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        qInfo() << line;
        QStringList items = line.split(',');
        if(items[1].contains("graphic"))
        {
            imgpath = items[0];
        }
        if(items[1].contains("sound"))
        {
            soundpath = items[0];
        }
        if(items[1].contains("Current"))
        {
            currentlesson = items[0];
        }
    }
    file.close();
}

void FileLoader::ReadLesson()
{
    QFile file(":/scripts/lessons.txt");

    if(!file.exists())
    {
        qInfo() << "file not found";
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical("Failed to open");
    }
    QTextStream stream(&file);
    stream.readLine();
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList items = line.split('|');
        gNote.append(items[0] + items[1]);
        gKey.append(items[0]);

        gTestGroup.append(items[4]);
    }
    qInfo() << gNote;
    qInfo() << gTestGroup;
    file.close();
}
