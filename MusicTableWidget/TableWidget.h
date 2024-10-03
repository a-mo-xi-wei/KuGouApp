#ifndef TABLEWIDGET_H_
#define TABLEWIDGET_H_

#include <QWidget>
#include<QHBoxLayout>
#include<QGridLayout>
#include<memory>
#include"SMaskWidget.h"

class QLabel;
class QToolButton;
class QPaintEvent;
class QEnterEvent;
class QEvent;
class QLine;
class ItemBlockWidget;


class TableWidget : public QWidget
{
    Q_OBJECT
public:
    enum KIND{
        BlockList,
        ItemList
    };
    explicit TableWidget(const QString& title ,KIND kind,QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* ev)override;
    void enterEvent(QEnterEvent* ev)override;
    void leaveEvent(QEvent* ev)override;
    void resizeEvent(QResizeEvent* event)override;
private:
    void initUi();
    //1.大图标TabWidget
    void initBlockListWidget();
    //2.小图标TabListWidget
    void initItemListWidget();

signals:
    void hideTitle();

    void gridChange(int len);

public slots:
    void onGridChange(int len);
private:
    std::unique_ptr<QHBoxLayout>m_tabHLayout;
    std::unique_ptr<QGridLayout>m_gridLayout;
    std::unique_ptr<QWidget>m_gridContainer;
    QLabel* m_titleLab{};
    KIND m_kindList;
    QToolButton* m_play_ToolBtn{};
    QToolButton* m_adjust_ToolBtn{};
    QLabel* line1{};
    QToolButton* m_refresh_ToolBtn{};
    QLabel* m_more_Lab{};
    int m_showCol = 5;
};

class ItemBlockWidget : public QWidget{
    Q_OBJECT
public:
    ItemBlockWidget(const QString& path,QWidget* parent = nullptr);
    void setTipLabText(const QString& text);
    void setPopularBtnText(const QString& text);
protected:
    void paintEvent(QPaintEvent* ev)override;
    void enterEvent(QEnterEvent* ev)override;
    void leaveEvent(QEvent* ev)override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void initUi();

private:
    QWidget* m_bacWidget{};
    std::unique_ptr<SMaskWidget>m_mask;
    QLabel* m_describeLab{};
    QLabel* m_tipLab{};
    QToolButton* m_popularBtn{};
    bool m_isHoverCoverLab = false;
};


class ItemListWidget : public QWidget{
    Q_OBJECT
public:
    ItemListWidget(QPixmap coverPix,const QString& name,const QString& author,QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* ev)override;
    void enterEvent(QEnterEvent* ev)override;
    void leaveEvent(QEvent* ev)override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void initUi();
public slots:
    void onHide();
private:
    std::unique_ptr<SMaskWidget>m_mask;
    QLabel* m_coverLab{};
    QLabel* m_nameLab{};
    QLabel* m_authorLab{};
    QToolButton* m_play_add_ToolBtn{};
    QToolButton* m_like_ToolBtn{};
    QToolButton* m_more_ToolBtn{};
    bool m_isHoverCoverLab = false;
    int m_aspectRatio = 6;
};



#endif // TABLEWIDGET_H_
