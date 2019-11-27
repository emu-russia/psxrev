#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QTimer>

class QSplitter;
class GraphicsScene;
class View;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = 0 );
    virtual ~MainWindow();

private  slots:
    void Open();
    void SaveAs();
    void CircuitUpdate();

    void InsertContainer();
    void InsertPin();
    void InsertGround();
    void InsertPower();
    void InsertNfet();
    void InsertPfet();

private:
    QSplitter* m_Splitter;
    QMenu* m_FileMenu;
    QToolBar* m_ToolBar;
    QAction* m_OpenAct;
    QAction* m_SaveAsAct;
    QAction* m_ExitAct;
    View* m_View;

    QTimer m_CircuitTimer;

    GraphicsScene* m_Scene;
};



#endif // MAINWINDOW_H
