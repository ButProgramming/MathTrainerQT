#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QDebug>
#include <QTimer>
#include <QQueue>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tmr=new QTimer(this);

    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->addGraph();

    //SIGNALS
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime()));


    // Create DB connection
    con = QSqlDatabase::addDatabase("QSQLITE");
    con.setDatabaseName("mathTrainer.db");
    if(!con.open())
    {
        QMessageBox mb;
        mb.setText("DB is not opened");
        mb.exec();
        return;
    }
    sqlDB = QSqlQuery(con);

    // DB create
    sqlDB.exec(
                "CREATE TABLE players ("
                    "ID	INTEGER NOT NULL UNIQUE,"
                    "name	TEXT UNIQUE,"
                    "PRIMARY KEY(ID AUTOINCREMENT));"
                );

    sqlDB.exec(
               "CREATE TABLE results ("
                    "ID	INTEGER NOT NULL,"
                    "level	TEXT NOT NULL,"
                    "game5	INTEGER,"
                    "game4	INTEGER,"
                    "game3	INTEGER,"
                    "game2	INTEGER,"
                    "game1	INTEGER,"
                    "FOREIGN KEY(ID) REFERENCES players(ID) ON DELETE NO ACTION ON UPDATE NO ACTION);"
                );

    // Set default stackedWidget
    ui->stackedWidget->setCurrentIndex(0);

    //Create items into the QListWidgets
    QListWidgetItem *item1 = new QListWidgetItem("Easy");
    item1->setTextAlignment(Qt::AlignCenter);
    ui->ListLevels->addItem(item1);
    QListWidgetItem *item2 = new QListWidgetItem("Normal");
    item2->setTextAlignment(Qt::AlignCenter);
    ui->ListLevels->addItem(item2);
    QListWidgetItem *item3 = new QListWidgetItem("Hard");
    item3->setTextAlignment(Qt::AlignCenter);
    ui->ListLevels->addItem(item3);
    ui->ListLevels->setCurrentRow(0);
    ui->ListLevels->setAcceptDrops(0);


    ui->label->setWordWrap(true);

    // Start value of lcdNumber
    ui->lcdNumber->display(0);

    // get the palette
    auto palette = ui->lcdNumber->palette();
    ui->lcdNumber->setPalette(Qt::black);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{

    if(ui->stackedWidget->currentIndex()==0){
        if(e->key()==Qt::Key_Return) {

            on_ButtonStart_clicked();
        }
    }
    else if(ui->stackedWidget->currentIndex()==1){
        if(e->key()==Qt::Key_Return) {
            if(isPlaying){
                on_CheckButton_clicked();
            }
            else on_PlayButton_clicked();
        }
    }
    else if(ui->stackedWidget->currentIndex()==2){
        on_Start3_clicked();
    }

}

bool MainWindow::getID(QString name)
{

    sqlDB.exec("SELECT * FROM players");

    while(sqlDB.next())
    {
        qDebug() << sqlDB.value(1).toString();// << " " <<name;
        if(sqlDB.value(1).toString()==name)
        {
            playerID=sqlDB.value(0).toString();
            qDebug()<<"playerID: "<<playerID;
            return true;
        }
    }
     return false;
}

void MainWindow::easyGame()
{
    isEasy=true;
    isNormal=false;
    isHard=false;
    ui->lineAnswer->setText("");
    ui->lineAnswer->setFocus();
    level=0;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(ETS);
    ui->progressBar->setValue(ETS);
    tmr->start(1);
}

void MainWindow::easyGameGenerate()
{
    ui->labelLevel->setText("Your level: "+QString::number(level));
    gen1 = QRandomGenerator::global()->bounded(0,10*(level+1));
    gen2 = QRandomGenerator::global()->bounded(0,10*(level+1));
    qDebug()<<gen1<<" "<<gen2;
    ui->term->setText(QString::number(gen1)+" + "+QString::number(gen2));
}

void MainWindow::normalGame()
{
    isEasy=false;
    isNormal=true;
    isHard=false;
    ui->lineAnswer->setText("");
    ui->lineAnswer->setFocus();
    level=0;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(NTS);
    ui->progressBar->setValue(NTS);
    tmr->start(1);
}

