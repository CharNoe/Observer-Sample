#pragma once

#include "core/BookmarkNodeEvent.hpp"
#include "core/BookmarkNode_UrlEvent.hpp"
#include "ctrl/BookmarkManagerEvent.hpp"
#include <QWidget>

namespace core
{
class BookmarkNode_Url;
} // namespace core
namespace ctrl
{
class BookmarkManager;
} // namespace ctrl

namespace gui
{

namespace Ui
{
class BookmarkEditor;
}

class BookmarkEditor final
    : public QWidget
    , public core::BookmarkNodeEvent
    , public core::BookmarkNode_UrlEvent
    , public ctrl::BookmarkManagerEvent
{
    Q_OBJECT

public:
    explicit BookmarkEditor(
        std::shared_ptr<ctrl::BookmarkManager> bookmarkManager, QWidget* parent = nullptr
    );
    ~BookmarkEditor();

private:
    void OnNameChanged(const QString& name);
    void OnUrlChanged(const QString& url);
    void OpenUrl();

    Ui::BookmarkEditor* ui;
    const std::shared_ptr<ctrl::BookmarkManager> m_bookmarkManager;
    std::shared_ptr<core::BookmarkNode> m_currentNode;
    std::shared_ptr<core::BookmarkNode_Url> m_currentNodeUrl;

    // BookmarkManagerEvent interface
private:
    void ReceiveEvent(const BookmarkManager_CurrentChanged& param) override;

    // BookmarkNode_UrlEvent interface
private:
    void ReceiveEvent(const BookmarkNode_Url_UrlChanged& param) override;

    // BookmarkNodeEvent interface
private:
    void ReceiveEvent(const BookmarkNode_NameChanged& param) override;
};

} // namespace gui
