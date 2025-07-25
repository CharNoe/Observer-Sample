#include "BookmarkItemModelTree.hpp"

#include "core/BookmarkNode.hpp"
#include "core/BookmarkNode_Url.hpp"
#include <QApplication>
#include <QStyle>

namespace ctrl
{

BookmarkItemModelTree::BookmarkItemModelTree(
    std::shared_ptr<core::BookmarkNode> rootNode, QObject* parent
)
    : QAbstractItemModel(parent)
    , m_rootNode{new ItemModelNode{}}
{
    const size_t childrenSize = rootNode->GetChildrenSize();
    m_rootNode->children.reserve(childrenSize);

    for (size_t i = 0; i < childrenSize; ++i)
    {
        auto childNode = rootNode->GetChild(i);
        m_rootNode->children.push_back(MakeItemModelNode(std::move(childNode)));
    }
    m_rootNode->bookmarkNode = std::move(rootNode);
    ConnectQt(*m_rootNode->bookmarkNode, this);
}

auto BookmarkItemModelTree::GetModelIndex(
    const std::shared_ptr<core::BookmarkNode>& node, int column
) const -> QModelIndex
{
    const auto [itemModelNode, row] = FindItemModelNode(node);
    if (!itemModelNode || itemModelNode == m_rootNode.get())
        return {};
    return createIndex(row, column, ToInternalId(node));
}

auto BookmarkItemModelTree::GetBookmarkNode(const QModelIndex& index) const
    -> std::shared_ptr<core::BookmarkNode>
{
    auto ptr = GetItemModelNode(index);
    return ptr->bookmarkNode;
}

auto BookmarkItemModelTree::GetRootNode() const -> std::shared_ptr<core::BookmarkNode>
{
    return m_rootNode->bookmarkNode;
}

auto BookmarkItemModelTree::ToInternalId(
    const std::shared_ptr<core::BookmarkNode>& bookmarkNode
) -> quintptr
{
    return reinterpret_cast<quintptr>(bookmarkNode.get());
}

auto BookmarkItemModelTree::headerData(int section, Qt::Orientation orientation, int role)
    const -> QVariant
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

auto BookmarkItemModelTree::index(int row, int column, const QModelIndex& parent) const
    -> QModelIndex
{
    if (!hasIndex(row, column, parent))
    {
        return {};
    }
    const ItemModelNode* itemModelNode = GetItemModelNode(parent);
    const quintptr childId = itemModelNode->children[static_cast<size_t>(row)];
    return createIndex(row, column, childId);
}

auto BookmarkItemModelTree::parent(const QModelIndex& index) const -> QModelIndex
{
    auto node = GetBookmarkNode(index);
    if (node)
    {
        node = node->GetParent();
    }

    return GetModelIndex(node);
}

auto BookmarkItemModelTree::rowCount(const QModelIndex& parent) const -> int
{
    const ItemModelNode* itemModelNode = GetItemModelNode(parent);
    return static_cast<int>(itemModelNode->children.size());
}

auto BookmarkItemModelTree::columnCount(const QModelIndex& parent) const -> int
{
    return ColumnCount;
}

auto BookmarkItemModelTree::data(const QModelIndex& index, int role) const -> QVariant
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
        if (role == Qt::DecorationRole)
        {
            if (node->GetKind() == core::BookmarkKind::Folder)
                return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
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

auto BookmarkItemModelTree::setData(
    const QModelIndex& index, const QVariant& value, int role
) -> bool
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

auto BookmarkItemModelTree::flags(const QModelIndex& index) const -> Qt::ItemFlags
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
) -> quintptr
{
    auto id = reinterpret_cast<quintptr>(bookmarkNode.get());
    auto& itemModelNode = m_nodeMap[id];

    const size_t childrenSize = bookmarkNode->GetChildrenSize();
    itemModelNode.children.reserve(childrenSize);

    for (size_t i = 0; i < childrenSize; ++i)
    {
        auto childNode = bookmarkNode->GetChild(i);
        itemModelNode.children.push_back(MakeItemModelNode(std::move(childNode)));
    }
    itemModelNode.bookmarkNode = std::move(bookmarkNode);

    return id;
}

auto BookmarkItemModelTree::GetItemModelNode(const QModelIndex& index) const
    -> const ItemModelNode*
{
    const auto nodeIt = m_nodeMap.find(index.internalId());
    if (nodeIt == m_nodeMap.end())
        return m_rootNode.get();
    else
        return &nodeIt->second;
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

    const auto internalId = ToInternalId(bookmarkNode);
    const auto childIt = std::ranges::find_if(
        parentItemModelNode->children,
        [internalId](quintptr child) { return child == internalId; }
    );
    if (childIt == parentItemModelNode->children.end())
        return {nullptr, -1};

    const auto nodeIt = m_nodeMap.find(internalId);
    if (nodeIt == m_nodeMap.end())
        return {nullptr, -1};

    const auto row =
        static_cast<int>(std::distance(parentItemModelNode->children.begin(), childIt));
    return {&(nodeIt->second), row};
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
    m_nodeMap.erase(ToInternalId(param.child));
    endRemoveRows();
}

} // namespace ctrl
