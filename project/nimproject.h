#pragma once

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include <QFileSystemWatcher>
#include <QElapsedTimer>
#include <QTimer>

namespace TextEditor { class TextDocument; }

namespace NimPlugin {

class NimProjectManager;
class NimProjectNode;

class NimProject Q_DECL_FINAL : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    NimProject(NimProjectManager *projectManager, const QString &fileName);

    QString displayName() const Q_DECL_OVERRIDE;
    ProjectExplorer::IProjectManager *projectManager() const Q_DECL_OVERRIDE;
    ProjectExplorer::ProjectNode *rootProjectNode() const Q_DECL_OVERRIDE;
    QStringList files(FilesMode) const Q_DECL_OVERRIDE;
    bool needsConfiguration() const Q_DECL_OVERRIDE;
    Utils::FileName path() const;
    bool supportsKit(ProjectExplorer::Kit *k, QString *errorMessage) const Q_DECL_OVERRIDE;
    Utils::FileNameList nimFiles() const;

private slots:
    void scheduleProjectScan();
    void populateProject();

private:
    void recursiveScanDirectory(const QDir &dir, QSet<QString> &container);
    void addNodes(const QSet<QString> &nodes);
    void removeNodes(const QSet<QString> &nodes);

    ProjectExplorer::FolderNode *findFolderFor(const QStringList &path);

    NimProjectManager *m_projectManager;
    NimProjectNode *m_rootNode;

    QDir m_projectDir;
    QSet<QString> m_files;
    QFileSystemWatcher m_fsWatcher;

    QElapsedTimer m_lastProjectScan;
    QTimer m_projectScanTimer;
};

}
