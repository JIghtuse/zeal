#ifndef ZEALDOCSETSREGISTRY_H
#define ZEALDOCSETSREGISTRY_H

#include <QDir>
#include <QIcon>
#include <QMap>
#include <QMutex>
#include <QSqlDatabase>

#include "zealsearchresult.h"
#include "zealdocsetinfo.h"
#include "zealdocsetmetadata.h"

namespace Zeal {

enum DocsetType {
    ZEAL, DASH, ZDASH
};

class DocsetsRegistry : public QObject
{
    Q_OBJECT
public:
    struct DocsetEntry {
        QString name;
        QString prefix;
        QSqlDatabase db;
        QDir dir;
        DocsetType type;
        DocsetMetadata metadata;
        DocsetInfo info;
    };

    static DocsetsRegistry *instance();

    int count() const;
    QStringList names() const;
    void remove(const QString &name);
    void clear();

    QSqlDatabase &db(const QString &name);
    const QDir &dir(const QString &name);
    const DocsetMetadata &meta(const QString &name);
    QIcon icon(const QString &docsetName) const;
    DocsetType type(const QString &name) const;

    DocsetEntry *entry(const QString &name);
    // Returns the list of links available in a given webpage.
    // Scans the list of related links for a given page. This lets you view the methods of a given object.
    QList<SearchResult> relatedLinks(const QString &name, const QString &path);
    QString prepareQuery(const QString &rawQuery);
    void runQuery(const QString &query);
    void invalidateQueries();
    const QList<SearchResult> &queryResults();
    QList<DocsetEntry> docsets();

    QString docsetsDir() const;
    void initialiseDocsets();

public slots:
    void addDocset(const QString &path);

signals:
    void queryCompleted();

private slots:
    void _runQuery(const QString &query, int queryNum);

private:
    DocsetsRegistry();
    Q_DISABLE_COPY(DocsetsRegistry)

    void addDocsetsFromFolder(const QDir &folder);
    void normalizeName(QString &itemName, QString &parentName, const QString &initialParent);

    static DocsetsRegistry *m_instance;
    QMap<QString, DocsetEntry> m_docs;
    QList<SearchResult> m_queryResults;
    int m_lastQuery = -1;
};

} // namespace Zeal

#endif // ZEALDOCSETSREGISTRY_H