void MainWindow::normalGameGenerate()
{
    ui->labelLevel->setText("Your level: "+QString::number(level));
    gen1 = QRandomGenerator::global()->bounded(0,5*(level+1));
    gen2 = QRandomGenerator::global()->bounded(0,5*(level+1));
    gen3 = QRandomGenerator::global()->bounded(2,3*(level+1));
    gen4 = QRandomGenerator::global()->bounded(2,3*(level+1));
    int gen5=gen3*gen4;
    qDebug()<<"Normal: "<<(gen1+gen2)*gen3*gen4/gen4;
    ui->term->setText("("+QString::number(gen1)+" + "+QString::number(gen2)+") * "+QString::number(gen5)+"/"+QString::number(gen4)); // (a+b)*c/d

}

void MainWindow::hardGame()
{
    isEasy=false;
    isNormal=false;
    isHard=true;
    ui->lineAnswer->setText("");
    ui->lineAnswer->setFocus();
    level=0;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(HTS);
    ui->progressBar->setValue(HTS);
    tmr->start(1);
}

void MainWindow::hardGameGenerate()
{
    ui->labelLevel->setText("Your level: "+QString::number(level));
    gen1 = QRandomGenerator::global()->bounded(0,5*(level+1));
    gen2 = QRandomGenerator::global()->bounded(0,5*(level+1));
    gen3 = QRandomGenerator::global()->bounded(2,3*(level+1));
    gen4 = QRandomGenerator::global()->bounded(2,3*(level+1));
    qDebug()<<"3!";
    gen6 = QRandomGenerator::global()->bounded(1,3*(level+1));//e
    qDebug()<<"4!";
    gen7 = QRandomGenerator::global()->bounded(1,3*(level+1));//f

    qDebug()<<"Hard : "<<(gen1+gen2)*gen3*gen4/gen4;

    while (true)
    {
        gen5 = QRandomGenerator::global()->bounded(2,60*(level+1));//d//b
        if (gen5%(gen6+gen7)==0)
            break;
        qDebug()<<"while2: "<<gen5%(gen6+gen7);
    }

    ui->term->setText("("+QString::number(gen1)+" + "+QString::number(gen2)+") * "+QString::number(gen3*gen4)+"/"+QString::number(gen4)+" + "+QString::number(gen4)+"/("+QString::number(gen5)+" + "+QString::number(gen6)+")");
    qDebug()<<"answer hard: "<<((gen1+gen2)*gen3)+(gen5/(gen6+gen7));
}

void MainWindow::on_ButtonStart_clicked()
{
    QString name=ui->InputName->text();
    ui->labelLevel->setText("Your level: "+QString::number(level));
    if (ui->InputName->text().isEmpty())
            ui->labelWarning->setText("Input your name please");
    else if (getID(name))
    {
        initializeVectors(playerID);
        //qDebug()<<ui->InputName->text();
        ui->stackedWidget->setCurrentIndex(1);
        ui->label->setText("Hello, "+(ui->InputName->text())+"!"/*+"!\n Choose your level \nand have fun playing!"*/);
    }
    else
    {
        createPlayer();
        initializeVectors(playerID);
    }
}

void MainWindow::on_ButtonBest_Score_clicked()
{
    namePlayer=ui->InputName->text();
    if (ui->InputName->text().isEmpty())
            ui->labelWarning->setText("Input your name please");
    else if (getID(namePlayer))
    {
     // qDebug()<<"Clicked on Best Score: "<<playerID;
     // qDebug()<<ui->InputName->text();

        initializeVectors(playerID);
        testfunk();
        ui->label->setText("Hello, "+(ui->InputName->text())/*+g"!\n Choose your level \nand have fun playing!"*/);
        ui->stackedWidget->setCurrentIndex(2);
        ui->StatName->setText("Player: "+ui->InputName->text());
    }
    else
    {
        createPlayer();
        getID(ui->InputName->text());

        testfunk();
        ui->label->setText("Hello, "+(ui->InputName->text()));
        ui->stackedWidget->setCurrentIndex(2);
        initializeVectors(playerID);
        ui->StatName->setText("Player: "+ui->InputName->text());
    }

}

