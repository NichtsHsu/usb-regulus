#include "usbdevicetreeview.h"

UsbDeviceTreeView::UsbDeviceTreeView(QWidget *parent)
    : QTreeView(parent)
{
    _menuDevice = new QMenu(this);
    _actionReset = new QAction(tr("Reset"));
    _menuDevice->addAction(_actionReset);
    _actionControlTransfer = new QAction(tr("Control Transfer"));
    _menuDevice->addAction(_actionControlTransfer);
    _menuInterface = new QMenu(this);
    _actionDataTransfer = new QAction(tr("Data Trasfer"));
    _menuInterface->addAction(_actionDataTransfer);
    _actionSetAltsetting = new QMenu(tr("Set Altsetting"));
    _menuInterface->addMenu(_actionSetAltsetting);
    _actionSetAltsettingFirst = new QAction(tr("First"));
    _actionSetAltsetting->addAction(_actionSetAltsettingFirst);
    _actionSetAltsettingPrevious = new QAction(tr("Previous"));
    _actionSetAltsetting->addAction(_actionSetAltsettingPrevious);
    _actionSetAltsettingNext = new QAction(tr("Next"));
    _actionSetAltsetting->addAction(_actionSetAltsettingNext);
    _actionSetAltsetting->addSeparator();
    _actionSetAltsettingCustom = new QAction(tr("Custom .."));
    _actionSetAltsetting->addAction(_actionSetAltsettingCustom);
    _model = new QStandardItemModel(this);
    setModel(_model);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &UsbDeviceTreeView::clicked, this, [this] () {
        emit currentIndexChanged(currentIndex());
    });
    connect(this, &UsbDeviceTreeView::customContextMenuRequested, this, [this] () {
        emit currentIndexChanged(currentIndex(), true);
    });
    connect(_actionReset, &QAction::triggered,
            this, &UsbDeviceTreeView::__resetDevice);
    connect(_actionControlTransfer, &QAction::triggered,
            this, &UsbDeviceTreeView::__openControlTransferWindow);
    connect(_actionDataTransfer, &QAction::triggered,
            this, &UsbDeviceTreeView::__openDataTransferWindow);
    connect(_actionSetAltsettingFirst, &QAction::triggered, this, [this] () {
        __setAltsetting(currentIndex(), 0);
    });
    connect(_actionSetAltsettingPrevious, &QAction::triggered, this, [this] () {
        UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(
                    _model->itemFromIndex(currentIndex()));
        __setAltsetting(currentIndex(), item->interface()->currentAltsetting() - 1);
    });
    connect(_actionSetAltsettingNext, &QAction::triggered, this, [this] () {
        UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(
                    _model->itemFromIndex(currentIndex()));
        __setAltsetting(currentIndex(), item->interface()->currentAltsetting() + 1);
    });
    connect(_actionSetAltsettingCustom, &QAction::triggered,
this, &UsbDeviceTreeView::__customSetAltsetting);
    connect(this, &UsbDeviceTreeView::customContextMenuRequested,
            this, &UsbDeviceTreeView::__customMenu);
    header()->hide();
}

static std::mutex __mutW;

void UsbDeviceTreeView::refresh(const QVector<usb::UsbDevice *> &devices)
{
    std::lock_guard<std::mutex> lock{ __mutW };
    _model->clear();
    foreach(usb::UsbDevice *const &device, devices)
    {
        UsbDeviceItem *item = new UsbDeviceItem;
        _model->appendRow(item);
        item->setUsbDevice(device);
        item->setEditable(false);
    }
}

void UsbDeviceTreeView::insert(int index, usb::UsbDevice *device)
{
    std::lock_guard<std::mutex> lock{ __mutW };
    _menuDevice->hide();
    _menuInterface->hide();
    UsbDeviceItem *item  = new UsbDeviceItem;
    _model->insertRow(index, item);
    item->setUsbDevice(device);
}

