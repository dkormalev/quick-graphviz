#ifndef QUICKGRAPHVIZ_GRAPHEDGE_H
#define QUICKGRAPHVIZ_GRAPHEDGE_H

#include <QObject>

namespace QuickGraphviz {

class GraphNode;
class GraphEdge : public QObject
{
    Q_OBJECT
public:
    explicit GraphEdge(const QString &sourceId, const QString &targetId);

    QString sourceId() const;
    QString targetId() const;

private:
    QString m_sourceId;
    QString m_targetId;
};

} // namespace QuickGraphviz

#endif // QUICKGRAPHVIZ_GRAPHEDGE_H
