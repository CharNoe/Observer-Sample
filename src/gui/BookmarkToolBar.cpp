#include "BookmarkToolBar.hpp"

#include "core/BookmarkNodeVisitor.hpp"
#include "core/BookmarkNode_Folder.hpp"
#include "core/BookmarkNode_Url.hpp"
#include <QDesktopServices>
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
    std::shared_ptr<core::BookmarkNode> rootNode, QWidget* parent
)
    : QToolBar(tr("Bookmark"), parent)
{
    ConnectQt(rootNode->eventSender, this);
}

QAction* BookmarkToolBar::MakeAction(
    std::shared_ptr<core::BookmarkNode> node, QWidget* parent
)
{
    ActionFactory factory{parent};
    node->Accept(factory);
    return factory.GetResult();
}

void BookmarkToolBar::ReceiveEvent(
    const core::BookmarkNodeEventParam::ChildInserted& param
)
{
    auto actionList = actions();
    QAction* before = nullptr;
    insertAction(before, MakeAction(param.child, this));
}

void BookmarkToolBar::ReceiveEvent(const core::BookmarkNodeEventParam::ChildErased& param)
{
    auto actionList = actions();
    auto action = actionList[param.index];
    if (action)
    {
        action->deleteLater();
        removeAction(action);
    }
}

BookmarkToolBar::FolderMenu::FolderMenu(
    std::shared_ptr<core::BookmarkNode> node, QWidget* parent
)
    : QMenu{node->GetName(), parent}
{
    ConnectQt(node->eventSender, this);
    // node が children を持っていたときの処理が必要だが、面倒なので省略
}

void BookmarkToolBar::FolderMenu::ReceiveEvent(
    const core::BookmarkNodeEventParam::NameChanged& param
)
{
    setTitle(param.name);
}

void BookmarkToolBar::FolderMenu::ReceiveEvent(
    const core::BookmarkNodeEventParam::ChildInserted& param
)
{
    auto actionList = actions();
    QAction* before = nullptr;
    insertAction(before, MakeAction(param.child, this));
}

void BookmarkToolBar::FolderMenu::ReceiveEvent(
    const core::BookmarkNodeEventParam::ChildErased& param
)
{
    auto actionList = actions();
    auto action = actionList[param.index];
    if (action)
    {
        action->deleteLater();
        removeAction(action);
    }
}

BookmarkToolBar::UrlAction::UrlAction(
    std::shared_ptr<core::BookmarkNode_Url> node, QObject* parent
)
    : QAction{node->GetName(), parent}
{
    connect(this, &QAction::triggered, this, &UrlAction::OnTriggered);
    ConnectQt(node->eventSender, this);
    ConnectQt(node->eventSenderUrl, this);
}

void BookmarkToolBar::UrlAction::OnTriggered()
{
    QDesktopServices::openUrl(QUrl{m_url});
}

void BookmarkToolBar::UrlAction::ReceiveEvent(
    const core::BookmarkNodeEventParam::NameChanged& param
)
{
    setText(param.name);
}

void BookmarkToolBar::UrlAction::ReceiveEvent(
    const core::BookmarkNode_UrlEventParam::UrlChanged& param
)
{
    m_url = param.url;
}

} // namespace gui
