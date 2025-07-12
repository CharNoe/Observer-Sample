#include "BookmarkTreeWidget.hpp"
#include "ui_BookmarkTreeWidget.h"

#include "core/BookmarkNode_Folder.hpp"
#include "core/BookmarkNode_Url.hpp"
#include "ctrl/BookmarkItemModelTree.hpp"
#include "ctrl/BookmarkManager.hpp"
#include "gui/BookmarkUrlFactoryDialog.hpp"
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
    const auto connectAction = [this](QAction* action, auto func)
    {
        connect(action, &QAction::triggered, this, func);
    };
    connectAction(ui->actionAddUrl, &BookmarkTreeWidget::AddBookmarkUrl);
    connectAction(ui->action_AddFolder, &BookmarkTreeWidget::AddBookmarkFolder);
    connect(
        ui->deleteButton,
        &QAbstractButton::clicked,
        this,
        &BookmarkTreeWidget::DeleteSelectBookmarks
    );
    connect(
        ui->treeView->selectionModel(),
        &QItemSelectionModel::currentChanged,
        this,
        &BookmarkTreeWidget::OnCurrentIndexChanged
    );
    connect(
        ui->treeView->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this,
        &BookmarkTreeWidget::OnSelectionIndexChanged
    );
    ConnectQt(*m_bookmarkManager, this);
    m_bookmarkManager->CallReceiveEvent(*this);

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

void BookmarkTreeWidget::OnSelectionIndexChanged(
    const QItemSelection& selected, const QItemSelection& deselected
)
{
    auto indexes = ui->treeView->selectionModel()->selectedRows();

    std::vector<std::shared_ptr<core::BookmarkNode>> selectNodes;
    for (const auto& index : indexes)
    {
        if (auto node = m_model->GetBookmarkNode(index))
        {
            selectNodes.emplace_back(std::move(node));
        }
    }

    m_bookmarkManager->SetSelectNodes(std::move(selectNodes));
}

void BookmarkTreeWidget::AddBookmarkUrl()
{
    auto node = BookmarkUrlFactoryDialog::Execute(this);
    if (!node)
        return;
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

void BookmarkTreeWidget::DeleteSelectBookmarks()
{
    m_bookmarkManager->DeleteSelectNodes();
}

void BookmarkTreeWidget::ReceiveEvent(const BookmarkManager_SelectChanged& param)
{
    ui->deleteButton->setEnabled(!param.selectNodes.empty());
}

} // namespace gui
