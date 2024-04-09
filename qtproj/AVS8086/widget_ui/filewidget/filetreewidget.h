#ifndef FILETREEWIDGET_H
#define FILETREEWIDGET_H

#include <QTreeWidget>
#include <QMenuBar>
#include <QDir>
#include "buildwidget.h"

namespace Ui {
class filewidget;
}
class QFile;

class FileTreewidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit FileTreewidget(QWidget *parent = nullptr);
    ~FileTreewidget();

    void FindFile(QDir* path, QTreeWidgetItem* parent);
    void buttonCollapseItem();
    bool setRootPath(QString* path);

    void addFile();
    void closeProject();
    void deleteFile();
    void reName();//重命名
    void deleteDir();
    void removeFile();
    void removeDir();

public slots:
    void bulidNewFile(QString name, QString path, QString suffix);
    void newProject(QString pathName);
    void createAsmCheckedSlots();
    // void

signals:
    void buildFile();//action
    void buildFolder();;
    // void newFile();//按钮
    void showTextSignal(QString path,QString name,QTreeWidgetItem *item);
    void collapseItemSignal(const QTreeWidgetItem *item);
    void renameSignal();
    void openFileSignals(QString path);
    void clickedFilePath(QTreeWidgetItem *item);
    void saveFile(QString path);
    void reNameSignals();
    void nowFilePath(QString path);

private:
    Ui::filewidget *ui;

    QTreeWidgetItem* root;
    QMenu* dirMenu;
    QMenu* fileMenu;
    QMenu * rootMenu;
    QMenuBar* menubar;

    QTreeWidgetItem* nowItem;
    int nowCol;

    BuildWidget* buildFileWidget;
    BuildWidget* buildDirWidget;
    BuildWidget* reNameWidget;

    QLineEdit* fileEdit;
    QLineEdit* dirEdit;
    QLineEdit* reNameEdit;
    QWidget* showWidget;

    QString preName;
    QString prePath;

    bool isChecked;

    void CreateTopItem(QString* path);
    void setBuildwidget();
    QTreeWidgetItem* findTargetItem(QTreeWidgetItem* item, QString target);
    void m_mkdir(QString path, QString name);

private slots:
    void DoubleClickpath(QTreeWidgetItem *item, int column);
    void itemPressedSlot(QTreeWidgetItem * pressedItem, int column);
    void tempActionInformation(QAction *act);
    void bulidNewFileSlot();
//    void closeBuildFileSlot();
    void buildNewDirSlot();
//    void cancelDirSlot();
//    void openFileDialog();
//    void nameChanged(QTreeWidgetItem*  );

    // QAbstractItemView interface

    // QObject interface
};

#endif // FILETREEWIDGET_H
