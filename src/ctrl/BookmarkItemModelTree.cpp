#include "BookmarkItemModelTree.hpp"

#include "core/BookmarkNode.hpp"

namespace ctrl
{

BookmarkItemModelTree::BookmarkItemModelTree(
    std::shared_ptr<core::BookmarkNode> rootNode, QObject* parent
)
    : QAbstractItemModel(parent)
    , m_rootNode{MakeItemModelNode(std::move(rootNode))}
{
    m_rootNode->bookmarkNode->eventSender.ConnectQt(this);
}

QModelIndex BookmarkItemModelTree::GetModelIndex(
    const std::shared_ptr<core::BookmarkNode>& node, int column
) const
{
    const auto [itemModelNode, row] = FindItemModelNode(node);
    if (!itemModelNode || itemModelNode == m_rootNode.get())
        return {};
    return createIndex(row, column, itemModelNode);
}

std::shared_ptr<core::BookmarkNode> BookmarkItemModelTree::GetBookmarkNode(
    const QModelIndex& index
) const
{
    return GetItemModelNode(index)->bookmarkNode;
}

auto BookmarkItemModelTree::GetRootNode() const -> std::shared_ptr<core::BookmarkNode>
{
    return m_rootNode->bookmarkNode;
}

QVariant BookmarkItemModelTree::headerData(
    int section, Qt::Orientation orientation, int role
) const
{
    if (orientation == Qt::Vertical)
        return {};

    switch (section)
    {
    case Column_Name:
    {
        if (role == Qt::DisplayRole)
            return tr("Name");
        break;
    }
    case Column_Url:
    {
        if (role == Qt::DisplayRole)
            return tr("Url");
        break;
    }
    default:
        break;
    } // switch (section)

    return {};
}

QModelIndex BookmarkItemModelTree::index(int row, int column, const QModelIndex& parent)
    const
{
    if (!hasIndex(row, column, parent))
    {
        return {};
    }
    const auto itemModelNode = GetItemModelNode(parent);
    ItemModelNode* childNode = itemModelNode->children[static_cast<size_t>(row)].get();
    return createIndex(row, column, childNode);
}

QModelIndex BookmarkItemModelTree::parent(const QModelIndex& index) const
{
    auto node = GetBookmarkNode(index);
    if (node)
    {
        node = node->GetParent();
    }

    return GetModelIndex(node);
}

int BookmarkItemModelTree::rowCount(const QModelIndex& parent) const
{
    const ItemModelNode* itemModelNode = GetItemModelNode(parent);
    return static_cast<int>(itemModelNode->children.size());
}

int BookmarkItemModelTree::columnCount(const QModelIndex& parent) const
{
    return ColumnCount;
}

QVariant BookmarkItemModelTree::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    auto node = GetBookmarkNode(index);
    if (!node)
        return {};

    switch (index.column())
    {
    case Column_Name:
    {
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            return node->GetName();
        }
        break;
    }
    default:
        break;
    } // switch (index.column())

    return {};
}

auto BookmarkItemModelTree::MakeItemModelNode(
    std::shared_ptr<core::BookmarkNode> bookmarkNode
) -> std::unique_ptr<ItemModelNode>
{
    std::unique_ptr<ItemModelNode> itemModelNode = std::make_unique<ItemModelNode>();
    const size_t childrenSize = bookmarkNode->GetChildrenSize();
    for (size_t i = 0; i < childrenSize; ++i)
    {
        auto childNode = bookmarkNode->GetChild(i);
        itemModelNode->children.push_back(MakeItemModelNode(std::move(childNode)));
    }
    itemModelNode->bookmarkNode = std::move(bookmarkNode);

    return itemModelNode;
}

auto BookmarkItemModelTree::GetItemModelNode(const QModelIndex& index) const
    -> const ItemModelNode*
{
    auto nodePtr = static_cast<ItemModelNode*>(index.internalPointer());
    return nodePtr ? nodePtr : m_rootNode.get();
}

auto BookmarkItemModelTree::FindItemModelNode(
    std::shared_ptr<core::BookmarkNode> bookmarkNode
) const -> std::pair<const ItemModelNode*, int>
{
    if (!bookmarkNode)
        return {nullptr, -1};
    if (m_rootNode->bookmarkNode == bookmarkNode)
        return {m_rootNode.get(), -1};

    const ItemModelNode* parentItemModelNode =
        FindItemModelNode(bookmarkNode->GetParent()).first;
    if (!parentItemModelNode)
        return {nullptr, -1};

    auto it = std::ranges::find_if(
        parentItemModelNode->children,
        [&bookmarkNode](const std::unique_ptr<ItemModelNode>& itemModelNode)
        { return itemModelNode->bookmarkNode == bookmarkNode; }
    );
    if (it == parentItemModelNode->children.end())
        return {nullptr, -1};

    const auto row =
        static_cast<int>(std::distance(parentItemModelNode->children.begin(), it));
    return {it->get(), row};
}

auto BookmarkItemModelTree::FindItemModelNode(
    std::shared_ptr<core::BookmarkNode> bookmarkNode
) -> std::pair<ItemModelNode*, int>
{
    const auto [ptr, row] =
        static_cast<const BookmarkItemModelTree*>(this)->FindItemModelNode(
            std::move(bookmarkNode)
        );
    return {const_cast<ItemModelNode*>(ptr), row};
}

void BookmarkItemModelTree::BookmarkNode_NameChangedRecursive(
    const std::shared_ptr<core::BookmarkNode>& node, const QString& name
)
{
    const auto index = GetModelIndex(node, Column_Name);
    emit dataChanged(index, index);
}

void BookmarkItemModelTree::BookmarkNode_ChildInsertedRecursive(
    const std::shared_ptr<core::BookmarkNode>& node,
    const std::shared_ptr<core::BookmarkNode>& child,
    size_t index
)
{
    const auto modelIndex = GetModelIndex(node);
    const auto row = static_cast<int>(index);
    qDebug() << modelIndex << row;

    beginInsertRows(modelIndex, row, row);
    ItemModelNode* itemModelNode = FindItemModelNode(node).first;
    itemModelNode->children.insert(
        std::next(itemModelNode->children.begin(), index), MakeItemModelNode(child)
    );
    endInsertRows();
}

void BookmarkItemModelTree::BookmarkNode_ChildErasedRecursive(
    const std::shared_ptr<core::BookmarkNode>& node,
    const std::shared_ptr<core::BookmarkNode>& name,
    size_t index
)
{
    const auto modelIndex = GetModelIndex(node);
    const auto row = static_cast<int>(index);

    beginRemoveRows(modelIndex, row, row);
    ItemModelNode* itemModelNode = FindItemModelNode(node).first;
    itemModelNode->children.erase(std::next(itemModelNode->children.begin(), index));
    endRemoveRows();
}

} // namespace ctrl
