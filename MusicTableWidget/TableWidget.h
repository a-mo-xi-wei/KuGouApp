#ifndef TABLEWIDGET_H_
#define TABLEWIDGET_H_

#include <QWidget>
#include<QHBoxLayout>

class QLabel;
class QToolButton;
class QPaintEvent;
class QEnterEvent;
class QEvent;
class QLine;
class SMaskWidget;


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
private:
    void initUi();
    //1.大图标TabWidget
    void initBlockListWidget();
    //2.小图标TabListWidget
    void initItemListWidget();
private:
    std::unique_ptr<QHBoxLayout>m_tabHLayout;
    QLabel* m_titleLab{};
    KIND m_kindList;
    QToolButton* m_play_ToolBtn{};
    QToolButton* m_adjust_ToolBtn{};
    QToolButton* m_refresh_ToolBtn{};
    QLabel* m_more_Lab{};
    QWidget* m_tabWidget{};

};

class ItemBlockWidget : public QWidget{
    Q_OBJECT
public:
    ItemBlockWidget(QPixmap coverPix,QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* ev)override;
    void enterEvent(QEnterEvent* ev)override;
    void leaveEvent(QEvent* ev)override;
private:
    void initUi();
private:
    SMaskWidget* m_mask;
    QLabel* m_coverLab{};
    QLabel* m_describeLab{};
    QLabel* m_tipLab{};
    QLabel* m_popularLab{};
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
private:
    SMaskWidget* m_mask;
    QLabel* m_coverLab{};
    QLabel* m_nameLab{};
    QLabel* m_authorLab{};
    QToolButton* m_play_add_ToolBtn{};
    QToolButton* m_like_ToolBtn{};
    QToolButton* m_refresh_ToolBtn{};
    QToolButton* m_more_ToolBtn{};
    bool m_isHoverCoverLab = false;
};



#endif // TABLEWIDGET_H_
