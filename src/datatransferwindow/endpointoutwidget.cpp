﻿#include "endpointoutwidget.h"
#include "ui_endpointoutwidget.h"

EndpointOutWidget::EndpointOutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndpointOutWidget),
    _writeOnce(false), _keepWrite(false),
    _resetButtonAfterSafelyStop(nullptr),
    _endpointDescriptor(nullptr)
{
    ui->setupUi(this);

    _hexEdit = new QHexEdit;
    layout()->replaceWidget(ui->hexEditPlaceHolder, _hexEdit);
    ui->hexEditPlaceHolder = nullptr;
    _writer = new usb::UsbEndpointWriter;
    _writer->moveToThread(&_workerThread);
    _workerThread.start();
    connect(_writer, &usb::UsbEndpointWriter::safelyStopped, this, &EndpointOutWidget::__resetButton);
    connect(this, &EndpointOutWidget::__writeOnceTriggered, _writer, &usb::UsbEndpointWriter::writeOnce);
    connect(this, &EndpointOutWidget::__keepWriteTriggered, _writer, &usb::UsbEndpointWriter::keepWrite);
    connect(ui->pushButtonWriteOnce, &QPushButton::released, this, &EndpointOutWidget::__buttonWriteOnceReleased);
    connect(ui->pushButtonKeepWrite, &QPushButton::released, this, &EndpointOutWidget::__buttonKeepWriteReleased);
}

EndpointOutWidget::~EndpointOutWidget()
{
    _writer->disconnect(this);
    disconnect(_writer);
    _writer->stopWrite();
    _workerThread.quit();
    _workerThread.wait();
    delete ui;
}

void EndpointOutWidget::setEndpoint(usb::UsbEndpointDescriptor *epDesc)
{
    _endpointDescriptor = epDesc;
    ui->labelEndpointName->setText(_endpointDescriptor->endpointAddressInfo());
    QByteArray data;
    data.fill('\0', _endpointDescriptor->wMaxPacketSize());
    _buffer.setData(data);
    _hexEdit->setData(_buffer);
    _writer->init(_endpointDescriptor);
}

void EndpointOutWidget::closeEvent(QCloseEvent *event)
{
    _writer->stopWrite();
    event->accept();
}

void EndpointOutWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    event->accept();
}

void EndpointOutWidget::__buttonWriteOnceReleased()
{
    if (!_writeOnce && !_keepWrite)
    {
        _writeOnce = true;
        ui->pushButtonKeepWrite->setEnabled(false);
        ui->pushButtonWriteOnce->setText(tr("Stop Write"));
        _hexEdit->setReadOnly(true);
        _writer->setData(_hexEdit->data());
        _resetButtonAfterSafelyStop = [this] () {
            _writeOnce = false;
            ui->pushButtonKeepWrite->setEnabled(true);
            ui->pushButtonWriteOnce->setEnabled(true);
            ui->pushButtonWriteOnce->setText(tr("Write Once"));
            _hexEdit->setReadOnly(false);
        };
        emit __writeOnceTriggered();
    }
    else if (_writeOnce)
    {
        ui->pushButtonWriteOnce->setEnabled(false);
        _writer->stopWrite();
    }
}

void EndpointOutWidget::__buttonKeepWriteReleased()
{
    if (!_writeOnce && !_keepWrite)
    {
        _keepWrite = true;
        ui->pushButtonWriteOnce->setEnabled(false);
        ui->pushButtonKeepWrite->setText(tr("Stop Write"));
        _hexEdit->setReadOnly(true);
        _writer->setData(_hexEdit->data());
        _resetButtonAfterSafelyStop = [this] () {
            _keepWrite = false;
            ui->pushButtonWriteOnce->setEnabled(true);
            ui->pushButtonKeepWrite->setEnabled(true);
            ui->pushButtonKeepWrite->setText(tr("Keep Write"));
            _hexEdit->setReadOnly(false);
        };
        emit __keepWriteTriggered();
    }
    else if (_keepWrite)
    {
        ui->pushButtonKeepWrite->setEnabled(false);
        _writer->stopWrite();
    }
}

void EndpointOutWidget::__resetButton()
{
    if (_resetButtonAfterSafelyStop)
        _resetButtonAfterSafelyStop();
    _resetButtonAfterSafelyStop = nullptr;
}
