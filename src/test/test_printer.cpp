//
// Created by jiang.wenqiang on 2018/9/6.
//

#include <QtCore/QFile>
#include <QtWidgets/QApplication>
#include <QtPrintSupport/QPrinter>
#include <QtGui/QTextDocument>
#include <QtGui/QPdfWriter>
#include <QtGui/QPainter>
#include <QtGui/QTextBlock>

int main(int argc, char *argv[]) {
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
}

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
