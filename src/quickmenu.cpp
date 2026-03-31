#include "quickmenu.h"
#include "ui_quickmenu.h"

#include <QScreen>
#include <QGuiApplication>
#include <QFocusEvent>
#include <QTimer>
#include <QSortFilterProxyModel>
#include <QClipboard>
#include <QMessageBox>
#include <QMenu>
#include <windows.h>

#include "clipboardmodel.h"
#include "clipboardmanager.h"
#include "listitemdelegate.h"
#include "databasemanager.h"
#include "settingswindow.h"
#include "traycontroller.h"
#include "appsettings.h"

QuickMenu::QuickMenu(TrayController* Tray, AppSettings* Settings, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuickMenu)
{
    ui->setupUi(this);

    //set flags
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // resize for different screen size
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    resize(screenGeometry.width() / 5, screenGeometry.height() / 2.2);

    int x = screenGeometry.width() - width() - 10;
    int y = screenGeometry.height() - height() - 10;
    move(x, y);

    // icons
    ui->CloseButton->setIcon(QIcon(":/icons/close.png"));
    ui->SettingsButton->setIcon(QIcon(":/icons/settings.png"));
    ui->clearButton->setIcon(QIcon(":/icons/bin.png"));

    // timer for update viewport
    Timer = new QTimer(this);
    connect(Timer, &QTimer::timeout, this, &QuickMenu::UpdateTimer);
    Timer->start(5000); // 5 sec update
}

QuickMenu::~QuickMenu()
{
    delete Delegate;
    delete ui;
}

bool QuickMenu::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY && msg->wParam == 100)
    {
        if (SettingsRef)
            return false;

        isHidden() ? ShowWindow() : HideWindow();
        return true;
    }

    return QWidget::nativeEvent(eventType, message, result);
}

void QuickMenu::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::ActivationChange && !isActiveWindow())
        HideWindow();

    QWidget::changeEvent(event);
}

void QuickMenu::HideWindow()
{
    hide();
    Model->Refresh();
}

void QuickMenu::ShowWindow()
{
    show();
    activateWindow();
    raise();
    ui->listView->scrollToTop();
}

void QuickMenu::SetModel(QSortFilterProxyModel* NewModel)
{
    ui->listView->setModel(NewModel);
}

void QuickMenu::OnItemClicked(const QModelIndex& index)
{
    int entryId = index.data(Qt::UserRole).toInt();

    QString fullText = Manager->GetDatabase()->GetFullText(entryId);

    // ?? TODO: Maybe better to save full text?
    QString text = index.data(ClipboardModel::PreviewRole).toString();
    if (Manager)
        Manager->SetLastSelectedString(text);

    QGuiApplication::clipboard()->setText(fullText);
    HideWindow();
}

void QuickMenu::InitList(ClipboardManager* NewManager)
{
    Manager = NewManager;

    if (!NewManager || !NewManager->GetDatabase())
        return;

    Model = new ClipboardModel(NewManager->GetDatabase(), this);

    // Delete entry with context menu
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, &QListView::customContextMenuRequested, this, [this](const QPoint &pos)
    {
        QModelIndex index = ui->listView->indexAt(pos);
        if (!index.isValid())
            return;

        QMenu menu(this);
        QAction *deleteAction = menu.addAction(QIcon(":/icons/bin.png"), "Удалить");
        QAction *selectedItem = menu.exec(ui->listView->viewport()->mapToGlobal(pos));

        if (selectedItem == deleteAction)
        {
            int ID = index.data(Qt::UserRole).toInt();
            if (Manager->GetDatabase())
                Manager->GetDatabase()->DeleteEntry(ID);

            Model->removeRow(index.row());

            Model->Refresh();
        }

    });

    Delegate = new ListItemDelegate();
    ui->listView->setItemDelegate(Delegate);

    ui->listView->setModel(Model);

    SearchTimer = new QTimer(this);
    SearchTimer->setSingleShot(true);

    connect(ui->lineSearch, &QLineEdit::textChanged, this, [this]() {
        SearchTimer->start(300);
    });

    connect(SearchTimer, &QTimer::timeout, this, [this](){
        Model->SetSearchText(ui->lineSearch->text());
    });

    connect(ui->listView, &QListView::clicked, this, &QuickMenu::OnItemClicked);

    connect(Manager, &ClipboardManager::OnHistoryUpdated, this, &QuickMenu::RefreshModel);

    Model->Refresh();
}

void QuickMenu::UpdateTimer()
{
    if (!isHidden())
        ui->listView->viewport()->update();
}

void QuickMenu::RefreshModel()
{
    ui->lineSearch->clear();
    Model->Refresh();
}

void QuickMenu::on_CloseButton_clicked()
{
    HideWindow();
}

void QuickMenu::on_clearButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Rio Clipboard Manager");
    msgBox.setText("Вся история будет удалена без возможности восстановления.");
    msgBox.setInformativeText("Вы уверены, что хотите очистить историю?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        Manager->GetDatabase()->ResetHistory();
        Model->Refresh();

        QMessageBox::information(nullptr, "Rio Clipboard Manager", "Готово!");
    }
}

void QuickMenu::on_SettingsButton_clicked()
{
    OpenSettings();
}

void QuickMenu::OpenSettings()
{
    if (bIsKeyBinded)
    {
        UnregisterHotKey((HWND)winId(), 100);
        bIsKeyBinded = false;
    }

    if (SettingsRef)
    {
        SettingsRef->activateWindow();
        SettingsRef->raise();
        return;
    }

    TrayController* Tray = qobject_cast<TrayController*>(Manager->parent());
    if (!Tray)
        return;

    SettingsRef = new SettingsWindow(Tray->GetSettings());

    // Delete on close (From docs: Makes Qt delete this widget when the widget has accepted the close event (see QWidget::closeEvent()).)
    SettingsRef->setAttribute(Qt::WA_DeleteOnClose);

    connect(SettingsRef, &SettingsWindow::destroyed, this, [this]()
    {
        this->SettingsRef = nullptr;

        if (!bIsKeyBinded && LastHotKey.valid)
            bIsKeyBinded = RegisterHotKey((HWND)winId(), 100, LastHotKey.modifiers, LastHotKey.vk);
    });

    SettingsRef->show();
}

void QuickMenu::BindKey(HotKey* NewKey)
{
    if (!NewKey->valid)
        return;

    if (bIsKeyBinded)
        UnregisterHotKey((HWND)winId(), 100);

    bIsKeyBinded = RegisterHotKey((HWND)winId(), 100, NewKey->modifiers, NewKey->vk);
    if (bIsKeyBinded)
        LastHotKey = *NewKey;
}