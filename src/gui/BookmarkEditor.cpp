#include "BookmarkEditor.hpp"
#include "ui_BookmarkEditor.h"

#include "core/BookmarkNodeVisitor.hpp"
#include "core/BookmarkNode_Url.hpp"
#include "ctrl/BookmarkManager.hpp"
#include <QDesktopServices>

namespace gui
{

BookmarkEditor::BookmarkEditor(
    std::shared_ptr<ctrl::BookmarkManager> bookmarkManager, QWidget* parent
)
    : QWidget(parent)
    , ui(new Ui::BookmarkEditor)
    , m_bookmarkManager{std::move(bookmarkManager)}
{
    ui->setupUi(this);
    connect(
        ui->nameLineEdit, &QLineEdit::textChanged, this, &BookmarkEditor::OnNameChanged
    );
    connect(
        ui->urlLineEdit, &QLineEdit::textChanged, this, &BookmarkEditor::OnUrlChanged
    );
    connect(ui->openUrlButton, &QToolButton::clicked, this, &BookmarkEditor::OpenUrl);

    AutoConnectQt<ctrl::BookmarkManagerEvent>(m_bookmarkManager, this);
    m_bookmarkManager->CallReceiveEvent(*this);
}

BookmarkEditor::~BookmarkEditor()
{
    delete ui;
}

void BookmarkEditor::OnNameChanged(const QString& name)
{
    if (m_currentNode)
        m_currentNode->SetName(name);
}

void BookmarkEditor::OnUrlChanged(const QString& url)
{
    ui->openUrlButton->setEnabled(!url.isEmpty());
    if (m_currentNodeUrl)
        m_currentNodeUrl->SetUrl(url);
}

void BookmarkEditor::OpenUrl()
{
    if (m_currentNodeUrl)
        QDesktopServices::openUrl(QUrl{m_currentNodeUrl->GetUrl()});
}

void BookmarkEditor::ReceiveEvent(const BookmarkManager_CurrentChanged& param)
{
    m_currentNode = param.currentNode;
    m_currentNodeUrl =
        std::dynamic_pointer_cast<core::BookmarkNode_Url>(param.currentNode);

    setEnabled(static_cast<bool>(m_currentNode));
    if (m_currentNode)
    {
        switch (m_currentNode->GetKind())
        {
        case core::BookmarkKind::Url:
            ui->stackedWidget->setCurrentWidget(ui->urlPage);
            break;
        case core::BookmarkKind::Root:
        case core::BookmarkKind::Folder:
            ui->stackedWidget->setCurrentWidget(ui->folderPage);
            break;
        } // switch (m_currentNode->GetKind())
    }

    AutoConnectQt<core::BookmarkNodeEvent>(m_currentNode, this);
    ReceiveEvent(
        BookmarkNode_NameChanged{m_currentNode ? m_currentNode->GetName() : QString{}}
    );

    AutoConnectQt<core::BookmarkNode_UrlEvent>(m_currentNodeUrl, this);
    ReceiveEvent(BookmarkNode_Url_UrlChanged{
        m_currentNodeUrl ? m_currentNodeUrl->GetUrl() : QString{}
    });
}

void BookmarkEditor::ReceiveEvent(const BookmarkNode_Url_UrlChanged& param)
{
    ui->urlLineEdit->setText(param.url);
}

void BookmarkEditor::ReceiveEvent(const BookmarkNode_NameChanged& param)
{
    ui->nameLineEdit->setText(param.name);
}

} // namespace gui
