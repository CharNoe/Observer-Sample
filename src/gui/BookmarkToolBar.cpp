#include "BookmarkToolBar.hpp"

#include "core/BookmarkNodeVisitor.hpp"
#include "core/BookmarkNode_Folder.hpp"
#include "core/BookmarkNode_Url.hpp"
#include "ctrl/BookmarkManager.hpp"
#include "gui/BookmarkUrlFactoryDialog.hpp"
#include <QDesktopServices>
#include <QInputDialog>
#include <QUrl>

namespace gui
{

class BookmarkToolBar::ActionFactory : public core::BookmarkNodeVisitor
{
    QWidget* m_parent;
    QAction* m_result = nullptr;

public:
    ActionFactory(QWidget* parent)
        : m_parent{parent}
    {
    }

    auto GetResult() const -> QAction*
    {
        return m_result;
    }

    // BookmarkNodeVisitor interface
public:
    void Visit(core::BookmarkNode_Root& node) override
    {
    }
    void Visit(core::BookmarkNode_Folder& node) override
    {
        auto menu = new FolderMenu{node.shared_from_this(), m_parent};
        m_result = menu->menuAction();
    }
    void Visit(core::BookmarkNode_Url& node) override
    {
        m_result = new UrlAction{node.shared_from_this(), m_parent};
    }
};

BookmarkToolBar::BookmarkToolBar(
    std::shared_ptr<ctrl::BookmarkManager> bookmarkManager, QWidget* parent
)
    : QToolBar(tr("ToolBar"), parent)
    , m_bookmarkManager{std::move(bookmarkManager)}
    , m_childrenSize{0}
{
    ConnectQt(*m_bookmarkManager->GetRootBookmarkBase(), this);
    auto action = addAction(tr("Add"));
    connect(action, &QAction::triggered, this, &BookmarkToolBar::PushAction);
    addSeparator();
}

auto BookmarkToolBar::MakeAction(
    std::shared_ptr<core::BookmarkNode> node, QWidget* parent
) -> QAction*
{
    ActionFactory factory{parent};
    node->Accept(factory);
    return factory.GetResult();
}

void BookmarkToolBar::PushAction()
{
    auto node = BookmarkUrlFactoryDialog::Execute(this);
    if (!node)
        return;

    auto target = m_bookmarkManager->GetCurrentNode();
    if (!target)
        target = m_bookmarkManager->GetRootBookmarkBase();

    while (target)
    {
        if (target->IsInsertable(*node))
        {
            target->PushChild(node);
            return;
        }
        target = target->GetParent();
    }
}

void BookmarkToolBar::ReceiveEvent(const BookmarkNode_ChildInserted& param)
{
    auto actionList = actions();
    QAction* before =
        (m_childrenSize == param.index) ? nullptr : actionList[param.index + 2];
    insertAction(before, MakeAction(param.child, this));
    ++m_childrenSize;
}

void BookmarkToolBar::ReceiveEvent(const BookmarkNode_ChildErased& param)
{
    auto actionList = actions();
    auto action = actionList[param.index + 2];
    if (action)
    {
        action->deleteLater();
        removeAction(action);
        --m_childrenSize;
    }
}

BookmarkToolBar::FolderMenu::FolderMenu(
    std::shared_ptr<core::BookmarkNode> node, QWidget* parent
)
    : QMenu{node->GetName(), parent}
    , m_childrenSize{0}
{
    ConnectQt(*node, this);
    // node が children を持っていたときの処理が必要だが、面倒なので省略
}

void BookmarkToolBar::FolderMenu::ReceiveEvent(const BookmarkNode_NameChanged& param
)
{
    setTitle(param.name);
}

void BookmarkToolBar::FolderMenu::ReceiveEvent(
    const BookmarkNode_ChildInserted& param
)
{
    auto actionList = actions();
    QAction* before = (m_childrenSize == param.index) ? nullptr : actionList[param.index];
    insertAction(before, MakeAction(param.child, this));
    ++m_childrenSize;
}

void BookmarkToolBar::FolderMenu::ReceiveEvent(const BookmarkNode_ChildErased& param
)
{
    auto actionList = actions();
    auto action = actionList[param.index];
    if (action)
    {
        action->deleteLater();
        removeAction(action);
        --m_childrenSize;
    }
}

BookmarkToolBar::UrlAction::UrlAction(
    std::shared_ptr<core::BookmarkNode_Url> node, QObject* parent
)
    : QAction{node->GetName(), parent}
    , m_url{node->GetUrl()}
{
    connect(this, &QAction::triggered, this, &UrlAction::OnTriggered);
    ConnectQt<BookmarkNodeEvent>(*node, this);
    ConnectQt<BookmarkNode_UrlEvent>(*node, this);
}

void BookmarkToolBar::UrlAction::OnTriggered()
{
    QDesktopServices::openUrl(QUrl{m_url});
}

void BookmarkToolBar::UrlAction::ReceiveEvent(const BookmarkNode_NameChanged& param)
{
    setText(param.name);
}

void BookmarkToolBar::UrlAction::ReceiveEvent(const BookmarkNode_Url_UrlChanged& param)
{
    m_url = param.url;
}

} // namespace gui
