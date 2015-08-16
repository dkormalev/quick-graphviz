#ifndef QUICKGRAPHVIZ_GRAPH_H
#define QUICKGRAPHVIZ_GRAPH_H

#include <QObject>
#include <QSet>
#include <QHash>
#include <QMutex>

namespace QuickGraphviz {

class GraphNode;
class GraphEdge;
//TODO: add subgraphs
class Graph : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit Graph(QObject *parent = 0);

    void addNode(GraphNode *node);
    void addEdge(GraphEdge *edge);

    QSet<GraphEdge *> edges() const;
    QSet<GraphNode *> nodes() const;

    Q_INVOKABLE void buildGraph();

    QString name() const;
    void setName(QString name);

signals:
    void graphBuilt(const QImage &graphImage);
    void nameChanged(QString name);

private:
    QSet<GraphEdge *> m_edges;
    QHash<QString, GraphNode *> m_nodes;
    QMutex m_mutex;
    QString m_name;
};

} // namespace QuickGraphviz

#endif // QUICKGRAPHVIZ_GRAPH_H
