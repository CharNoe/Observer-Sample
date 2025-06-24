#include "BookmarkItemModelTree.hpp"

#include "core/BookmarkNode.hpp"
#include "core/BookmarkNode_Url.hpp"

namespace ctrl
{

BookmarkItemModelTree::BookmarkItemModelTree(
    std::shared_ptr<core::BookmarkNode> rootNode, QObject* parent
)
    : QAbstractItemModel(parent)
    , m_rootNode{MakeItemModelNode(std::move(rootNode))}
{
    ConnectQt(*m_rootNode->bookmarkNode, this);
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
    case Column_Url:
    {
        auto urlNode = std::dynamic_pointer_cast<core::BookmarkNode_Url>(node);
        if (urlNode)
        {
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return urlNode->GetUrl();
        }
        break;
    }

    default:
        break;
    } // switch (index.column())

    return {};
}

bool BookmarkItemModelTree::setData(
    const QModelIndex& index, const QVariant& value, int role
)
{
    if (!index.isValid())
        return false;

    auto node = GetBookmarkNode(index);
    if (!node)
        return false;

    switch (index.column())
    {
    case Column_Name:
    {
        return node->SetName(value.toString());
    }
    case Column_Url:
    {
        auto urlNode = std::dynamic_pointer_cast<core::BookmarkNode_Url>(node);
        if (urlNode)
        {
            return urlNode->SetUrl(value.toString());
        }
        break;
    }
    default:
        break;
    } // switch (index.column())

    return {};
}

Qt::ItemFlags BookmarkItemModelTree::flags(const QModelIndex& index) const
{
    Qt::ItemFlags result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (auto node = GetBookmarkNode(index))
    {
        switch (index.column())
        {
        case Column_Name:
        {
            result |= Qt::ItemIsEditable;
            break;
        }
        case Column_Url:
        {
            if (node->GetKind() == core::BookmarkKind::Url)
                result |= Qt::ItemIsEditable;
            break;
        }
        default:
            break;
        } // switch (index.column())
    }

    return result;
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

void BookmarkItemModelTree::ReceiveEvent(
    const BookmarkNode_NameChangedRecursive& param
)
{
    const auto index = GetModelIndex(param.node, Column_Name);
    emit dataChanged(index, index);
}

void BookmarkItemModelTree::ReceiveEvent(
    const BookmarkNode_ChildInsertedRecursive& param
)
{
    const auto modelIndex = GetModelIndex(param.node);
    const auto row = static_cast<int>(param.index);

    beginInsertRows(modelIndex, row, row);
    ItemModelNode* itemModelNode = FindItemModelNode(param.node).first;
    itemModelNode->children.insert(
        std::next(itemModelNode->children.begin(), param.index),
        MakeItemModelNode(param.child)
    );
    endInsertRows();
}

void BookmarkItemModelTree::ReceiveEvent(
    const BookmarkNode_ChildErasedRecursive& param
)
{
    const auto modelIndex = GetModelIndex(param.node);
    const auto row = static_cast<int>(param.index);

    beginRemoveRows(modelIndex, row, row);
    ItemModelNode* itemModelNode = FindItemModelNode(param.node).first;
    itemModelNode->children.erase(std::next(itemModelNode->children.begin(), param.index)
    );
    endRemoveRows();
}

} // namespace ctrl
