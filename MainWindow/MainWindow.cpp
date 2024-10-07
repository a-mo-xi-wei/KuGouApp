#include "MainWindow.h"
#include<QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{

}

MainWindow::~MainWindow() {

}

void MainWindow::mousePressEvent(QMouseEvent *ev) {
    QWidget::mousePressEvent(ev);
    if (ev->button() == Qt::LeftButton) {
        //qDebug()<<"按下 :"<<ev->pos();
        // 如果是鼠标左键
        // 获取当前窗口位置,以窗口左上角
        windowsLastPs = pos();
        // 获取鼠标在屏幕的位置  就是全局的坐标 以屏幕左上角为坐标系
        mousePs = ev->globalPosition().toPoint();
        isPress = true;
        // 获取鼠标在那个区域
        mouse_press_region = GetMouseRegion(ev->pos().x(), ev->pos().y());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        isPress = false;
    }
    setCursor(QCursor{});
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    //qDebug()<<"鼠标位置 ： "<<ev->pos();
    // 设置鼠标的形状
    SetMouseCursor(event->pos().x(), event->pos().y());

}

void MainWindow::SetMouseCursor(int x, int y) {
    // 鼠标形状对象
    Qt::CursorShape cursor{Qt::ArrowCursor};
    int region = GetMouseRegion(x, y);
    switch (region) {
        case kMousePositionLeftTop:
        case kMousePositionRightBottom:
            cursor = Qt::SizeFDiagCursor;
        break;
        case kMousePositionRightTop:
        case kMousePositionLeftBottom:
            cursor = Qt::SizeBDiagCursor;
        break;
        case kMousePositionLeft:
        case kMousePositionRight:
            cursor = Qt::SizeHorCursor;
        break;
        case kMousePositionTop:
        case kMousePositionBottom:
            cursor = Qt::SizeVerCursor;
        break;
        default:
            cursor = Qt::ArrowCursor;
        break;
    }
    setCursor(cursor);
}

int MainWindow::GetMouseRegion(int x, int y) {
    int region_x = 0, region_y = 0;
    // 鼠标的X坐标小于 边界5 说明他在最上层区域 第一区域
    if (x < Area::kMouseRegionLeft) {
        region_x = 1;
    } else if (x > (this->width()/*窗体宽度*/ - Area::kMouseRegionRight/*边界宽度5*/)) {
        // 如果鼠标X坐标 大于 最右侧的边界 说明他在第三区域
        region_x = 3;
    } else {
        region_x = 2;
    }
    if (y < Area::kMouseRegionTop) {
        // 同理 鼠标Y坐标 小于上层边界  说明鼠标在第一区域
        region_y = 1;
    } else if (y > (this->height() - Area::kMouseRegionBottom)) {
        // 鼠标Y坐标的 大于 最下面的坐标,鼠标就在 第三区
        region_y = 3;
    } else {
        region_y = 2;
    }
    // 最后计算 表示区域的 数值 (x=1, y=1) 计算 = 1*10+1 =11
    // x=2,y=3 = 3*10+2 = 32 在图的 3,2 区域
    return region_y * 10 + region_x;
}