void UsbDeviceTreeView::remove(int index)
{
    std::lock_guard<std::mutex> lock{ __mutW };
    _menuDevice->hide();
    _menuInterface->hide();

    /* QStandardItem was not declared with Q_OBJECT,
     * we can not use qobject_cast here.
     */
    UsbDeviceItem *deviceItem = dynamic_cast<UsbDeviceItem *>(_model->item(index, 0));
    for (int i = 0; i < deviceItem->rowCount(); ++i)
    {
        /* QStandardItem was not declared with Q_OBJECT,
         * we can not use qobject_cast here.
         */
        UsbInterfaceItem *interfaceItem = dynamic_cast<UsbInterfaceItem *>(deviceItem->child(i));
        if (_dataTransferWindows.contains(interfaceItem->interface()))
        {
            LOGD(tr("Remove a data transfer window which based on an interface of a detached device."));
            _dataTransferWindows[interfaceItem->interface()]->close();
            delete _dataTransferWindows[interfaceItem->interface()];
            _dataTransferWindows.remove(interfaceItem->interface());
        }
    }
    _model->removeRow(index);
}

void UsbDeviceTreeView::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        __updateTranslations();
    event->accept();
}

void UsbDeviceTreeView::closeAllDataTransferWindow()
{
    foreach (const auto &window, _dataTransferWindows)
        window->close();
}

void UsbDeviceTreeView::__customMenu(const QPoint &point)
{
    QModelIndex index = indexAt(point);
    if (index.parent() == QModelIndex())
        _menuDevice->exec(viewport()->mapToGlobal(point));
    else
    {
        /* QStandardItem was not declared with Q_OBJECT,
         * we can not use qobject_cast here.
         */
        UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(
                    _model->itemFromIndex(index));
        if (item->interface()->numAltsetting() > 1)
        {
            _actionSetAltsetting->setEnabled(true);
            if (item->interface()->currentAltsetting() > 0)
                _actionSetAltsettingPrevious->setEnabled(true);
            else
                _actionSetAltsettingPrevious->setEnabled(false);
            if (item->interface()->currentAltsetting() < item->interface()->numAltsetting() - 1)
                _actionSetAltsettingNext->setEnabled(true);
            else
                _actionSetAltsettingNext->setEnabled(false);
        }
        else
            _actionSetAltsetting->setEnabled(false);
        _menuInterface->exec(viewport()->mapToGlobal(point));
    }
}

void UsbDeviceTreeView::__openDataTransferWindow()
{
    /* QStandardItem was not declared with Q_OBJECT,
     * we can not use qobject_cast here.
     */
    UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(
                _model->itemFromIndex(currentIndex()));
    if (_dataTransferWindows.contains(item->interface()))
        _dataTransferWindows[item->interface()]->show();
    else
    {
        DataTransferWindow *window = new DataTransferWindow;
        window->setInterface(item->interface());
        _dataTransferWindows.insert(item->interface(), window);
        window->show();
    }
}

void UsbDeviceTreeView::__openControlTransferWindow()
{
    /* QStandardItem was not declared with Q_OBJECT,
     * we can not use qobject_cast here.
     */
    UsbDeviceItem *item = dynamic_cast<UsbDeviceItem *>(
                _model->itemFromIndex(currentIndex()));
    if (_dataTransferWindows.contains(item->device()))
        _dataTransferWindows[item->device()]->show();
    else
    {
        DataTransferWindow *window = new DataTransferWindow;
        window->setDevice(item->device());
        _dataTransferWindows.insert(item->device(), window);
        window->show();
    }
}

void UsbDeviceTreeView::__resetDevice()
{
    /* QStandardItem was not declared with Q_OBJECT,
     * we can not use qobject_cast here.
     */
    UsbDeviceItem *deviceItem = dynamic_cast<UsbDeviceItem *>(
                _model->itemFromIndex(currentIndex()));
    if (_dataTransferWindows.contains(deviceItem->device()))
    {
        _dataTransferWindows[deviceItem->device()]->close();
        delete _dataTransferWindows[deviceItem->device()];
        _dataTransferWindows.remove(deviceItem->device());
    }
    for (int i = 0; i < deviceItem->rowCount(); ++i)
    {
        /* QStandardItem was not declared with Q_OBJECT,
         * we can not use qobject_cast here.
         */
        UsbInterfaceItem *interfaceItem = dynamic_cast<UsbInterfaceItem *>(
                    deviceItem->child(i));
        if (_dataTransferWindows.contains(interfaceItem->interface()))
        {
            _dataTransferWindows[interfaceItem->interface()]->close();
            delete _dataTransferWindows[interfaceItem->interface()];
            _dataTransferWindows.remove(interfaceItem->interface());
        }
    }
    usb::UsbDevice::reset(deviceItem->device());
}

