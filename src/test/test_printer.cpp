//
// Created by jiang.wenqiang on 2018/9/6.
//

#include <QtCore/QFile>
#include <QtCore/QVector>
#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include <QtPrintSupport/QPrinter>
#include <QtGui/QTextBlock>
#include <QtTextToSpeech/QTextToSpeech>
#include <MRange.h>
#include <QtCore/QProcess>
#include <QtCore/QSettings>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("BYD");
    app.setApplicationName("printer");
    app.setOrganizationDomain("byd.com.cn");
    app.setApplicationDisplayName("Refine");
/*    QTextToSpeech speech;
    speech.setLocale(QLocale::Chinese);
    qDebug() << speech.availableVoices()[0].name();

//    speech.setVoice();
    speech.say(
            QString("从2016年3月开始，陆陆续续接到售后关于唐车型电动油泵电机控制器"
                    "失效的投诉，目前数量已达到57例，经过对这57例失效件的分析，发现"
                    "失效原因大概分为四大类：电机控制器进水腐蚀、假焊、撞件、功率管"
                    "损坏，其中功率管损坏故障格外引人注意，因为它们失效表现模式完全"
                    "一样，都是在 MOSFET 的基板与塑料基材的结合处开裂同时外观并没有"
                    "出现碳化、色泽暗淡等现象，这与我们之前遇到的过流、静电损坏失效"
                    "表现完全不一样，由于失效模式不确定，这就给产品品质改善带来很大"
                    "的困难，同时电动油泵电机控制器在整车又属于关重器件，它的失效将"
                    "导致车辆无 EV 模式，直接影响用户驾乘体验，如果不解决失效问题，"
                    "将带来客户极大的抱怨及严重影响公司产品口碑。 "));*/
//    while (speech.state() == QTextToSpeech::Speaking) {}
    MRange range({1,2,3,4,5,6,7,8,9,10});
    int i = 0;
//    for (auto &iter : range) {
//        i += 1;
//        iter = i;
//    }

    for (const auto &iter : range) {
        qDebug() << iter;
    }
    qDebug() << *range.end();
    qDebug() << QProcess::execute(QString("D:\\software\\7-Zip\\7zFM.exe"));
    QSettings settings;
    settings.setValue("key", QPoint(1,2));
    settings.
//    QProcess::execute(QString("D:\\software\\7-Zip\\7zFM.exe"));
//    proc.
    return QApplication::exec();
}

/*int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString fileName = "test.pdf";
    QFile pdfFile(fileName);
    pdfFile.open(QIODevice::WriteOnly);                 // 打开要写入的pdf文件
    auto *pPdfWriter = new QPdfWriter(&pdfFile);  // 创建pdf写入器
    pPdfWriter->setPageSize(QPageSize(QPageSize::A4));     // 设置纸张为A4
    pPdfWriter->setResolution(
            300);                     // 设置纸张的分辨率为300,因此其像素为3508X2479

    int iMargin = 60;                   // 页边距
    pPdfWriter->setPageMargins(QMarginsF(iMargin, iMargin, iMargin, iMargin));

    auto *pPdfPainter = new QPainter(pPdfWriter);   // qt绘制工具

// 标题,居中
    QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WordWrap);

// 标题上边留白
    int iTop = 100;

// 文本宽度2100
    int iContentWidth = 2100;

// 标题,22号字
    QFont font;
    font.setFamily("微软雅黑");
    int fontSize = 22;
    font.setPointSize(fontSize);
    pPdfPainter->setFont(font);                    // 为绘制工具设置字体
    pPdfPainter->drawLine(0,0,2048,0);
    pPdfPainter->drawText(QRect(0, iTop, iContentWidth, 90),
                          QString("我的标题我骄傲"), option);
// 内容,16号字，左对齐
    fontSize = 16;
    font.setPointSize(fontSize);
    pPdfPainter->setFont(font);
    option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    iTop += 100;
    pPdfPainter->drawText(QRect(0, iTop, iContentWidth, 100),
                          QString("1、目录一"));
    iTop += 100;
// 左侧缩进2字符
    int iLeft = 120;
    pPdfPainter->drawText(QRect(iLeft, iTop, iContentWidth - iLeft, 100),
                          QString("我的目录一的内容。"), option);
    iTop += 100;
    pPdfPainter->drawText(QRect(0, iTop, iContentWidth, 100),
                          QString("2、目录二"));
    iTop += 100;
    pPdfPainter->drawText(QRect(iLeft, iTop, iContentWidth - iLeft, 100),
                          QString("我的目录一的内容"), option);

    delete pPdfPainter;
    delete pPdfWriter;
    pdfFile.close();
    qDebug("生成完毕");
    return QApplication::exec();
}*/

/*
int main(int argc, char **argv) {
    QApplication app(argc, argv);
    const QString path("D:/jiang.wenqiang/code/refine");
    QFile f(path + "/data/BYDICU T8SA_2108050D测试报告.htm");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!f.isOpen()) {
        qDebug() << "打不开文件！";
        return -1;
    }
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path + "/data/BYDICU T8SA_2108050D测试报告.pdf");
    QTextDocument text_document;
    QTextStream stream(&f);
    stream.setCodec("gbk");
    QString html = stream.readAll();
    qDebug() << html;
    f.close();
    text_document.setHtml(html);
    text_document.print(&printer);
    text_document.end();
    qDebug("end");
    return QApplication::exec();
}*/
