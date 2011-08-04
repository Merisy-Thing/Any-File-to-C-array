#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    QString input_file;
    QString output_file;

private slots:
    void on_pB_start_convert_clicked();
    void on_pB_select_output_file_clicked();
    void on_pB_select_input_file_clicked();
};

#endif // WIDGET_H
