#include "graphedge.h"

using namespace QuickGraphviz;

GraphEdge::GraphEdge(const QString &sourceId, const QString &targetId)
    : QObject(0), m_sourceId(sourceId), m_targetId(targetId)
{
}

QString GraphEdge::sourceId() const
{
    return m_sourceId;
}

QString GraphEdge::targetId() const
{
    return m_targetId;
}
