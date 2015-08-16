#ifndef QUICKGRAPHVIZ_GRAPHNODE_H
#define QUICKGRAPHVIZ_GRAPHNODE_H

#include <QObject>

namespace QuickGraphviz {

class GraphNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nodeId READ nodeId WRITE setNodeId NOTIFY nodeIdChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    explicit GraphNode(const QString &nodeId, const QString &text = QString());

    QString nodeId() const;
    void setNodeId(QString nodeId);
    QString text() const;
    void setText(QString text);


signals:
    void nodeIdChanged(QString nodeId);
    void textChanged(QString text);

private:
    QString m_nodeId;
    QString m_text;

};

} // namespace QuickGraphviz

#endif // QUICKGRAPHVIZ_GRAPHNODE_H