void MainWindow::on_PlayButton_clicked()
{
    if (!isPlaying)
    {
        //level
        isPlaying=true;
        if (ui->ListLevels->currentRow()==0)
        {
            timer=ETS;

            ui->lcdNumber->display(timer/1000);
            easyGame();
            easyGameGenerate();
        }
        else if (ui->ListLevels->currentRow()==1)
        {
            timer=NTS;
            ui->lcdNumber->display(timer);
            normalGame();
            normalGameGenerate();
        }
        else if (ui->ListLevels->currentRow()==2)
        {
            timer=HTS;
            ui->lcdNumber->display(timer);
            hardGame();
            hardGameGenerate();
        }
    }
}

void MainWindow::on_CheckButton_clicked()
{
    //qDebug()<<"checkButton";
    QString Qanswer = ui->lineAnswer->text();
    int Ianswer=Qanswer.toInt();
    if (isEasy) {
        if (Ianswer==gen1+gen2)
        {
            level++;
            ui->lineAnswer->setText("");
            ui->lineAnswer->setFocus();
            timer+=5000;
            if (timer>ETS)
                ui->progressBar->setMaximum(timer);
            ui->progressBar->setValue(timer);
            ui->lcdNumber->display(timer);
            easyGameGenerate();
        }

    }
    else if (isNormal) {
        if (Ianswer==(gen1+gen2)*gen3)
        {
            //qDebug()<<"isNormal";
            level++;
            //ui->labelLevel->setText(QString::number(level));
            ui->lineAnswer->setText("");
            ui->lineAnswer->setFocus();
            timer+=7000;
            if (timer>NTS)
                ui->progressBar->setMaximum(timer);
            ui->progressBar->setValue(timer);
            ui->lcdNumber->display(timer);
            normalGameGenerate();
           }

    }
    else {
        if (Ianswer==((gen1+gen2)*gen3)+(gen5/(gen6+gen7)))
        {
            //qDebug()<<"isHard: "<<(gen1+gen2)/3+gen4/(gen5+gen6);
            level++;
            ui->lineAnswer->setText("");
            ui->lineAnswer->setFocus();
            timer+=12000;
            if (timer>NTS)
                ui->progressBar->setMaximum(timer);
            ui->progressBar->setValue(timer);
            ui->lcdNumber->display(timer);
            hardGameGenerate();
           }
    }


}

void MainWindow::updateTime()
{
    if(isEasy || isNormal ||isHard)
    {
        timer--;

        ui->progressBar->setValue(timer);
        ui->lcdNumber->display(timer/1000);
        //Debug()<<timer;
        if (!timer)
        {
            tmr->stop();
            isPlaying=false;
            timer=ETS;
            ui->progressBar->setMaximum(timer);
            ui->progressBar->setValue(timer);
            ui->lcdNumber->display(timer/1000);
            ui->term->setText("");
            ui->lineAnswer->setText("");
            if (isEasy)
            {
                VEasy.erase(VEasy.begin());
                qDebug()<<"level is: "<<level;
                VEasy.push_back(level);
                sqlDB.exec( "update results set "
                            "game1="+QString::number(VEasy[4])+", "
                            "game2="+QString::number(VEasy[3])+", "
                            "game3="+QString::number(VEasy[2])+", "
                            "game4="+QString::number(VEasy[1])+", "
                            "game5="+QString::number(VEasy[0])+" "
                            "where level='easy' and ID="+playerID+""
                          );
            }
            else if (isNormal)
            {
                VNormal.erase(VNormal.begin());
                qDebug()<<"level is: "<<level;
                VNormal.push_back(level);
                sqlDB.exec( "update results set "
                            "game1="+QString::number(VNormal[4])+", "
                            "game2="+QString::number(VNormal[3])+", "
                            "game3="+QString::number(VNormal[2])+", "
                            "game4="+QString::number(VNormal[1])+", "
                            "game5="+QString::number(VNormal[0])+" "
                            "where level='normal' and ID="+playerID+""
                          );
            }
            else //if (isHard)
            {
                VHard.erase(VHard.begin());
                qDebug()<<"level is: "<<level;
                VHard.push_back(level);
                sqlDB.exec( "update results set "
                            "game1="+QString::number(VHard[4])+", "
                            "game2="+QString::number(VHard[3])+", "
                            "game3="+QString::number(VHard[2])+", "
                            "game4="+QString::number(VHard[1])+", "
                            "game5="+QString::number(VHard[0])+" "
                            "where level='hard' and ID="+playerID+""
                          );
            }
            qDebug()<<endl;
            level=0;
            ui->labelLevel->setText("Your level: "+QString::number(level));
            qDebug()<<"In updatetime()";
        }
    }
}



