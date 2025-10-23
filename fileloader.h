#ifndef FILELOADER_H
#define FILELOADER_H

#include <QObject>

extern QString imgpath;
extern QString soundpath;
extern QString currentlesson;
extern QList<QString> gNote;
extern QList<QString> gKey;
extern QList<QString> gTestGroup;

class FileLoader : public QObject
{
    Q_OBJECT
public:
    explicit FileLoader(QObject *parent = nullptr);

    static void ReadConfig();
    static void ReadLesson();


signals:

};

#endif // FILELOADER_H
