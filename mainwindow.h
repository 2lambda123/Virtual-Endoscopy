#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QtGui>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <QWidget>
#include <QSplitter>

#include "modules/centerlineproc.h"
#include "modules/showutility.h"
#include "modules/displayutils.h"
#include "modules/globalviewutils.h"
#include "tools/initials.h"
#include "tools/fastdef.h"

#include <fstream>
#include <vector>
#include <string>

# include <vtkRenderer.h>
# include <vtkSTLReader.h>
# include <vtkOrientationMarkerWidget.h>
# include <vtkAxesActor.h>
# include <vtkInteractorObserver.h>
# include <vtkCamera.h>

#include <vtkMetaImageReader.h>
#include <vtkVolume16Reader.h>
#include <vtkNew.h>
#include <vtkNrrdReader.h>

#include <vtkEventQtSlotConnect.h>
//forward declaration
class QMenu;
class QAction;
class QRadioButton;
class QHBoxLayout;
class QPixmap;
class QElapsedTimer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVTKWidget *m_qvtk;

    vtkEventQtSlotConnect *Connections;
    QMenu *popup2;

    displayUtils *m_mainShowUtil;

//    showutility *leftshowUtil;
//    centerLineProc *clnproc;

    QVTKWidget *m_smallqvtk;
    GlobalViewUtils * m_assistShowUtil;

    std::vector<OutputImageType::IndexType> localMaxVec;

    QAction *openSTLAct;
    QAction *exitAct;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *aboutAct;

    QMenu *procMenu;
    QAction *clnAct;

    QMenu *viewMenu;
    QAction *viewdockAct;
    QMenu *renderingMenu;
    QAction *volRenderAct;
    QAction *sufRenderAct;

    QMenu *testMenu;
    QAction *testAct;

    vtkRenderer * renderer;
    vtkAxesActor * axes;
    vtkOrientationMarkerWidget * axesWidget;
    void initLayout();
    void initActions();
    void initMenus();
    void initModegroup();
    void initDockContents();
    void initModules();
    void initPopup();
    void PreRenderWindow();
    void UpdateRenderWindow();

    void initSmallLayout();

    void loadSeriesImg();

private slots:

    void popup(vtkObject * obj, unsigned long,
                 void * client_data, void *,
                 vtkCommand * command);
    void color2(QAction*);
    void exit()
    {
        QApplication::quit();
    }
    void about()
    {

        QMessageBox message(QMessageBox::NoIcon,tr("information about VirtualEndo"),
                            tr("A test platform for medical visualization \
                                                          copyright by NiuMing \
                                                                    \
                                                contact by mingknight@sjtu.edu.cn"));
        message.setIconPixmap(QPixmap("demo.png"));
        message.exec();
    }
    void processImg();
    void openSTL();
    void viewdockpanel(bool checked);
    void testModule();
    void volumerendering();
    void surfacerendering();

    void on_LineCbx_toggled();
    void on_ModelCbx_toggled();
    //display mode
    void on_PointModeBtn_toggled();
    void on_LineModeBtn_toggled();
    void on_FrameModeBtn_toggled();
    //roaming setting
    void on_PointPickerCbx_toggled(bool checked);
    void on_StartBtn_clicked();
    void on_PreviousBtn_clicked();
    void on_NextBtn_clicked();
};

#endif // MAINWINDOW_H
