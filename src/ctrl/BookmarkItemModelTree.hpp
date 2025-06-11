#pragma once

#include "core/BookmarkNodeEvent.hpp"
#include <QAbstractItemModel>
#include <vector>

namespace ctrl
{

class BookmarkItemModelTree final
    : public QAbstractItemModel
    , public core::BookmarkNodeEvent
{
    Q_OBJECT

public:
    explicit BookmarkItemModelTree(
        std::shared_ptr<core::BookmarkNode> rootNode, QObject* parent = nullptr
    );

    enum Column
    {
        Column_Name,
        Column_Url,

        ColumnCount
    };

    auto GetModelIndex(const std::shared_ptr<core::BookmarkNode>& node, int column = 0)
        const -> QModelIndex;
    auto GetBookmarkNode(const QModelIndex& index) const
        -> std::shared_ptr<core::BookmarkNode>;
    auto GetRootNode() const -> std::shared_ptr<core::BookmarkNode>;

    auto index(int row, int column, const QModelIndex& parent = QModelIndex()) const
        -> QModelIndex override;
    auto parent(const QModelIndex& index) const -> QModelIndex override;

    auto rowCount(const QModelIndex& parent = QModelIndex()) const -> int override;
    auto columnCount(const QModelIndex& parent = QModelIndex()) const -> int override;

    auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole)
        const -> QVariant override;
    auto data(const QModelIndex& index, int role = Qt::DisplayRole) const
        -> QVariant override;

private:
    struct ItemModelNode
    {
        std::shared_ptr<core::BookmarkNode> bookmarkNode;
        std::vector<std::unique_ptr<ItemModelNode>> children;
    };

    static auto MakeItemModelNode(std::shared_ptr<core::BookmarkNode> bookmarkNode)
        -> std::unique_ptr<ItemModelNode>;
    auto GetItemModelNode(const QModelIndex& index) const -> const ItemModelNode*;
    auto FindItemModelNode(std::shared_ptr<core::BookmarkNode> bookmarkNode) const
        -> std::pair<const ItemModelNode*, int>;
    auto FindItemModelNode(std::shared_ptr<core::BookmarkNode> bookmarkNode)
        -> std::pair<ItemModelNode*, int>;

    std::unique_ptr<ItemModelNode> m_rootNode;

    // BookmarkNodeEvent interface
private:
    void ReceiveEvent(const core::BookmarkNodeEventParam::NameChangedRecursive& param
    ) override;
    void ReceiveEvent(const core::BookmarkNodeEventParam::ChildInsertedRecursive& param
    ) override;
    void ReceiveEvent(const core::BookmarkNodeEventParam::ChildErasedRecursive& param
    ) override;
};

} // namespace ctrl