void UsbDeviceTreeView::__customSetAltsetting()
{
    const QModelIndex &index = currentIndex();

    /* QStandardItem was not declared with Q_OBJECT,
     * we can not use qobject_cast here.
     */
    UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(
                _model->itemFromIndex(index));
    bool ok;
    uint16_t value = QInputDialog::getInt(this->parentWidget(),
                                          tr("Custom"),
                                          tr("Set custom altsetting index"),
                                          item->interface()->currentAltsetting(),
                                          0,
                                          item->interface()->numAltsetting() - 1,
                                          1,
                                          &ok);
    if (ok)
        __setAltsetting(index, value);
}

void UsbDeviceTreeView::__setAltsetting(const QModelIndex &index, int altsetting)
{
    if (index.parent() != QModelIndex())
    {
        /* QStandardItem was not declared with Q_OBJECT,
         * we can not use qobject_cast here.
         */
        UsbInterfaceItem *item = dynamic_cast<UsbInterfaceItem *>(
                    _model->itemFromIndex(index));
        if (_dataTransferWindows.contains(item->interface()))
            _dataTransferWindows[item->interface()]->close();

        int ret = item->interface()->setAltsetting(altsetting);
        if (ret == 0)
            emit currentIndexChanged(index);
    }
}

void UsbDeviceTreeView::__updateTranslations()
{
    _actionReset->setText(tr("Reset"));
    _actionControlTransfer->setText(tr("Control Transfer"));
    _actionDataTransfer->setText(tr("Data Trasfer"));
    _actionSetAltsetting->setTitle(tr("Set Altsetting"));
    _actionSetAltsettingFirst->setText(tr("First"));
    _actionSetAltsettingPrevious->setText(tr("Previous"));
    _actionSetAltsettingNext->setText(tr("Next"));
    _actionSetAltsettingCustom->setText(tr("Custom .."));
}

UsbDeviceItem::UsbDeviceItem(const QString &text) : QStandardItem(text)
{

}

UsbDeviceItem::UsbDeviceItem(const QIcon &icon, const QString &text) : QStandardItem(icon, text)
{

}

UsbDeviceItem::UsbDeviceItem(int rows, int columns) : QStandardItem(rows, columns)
{

}

void UsbDeviceItem::setUsbDevice(usb::UsbDevice *usbDevice)
{
    _device = usbDevice;

    setText(_device->displayName());
    setToolTip(_device->displayName());

    /* Clear children at first */
    removeRows(0, rowCount());
    for (int i = 0; i < usbDevice->configurationDescriptor()->bNumInterfaces(); ++i)
    {
        usb::UsbInterface *interface = usbDevice->configurationDescriptor()->interface(i);
        UsbInterfaceItem *item  = new UsbInterfaceItem;
        appendRow(item);
        item->setUsbInterface(interface);
        item->setEditable(false);
    }
}

usb::UsbDevice *UsbDeviceItem::device() const
{
    return _device;
}

UsbInterfaceItem::UsbInterfaceItem(const QString &text) : QStandardItem(text)
{

}

UsbInterfaceItem::UsbInterfaceItem(const QIcon &icon, const QString &text) : QStandardItem(icon, text)
{

}

UsbInterfaceItem::UsbInterfaceItem(int rows, int columns) : QStandardItem(rows, columns)
{

}

void UsbInterfaceItem::setUsbInterface(usb::UsbInterface *usbInterface)
{
    _interface = usbInterface;

    setText(_interface->displayName());
    setToolTip(_interface->displayName());
}

usb::UsbInterface *UsbInterfaceItem::interface() const
{
    return _interface;
}
