#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>
#include <QDataStream>

const char array_file_end[] ="};\r\n";
const char array_file_head[] =
"/**\r\n"
"  ******************************************************************************\r\n"
"  * @file    ???.c\r\n"
"  * @author  ???\r\n"
"  * @version V0.1\r\n"
"  * @date    01/01/2011\r\n"
"  * @brief   ???\r\n"
"  ******************************************************************************\r\n"
"  */\r\n\r\n"
"const char File_to_C_array[] = {\r\n";

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    input_file = "";
    output_file = "";
    ui->pB_start_convert->setEnabled(false);
    ui->sB_line_byte_num->setValue(16);
    this->setWindowTitle("File to C array --- by pajoke");
}

Widget::~Widget()
{
    delete ui;
}

/*打开输入文件*/
void Widget::on_pB_select_input_file_clicked()
{
    input_file = QFileDialog::getOpenFileName(this, "选择源文件",
                                "", tr("All (*.*)"));
    if(input_file.isEmpty() == true) {
        ui->lE_input_file->setText("请选择文件");
        return;
    }
    ui->lE_input_file->setText(input_file);

    if(output_file.isEmpty() == true) {
        ui->pB_start_convert->setEnabled(false);
    } else {
        ui->pB_start_convert->setEnabled(true);
    }
    ui->lb_notice->setText("输入文件已选择");
}

/*打开输出文件*/
void Widget::on_pB_select_output_file_clicked()
{
    output_file = QFileDialog::getSaveFileName(this, tr("选择保存文件"),
                                "R:\\", tr("*.c (*.c);;*.h (*.h);;*.cpp (*.cpp);;All (*.*)"));
    if(output_file.isEmpty() == true) {
        ui->lE_output_file->setText("请选择文件");
        return;
    }
    ui->lE_output_file->setText(output_file);

    if(input_file.isEmpty() == true) {
        ui->pB_start_convert->setEnabled(false);
    } else {
        ui->pB_start_convert->setEnabled(true);
    }
    ui->lb_notice->setText("输出文件已选择");
}

/*开始转换处理*/
void Widget::on_pB_start_convert_clicked()
{
    QFile file_in;
    QFile file_out;
    int i, ret;
    char read_buffer[64];
    char write_buffer[256];
    int write_buffer_len;
    QDataStream stream_in, stream_out;
    int line_byte_num = ui->sB_line_byte_num->value();

    ui->lb_notice->setText("准备转换...");
    /*打开输入文件*/
    file_in.setFileName(input_file);
    if (!file_in.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("出错警告"),
                               tr("打开输入文件失败，请确认文件是否可用。"),
                               QMessageBox::Ok,QMessageBox::Ok);
        return;
    }

    if(file_in.size() > 512*1024) {
        ret = QMessageBox::warning(this, tr("警告"), tr("文件大小超过512KB，转换需要较长时间，是否继续？"),
                                       QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
        if(ret != QMessageBox::Ok) {
            file_in.close();
            return;
        }
    }

    /*打开输出文件*/
    file_out.setFileName(output_file);
    if (!file_out.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("出错警告"),
                               tr("打开输出文件失败，请确认文件是否可用。"),
                               QMessageBox::Ok,QMessageBox::Ok);
        file_in.close();
        return;
    }

    stream_in.setDevice(&file_in);
    stream_out.setDevice(&file_out);

    if(stream_out.writeRawData(array_file_head, sizeof(array_file_head)-1) < 0) {
        QMessageBox::warning(this, tr("出错警告"),
                               tr("写输出文件失败，请确认文件是否可用，或是否有足够的磁盘空间。"),
                               QMessageBox::Ok,QMessageBox::Ok);
        goto LB_return;
    }

    while(1) {
        ret = stream_in.readRawData(read_buffer, line_byte_num);
        if((ret < 0) || (ret > line_byte_num)) {
            QMessageBox::warning(this, tr("出错警告"),
                                   tr("读取输入文件失败，请确认文件是否可用。"),
                                   QMessageBox::Ok,QMessageBox::Ok);
            break;
        }
        write_buffer[0] = '\t';
        for(i=0; i<ret; i++) {
            sprintf(&write_buffer[5*i+1], "0x%02x,", (uchar)read_buffer[i]);
        }

        //TODO: add line feed mode
        sprintf(&write_buffer[5*i+1], "\r\n");
        write_buffer_len = 5*i + 2 +1;

        if(stream_out.writeRawData(write_buffer, write_buffer_len) < 0) {
            QMessageBox::warning(this, tr("出错警告"),
                                   tr("写输出文件失败，请确认文件是否可用，或是否有足够的磁盘空间。"),
                                   QMessageBox::Ok,QMessageBox::Ok);
            goto LB_return;
        }

        if(ret < line_byte_num) {
            break;
        }
    }

    if(stream_out.writeRawData(array_file_end, sizeof(array_file_end)-1) < 0) {
        QMessageBox::warning(this, tr("出错警告"),
                               tr("写输出文件失败，请确认文件是否可用，或是否有足够的磁盘空间。"),
                               QMessageBox::Ok,QMessageBox::Ok);
        goto LB_return;
    }
    ui->lb_notice->setText("转换完成");
    QMessageBox::warning(this, tr("提示"), tr("转换完成。"), QMessageBox::Ok,QMessageBox::Ok);

LB_return:
    file_out.close();
    file_in.close();
}
