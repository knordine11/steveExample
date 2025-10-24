#include "widget.h"
#include "ui_widget.h"
#include "fftwstuff.h"
#include "fileloader.h"
#include "test_class_1.h"
#include <QAudioDevice>
#include <QAudioSource>
#include <QtEndian>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <QMessageBox>

int curLessonInt = 0;
int orientation [21] = {1,2,3,4,5,6,7,8,1,8,1,8,1,8,7,6,5,4,3,2,1};

AudioInfo::AudioInfo(const QAudioFormat &format) : m_format(format)
{

}

void AudioInfo::start()
{
    open(QIODevice::WriteOnly);
}

void AudioInfo::stop()
{
    close();
}

qint64 AudioInfo::readData(char * /* data */, qint64 /* maxlen */)
{
    return 0;
}

qreal AudioInfo::calculateLevel(const char *data, qint64 len)
{
    const int channelBytes = m_format.bytesPerSample();
    const int sampleBytes = m_format.bytesPerFrame();
    const int numSamples = len / sampleBytes;

    float maxValue = 0;
    auto *ptr = reinterpret_cast<const signed char *>(data);

    for (int i = 0; i < numSamples; ++i) {
        for (int j = 0; j < m_format.channelCount(); ++j) {
            float value = m_format.normalizedSampleValue(ptr);

            rec_arr[rec_arr_cnt] = value;

            maxValue = qMax(value, maxValue);
            ptr += channelBytes;
            if (rec_arr_cnt == 1000)
                qDebug() << "  " <<  i << "  " << value;
            rec_arr_cnt++;
        }
    }
    frame_cnt++;
    qDebug() << "rec_arr_cnt in widget: " << rec_arr_cnt;
    qDebug() << "frame " << frame_cnt << "  " << maxValue;
    return maxValue;
}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{
    m_level = calculateLevel(data, len);

    emit levelChanged(m_level);

    return len;
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Ear Trainer Qt6 version 6.10.0");
    this->setWindowIcon(QIcon("://note.png"));
    QPixmap pix("://down-arrow.png");
    ui->lb_arrow->setPixmap(pix);
    std::cout << "Hello World!  START" << std::endl;
    Test_Class_1 tc;
    tc.zero_rec_arr();
    tc.make_sin(440.0,0,1024);
    // tc.look_rec_arr();
    tc.look_rec_arr(80,100);
    std::cout << "Hello World!   END  /" << std::endl;
    rec_arr_cnt = 0;
    FileLoader::ReadConfig();
    FileLoader::ReadLesson();
    ui->lb_review->setText("Start");
    ui->lb_title->setText("Lesson");

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPixmap pix2(100,60);
    pix2.fill(Qt::white);
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter *painter = new QPainter(&pix2);
    QPen outsidePen(Qt::red, 4, Qt::SolidLine);
    painter->setPen(outsidePen);
    painter->drawEllipse(35, 15, 30, 30);
    QPen insidePen(Qt::green, 4, Qt::SolidLine);
    painter->setPen(insidePen);
    painter->drawEllipse(40, 20, 20, 20);
    QPen linePen(Qt::black, 1, Qt::SolidLine);
    painter->setPen(linePen);
    painter->drawLine(0, 30, 100, 30);
    painter->drawLine(50, 0, 50, 60);
    painter->end();
    ui->lb_tuner->setPixmap(pix2);
}

void Widget::on_btnStart_clicked()
{
    curLessonInt = currentlesson.toInt();
    if (currentlesson != "1")
    {
        ui->lb_review->setText("REVIEW");
        curLessonInt -= 1;
        currentlesson = QString::number(curLessonInt);
    }
    else
    {
        ui->lb_review->setText("CURRENT");
    }
    QString temp = "Lesson #" + currentlesson + " Key " + gKey[curLessonInt-1]
                   + " Test Notes " + gTestGroup[curLessonInt-1];
    ui->lb_title->setText(temp);

    ui->lb_arrow->move(100, 100);
    ui->note2->setStyleSheet("QLabel { background-color: red;"
                             "border-style: outset;"
                             " border-width: 3px; border-color: black;}");

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Timed Out", "Continue?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "continuing...";
    } else {
        qDebug() << "No was clicked";
        QApplication::quit();
    }
}

void Widget::initializeWindow()
{
    const QAudioDevice &defaultDeviceInfo = QMediaDevices::defaultAudioInput();
}

void Widget::initializeAudio(const QAudioDevice &deviceInfo)
{
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioInfo.reset(new AudioInfo(format));
    m_audioSource.reset(new QAudioSource(deviceInfo, format));
    m_audioInfo->start();
    restartAudioStream();
}

void Widget::restartAudioStream()
{
    m_audioSource->stop();

    // pull mode: QAudioSource provides a QIODevice to pull from
    auto *io = m_audioSource->start();
    if (!io)
        return;

    connect(io, &QIODevice::readyRead, [this, io]() {
        static const qint64 BufferSize = 4096;
        const qint64 len = qMin(m_audioSource->bytesAvailable(), BufferSize);

        QByteArray buffer(len, 0);
        qint64 l = io->read(buffer.data(), len);
        if (l > 0) {
            const qreal level = m_audioInfo->calculateLevel(buffer.constData(), l);
            qDebug() << level;
            if (m_audioInfo->frame_cnt > 100){
                int frame_length = 1024;
                int start = 10 * frame_length;
                fftwStuff::DoIt(start, frame_length);
            }
        }
        m_audioInfo->stop();
        // message box stall
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Timed Out", "Continue?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qDebug() << "continuing...";
        } else {
            qDebug() << "No was clicked";
            QApplication::quit();
        }
    });
}

void Widget::on_btnInput_clicked()
{
    initializeWindow();
    initializeAudio(QMediaDevices::defaultAudioInput());
}