void MainWindow::initializeVectors(QString playerID)
{


    //easy
    sqlDB.exec("SELECT * FROM results where level='easy' and ID="+playerID+"");
    while(sqlDB.next())
    {
        for (int i=2; i<=6; i++)
            VEasy.push_back(sqlDB.value(i).toInt());
    }
    qDebug()<<"in initializeVectors function";
    for (double d: VEasy)
        qDebug()<<"VEasy: "<<d;

    //normal
    sqlDB.exec("SELECT * FROM results where level='normal' and ID="+playerID+"");
    while(sqlDB.next())
    {
        for (int i=2; i<=6; i++)
            VNormal.push_back(sqlDB.value(i).toInt());
    }

    //hard
    sqlDB.exec("SELECT * FROM results where level='hard' and ID="+playerID+"");
    while(sqlDB.next())
    {
        for (int i=2; i<=6; i++)
            VHard.push_back(sqlDB.value(i).toInt());
    }


}

void MainWindow::on_ButtonBest_Score2_clicked()
{
    testfunk();
    ui->stackedWidget->setCurrentIndex(2);
    ui->StatName->setText("Player: "+ui->InputName->text());
}

void MainWindow::on_Start3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::testfunk()
{
    ui->plot->replot();
    //qDebug()<<"In testfunk()";
    QVector<double> x(5);
    for (int i=0; i<5;i++)
    {
        x[i]=i+1;
    }

    qDebug()<<*std::max_element(VEasy.begin(),VEasy.end());
    auto itE = *std::max_element(VEasy.begin(),VEasy.end());
    auto itN = *std::max_element(VNormal.begin(),VNormal.end());
    auto itH = *std::max_element(VHard.begin(),VHard.end());
    //auto itMax;
    qDebug()<<"itE: "<<itE<<" itN: "<<itN<<" itH :"<<itH;
    QVector <double> VMax = {itE, itN, itH};
    auto itMax = *std::max_element(VMax.begin(), VMax.end());
    qDebug()<<itMax<<endl;
    ui->plot->xAxis->setRange(1,5);
    ui->plot->yAxis->setRange(0,itMax*1.5);

    //easy
    ui->plot->graph(0)->setData(x, VEasy);
    ui->plot->graph(0)->setName("Easy");
    ui->plot->replot();
    ui->plot->update();

    //normal
    ui->plot->graph(1)->setPen(QPen( Qt::darkYellow));
    ui->plot->graph(1)->setData(x, VNormal);
    ui->plot->graph(1)->setName("Normal");
    ui->plot->replot();
    ui->plot->update();

    //hard
    ui->plot->graph(2)->setPen(QPen(Qt::red));
    ui->plot->graph(2)->setData(x, VHard);
    ui->plot->graph(2)->setName("Hard");
    ui->plot->replot();
    ui->plot->update();

    //legend is on
    ui->plot->legend->setVisible(true);


}


void MainWindow::createPlayer() {
    sqlDB.exec("INSERT INTO players (name) VALUES ('"+(ui->InputName->text())+"')");
    getID(ui->InputName->text());
    sqlDB.exec("INSERT INTO results VALUES ('"+playerID+"', 'easy', 0, 0, 0, 0, 0 )");
    sqlDB.exec("INSERT INTO results VALUES ('"+playerID+"', 'normal', 0, 0, 0, 0, 0 )");
    sqlDB.exec("INSERT INTO results VALUES ('"+playerID+"', 'hard', 0, 0, 0, 0, 0 )");
    ui->stackedWidget->setCurrentIndex(1);
    ui->label->setText("Hello, "+(ui->InputName->text()));
}
