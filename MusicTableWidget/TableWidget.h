#ifndef TABLEWIDGET_H_
#define TABLEWIDGET_H_

#include <QWidget>
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
    explicit TableWidget(const QString& title ,QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* ev)override;
    void enterEvent(QEnterEvent* ev)override;
    void leaveEvent(QEvent* ev)override;
private:
    void initUi();
private:
    QLabel* m_titleLab{};
    QToolButton* m_play_ToolBtn{};
    QToolButton* m_adjust_ToolBtn{};
    QToolButton* m_refresh_ToolBtn{};
    QLabel* m_more_Lab{};
    QWidget* m_tabWidget{};
};

class ItemListWidget : public QWidget{
    Q_OBJECT
public:
    ItemListWidget(QPixmap coverPix,const QString& name,const QString& author,QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* ev)override;
    void enterEvent(QEnterEvent* ev)override;
    void leaveEvent(QEvent* ev)override;
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
