#include "BookmarkTreeWidget.hpp"
#include "ui_BookmarkTreeWidget.h"

#include "core/BookmarkNode_Folder.hpp"
#include "core/BookmarkNode_Url.hpp"
#include "ctrl/BookmarkItemModelTree.hpp"
#include "ctrl/BookmarkManager.hpp"
#include <QInputDialog>

namespace gui {

BookmarkTreeWidget::BookmarkTreeWidget(
    std::shared_ptr<ctrl::BookmarkManager> bookmarkManager, QWidget* parent
)
    : QWidget(parent)
    , ui(new Ui::BookmarkTreeWidget)
    , m_bookmarkManager{std::move(bookmarkManager)}
    , m_model{
          new ctrl::BookmarkItemModelTree{m_bookmarkManager->GetRootBookmarkBase(), this}
      }
{
    ui->setupUi(this);
    ui->treeView->setModel(m_model);
    connect(
        ui->actionAddUrl, &QAction::triggered, this, &BookmarkTreeWidget::AddBookmarkUrl
    );
    connect(
        ui->action_AddFolder,
        &QAction::triggered,
        this,
        &BookmarkTreeWidget::AddBookmarkFolder
    );
    connect(
        ui->treeView->selectionModel(),
        &QItemSelectionModel::currentChanged,
        this,
        &BookmarkTreeWidget::OnCurrentIndexChanged
    );

    ui->addButton->addAction(ui->actionAddUrl);
    ui->addButton->addAction(ui->action_AddFolder);
}

BookmarkTreeWidget::~BookmarkTreeWidget()
{
    delete ui;
}

void BookmarkTreeWidget::OnCurrentIndexChanged(
    const QModelIndex& after, const QModelIndex& before
)
{
    auto node = m_model->GetBookmarkNode(after);
    m_bookmarkManager->SetCurrentNode(std::move(node));
}

void BookmarkTreeWidget::AddBookmarkUrl()
{
    QString name = QInputDialog::getText(this, tr("Add Url"), tr("Name"));
    if (name.isEmpty())
        return;

    auto node = std::make_shared<core::BookmarkNode_Url>(std::move(name), "");
    AddBookmark(std::move(node));
}

void BookmarkTreeWidget::AddBookmarkFolder()
{
    QString name = QInputDialog::getText(this, tr("Add Folder"), tr("Name"));
    if (name.isEmpty())
        return;

    auto node = std::make_shared<core::BookmarkNode_Folder>(std::move(name));
    AddBookmark(std::move(node));
}

void BookmarkTreeWidget::AddBookmark(std::shared_ptr<core::BookmarkNode> node)
{
    if (!node)
        return;

    auto target = m_bookmarkManager->GetCurrentNode();
    if (!target)
        target = m_bookmarkManager->GetRootBookmarkBase();

    while (target)
    {
        if (target->IsInsertable(*node))
        {
            if (target->PushChild(node))
            {
                auto index = m_model->GetModelIndex(node);
                ui->treeView->setCurrentIndex(index);
            }
            return;
        }
        target = target->GetParent();
    }
}

} // namespace gui
