#include "endpointinwidget.h"
#include "ui_endpointinwidget.h"
#include "usb/__usbmacro.h"
#include <QStyleOption>
#include <QPainter>

EndpointInWidget::EndpointInWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndpointInWidget), _dataReady(false), _readOnce(false), _keepRead(false),
    _resetButtonAfterSafelyStop(nullptr),
    _endpointDescriptor(nullptr)
{
    ui->setupUi(this);
    _hexEdit = new QHexEdit;
    _hexEdit->setReadOnly(true);
    layout()->replaceWidget(ui->hexEditPlaceHolder, _hexEdit);
    ui->hexEditPlaceHolder->deleteLater();
    ui->hexEditPlaceHolder = nullptr;
    _reader = new usb::UsbEndpointReader;
    _workerThread = new QThread;
    _reader->moveToThread(_workerThread);
    _workerThread->start();
    /* We only set a flag here, to avoid blocking
     * the main thread because of high frequency data
     */
    connect(_reader, &usb::UsbEndpointReader::dataRead,
            this, &EndpointInWidget::__setDataReady);
    connect(_reader, &usb::UsbEndpointReader::safelyStopped,
            this, &EndpointInWidget::__resetButtons);
    connect(_reader, &usb::UsbEndpointReader::readFailed,
            this, [this] (int ret) {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Failed to read data, error: %1.")
                              .arg(usb_error_name(ret)));
    });
    connect(this, &EndpointInWidget::__readOnceTriggered,
            _reader, &usb::UsbEndpointReader::readOnce);
    connect(this, &EndpointInWidget::__keepReadTriggered,
            _reader, &usb::UsbEndpointReader::keepRead);
    /* Initialize a QTimer without setting interval,
     * so that the timeout signal will be emitted
     * every event loop.
     */
    _eventLoopTimer = new QTimer(this);
    connect(_eventLoopTimer, &QTimer::timeout,
            this, &EndpointInWidget::__updateData);

    connect(ui->pushButtonReadOnce, &QPushButton::released,
            this, &EndpointInWidget::__buttonReadOnceReleased);
    connect(ui->pushButtonKeepRead, &QPushButton::released,
            this, &EndpointInWidget::__buttonKeepReadReleased);
    connect(ui->pushButtonSetBufferSize, &QPushButton::released,
            this, &EndpointInWidget::__buttonSetBufferSizeReleased);
}

EndpointInWidget::~EndpointInWidget()
{
    _eventLoopTimer->stop();
    _reader->disconnect(this);
    disconnect(_reader);
    if (!_readOnce && !_keepRead)
    {
        _reader->stopRead();
        _workerThread->quit();
        _reader->deleteLater();
        _workerThread->deleteLater();
    }
    else
    {
        connect(_reader, &usb::UsbEndpointReader::safelyStopped,
                _reader, &usb::UsbEndpointReader::deleteLater);
        connect(_reader, &usb::UsbEndpointReader::safelyStopped,
                _workerThread, &QThread::quit);
        connect(_workerThread, &QThread::finished,
                _workerThread, &QThread::deleteLater);
        _reader->stopRead();
    }
    delete ui;
}

void EndpointInWidget::setEndpoint(usb::UsbEndpointDescriptor *epDesc)
{
    _endpointDescriptor = epDesc;
    ui->labelEndpointName->setText(_endpointDescriptor->endpointAddressInfo());
    _reader->init(_endpointDescriptor);
}

void EndpointInWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    QWidget::changeEvent(event);
}

void EndpointInWidget::hideEvent(QHideEvent *event)
{
    _reader->stopRead();
    QWidget::hideEvent(event);
}

void EndpointInWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void EndpointInWidget::__buttonReadOnceReleased()
{
    if (!_readOnce && !_keepRead)
    {
        _readOnce = true;
        ui->pushButtonSetBufferSize->setEnabled(false);
        ui->pushButtonKeepRead->setEnabled(false);
        ui->pushButtonReadOnce->setText(tr("Stop Read"));
        _eventLoopTimer->start();
        _resetButtonAfterSafelyStop = [this] () {
            _readOnce = false;
            ui->pushButtonReadOnce->setText(tr("Read Once"));
        };
        emit __readOnceTriggered();
    }
    else if (_readOnce)
    {
        ui->pushButtonReadOnce->setEnabled(false);
        _reader->stopRead();
        _eventLoopTimer->stop();
        _dataReady = false;
    }
}

void EndpointInWidget::__buttonKeepReadReleased()
{
    if (!_readOnce && !_keepRead)
    {
        _keepRead = true;
        ui->pushButtonSetBufferSize->setEnabled(false);
        ui->pushButtonReadOnce->setEnabled(false);
        ui->pushButtonKeepRead->setText(tr("Stop Read"));
        _eventLoopTimer->start();
        _resetButtonAfterSafelyStop = [this] () {
            _keepRead = false;
            ui->pushButtonKeepRead->setText(tr("Keep Read"));
        };
        emit __keepReadTriggered();
    }
    else if (_keepRead)
    {
        ui->pushButtonKeepRead->setEnabled(false);
        _reader->stopRead();
        _eventLoopTimer->stop();
        _dataReady = false;
    }
}

void EndpointInWidget::__buttonSetBufferSizeReleased()
{
    bool ok;
    int value = QInputDialog::getInt(this,
                                     tr("Set Read Buffer Size"),
                                     tr("Set read buffer size to:"),
                                     _reader->readBufferSize(),
                                     0,
                                     0xFFFF,
                                     1,
                                     &ok);
    if (ok)
        _reader->setReadBufferSize(value);
}

void EndpointInWidget::__setDataReady()
{
    _dataReady = true;
}

void EndpointInWidget::__resetButtons()
{
    ui->pushButtonSetBufferSize->setEnabled(true);
    ui->pushButtonReadOnce->setEnabled(true);
    ui->pushButtonKeepRead->setEnabled(true);
    if (_resetButtonAfterSafelyStop)
        _resetButtonAfterSafelyStop();
    _resetButtonAfterSafelyStop = nullptr;
}

void EndpointInWidget::__stopRead()
{
    _reader->stopRead();
}

void EndpointInWidget::__updateData()
{
    if (_dataReady)
    {
        _buffer.setData(_reader->data());
        _hexEdit->setData(_buffer);
        _dataReady = false;
    }
}
