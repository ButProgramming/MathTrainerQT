#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define ETS 10000  //start value in milliseconds for easygame
#define NTS 10000  //start value for in milliseconds normalgame
#define HTS 10000  //start value for in milliseconds hardgame (hard timer start)
#include <QMainWindow>
#include <QtSql/QtSql>
#include <QRandomGenerator>
#include <algorithm>
#include <QSet>
#include <QtCharts>
#include <QChartView>
#include <QLine>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ButtonStart_clicked();
    void on_ButtonBest_Score_clicked();
    void on_PlayButton_clicked();
    void on_CheckButton_clicked();
    void updateTime();
    void on_ButtonBest_Score2_clicked();
    void on_Start3_clicked();
    void keyPressEvent(QKeyEvent*);

private:
    Ui::MainWindow *ui;
    QSqlDatabase con;
    QSqlQuery sqlDB;
    int gen1, gen2, gen3, gen4, gen5, gen6, gen7;
    int timer=0;
    QTimer *tmr;
    int easyLvl=1;
    bool isPlaying=false;
    QString playerID;
    void testfunk();
    int level=0;
    QVector<double> VEasy;
    QVector<double> VNormal;
    QVector<double> VHard;
    void initializeVectors(QString); // use results from DB to initialize Vectors
    bool isEasy=false;
    bool isNormal=false;
    bool isHard=false;
    QLineSeries *series = new QLineSeries();
    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);
    QString namePlayer;

    void createPlayer();
    bool getID(QString);
    void easyGameGenerate();
    void easyGame();
    void normalGameGenerate();
    void normalGame();
    void hardGameGenerate();
    void hardGame();


};
#endif // MAINWINDOW_H
