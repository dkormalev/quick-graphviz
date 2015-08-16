#include "graphnode.h"

using namespace QuickGraphviz;

GraphNode::GraphNode(const QString &nodeId, const QString &text)
    : QObject(0), m_nodeId(nodeId), m_text(text)
{
    if (m_text.isEmpty())
        m_text = m_nodeId;
}

QString GraphNode::nodeId() const
{
    return m_nodeId;
}

void GraphNode::setNodeId(QString nodeId)
{
    if (m_nodeId == nodeId)
        return;
    m_nodeId = nodeId;
    emit nodeIdChanged(nodeId);
}

QString GraphNode::text() const
{
    return m_text;
}

void GraphNode::setText(QString text)
{
    if (m_text == text)
        return;
    m_text = text;
    emit textChanged(text);
}
