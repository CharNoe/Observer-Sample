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
    , m_model{
          new ctrl::BookmarkItemModelTree{bookmarkManager->GetRootBookmarkBase(), this}
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

    ui->addButton->addAction(ui->actionAddUrl);
    ui->addButton->addAction(ui->action_AddFolder);
}

BookmarkTreeWidget::~BookmarkTreeWidget()
{
    delete ui;
}

void BookmarkTreeWidget::AddBookmarkUrl()
{
    QString name = QInputDialog::getText(this, tr("Add Url"), tr("Name"));
    if (name.isEmpty())
        return;

    auto node = std::make_shared<core::BookmarkNode_Url>(std::move(name), "");
    m_model->GetRootNode()->PushChild(std::move(node));
}

void BookmarkTreeWidget::AddBookmarkFolder()
{
    QString name = QInputDialog::getText(this, tr("Add Folder"), tr("Name"));
    if (name.isEmpty())
        return;

    auto node = std::make_shared<core::BookmarkNode_Folder>(std::move(name));
    m_model->GetRootNode()->PushChild(std::move(node));
}

} // namespace gui
