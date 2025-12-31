/******************************************************************************
 * @file       messagetextedit.h
 * @brief      消息输入框头文件
 * @author     ggggtxgt
 * @date       2025/12/31 18:33
 * @history
*****************************************************************************/
#ifndef CLIENT_MESSAGETEXTEDIT_H
#define CLIENT_MESSAGETEXTEDIT_H

#include "global.h"

#include <QDrag>
#include <QVector>
#include <QObject>
#include <QPainter>
#include <QMimeData>
#include <QMimeType>
#include <QFileInfo>
#include <QTextEdit>
#include <QMouseEvent>
#include <QFileIconProvider>

class MessageTextEdit : public QTextEdit {
Q_OBJECT

public:
    explicit MessageTextEdit(QWidget *parent = nullptr);                        // 构造函数
    ~MessageTextEdit();                                                         // 析构函数
    QVector<MsgInfo> GetMsgList();                                              // 获取当前输入框中的所有消息
    void insertFileFromUrl(const QStringList &urls);                            // 根据传入的URL列表插入文件

signals:

    void send();                                                                // 按下[发送]键，发出此信号

protected:
    void dragEnterEvent(QDragEnterEvent *event);                                // 拖拽进入事件处理
    void dropEvent(QDropEvent *event);                                          // 拖拽释放事件处理
    void keyPressEvent(QKeyEvent *e);                                           // 键盘事件处理(支持键盘enter键)

private:
    void insertImages(const QString &url);                                      // 插入图片文件
    void insertTextFile(const QString &url);                                    // 插入非图片文件
    bool canInsertFromMimeData(const QMimeData *source) const;                  // 判断是否可以从 MIME 数据中插入内容
    void insertFromMimeData(const QMimeData *source);                           // 从剪贴板或拖拽的 MIME数据中插入文件

private:
    bool isImage(QString url);                                                  // 判断文件是否为图片
    void insertMsgList(QVector<MsgInfo> &list, QString flag,
                       QString text, QPixmap pix);                              // 将一条消息信息（类型、内容、图标）添加到给定的消息列表中

    QStringList getUrl(QString text);                                           // 从拖拽文本中提取文件 URL 列表
    QPixmap getFileIconPixmap(const QString &url);                              // 获取文件图标及大小信息，并转化成图片
    QString getFileSize(qint64 size);                                           // 获取文件大小

private slots:

    void textEditChanged();                                                     // 文本内容变化时的槽函数

private:
    QVector<MsgInfo> mMsgList;                              // 存储当前输入框中所有消息（图片、文件）的列表
    QVector<MsgInfo> mGetMsgList;                           // 临时存储 GetMsgList() 返回的消息列表
};


#endif //CLIENT_MESSAGETEXTEDIT_H
