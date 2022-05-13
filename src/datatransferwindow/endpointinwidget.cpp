#include "endpointinwidget.h"
#include "ui_endpointinwidget.h"

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
    ui->hexEditPlaceHolder = nullptr;
    _reader = new usb::UsbEndpointReader;
    _reader->moveToThread(&_workerThread);
    _workerThread.start();
    /* We only set a flag here, to avoid blocking
     * the main thread because of high frequency data
     */
    connect(_reader, &usb::UsbEndpointReader::dataRead, this, &EndpointInWidget::__setDataReady);
    connect(_reader, &usb::UsbEndpointReader::safelyStopped, this, &EndpointInWidget::__resetButton);
    connect(this, &EndpointInWidget::__readOnceTriggered, _reader, &usb::UsbEndpointReader::readOnce);
    connect(this, &EndpointInWidget::__keepReadTriggered, _reader, &usb::UsbEndpointReader::keepRead);
    /* Initialize a QTimer without setting interval,
     * so that the timeout signal will be emitted
     * every event loop.
     */
    _eventLoopTimer = new QTimer(this);
    connect(_eventLoopTimer, &QTimer::timeout, this, &EndpointInWidget::__updateData);

    connect(ui->pushButtonReadOnce, &QPushButton::released, this, &EndpointInWidget::__buttonReadOnceReleased);
    connect(ui->pushButtonKeepRead, &QPushButton::released, this, &EndpointInWidget::__buttonKeepReadReleased);
}

EndpointInWidget::~EndpointInWidget()
{
    _eventLoopTimer->stop();
    _reader->disconnect(this);
    disconnect(_reader);
    _reader->stopRead();
    _workerThread.quit();
    _workerThread.wait();
    delete ui;
}

void EndpointInWidget::setEndpoint(usb::UsbEndpointDescriptor *epDesc)
{
    _endpointDescriptor = epDesc;
    ui->labelEndpointName->setText(_endpointDescriptor->endpointAddressInfo());
    _reader->init(_endpointDescriptor);
}

void EndpointInWidget::closeEvent(QCloseEvent *event)
{
    _reader->stopRead();
    event->accept();
}

void EndpointInWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
    event->accept();
}

void EndpointInWidget::__buttonReadOnceReleased()
{
    if (!_readOnce && !_keepRead)
    {
        _readOnce = true;
        ui->pushButtonKeepRead->setEnabled(false);
        ui->pushButtonReadOnce->setText(tr("Stop Read"));
        _eventLoopTimer->start();
        _resetButtonAfterSafelyStop = [this] () {
            _readOnce = false;
            ui->pushButtonKeepRead->setEnabled(true);
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
        ui->pushButtonReadOnce->setEnabled(false);
        ui->pushButtonKeepRead->setText(tr("Stop Read"));
        _eventLoopTimer->start();
        _resetButtonAfterSafelyStop = [this] () {
            _keepRead = false;
            ui->pushButtonReadOnce->setEnabled(true);
            ui->pushButtonKeepRead->setText(tr("Read Once"));
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

void EndpointInWidget::__setDataReady()
{
    _dataReady = true;
}

void EndpointInWidget::__resetButton()
{
    if (_resetButtonAfterSafelyStop)
        _resetButtonAfterSafelyStop();
    _resetButtonAfterSafelyStop = nullptr;
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
