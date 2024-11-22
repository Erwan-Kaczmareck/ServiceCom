/********************************************************************
    created:	2022-01-06
    last update: 2022-04-23
    file path:	mainwindow.cpp
    author:		Lucas Journoud
    copyright:	W.I.P

    purpose: 	file containing all the features:
    - Possibility to display the different services and their activities.
    - Possibility to launch a service.
    - Administrator interface allowing to modify the database associated to the service and activity

    todo:
    - increases the size of QLabel, QLineEdit and QPushButton
    - change the icon of the application
    - ctrl + f: W.I.P
*********************************************************************/

// s -> string, qstring
// n -> nombre entier
// b -> booléen
// p -> pointer
// m_ -> variable membre
// préfixe _in -> rajouter aux paramètres des fonctions
// préfixe _out -> rajouter aux variable retourner par une fonction
// camelCase utilisé

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow Main window constructor
 * \param parent Parent of the window
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    p_Base_De_Donnee = new CBase_De_Donnee();
    p_Son = new CSon();
    InitialConfiguration();
    QString Qs_mainWindow = "MainWindow";

    ui->tableWidgetServicesAdmin->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetServicesAdmin->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetServicesStatistique->setSelectionBehavior(QAbstractItemView::SelectColumns);

    //Preparation of the database
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,Qs_mainWindow);
    QString sQuery1 = QString("SELECT nomSon , duree , muteBool FROM tson");

    QSqlQuery query(sQuery1);
    query = p_Base_De_Donnee->Requete_Base(&db,Qs_mainWindow,query);
    while(query.next())
    {
        m_sLienSon = query.value(0).toString();
        m_iDureeSon = query.value(1).toInt();
        m_bMuteSon = query.value(2).toBool();
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");

    ui->pushButtonServiceProgress->setEnabled(false);
}

/*!
 * \brief MainWindow::~MainWindow Main window destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::TableView Initialize the 2 table views
 */
void MainWindow::TableView()
{
    QFont fixedFont;
    fixedFont.setPointSize(25);

    //TableWidgetServices
    ui->tableWidgetServices->setColumnCount(3);
    ui->tableWidgetServices->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetServices->setShowGrid(false);
    ui->tableWidgetServices->setColumnWidth(0, 1000);
    ui->tableWidgetServices->setColumnWidth(1, 400);
    ui->tableWidgetServices->setColumnWidth(2, 100);
    ui->tableWidgetServices->verticalScrollBar()->setStyleSheet("QScrollBar {background-color: #92d04f; width: 100px;}");
    ui->tableWidgetServices->horizontalScrollBar()->setStyleSheet("QScrollBar {background-color: #92d04f;}");

    ui->tableWidgetServices->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidgetServices->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //TableWidgetServicesAdmin
    ui->tableWidgetServicesAdmin->setColumnCount(4);
    ui->tableWidgetServicesAdmin->setFont(fixedFont);
    ui->tableWidgetServicesAdmin->horizontalScrollBar()->setStyleSheet("QScrollBar {background-color: #92d04f;}");
    ui->tableWidgetServicesAdmin->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetServicesAdmin->setShowGrid(true);
    ui->tableWidgetServicesAdmin->setColumnWidth(0, 700);
    ui->tableWidgetServicesAdmin->setColumnWidth(1, 300);
    ui->tableWidgetServicesAdmin->setColumnWidth(2, 250);
    ui->tableWidgetServicesAdmin->setColumnWidth(3, 250);

    ui->tableWidgetServicesAdmin->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidgetServicesAdmin->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //Create a new element and set its height
    QTableWidgetItem *pHeaderItem = new QTableWidgetItem("Nom de l'activité");
    QTableWidgetItem *pHeaderItem3 = new QTableWidgetItem("Temps répétitions");
    QTableWidgetItem *pHeaderItem4 = new QTableWidgetItem("Temps minimum");
    pHeaderItem->setSizeHint(QSize(0, 40));
    ui->tableWidgetServicesAdmin->setHorizontalHeaderItem(0, pHeaderItem);
    ui->tableWidgetServicesAdmin->setHorizontalHeaderItem(2, pHeaderItem3);
    ui->tableWidgetServicesAdmin->setHorizontalHeaderItem(3, pHeaderItem4);
    //TableWidgetServicesStatistique
    ui->tableWidgetServicesStatistique->setColumnCount(4);
    ui->tableWidgetServicesStatistique->setFont(fixedFont);
    ui->tableWidgetServicesStatistique->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetServicesStatistique->verticalScrollBar()->setStyleSheet("QScrollBar {background-color: #92d04f; width: 100px;}");
    ui->tableWidgetServicesStatistique->horizontalScrollBar()->setStyleSheet("QScrollBar {background-color: #92d04f;}");
    ui->tableWidgetServicesStatistique->setShowGrid(true);
    ui->tableWidgetServicesStatistique->setColumnWidth(0, 500);
    ui->tableWidgetServicesStatistique->setColumnWidth(1, 200);
    ui->tableWidgetServicesStatistique->setColumnWidth(2, 200);
    ui->tableWidgetServicesStatistique->setColumnWidth(3, 200);

    //ScrollBar
    ui->tableWidgetServicesStatistique->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableWidgetServicesStatistique->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

/*!
 * \brief MainWindow::ResizeRow Allows you to change the size of the lines in relation to the size of the content inside
 */
void MainWindow::ResizeRow()
{
    for(int nNumberOfRow=0; nNumberOfRow<ui->tableWidgetServices->rowCount(); nNumberOfRow++) //Browse each line
    {
        ui->tableWidgetServices->resizeRowToContents(nNumberOfRow);
        ui->tableWidgetServicesAdmin->resizeRowToContents(nNumberOfRow);
    }
    for(int nNumberOfRowStat=0; nNumberOfRowStat<ui->tableWidgetServicesStatistique->rowCount(); nNumberOfRowStat++)
    {
        ui->tableWidgetServicesStatistique->resizeRowToContents(nNumberOfRowStat);
    }
    ui->tableWidgetServicesStatistique->resizeColumnsToContents();
}

///
/// \brief MainWindow::ConfigComboBox Allows you to connect to a different service and recognise the service you have chosen
///
void MainWindow::ConfigComboBox()
{
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");

    QString nom_ConfigComboBox_in = "ConfigComboBox";

    p_Base_De_Donnee->Connection_Base(&db,nom_ConfigComboBox_in);

    QSqlQuery query("SELECT NameService FROM tservice");
    query = p_Base_De_Donnee->Requete_Base(&db,nom_ConfigComboBox_in,query);

    while (query.next())
    {
        ui->comboBoxServices->addItem(query.value(0).toString());
        ui->comboBoxServicesAdmin->addItem(query.value(0).toString());
        ui->comboBoxServicesStatistique->addItem(query.value(0).toString());
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
    return;
}

/*!
 * \brief MainWindow::CloseButtonConfiguration Allows you to configure the close button
 */
void MainWindow::CloseButtonConfiguration()
{
    ui->pushButtonClose->setIcon(QIcon(QPixmap(":/images/close.png")));
    ui->pushButtonClose->setIconSize(QSize(75,75));
    ui->pushButtonClose->setFlat(true);
    //Exit the application by pressing
    QObject::connect(ui->pushButtonClose, &QPushButton::clicked, &QApplication::quit);
}

/*!
 * \brief MainWindow::SettingsButtonConfiguration Allows you to configure the settings button
 */
void MainWindow::SettingsButtonConfiguration()
{
    ui->pushButtonSettings->setIcon(QIcon(QPixmap(":/images/settings.png")));
    ui->pushButtonSettings->setIconSize(QSize(75,75));
    ui->pushButtonSettings->setFlat(true);
    //Goes to the admin interface when you click on the button and enter the correct password
}

/*!
 * \brief HashingAndSalling Hasher and salt the password passed to it in parameter
 * \param sTextToHash Text to hasher
 * \param saltValue Salt to use
 * \return hashedPassword the asher password
 */
QByteArray MainWindow::HashingAndSalling(QString sTextToHash_in, quint32 saltValue_in)
{
    unsigned int saltUInt = static_cast<unsigned int>(saltValue_in);
    char salt = static_cast<char>(saltUInt);
    QByteArray passwordBytes = (sTextToHash_in+salt).toUtf8();
    QByteArray hashedPassword_out = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    return hashedPassword_out;
}

///
/// \brief MainWindow::ButtonTopConfiguration Configuring the button style to move the order upwards
///
void MainWindow::ButtonTopConfiguration()
{
    ui->pushButton_MoveTopActivity->setIcon(QIcon(QPixmap(":/images/arrow_top.png")));
    ui->pushButton_MoveTopActivity->setIconSize(QSize(70,70));
}

///
/// \brief MainWindow::ButtonBottomConfiguration Configuring the button style to move the order downwards
///
void MainWindow::ButtonBottomConfiguration()
{
    ui->pushButton_MoveBotActivity->setIcon(QIcon(QPixmap(":/images/arrow_bot.png")));
    ui->pushButton_MoveBotActivity->setIconSize(QSize(70,70));
}

/*!
 * \brief MainWindow::InitialConfiguration Function to set all elements to their initial configuration
 */
void MainWindow::InitialConfiguration()
{
    m_bInitialConfigurationIsDone=false;
    showFullScreen();
    m_bUpdate=true;
    m_bServiceIsStart=false;
    m_pValidatorName = new QRegularExpressionValidator(QRegularExpression("[A-Za-z0-9 éèêàâæçôöùûüïî!?.,]*"));
    ResetServices();
    CloseButtonConfiguration();
    SettingsButtonConfiguration();
    ButtonTopConfiguration();
    ButtonBottomConfiguration();
    TableView();

    UpdateServices(GetLastSelectedIndexService());
    m_pEngine = new QTextToSpeech;
    m_bServiceIsStart=false;
    ui->stackedWidget->setCurrentIndex(0); //show the MainWindow
    m_bInitialConfigurationIsDone=true;
}

/*!
 * \brief MainWindow::StartButtonConfiguration Function to configure the start button
 */
void MainWindow::StartButtonConfiguration()
{
    ui->pushButtonStartService->setText("Lancer");
    ui->pushButtonStartService->setStyleSheet("QPushButton {background-color: #92d04f; color: white;}");
}

/*!
 * \brief MainWindow::on_comboBoxServices_currentIndexChanged Function that calls the ComboBoxIndexChange function when a service is changed in the combobox
 * \param nIndex_in Index of the new service select
 */
void MainWindow::on_comboBoxServices_currentIndexChanged(int nIndex_in)
{
    ComboboxIndexChange(nIndex_in);
}

/*!
 * \brief MainWindow::on_comboBoxServicesAdmin_currentIndexChanged Function to call when changing service in the combobox and which allows to call the necessary functions to update the tableWidget
 * \param nIndex_in Index of the service that will be updated
 */
void MainWindow::on_comboBoxServicesAdmin_currentIndexChanged(int nIndex_in)
{
    ComboboxIndexChange(nIndex_in);
}

///
/// \brief MainWindow::on_comboBoxServicesStatistique_currentIndexChanged Function to call when changing service in the combobox and which allows to call the necessary functions to update the tableWidget
/// \param nIndex_in Index of the service that will be updated
///
void MainWindow::on_comboBoxServicesStatistique_currentIndexChanged(int nIndex_in)
{
    ComboboxIndexChange(nIndex_in);
}

/*!
 * \brief MainWindow::ComboboxIndexChange Function to call when changing service in the combobox and which allows to call the necessary functions to update the tableWidget
 * \param nIndex_in Index of the service that will be updated
 */
void MainWindow::ComboboxIndexChange(int nIndex_in)
{
    ui->comboBoxServicesAdmin->setCurrentIndex(nIndex_in);
    ui->comboBoxServices->setCurrentIndex(nIndex_in);
    ui->comboBoxServicesStatistique->setCurrentIndex(nIndex_in);
    if(m_bServiceIsStart) //If a service is started
    {
        StopServices();
        ResetServices();
    }
    if(m_bInitialConfigurationIsDone&&m_bUpdate) //If the initial configuration is completed and the update of the services enabled
    {
        UpdateActivity(nIndex_in+1);
        ChangeLastSelectedIndexService(nIndex_in);
    }
}

/*!
 * \brief MainWindow::UpdateActivity Allows to update (especially tableWidget) a service passed in parameter
 * \param IndexNombre_in The index of the service to be updated
 */
void MainWindow::UpdateActivity(int nIndexOfServices_in)
{
    QString nom_UpdateActivity_in = "UpdateActivity";
    ui->tableWidgetServices->setRowCount(0);
    ui->tableWidgetServicesAdmin->setRowCount(0);
    //Preparation of the database
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);

    p_Base_De_Donnee->Connection_Base(&db,nom_UpdateActivity_in);

    QSqlQuery query("SELECT * FROM titem WHERE fk_titem_tservice = "+QString::number(nIndexOfServices_in) + " ORDER BY OrdreItem ASC");

    query = p_Base_De_Donnee->Requete_Base(&db,nom_UpdateActivity_in,query);

    m_iNbActivite = 0;
    if(!m_qslActivite.empty())
    {
        m_qslActivite.clear();
        m_qlTime.clear();
        m_qlTimeReset.clear();
        m_qlTimeMin.clear();
        m_qlIndexTask.clear();
        m_qlIdItem.clear();
        m_qlTimeCompleted.clear();
    }
    //Display of results
    while (query.next())
    {
        ui->tableWidgetServices->setRowCount((ui->tableWidgetServices->rowCount())+1);
        ui->tableWidgetServicesAdmin->setRowCount((ui->tableWidgetServicesAdmin->rowCount())+1);
        ui->tableWidgetServices->setItem(query.value(3).toInt()-1,0,new QTableWidgetItem(query.value(1).toString())); //NomActiviter
        ui->tableWidgetServicesAdmin->setItem(query.value(3).toInt()-1,0,new QTableWidgetItem(query.value(1).toString())); //NomActiviter
        QString sTimeHoursMinutesSeconds = QString("%1h %2m %3s").arg(query.value(2).toInt()/3600).arg((query.value(2).toInt()%3600)/60).arg(query.value(2).toInt()%60);
        ui->tableWidgetServices->setItem(query.value(3).toInt()-1,1,new QTableWidgetItem(sTimeHoursMinutesSeconds)); //TempHeureMinuteSeconde
        ui->tableWidgetServicesAdmin->setItem(query.value(3).toInt()-1,1,new QTableWidgetItem(sTimeHoursMinutesSeconds)); //TempHeureMinuteSeconde
        ui->tableWidgetServices->setItem(query.value(3).toInt()-1,2,new QTableWidgetItem(QString::number(m_siRepetition)));
        QString sTimeResetHoursMinutesSeconds = QString("%1h %2m %3s").arg(query.value(5).toInt()/3600).arg((query.value(5).toInt()%3600)/60).arg(query.value(5).toInt()%60);
        ui->tableWidgetServicesAdmin->setItem(query.value(3).toInt()-1,2,new QTableWidgetItem(sTimeResetHoursMinutesSeconds)); //TempResetHeureMinuteSeconde

        QString sTimeMinHoursMinutesSeconds = QString("%1h %2m %3s").arg(query.value(6).toInt()/3600).arg((query.value(6).toInt()%3600)/60).arg(query.value(6).toInt()%60);
        ui->tableWidgetServicesAdmin->setItem(query.value(3).toInt()-1,3,new QTableWidgetItem(sTimeMinHoursMinutesSeconds)); //TempMinHeureMinuteSeconde

        if(query.value(2).toInt()/3600 >=1)
        {
            QString sTimeHoursMinutesSeconds = QString("%1h %2m %3s").arg(query.value(2).toInt()/3600).arg((query.value(2).toInt()%3600)/60).arg(query.value(2).toInt()%60);
            ui->tableWidgetServices->setItem(query.value(3).toInt()-1,1,new QTableWidgetItem(sTimeHoursMinutesSeconds)); //TempHeureMinuteSeconde
        }
        else
        {
            if(query.value(2).toInt()/60 >= 1)
            {
                QString sTimeHoursMinutesSeconds = QString("%1m %2s").arg((query.value(2).toInt()%3600)/60).arg(query.value(2).toInt()%60);
                ui->tableWidgetServices->setItem(query.value(3).toInt()-1,1,new QTableWidgetItem(sTimeHoursMinutesSeconds)); //TempHeureMinuteSeconde
            }
            else
            {
                QString sTimeHoursMinutesSeconds = QString("%1s").arg(query.value(2).toInt()%60);
                ui->tableWidgetServices->setItem(query.value(3).toInt()-1,1,new QTableWidgetItem(sTimeHoursMinutesSeconds)); //TempHeureMinuteSeconde
            }
        }

        //Récupération des données dans des variables pour le lancement du service
        m_qslActivite.append(query.value(1).toString());
        m_qlTime.append(query.value(2).toInt());
        m_qlTimeReset.append(query.value(5).toInt());
        m_qlTimeMin.append((query.value(6)).toInt());

        m_qlIndexTask.append(query.value(3).toInt());
        m_qlIdItem.append(query.value(0).toInt());

        m_iNbActivite++;
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
    //Resize the Row
    StatisticsDisplay();
    ResizeRow();
    return;
}

/*!
 * \brief MainWindow::StartServices Allows you to launch the service
 */
void MainWindow::StartServices()
{
    ui->pushButtonStartService->setStyleSheet("QPushButton {background-color: orange; color: white;}");
    ui->pushButtonStartService->setText("Continuer");
    m_pTimerNextSequence = new QTimer();
    m_pTimerSeconds = new QTimer();
    ui->tableWidgetServices->setRowCount(2);
    //When the service time is over
    m_pTimerNextSequence->connect(m_pTimerNextSequence, &QTimer::timeout, this, &MainWindow::ElapsedTime);

    //Every second
    m_pTimerSeconds->connect(m_pTimerSeconds, &QTimer::timeout, this, &MainWindow::Timer);
    //Launch timers
    m_pTimerNextSequence->start(0);
    m_pTimerSeconds->start(0);
}

/*!
 * \brief MainWindow::Timer Function call every second when the service is launched. Allows to decrement the timer
 */
void MainWindow::Timer()
{
    m_pTimerSeconds->setInterval(1000);
    m_nSecondsRemaining--;

    if(m_nSecondsRemaining % m_iRepeatActivitie == 0)
    {
        if(m_bRepeatActivitie==false)
        {
            m_bRepeatActivitie = true;
        }
        else
        {
            if(m_nSecondsRemaining >= 10)
            {
                m_pEngine->say(ui->tableWidgetServices->item(0,0)->text());
            }
        }
    }

    if(m_pTimerSeconds->isActive()&& m_nSecondsRemaining >=0)
    {
       //ui->tableWidgetServices->item(0,1)->setText(QString("%1h %2m %3s").arg(m_nSecondsRemaining/3600).arg((m_nSecondsRemaining%3600)/60).arg(m_nSecondsRemaining%60));
       if(m_nSecondsRemaining/3600 >=1)
       {
           ui->tableWidgetServices->item(0,1)->setText(QString("%1h %2m %3s").arg(m_nSecondsRemaining/3600).arg((m_nSecondsRemaining%3600)/60).arg(m_nSecondsRemaining%60));
       }
       else
       {
           if(m_nSecondsRemaining/60 >= 1)
           {
               ui->tableWidgetServices->item(0,1)->setText(QString(" %1m %2s").arg((m_nSecondsRemaining%3600)/60).arg(m_nSecondsRemaining%60));
           }
           else
           {
               ui->tableWidgetServices->item(0,1)->setText(QString(" %1s").arg(m_nSecondsRemaining%60));
           }
       }

       //Condition pour la visualisation du temps pour une activité
       if(m_bCheckRepetition == true){
           //Si on est dans le temps répétitions
           double tempTime = m_qlTimeReset.at(m_siNumeroActivite);
           double resultatTime = ((tempTime)-(m_nSecondsRemaining))/tempTime;
           QString styleSheet = QString("QPushButton {background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:%1, y2:0, stop:0 rgba(237, 28, 36, 255), stop:0.495 rgba(237, 28, 36, 255), stop:0.505 rgba(255, 165, 0, 255), stop:1 rgba(255, 165, 0, 255, 255));}")
                   .arg(resultatTime*2);
           ui->pushButtonStartService->setStyleSheet(styleSheet);
       }
       else {
           //Si on est dans le temps de l'activité
           double tempTime = m_qlTime.at(m_siNumeroActivite);
           double resultatTime = ((tempTime)-(m_nSecondsRemaining))/tempTime;
           QString styleSheet = QString("QPushButton {background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:%1, y2:0, stop:0 rgba(255, 165, 0, 255), stop:0.495 rgba(255, 165, 0, 255), stop:0.505 rgba(146,208,79, 255), stop:1 rgba(146,208,79, 255));}")
                   .arg(resultatTime*2);
           ui->pushButtonStartService->setStyleSheet(styleSheet);
       }
    }
}

/*!
 * \brief MainWindow::ElapsedTime Call function when the service time is over. Allows you to move to the next activity and turn your current line red
 */
void MainWindow::ElapsedTime() //If the time is over
{

    if(m_nService<=ui->tableWidgetServicesAdmin->rowCount())
    {
        m_bRepeatActivitie=false;
        if(m_nService!=0 && m_qlTimeReset.at(m_siNumeroActivite) != 0)
        {
            m_nSecondsRemaining = m_qlTimeReset.at(m_siNumeroActivite);
            m_pTimerNextSequence->setInterval(m_qlTimeReset.at(m_siNumeroActivite)*1000);
            ui->tableWidgetServices->item(0,1)->setText("0s");
            m_siRepetition++;
            ui->tableWidgetServices->item(0,2)->setText(QString::number(m_siRepetition));
            m_pEngine->say(ui->tableWidgetServices->item(0,0)->text());
            if(m_bMuteSon == false)
            {
                p_Son->JouerLeSon(m_sLienSon,m_iDureeSon);
            }
            m_bCheckRepetition = true;
        }
        else {
            if(m_nService >= m_qslActivite.size() && m_qlTimeReset.at(m_siNumeroActivite) == 0 && m_bCheckElapseTime == true)
            {
                m_qlTimeCompleted.append(m_qlTime.at(m_siNumeroActivite));
                m_siNumeroActivite++;
                SequenceProgress();
                EndOfServiceStatistics();
                StopServices();
                //UpdateActivity(ui->comboBoxServices->currentIndex()+1);
                m_siRepetition = 0;

                SequenceFinished();
                ResetServices();
            }
            else {
                if(m_siNumeroActivite >= 0)
                {
                    m_qlTimeCompleted.append(m_qlTime.at(m_siNumeroActivite));
                }
                m_siNumeroActivite++;
                SequenceProgress();
                ReadServices();
                m_bCheckElapseTime = true;
            }
        }
    }
    else {
        StopServices();
        EndOfServiceStatistics();
        UpdateActivity(ui->comboBoxServices->currentIndex()+1);
        m_siRepetition = 0;

        SequenceFinished();
        ResetServices();
    }
}

/*!
 * \brief MainWindow::ReadServices Allows to read orally the current service and to pass its orange line
 */
void MainWindow::ReadServices()
{
    ui->tableWidgetServices->setRowCount(0);
    ui->tableWidgetServices->setRowCount(2);
    ui->tableWidgetServices->setColumnCount(3);

    ui->tableWidgetServices->setItem(0,0,new QTableWidgetItem(m_qslActivite.at(m_siNumeroActivite)));

    QString sMinHoursMinutesSeconds = QString("%1h %2m %3s").arg(m_qlTime.at(m_siNumeroActivite)/3600).arg((m_qlTime.at(m_siNumeroActivite)%3600)/60).arg(m_qlTime.at(m_siNumeroActivite)%60);
    //ui->tableWidgetServices->setItem(0,1,new QTableWidgetItem(sMinHoursMinutesSeconds));

    ui->tableWidgetServices->setItem(0,2,new QTableWidgetItem(" "));
    if(m_qlTime.at(m_siNumeroActivite)/3600 >=1)
    {
        QString sMinHoursMinutesSeconds = QString("%1h %2m %3s").arg(m_qlTime.at(m_siNumeroActivite)/3600).arg((m_qlTime.at(m_siNumeroActivite)%3600)/60).arg(m_qlTime.at(m_siNumeroActivite)%60);
        ui->tableWidgetServices->setItem(0,1,new QTableWidgetItem(sMinHoursMinutesSeconds));
    }
    else
    {
        if(m_qlTime.at(m_siNumeroActivite)/60 >= 1)
        {
            QString sMinHoursMinutesSeconds = QString(" %1m %2s").arg((m_qlTime.at(m_siNumeroActivite)%3600)/60).arg(m_qlTime.at(m_siNumeroActivite)%60);
            ui->tableWidgetServices->setItem(0,1,new QTableWidgetItem(sMinHoursMinutesSeconds));
        }
        else
        {
            QString sMinHoursMinutesSeconds = QString(" %1s").arg(m_qlTime.at(m_siNumeroActivite)%60);
            ui->tableWidgetServices->setItem(0,1,new QTableWidgetItem(sMinHoursMinutesSeconds));
        }
    }
    ui->tableWidgetServices->setItem(0,1,new QTableWidgetItem(sMinHoursMinutesSeconds));
    ui->tableWidgetServices->setItem(0,2,new QTableWidgetItem(" "));
    if(m_siNumeroActivite < m_qslActivite.size()-1)
    {
        ui->tableWidgetServices->setItem(1,0,new QTableWidgetItem(m_qslActivite.at(m_siNumeroActivite+1)));
        //sMinHoursMinutesSeconds = QString("%1h %2m %3s").arg(m_qlTime.at(m_siNumeroActivite+1)/3600).arg((m_qlTime.at(m_siNumeroActivite+1)%3600)/60).arg(m_qlTime.at(m_siNumeroActivite+1)%60);
        //ui->tableWidgetServices->setItem(1,1,new QTableWidgetItem(sMinHoursMinutesSeconds));
        if(m_qlTime.at(m_siNumeroActivite+1)/3600 >=1)
        {
            QString sMinHoursMinutesSeconds = QString("%1h %2m %3s").arg(m_qlTime.at(m_siNumeroActivite+1)/3600).arg((m_qlTime.at(m_siNumeroActivite+1)%3600)/60).arg(m_qlTime.at(m_siNumeroActivite+1)%60);
            ui->tableWidgetServices->setItem(1,1,new QTableWidgetItem(sMinHoursMinutesSeconds));
        }
        else
        {
            if(m_qlTime.at(m_siNumeroActivite+1)/60 >= 1)
            {
                QString sMinHoursMinutesSeconds = QString(" %1m %2s").arg((m_qlTime.at(m_siNumeroActivite+1)%3600)/60).arg(m_qlTime.at(m_siNumeroActivite+1)%60);
                ui->tableWidgetServices->setItem(1,1,new QTableWidgetItem(sMinHoursMinutesSeconds));
            }
            else
            {
                QString sMinHoursMinutesSeconds = QString(" %1s").arg(m_qlTime.at(m_siNumeroActivite+1)%60);
                ui->tableWidgetServices->setItem(1,1,new QTableWidgetItem(sMinHoursMinutesSeconds));
            }
        }
    }
    ResizeRow();
    ColorRow(0, COLOR_ORANGE); //Color actual row in orange

    m_pEngine->say(ui->tableWidgetServices->item(0,0)->text()); //Say the actual row
    m_nSecondsRemaining=m_qlTime.at(m_siNumeroActivite);
    m_Temp_Time = m_nSecondsRemaining;
    //a convertir les hn les m et s du string en ms
    m_pTimerNextSequence->setInterval((m_qlTime.at(m_siNumeroActivite))*1000); //Set Timer for the next sequence
    m_nService++;
}

/*!
 * \brief MainWindow::ColorRow Allows to change the color (pass as parameter) of the background line (pass as parameter) of the main QTableWidget
 * \param nRow_in Line to change the background color
 * \param color_in Color that will be applied to the background
 */
void MainWindow::ColorRow(int nRow_in, QColor color_in)
{
    for(int nCurrentColumn=0;nCurrentColumn<ui->tableWidgetServices->columnCount();nCurrentColumn++) //All the cells of the column
    {
        QTableWidgetItem *pItem = ui->tableWidgetServices->item(nRow_in, nCurrentColumn);
        pItem->setBackground(QBrush(color_in));
    }
}

/*!
 * \brief MainWindow::ResetServices Allows you to re-initialize services
 */
void MainWindow::ResetServices()
{
    m_nService=0;
    UpdateActivity(ui->comboBoxServices->currentIndex()+1);
    StartButtonConfiguration();
}

/*!
 * \brief MainWindow::StopServices Allows to stop the services
 */
void MainWindow::StopServices()
{
    m_pTimerNextSequence->stop();
    m_pTimerSeconds->stop();
    m_bServiceIsStart=false;
    m_bCheckElapseTime = false;
}

/*!
 * \brief MainWindow::on_pushButtonStartService_clicked Allows to launch the service if it is not launched and to go to the next activity if not
 */
void MainWindow::on_pushButtonStartService_clicked()
{
    if(ui->tableWidgetServices->rowCount()!=0) //If there is at least 1 activity in the department
    {
        m_bCheckRepetition = false;
        if(!m_bServiceIsStart) //If the service is not started
        {
            m_bServiceIsStart=true;
            m_bRepeatActivitie=false;
            m_qdtDateTime = QDateTime::currentDateTime();
            m_qsFormattedDate = m_qdtDateTime.toString("dd/MM/yyyy");
            StartServices();
            m_siNumeroActivite = -1;
            m_qlTimeCompleted.clear();
            SequenceProgress();
        }
        else //If the service is started
        {
            if(m_siNumeroActivite < m_qslActivite.length()-1) //If the service is not finished
            {
                if(m_nSecondsRemaining >= m_qlTime.at(m_siNumeroActivite) - m_qlTimeMin.at(m_siNumeroActivite) && m_siRepetition == 0)
                {
                    m_pEngine->say("Le temps pour changer d'activité est trop court, Veuillez continuer la tâche");
                }
                else
                {
                    if(m_siRepetition == 0)
                    {
                        int tempTemp = m_qlTime.at(m_siNumeroActivite);
                        int tempCalcul = tempTemp - m_nSecondsRemaining;
                        m_qlTimeCompleted.append(tempCalcul);
                    }
                    else
                    {
                        int tempTemp = m_qlTime.at(m_siNumeroActivite);
                        int tempTempReset = m_qlTimeReset.at(m_siNumeroActivite);
                        int tempCalcul = tempTemp + tempTempReset - m_nSecondsRemaining + tempTempReset * (m_siRepetition-1);
                        m_qlTimeCompleted.append(tempCalcul);
                    }
                    m_bRepeatActivitie=false;
                    m_siNumeroActivite++;
                    SequenceProgress();
                    ReadServices();
                    m_siRepetition = 0;

                }
            }
            else //If the last line is reached
            {
                if(m_nSecondsRemaining >= m_qlTime.at(m_siNumeroActivite) - m_qlTimeMin.at(m_siNumeroActivite) && m_siRepetition == 0)
                {
                    m_pEngine->say("Le temps pour changer d'activité est trop court, Veuillez continuer la tâche");
                }
                else
                {
                    if(m_siRepetition == 0)
                    {
                        int tempTemp = m_qlTime.at(m_siNumeroActivite);
                        int tempCalcul = tempTemp - m_nSecondsRemaining;
                        m_qlTimeCompleted.append(tempCalcul);
                    }
                    else
                    {
                        int tempTemp = m_qlTime.at(m_siNumeroActivite);
                        int tempTempReset = m_qlTimeReset.at(m_siNumeroActivite);
                        int tempCalcul = tempTemp + tempTempReset - m_nSecondsRemaining + tempTempReset * (m_siRepetition-1);
                        m_qlTimeCompleted.append(tempCalcul);
                    }
                    m_siNumeroActivite++;
                    SequenceProgress();
                    StopServices();
                    EndOfServiceStatistics();
                    UpdateActivity(ui->comboBoxServices->currentIndex()+1);
                    m_siRepetition = 0;
                    SequenceFinished();
                    ResetServices();
                }
            }
        }
    }
}

/*!
 * \brief MainWindow::on_pushButtonGoToMainWindow_clicked Allows to return to the main interface
 */
void MainWindow::on_pushButtonGoToMainWindow_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

///
/// \brief MainWindow::on_pushButtonGoToMainWindow_2_clicked
///
void MainWindow::on_pushButtonGoToMainWindow_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

/*!
 * \brief MainWindow::UpdateServices Update the combobox services
 * \param nSelectedSequence_in The service to select after updating the combobox
 */
void MainWindow::UpdateServices(int nSelectedSequence_in)
{
    m_bUpdate=false;
    ui->comboBoxServices->clear();
    ui->comboBoxServicesAdmin->clear();
    ui->comboBoxServicesStatistique->clear();
    ConfigComboBox();
    m_bUpdate=true;
    ui->comboBoxServices->setCurrentIndex(nSelectedSequence_in);
    ui->comboBoxServicesAdmin->setCurrentIndex(nSelectedSequence_in);
    ui->comboBoxServicesStatistique->setCurrentIndex(nSelectedSequence_in);
    UpdateActivity(nSelectedSequence_in+1);
}

/*!
 * \brief MainWindow::on_pushButtonAddActivity_clicked Allows you to add a new activity to the current service
 */
void MainWindow::on_pushButtonAddActivity_clicked()
{
    QString QSon_pushButtonAddActivity_clicked = "on_pushButtonAddActivity_clicked";

    int nMaxIdActivity= GetMaxIdActivity();

    QDialog *pDialog = new QDialog(this);
    pDialog->setWindowTitle("Ajouter une activité");
    QVBoxLayout *mainLayout = new QVBoxLayout(pDialog);
    pDialog->setMinimumSize(400,200);
    QHBoxLayout *HboxLayout = new QHBoxLayout;
    QLabel *pLabelActivityExisting = new QLabel("Activité deja existante : ");
    pLabelActivityExisting->setFont(QFont("Arial", 16));
    QCheckBox *pCheckBoxExisting = new QCheckBox;
    pCheckBoxExisting->setFont(QFont("Arial", 16));
    HboxLayout->addWidget(pLabelActivityExisting);
    HboxLayout->addWidget(pCheckBoxExisting);

    QLabel *pLabelNameActivity = new QLabel("Nom d'activité");
    pLabelNameActivity->setFont(QFont("Arial", 16));
    QLineEdit *pLineEditNameActivity = new QLineEdit;
    pLineEditNameActivity->setFont(QFont("Arial", 16));
    pLineEditNameActivity->setValidator(m_pValidatorName);
    QPushButton *pOkButton = new QPushButton("OK");
    pOkButton->setFont(QFont("Arial", 16));
    QLabel *pLabelTime = new QLabel("Temps:");
    pLabelTime->setFont(QFont("Arial", 16));
    QTimeEdit *pTimeEdit = new QTimeEdit;
    pTimeEdit->setDisplayFormat("hh:mm:ss");
    pTimeEdit->setFont(QFont("Arial", 16));
    //Ajout
    QLabel *pLabelTimeReset = new QLabel("Temps répétitions");
    pLabelTimeReset->setFont(QFont("Arial", 16));
    QTimeEdit *pTimeResetEdit = new QTimeEdit;
    pTimeResetEdit->setDisplayFormat("hh:mm:ss");
    pTimeResetEdit->setFont(QFont("Arial", 16));

    QLabel *pLabelTimeMin = new QLabel("Temps minimum");
    pLabelTimeMin->setFont(QFont("Arial", 16));
    QTimeEdit *pTimeMinEdit = new QTimeEdit;
    pTimeMinEdit->setDisplayFormat("hh:mm:ss");
    pTimeMinEdit->setFont(QFont("Arial", 16));

    QComboBox *pComboBoxActivityExisting = new QComboBox(pDialog);
    pComboBoxActivityExisting->setFont(QFont("Arial", 16));

    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);

    p_Base_De_Donnee->Connection_Base(&db,QSon_pushButtonAddActivity_clicked);

    QSqlQuery query("SELECT DISTINCT NameItem FROM titem ORDER BY NameItem ASC");

    query = p_Base_De_Donnee->Requete_Base(&db,QSon_pushButtonAddActivity_clicked,query);


    //Récupération des données dans la ComboBox des activités existantes
    pComboBoxActivityExisting->addItem(nullptr);
    //Display of results
    while (query.next())
    {
        pComboBoxActivityExisting->addItem(query.value(0).toString());
    }

    mainLayout->addLayout(HboxLayout);
    mainLayout->addWidget(pComboBoxActivityExisting);
    mainLayout->addWidget(pLabelNameActivity);
    mainLayout->addWidget(pLineEditNameActivity);
    mainLayout->addWidget(pLabelTime);
    mainLayout->addWidget(pTimeEdit);
    //Ajout
    mainLayout->addWidget(pLabelTimeReset);
    mainLayout->addWidget(pTimeResetEdit);
    mainLayout->addWidget(pLabelTimeMin);
    mainLayout->addWidget(pTimeMinEdit);
    mainLayout->addWidget(pOkButton);

    //Pour quitter la page créer sans rien d'autre
    QPushButton *pOkButtonQuit = new QPushButton("Quitter");
    pOkButtonQuit->setFont(QFont("Arial", 16));
    mainLayout->addWidget(pOkButtonQuit);

    QObject::connect(pOkButtonQuit, &QPushButton::clicked, pDialog, &QDialog::close);

    pDialog->connect(pOkButton, &QPushButton::clicked, pDialog,[&,pLineEditNameActivity,pCheckBoxExisting , pComboBoxActivityExisting, pTimeEdit, pTimeResetEdit, pTimeMinEdit, nMaxIdActivity]() //When validation button was pressed
    {
        if ((!pLineEditNameActivity->text().isEmpty()&&pTimeEdit->time().toString()!="00:00:00") || (pCheckBoxExisting->isChecked() && pComboBoxActivityExisting->currentText() != nullptr)) //If the user has entered an activity name and a time
        {
            pDialog->accept();
            if(pCheckBoxExisting->isChecked() && pComboBoxActivityExisting->currentText() != nullptr)
            {
                qDebug() << pComboBoxActivityExisting->currentText();
                p_Base_De_Donnee->Connection_Base(&db,QSon_pushButtonAddActivity_clicked);
                QString sQuery1 = QString("SELECT DISTINCT NameItem, Time, TimeReset, TimeMin FROM titem WHERE NameItem = '%1'")
                                    .arg(pComboBoxActivityExisting->currentText());
                QSqlQuery query1; //W.I.P
                query1.prepare(sQuery1);
                query1 = p_Base_De_Donnee->Requete_Base(&db,QSon_pushButtonAddActivity_clicked,query1);

                QString nameActivity;
                int tempTime = 0;
                int tempTimeReset = 0;
                int tempTimeMin = 0;
                while (query1.next())     //For all the results of the query
                {
                    nameActivity = query1.value(0).toString();
                    tempTime = query1.value(1).toInt();
                    tempTimeReset = query1.value(2).toInt();
                    tempTimeMin = query1.value(3).toInt();
                }

                p_Base_De_Donnee->Connection_Base(&db,QSon_pushButtonAddActivity_clicked);
                QString sQuery2 = QString("INSERT INTO titem (IdItem, NameItem, Time, OrdreItem, fk_titem_tservice, TimeReset, TimeMin) VALUES(%1, '%2', %3, %4, %5, %6, %7)")
                                     .arg(QString::number(nMaxIdActivity+1),
                                          nameActivity,
                                          QString::number(tempTime),
                                          QString::number(ui->tableWidgetServicesAdmin->rowCount()+1),
                                          QString::number(ui->comboBoxServicesAdmin->currentIndex()+1),
                                          QString::number(tempTimeReset),
                                          QString::number(tempTimeMin));
                QSqlQuery query2; //W.I.P
                query2.prepare(sQuery2);
                query2 = p_Base_De_Donnee->Requete_Base(&db,QSon_pushButtonAddActivity_clicked,query2);
                UpdateActivity(ui->comboBoxServicesAdmin->currentIndex()+1);
                //Closing the connection
                db.close();
                db.removeDatabase("QSQLITE");
            }
            else
            {
                p_Base_De_Donnee->Connection_Base(&db,QSon_pushButtonAddActivity_clicked);

                QSqlQuery sQuery;
                sQuery.prepare("INSERT INTO titem (IdItem, NameItem, Time, OrdreItem, fk_titem_tservice, TimeReset, TimeMin) VALUES (?, ?, ?, ?, ?, ?, ?)");
                sQuery.addBindValue(nMaxIdActivity + 1);
                sQuery.addBindValue(pLineEditNameActivity->text());
                sQuery.addBindValue(pTimeEdit->time().hour() * 3600 + pTimeEdit->time().minute() * 60 + pTimeEdit->time().second());
                sQuery.addBindValue(ui->tableWidgetServicesAdmin->rowCount() + 1);
                sQuery.addBindValue(ui->comboBoxServicesAdmin->currentIndex() + 1);
                sQuery.addBindValue(pTimeResetEdit->time().hour() * 3600 + pTimeResetEdit->time().minute() * 60 + pTimeResetEdit->time().second());
                sQuery.addBindValue(pTimeMinEdit->time().hour() * 3600 + pTimeMinEdit->time().minute() * 60 + pTimeMinEdit->time().second());

                QSqlQuery query; //W.I.P
                query = p_Base_De_Donnee->Requete_Base(&db,QSon_pushButtonAddActivity_clicked,sQuery);
                UpdateActivity(ui->comboBoxServicesAdmin->currentIndex()+1);
                //Closing the connection
                db.close();
                db.removeDatabase("QSQLITE");
            }
        }
        else
        {
            if (pLineEditNameActivity->text().isEmpty()) //If the user has not entered an activity name
            {
                if(pCheckBoxExisting->isChecked() || pComboBoxActivityExisting->currentText() != nullptr)
                {
                    QMessageBox::critical(this,tr("Erreur"), tr("Veuillez bien cochez la croix et selectionner une activité deja existante"));
                }
                else
                {
                    QMessageBox::critical(this,tr("Erreur"), tr("Veuillez entrez un nom d'activité"));
                }
            }
            else if (pTimeEdit->time().toString()=="00:00:00") //If the user has not entered any time
            {
                QMessageBox::critical(this,tr("Erreur"), tr("Veuillez entrez un temps pour l'activité"));
            }

        }
    });
    ResizeRow();
    pDialog->exec();
}

/*!
 * \brief MainWindow::GetMaxIdActivity Function that returns the maximum id of the activities
 * \return The maximum id of the activities
 */
int MainWindow::GetMaxIdActivity()
{
    QString QsGetMaxIdActivity = "GetMaxIdActivity";
    int nMaxIdActivity = 0;
    //Preparation of the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,QsGetMaxIdActivity);
    QSqlQuery query("SELECT MAX(IdItem) FROM titem");
    query = p_Base_De_Donnee->Requete_Base(&db,QsGetMaxIdActivity,query);
    //Take the results
    if (query.next())
    {
        nMaxIdActivity = query.value(0).toInt();
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
    //return the max id activity
    return nMaxIdActivity;
}


/*!
 * \brief MainWindow::on_pushButtonModifyActivity_clicked Allows you to modify the selected activity
 */
void MainWindow::on_pushButtonModifyActivity_clicked()
{
    QString Qson_pushButtonModifyActivity_clicked = "on_pushButtonModifyActivity_clicked";

    QList<QTableWidgetSelectionRange> selectionRanges = ui->tableWidgetServicesAdmin->selectedRanges();
    int nNumberOfRow=0;
    int nFirstRow=0;
    int nLastRow;
    if(!selectionRanges.isEmpty()) //If the user has selected the right lines
    {
        // Récupérer le numéro de ligne de la première case sélectionnée
        nFirstRow = selectionRanges.first().topRow();
        // Récupérer le numéro de ligne de la dernière case sélectionnée
        nLastRow = selectionRanges.last().bottomRow();
        nNumberOfRow=nLastRow-nFirstRow;
        nNumberOfRow++;
    }
    if(selectionRanges.isEmpty()) //If the user has not selected any lines
    {
        QMessageBox::critical(this,tr("Erreur"), tr("Veuillez séléctionnez une ligne a modifier"));
    }
    else if(nNumberOfRow==1) //If the user has selected only one line
    {
        //Initialization of the dialog window
        QDialog *pDialog = new QDialog(this);
        pDialog->setMinimumSize(500, 200);
        pDialog->setWindowTitle("Modifier une activité");
        QVBoxLayout *pMainLayout = new QVBoxLayout(pDialog);
        QLabel *pLabelNameActivity = new QLabel("Nom d'activité");
        pLabelNameActivity->setFont(QFont("Arial", 16));

        QLineEdit *pLineEditNameActivite = new QLineEdit(ui->tableWidgetServicesAdmin->item(nFirstRow,0)->text()); //nameActivite
        pLineEditNameActivite->setFont(QFont("Arial", 16));
        QLabel *pLabelTimeEdit = new QLabel("Temps (hh:mm:ss)");
        pLabelTimeEdit->setFont(QFont("Arial", 16));
        QString sTime = ui->tableWidgetServicesAdmin->item(nFirstRow, 1)->text();
        QStringList timeSplit = sTime.split(' ');
        int hours = timeSplit[0].split('h')[0].toInt();
        int minutes = timeSplit[1].split('m')[0].toInt();
        int seconds = timeSplit[2].split('s')[0].toInt();
        QTimeEdit *pTimeEdit = new QTimeEdit(QTime(hours, minutes, seconds));
        //QTimeEdit *timeEdit = new QTimeEdit;
        pTimeEdit->setDisplayFormat("hh:mm:ss");
        pTimeEdit->setFont(QFont("Arial", 16));

        QLabel *pLabelTimeReset = new QLabel("Temps répétitions (hh:mm:ss)");
        pLabelTimeReset->setFont(QFont("Arial", 16));
        QString sTimeReset = ui->tableWidgetServicesAdmin->item(nFirstRow,2)->text();
        QStringList timeResetSplit = sTimeReset.split(' ');
        hours = timeResetSplit[0].split('h')[0].toInt();
        minutes = timeResetSplit[1].split('m')[0].toInt();
        seconds = timeResetSplit[2].split('s')[0].toInt();
        QTimeEdit *pTimeResetEdit = new QTimeEdit(QTime(hours,minutes,seconds));
        pTimeResetEdit->setDisplayFormat("hh:mm:ss");
        pTimeResetEdit->setFont(QFont("Arial", 16));

        QLabel *pLabelTimeMin = new QLabel("Temps minimum (hh:mm:ss)");
        pLabelTimeMin->setFont(QFont("Arial", 16));
        QString sTimeMin = ui->tableWidgetServicesAdmin->item(nFirstRow,3)->text();
        QStringList timeMinSplit = sTimeMin.split(' ');
        hours = timeMinSplit[0].split('h')[0].toInt();
        minutes = timeMinSplit[1].split('m')[0].toInt();
        seconds = timeMinSplit[2].split('s')[0].toInt();
        QTimeEdit *pTimeMinEdit = new QTimeEdit(QTime(hours,minutes,seconds));
        pTimeMinEdit->setDisplayFormat("hh:mm:ss");
        pTimeMinEdit->setFont(QFont("Arial", 16));

        QPushButton *pOkButton = new QPushButton("OK");
        pOkButton->setFont(QFont("Arial", 16));

        pLineEditNameActivite->setValidator(m_pValidatorName);
        pMainLayout->addWidget(pLabelNameActivity);
        pMainLayout->addWidget(pLineEditNameActivite);
        pMainLayout->addWidget(pLabelTimeEdit);
        pMainLayout->addWidget(pTimeEdit);
        //Ajout
        pMainLayout->addWidget(pLabelTimeReset);
        pMainLayout->addWidget(pTimeResetEdit);
        pMainLayout->addWidget(pLabelTimeMin);
        pMainLayout->addWidget(pTimeMinEdit);

        pMainLayout->addWidget(pOkButton);
        pDialog->setLayout(pMainLayout);
        QPushButton *pCancelButton = new QPushButton("Annuler");
        pCancelButton->setFont(QFont("Arial", 16));
        pMainLayout->addWidget(pCancelButton);
        pDialog->connect(pCancelButton, &QPushButton::clicked, this, [&, pDialog]() //When you click on the cancel button
        {
            pDialog->reject();
        });
        pDialog->connect(pOkButton, &QPushButton::clicked, pDialog,[&, pLineEditNameActivite, pTimeEdit, pTimeResetEdit, pTimeMinEdit, nFirstRow]() //When validation button was pressed
        {
            if (!pLineEditNameActivite->text().isEmpty()&&pTimeEdit->time().toString()!="00:00:00") //If the user has entered a text and a time
            {
                //Preparation of the database
                QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
                db.setDatabaseName(m_sDatabasePath);
                p_Base_De_Donnee->Connection_Base(&db,Qson_pushButtonModifyActivity_clicked);

                QString sQuery = QString("UPDATE titem SET NameItem = '%1', Time = %2, OrdreItem = %3, fk_titem_tservice = %4, TimeReset = %5, TimeMin = %6 WHERE fk_titem_tservice = %4 AND OrdreItem = %3").arg(
                    pLineEditNameActivite->text(),
                    QString::number(pTimeEdit->time().hour()*3600+pTimeEdit->time().minute()*60+pTimeEdit->time().second()),
                    QString::number(nFirstRow+1),
                    QString::number(ui->comboBoxServicesAdmin->currentIndex()+1),
                    QString::number(pTimeResetEdit->time().hour()*3600+pTimeResetEdit->time().minute()*60+pTimeResetEdit->time().second()),
                    QString::number(pTimeMinEdit->time().hour()*3600+pTimeMinEdit->time().minute()*60+pTimeMinEdit->time().second()));

                QSqlQuery query(sQuery);
                query = p_Base_De_Donnee->Requete_Base(&db,Qson_pushButtonModifyActivity_clicked,query);
                //Closing the connection
                db.close();
                db.removeDatabase("QSQLITE");
                //Close the window and update the TableWidget
                pDialog->accept();
                UpdateActivity(ui->comboBoxServicesAdmin->currentIndex()+1);
            }
            else
            {
                if (pLineEditNameActivite->text().isEmpty()) //If the user has not entered any text for the activity name
                {
                    QMessageBox::critical(this,tr("Erreur"), tr("Veuillez entrez un nom d'activité"));
                }
                if (pTimeEdit->time().toString()=="00:00:00") //If the user has not entered any time
                {
                    QMessageBox::critical(this,tr("Erreur"), tr("Veuillez entrez un temps"));
                }
            }
        });
        pDialog->exec();
    }
    else //If the user has selected more than one line to modify
    {
        QMessageBox::critical(this,tr("Erreur"), tr("Veuillez ne séléctionnez qu'une seule ligne a modifier"));
    }
}

/*!
 * \brief MainWindow::on_pushButtonDeleteActivity_clicked Allows you to delete the selected activity
 */
void MainWindow::on_pushButtonDeleteActivity_clicked()
{
    QString Qson_pushButtonDeleteActivity_clicked = "on_pushButtonDeleteActivity_clicked";

    //Retrieve the selected ranges
    QList<QTableWidgetSelectionRange> selectionRanges = ui->tableWidgetServicesAdmin->selectedRanges();
    if(!selectionRanges.isEmpty()) //If the user has selected lines
    {
        //Retrieve the line number of the first selected box
        int nFirstRow = selectionRanges.first().topRow();
        //Retrieve the line number of the last selected box
        int nLastRow = selectionRanges.last().bottomRow();
        //Preparation of the database
        QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(m_sDatabasePath);
        p_Base_De_Donnee->Connection_Base(&db,Qson_pushButtonDeleteActivity_clicked);
        QSqlQuery query("DELETE FROM titem WHERE fk_titem_tservice = "+QString::number(ui->comboBoxServicesAdmin->currentIndex()+1)+" AND OrdreItem BETWEEN " + QString::number(nFirstRow+1) + " AND "+QString::number(nLastRow+1));
        query = p_Base_De_Donnee->Requete_Base(&db,Qson_pushButtonDeleteActivity_clicked,query);
        //Closing the connection
        db.close();
        db.removeDatabase("QSQLITE");
        //Put the right id for the activities and update the tableView
        ReorganizesIdActivity();
        UpdateActivity(ui->comboBoxServicesAdmin->currentIndex()+1);
    }
    else
    {
        QMessageBox::critical(this,tr("Erreur"), tr("Veuillez séléctionnez une ou plusieurs lignes a supprimer"));
    }
    return;
}

/*!
 * \brief MainWindow::ReorganizesIdActivity Reorganize the id of the activities in order to put them correctly in order and without holes
 */
void MainWindow::ReorganizesIdActivityId()
{
    QString QsReorganizesIdActivityId = "ReorganizesIdActivityId";
    //Preparation of the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,QsReorganizesIdActivityId);
    QSqlQuery query("SELECT * FROM titem");
    int nIdActivity = 0;
    //For all the results of the query
    while (query.next()) //Try to execute the query
    {
        nIdActivity++;
        int nCurrentIdActivity = query.value(0).toInt();
        if (nIdActivity != nCurrentIdActivity) //The id is not the right one, we correct it
        {
            //The id is not the right one, we correct it
            QString update_Query = QString("UPDATE titem SET IdItem = %1 WHERE IdItem = %2").arg(nIdActivity).arg(nCurrentIdActivity);
            query = p_Base_De_Donnee->Requete_Base_Update(&db,QsReorganizesIdActivityId,query, update_Query);

            QString update_QueryStat = QString("UPDATE tstatistique SET fk_tstatistique_IdItem = %1 WHERE fk_tstatistique_IdItem = %2").arg(nIdActivity).arg(nCurrentIdActivity);
            query = p_Base_De_Donnee->Requete_Base_Update(&db,QsReorganizesIdActivityId,query, update_QueryStat);

            db.close();
            db.removeDatabase("QSQLITE");
            ReorganizesIdActivityId();
        }
    }
    db.close();
    db.removeDatabase("QSQLITE");
}

/*!
 * \brief MainWindow::ReorganizesIdActivity Reorganize the order of the activities in order to put them correctly in order and without holes
 */
void MainWindow::ReorganizesIdActivityOrder()
{
    QString QsReorganizesIdActivityOrder = "ReorganizesIdActivityOrder";

    //Preparation of the database
    QSqlDatabase secondDb = QSqlDatabase::addDatabase("QSQLITE");
    secondDb.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&secondDb,QsReorganizesIdActivityOrder);
    QSqlQuery secondQuery("SELECT * FROM titem");
    secondQuery.exec("SELECT * FROM titem WHERE fk_titem_tservice = "+QString::number(ui->comboBoxServicesAdmin->currentIndex()+1)); //W.I.P (pas besoin de exec)
    int nOrderActivity=0;
    while (secondQuery.next())     //For all the results of the query
    {
        nOrderActivity++;
        int currentIdOrder = secondQuery.value(3).toInt();
        if (nOrderActivity != currentIdOrder) //The order is not the right one, we correct it
        {
            QString sUpdateQuery = QString("UPDATE titem SET OrdreItem = %1 WHERE OrdreItem = %2 AND fk_titem_tservice = %3").arg(nOrderActivity).arg(currentIdOrder).arg(QString::number(ui->comboBoxServicesAdmin->currentIndex()+1));
            //W.I.P pas besoin du QString
            secondQuery = p_Base_De_Donnee->Requete_Base_Update(&secondDb,QsReorganizesIdActivityOrder,secondQuery,sUpdateQuery);
            secondDb.close();
            secondDb.removeDatabase("QSQLITE");
            ReorganizesIdActivityOrder();
        }
    }
    //Closing the connection
    secondDb.close();
    secondDb.removeDatabase("QSQLITE");
}

/*!
 * \brief MainWindow::ReorganizesActivity Calls for the reorganization of id and order
 */
void MainWindow::ReorganizesIdActivity()
{
    ReorganizesIdActivityId();
    ReorganizesIdActivityOrder();
}

/*!
 * \brief MainWindow::on_pushButtonAddService_clicked Allows you to create a new service
 */
void MainWindow::on_pushButtonAddService_clicked()
{
    QString Qs_on_pushButtonAddService_clicked = "on_pushButtonAddService_clicked";

    int nMaxIdService = GetMaxIdService();

    //Initialization of the dialog window
    QDialog *pDialog = new QDialog(this);
    pDialog->setMinimumSize(500, 200);
    pDialog->setWindowTitle("Ajouter un service");
    QVBoxLayout *pMainLayout = new QVBoxLayout(pDialog);
    QLabel *pLabel = new QLabel("Nom du service");
    pLabel->setFont(QFont("Arial", 16));
    QLineEdit *pNameActivitelineEdit = new QLineEdit;
    pNameActivitelineEdit->setFont(QFont("Arial", 16));
    QPushButton *pOkButton = new QPushButton("OK");
    pOkButton->setFont(QFont("Arial", 16));
    pNameActivitelineEdit->setValidator(m_pValidatorName);
    pMainLayout->addWidget(pLabel);
    pMainLayout->addWidget(pNameActivitelineEdit);
    pMainLayout->addWidget(pOkButton);
    pDialog->setLayout(pMainLayout);
    QPushButton *pCancelButton = new QPushButton("Annuler");
    pCancelButton->setFont(QFont("Arial", 16));
    pMainLayout->addWidget(pCancelButton);

    QObject::connect(pCancelButton, &QPushButton::clicked, pDialog, &QDialog::close);

    pDialog->connect(pOkButton, &QPushButton::clicked, pDialog,[&, pNameActivitelineEdit, nMaxIdService]() //When validation button was pressed
    {
        if (!pNameActivitelineEdit->text().isEmpty()) //If the user has entered a name to the service
        {
            pDialog->accept();
            //Preparation of the database
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // Définir le type de la base de donnée
            db.setDatabaseName(m_sDatabasePath); // Assigner le chemin à la variable de la base de donné
            p_Base_De_Donnee->Connection_Base(&db,Qs_on_pushButtonAddService_clicked);
            QString sQuery("INSERT INTO tservice (IdService, NameService) VALUES(" + QString::number(nMaxIdService+1) + ",'" + pNameActivitelineEdit->text() + "')"); //W.I.P pas besoin du QString
            QSqlQuery query;
            query = p_Base_De_Donnee->Requete_Base_Update(&db,Qs_on_pushButtonAddService_clicked,query,sQuery);
            //Update Service
            UpdateServices(ui->comboBoxServicesAdmin->currentIndex());
            ui->comboBoxServicesAdmin->setCurrentIndex(ui->comboBoxServicesAdmin->count()-1);
            //Closing the connection
            db.close();
            db.removeDatabase("QSQLITE");
        }
        else //If the user has not entered a service name
        {
            QMessageBox::critical(this,tr("Erreur"), tr("Veuillez entrez un nom de service"));
        }
    });
    pDialog->exec();
}

/*!
 * \brief MainWindow::GetMaxIdService Function that returns the maximum id of the service
 * \return The maximum id of the service
 */
int MainWindow::GetMaxIdService()
{
    QString Qs_GetMaxIdService = "GetMaxIdService";
    //Preparation of the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,Qs_GetMaxIdService);
    int nMaxIdService = 0;
    QSqlQuery query("SELECT MAX(IdService) FROM tservice");
    query = p_Base_De_Donnee->Requete_Base(&db,Qs_GetMaxIdService,query);
    //Take the results
    if (query.next())
    {
       nMaxIdService = query.value(0).toInt();
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
    //return the max id service
    return nMaxIdService;
}

/*!
 * \brief MainWindow::ReorganizesIdServices Reorganize the id of the services in order to put them correctly in order is without hole
 */
void MainWindow::ReorganizesIdServices()
{
    QString Qs_ReorganizesIdServices = "ReorganizesIdServices";

    //Preparation of the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,Qs_ReorganizesIdServices);
    QSqlQuery query;
    query.exec("SELECT * FROM tservice"); //W.I.P
    int nIdService = 0;
    while (query.next()) //For all the results of the query
    {
        nIdService++;
        int nCurrentIdService = query.value(0).toInt();
        if (nIdService != nCurrentIdService) //The id is not the right one, we correct it
        {
            QString sUpdateQuery = QString("UPDATE tservice SET IdService = %1 WHERE IdService = %2").arg(nIdService).arg(nCurrentIdService); //W.I.P pas besoin du QString
            query = p_Base_De_Donnee->Requete_Base_Update(&db,Qs_ReorganizesIdServices,query,sUpdateQuery);
            QString sUpdateQueryStat = QString("UPDATE tstatistique SET fk_texecutionservice_IdService = %1 WHERE fk_texecutionservice_IdService = %2").arg(nIdService).arg(nCurrentIdService); //W.I.P pas besoin du QString
            query = p_Base_De_Donnee->Requete_Base_Update(&db,Qs_ReorganizesIdServices,query,sUpdateQueryStat);
            db.close();
            db.removeDatabase("QSQLITE");
            ReorganizesIdServices();
        }
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
}

/*!
 * \brief MainWindow::on_pushButtonDeleteService_clicked Delete the currently selected service
 */
void MainWindow::on_pushButtonDeleteService_clicked()
{
    QString Qs_on_pushButtonDeleteService_clicked = "on_pushButtonDeleteService_clicked";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Suppresion service", "Voulez-vous vraiment supprimer le service <b>"+ui->comboBoxServicesAdmin->currentText() +"</b> qui contient <b>" + QString::number(ui->tableWidgetServicesAdmin->rowCount())+ "</b> activité(s) ?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) //If the user answered yes to the confirmation
    {
        //Preparation of the database
        QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(m_sDatabasePath);
        p_Base_De_Donnee->Connection_Base(&db,Qs_on_pushButtonDeleteService_clicked);
        //Delete the service
        QSqlQuery query(QString("DELETE FROM tservice WHERE IdService = %1").arg(ui->comboBoxServicesAdmin->currentIndex()+1));
        query = p_Base_De_Donnee->Requete_Base(&db,Qs_on_pushButtonDeleteService_clicked,query);
        //Delete all the activites of the service
        QSqlQuery secondQuery(QString("DELETE FROM titem WHERE fk_titem_tservice = %1").arg(ui->comboBoxServicesAdmin->currentIndex()+1));
        secondQuery = p_Base_De_Donnee->Requete_Base(&db,Qs_on_pushButtonDeleteService_clicked,secondQuery);
        //Delete all the statistics of the service
        QSqlQuery queryDeleteStat(QString("DELETE FROM texecutionservice WHERE fk_texecutionservice_IdService = %1").arg(ui->comboBoxServicesAdmin->currentIndex()+1));
        queryDeleteStat = p_Base_De_Donnee->Requete_Base(&db,Qs_on_pushButtonDeleteService_clicked,queryDeleteStat);
        QSqlQuery queryDeleteStat2(QString("DELETE FROM tstatistique WHERE fk_tstatistique_IdItem = %1").arg(ui->comboBoxServicesAdmin->currentIndex()+1));
        queryDeleteStat2 = p_Base_De_Donnee->Requete_Base(&db,Qs_on_pushButtonDeleteService_clicked,queryDeleteStat2);
        qDebug()<<"UPDATE titem SET fk_titem_tservice = fk_titem_tservice - 1 WHERE fk_titem_tservice > "+QString::number(ui->comboBoxServicesAdmin->currentIndex()+1);
        //Decrements the foreign key of the services being after the one delete
        QSqlQuery thirdQuery(QString("UPDATE titem SET fk_titem_tservice = fk_titem_tservice - 1 WHERE fk_titem_tservice > "+QString::number(ui->comboBoxServicesAdmin->currentIndex()+1)));
        thirdQuery = p_Base_De_Donnee->Requete_Base(&db,Qs_on_pushButtonDeleteService_clicked,thirdQuery);

        //Closing the connection
        db.close();
        db.removeDatabase("QSQLITE");
        //Updates and reorganizes the id of the activities
        ReorganizesIdServices();
        ReorganizesIdActivity();
        UpdateServices(ui->comboBoxServicesAdmin->currentIndex()-1);
        ui->comboBoxServices->setCurrentIndex(0);
    }
}

/*!
 * \brief MainWindow::ChangeLastSelectedIndexService Changes the index of the last selected service in the database (for subsequent runs).
 * \param nIndex_in The index of the last selected service
 */
void MainWindow::ChangeLastSelectedIndexService(int nIndex_in)
{
    QString Qs_ChangeLastSelectedIndexService = "ChangeLastSelectedIndexService";

    //Preparation of the database
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,Qs_ChangeLastSelectedIndexService);
    QString sQuery = QString("UPDATE tsettings SET ValueOfTheSettings = %1 WHERE IdSettings = 1").arg(nIndex_in); //W.I.P pas besoin du QString
    QSqlQuery query;
    query = p_Base_De_Donnee->Requete_Base_Update(&db,Qs_ChangeLastSelectedIndexService,query,sQuery);
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
}

/*!
 * \brief MainWindow::GetLastSelectedIndexService Retrieve from the database the index of the last selected service in the database (for previous executions)
 * \return nIndex_out The index of the last selected service
 */
int MainWindow::GetLastSelectedIndexService()
{
    QString Qs_GetLastSelectedIndexService = "GetLastSelectedIndexService";

    int nIndex_out = 0;
    //Preparation of the database
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,Qs_GetLastSelectedIndexService);
    QSqlQuery query("SELECT ValueOfTheSettings FROM tsettings WHERE IdSettings = 1");
    query = p_Base_De_Donnee->Requete_Base(&db,Qs_GetLastSelectedIndexService,query);
    //Take the results
    while(query.next())
    {
        nIndex_out = query.value(0).toInt();
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
    //Return the index of the last selected service
    return nIndex_out;
}

/*!
 * \brief MainWindow::GetPasswordOfArhm Retrieves the password needed to access the admin interface from the database and returns it
 * \return sPassword_out The password allowing to go on the admin interface
 */
QString MainWindow::GetPasswordOfArhm()
{
    QString Qs_GetPasswordOfArhm = "GetPasswordOfArhm";

    QString sPassword_out;
    //Preparation of the database
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,Qs_GetPasswordOfArhm);
    QSqlQuery query("SELECT ValueOfTheSettings FROM tsettings WHERE IdSettings = 2");
    query = p_Base_De_Donnee->Requete_Base(&db,Qs_GetPasswordOfArhm,query);
    query.next();
    sPassword_out = query.value(0).toString();
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
    //Return the password allowing to go on the admin interface
    return sPassword_out;
}

/*!
 * \brief MainWindow::on_pushButtonSettings_clicked Call function when you press the settings button to go to the admin interface if the right password is entered
 */
void MainWindow::on_pushButtonSettings_clicked()
{
    //Hash and salt (randomly generated salt) of the password
    QRandomGenerator generator;
    quint32 saltValue = generator.generate();
    QByteArray hashedPassword = HashingAndSalling(GetPasswordOfArhm(), saltValue);
    //Configure the window to enter the password
    QDialog *pDialogPassword = new QDialog(this);
    QVBoxLayout *pMainLayoutDialogPassword = new QVBoxLayout(pDialogPassword);
    QLabel *pLabelPassword = new QLabel("Entrez le mot de passe :");
    pLabelPassword->setFont(QFont("Arial", 16));
    QLineEdit *pLineEditPassword = new QLineEdit;
    pLineEditPassword->setFont(QFont("Arial", 16));
    pLineEditPassword->setEchoMode(QLineEdit::Password);
    QPushButton *pOkButtonPassword = new QPushButton("OK");
    pOkButtonPassword->setFont(QFont("Arial", 16));
    QPushButton *pCancelButtonPassword = new QPushButton("Annuler");
    pCancelButtonPassword->setFont(QFont("Arial", 16));
    //Button to display the password or not
    QAction *pShowPassword = new QAction(this);
    pShowPassword->setIcon(QIcon(":/images/close eye.png"));
    connect(pShowPassword, &QAction::triggered, this, [&, pLineEditPassword, pShowPassword]()
            {
                if (pLineEditPassword->echoMode() == QLineEdit::Password) {
                    pLineEditPassword->setEchoMode(QLineEdit::Normal);
                    pShowPassword->setIcon(QIcon(":/images/open eye.png"));
                }
                else {
                    pLineEditPassword->setEchoMode(QLineEdit::Password);
                    pShowPassword->setIcon(QIcon(":/images/close eye.png"));
                }
            });
    pLineEditPassword->addAction(pShowPassword, QLineEdit::TrailingPosition);
    pMainLayoutDialogPassword->addWidget(pLabelPassword);
    pMainLayoutDialogPassword->addWidget(pLineEditPassword);
    pMainLayoutDialogPassword->addWidget(pOkButtonPassword);
    pMainLayoutDialogPassword->addWidget(pCancelButtonPassword);
    pDialogPassword->setLayout(pMainLayoutDialogPassword);
    pDialogPassword->connect(pCancelButtonPassword, &QPushButton::clicked, this, [&, pDialogPassword]() //When you click on the cancel button
    {
        pDialogPassword->reject();
    });
    pDialogPassword->connect(pOkButtonPassword, &QPushButton::clicked, this, [&, pLineEditPassword,saltValue,hashedPassword,pDialogPassword]() //When you click on the ok button
    {
        //Hash of the entered text
        QString sEntryText = pLineEditPassword->text().toLower();
        QByteArray hashedEnterText = HashingAndSalling(sEntryText, saltValue);

        if(hashedEnterText==hashedPassword) //If the password entered is correct
        {
            pDialogPassword->accept();
            ui->stackedWidget->setCurrentIndex(1);
            ui->tabWidgetAdmin->setCurrentIndex(0);
        }
        else //If the password entered is incorrect
        {
            QMessageBox::critical(this, tr("Erreur"), tr("Le mot de passe entré est incorect.\nVeuillez réessayez"));
        }
    });
    pDialogPassword->exec();
}


///
/// \brief MainWindow::on_pushButton_MoveTopActivity_clicked This allows you to change the order of the activities by taking the one you have chosen and the one above.
///
void MainWindow::on_pushButton_MoveTopActivity_clicked()
{
    QString Qson_pushButton_MoveTopActivity_clicked = "on_pushButton_MoveTopActivity_clicked";
    QList<QTableWidgetSelectionRange> selectionRanges = ui->tableWidgetServicesAdmin->selectedRanges();
    int nNumberOfRow=0;
    int nFirstRow=0;
    int nLastRow;
    if(!selectionRanges.isEmpty()) //If the user has selected the right lines
    {
        // Récupérer le numéro de ligne de la première case sélectionnée
        nFirstRow = selectionRanges.first().topRow();
        // Récupérer le numéro de ligne de la dernière case sélectionnée
        nLastRow = selectionRanges.last().bottomRow();
        nNumberOfRow=nLastRow-nFirstRow;
        nNumberOfRow++;
    }
    if(selectionRanges.isEmpty()) //If the user has not selected any lines
    {
        QMessageBox::critical(this,tr("Erreur"), tr("Veuillez séléctionnez une seule ligne"));
    }
    else if(nNumberOfRow==1) //If the user has selected only one line
    {
        if(nFirstRow > 0)
        {
            //Preparation of the database
            QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(m_sDatabasePath);
            p_Base_De_Donnee->Connection_Base(&db,Qson_pushButton_MoveTopActivity_clicked);

            QString sQuery1 = QString("UPDATE titem SET OrdreItem = %1 WHERE IdItem = %2")
                                                .arg(m_qlIndexTask.at(nFirstRow))
                                                .arg(m_qlIdItem.at(nFirstRow-1));

            QSqlQuery query(sQuery1);
            query = p_Base_De_Donnee->Requete_Base(&db,Qson_pushButton_MoveTopActivity_clicked,query);


            //Closing the connection
            db.close();
            db.removeDatabase("QSQLITE");

            //Preparation of the database
            QSqlDatabase db2= QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(m_sDatabasePath);
            p_Base_De_Donnee->Connection_Base(&db2,Qson_pushButton_MoveTopActivity_clicked);

            QString sQuery2 = QString("UPDATE titem SET OrdreItem = %1 WHERE IdItem = %2")
                                                .arg(m_qlIndexTask.at(nFirstRow-1))
                                                .arg(m_qlIdItem.at(nFirstRow));

            QSqlQuery query2(sQuery2);
            query = p_Base_De_Donnee->Requete_Base(&db2,Qson_pushButton_MoveTopActivity_clicked,query2);
            //Closing the connection
            db.close();
            db.removeDatabase("QSQLITE");


            UpdateActivity(ui->comboBoxServicesAdmin->currentIndex()+1);
        }
        else
        {
            QMessageBox::critical(this,tr("Erreur"), tr("Ne peut pas être déplacer plus haut"));
        }
    }
}

///
/// \brief MainWindow::on_pushButton_MoveBotActivity_clicked This allows the order of the activities to be interchanged by taking the one chosen and the one below.
///
void MainWindow::on_pushButton_MoveBotActivity_clicked()
{
    QString Qson_pushButton_MoveBotActivity_clicked = "on_pushButton_MoveBotActivity_clicked";
    QList<QTableWidgetSelectionRange> selectionRanges = ui->tableWidgetServicesAdmin->selectedRanges();
    int nNumberOfRow=0;
    int nFirstRow=0;
    int nLastRow=0;
    if(!selectionRanges.isEmpty()) //If the user has selected the right lines
    {
        // Récupérer le numéro de ligne de la première case sélectionnée
        nFirstRow = selectionRanges.first().topRow();
        // Récupérer le numéro de ligne de la dernière case sélectionnée
        nLastRow = selectionRanges.last().bottomRow();
        nNumberOfRow=nLastRow-nFirstRow;
        nNumberOfRow++;
    }
    if(selectionRanges.isEmpty()) //If the user has not selected any lines
    {
        QMessageBox::critical(this,tr("Erreur"), tr("Veuillez séléctionnez une seule ligne"));
    }
    else if(nNumberOfRow==1) //If the user has selected only one line
    {
        if(ui->tableWidgetServicesAdmin->rowCount()-1 > nFirstRow)
        {
            //Preparation of the database
            QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(m_sDatabasePath);
            p_Base_De_Donnee->Connection_Base(&db,Qson_pushButton_MoveBotActivity_clicked);

            QString sQuery1 = QString("UPDATE titem SET OrdreItem = %1 WHERE IdItem = %2")
                                                .arg(m_qlIndexTask.at(nFirstRow+1))
                                                .arg(m_qlIdItem.at(nFirstRow));

            QSqlQuery query(sQuery1);
            query = p_Base_De_Donnee->Requete_Base(&db,Qson_pushButton_MoveBotActivity_clicked,query);
            //Closing the connection
            db.close();
            db.removeDatabase("QSQLITE");
            //Preparation of the database
            QSqlDatabase db2= QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(m_sDatabasePath);
            p_Base_De_Donnee->Connection_Base(&db2,Qson_pushButton_MoveBotActivity_clicked);

            QString sQuery2 = QString("UPDATE titem SET OrdreItem = %1 WHERE IdItem = %2")
                                                .arg(m_qlIndexTask.at(nFirstRow))
                                                .arg(m_qlIdItem.at(nFirstRow+1));

            QSqlQuery query2(sQuery2);
            query = p_Base_De_Donnee->Requete_Base(&db2,Qson_pushButton_MoveBotActivity_clicked,query2);

            //Closing the connection
            db.close();
            db.removeDatabase("QSQLITE");

            UpdateActivity(ui->comboBoxServicesAdmin->currentIndex()+1);
        }
        else
        {
            QMessageBox::critical(this,tr("Erreur"), tr("Ne peut pas être déplacer plus haut"));
        }
    }
}

///
/// \brief MainWindow::on_pushButton_Son_clicked
///
void MainWindow::on_pushButton_Son_clicked()
{
    QString m_qson_pushButton_Son_clicked = "on_pushButton_Son_clicked";

    QStringList ListeSon = p_Son->RecuperationDesSons();

    QDialog *pDialog = new QDialog(this);
    pDialog->setWindowTitle("Son à utiliser");
    QVBoxLayout *mainLayout = new QVBoxLayout(pDialog);
    mainLayout->setAlignment(Qt::AlignTop);
    QLabel *pLabelNameSon = new QLabel("Son disponible : ");
    pLabelNameSon->setFont(QFont("Arial", 16));
    QComboBox *pComboBoxSon = new QComboBox(pDialog);
    pComboBoxSon->setFont(QFont("Arial", 16));

    for(int i =0;i < ListeSon.size(); i++)
    {
        pComboBoxSon->addItem(ListeSon.at(i));
    }
    QHBoxLayout *HboxLayout = new QHBoxLayout;

    QLabel *pLabelDureeSon = new QLabel("Temps du son :");
    pLabelDureeSon->setFont(QFont("Arial", 16));
    QSpinBox *pSpinBoxTime = new QSpinBox;
    pSpinBoxTime->setFont(QFont("Arial", 16));
    HboxLayout->addWidget(pLabelDureeSon);
    HboxLayout->addWidget(pSpinBoxTime);

    QHBoxLayout *HboxLayout2 = new QHBoxLayout;

    QLabel *pLabelDureeSon2 = new QLabel("Rendre muet les son de fin d'activité : ");
    pLabelDureeSon2->setFont(QFont("Arial", 16));
    QCheckBox *CheckBoxMute = new QCheckBox;
    CheckBoxMute->setFixedSize(200,40);
    HboxLayout2->addWidget(pLabelDureeSon2);
    HboxLayout2->addWidget(CheckBoxMute);

    QPushButton *pOkButton = new QPushButton("OK");
    pOkButton->setFont(QFont("Arial", 16));

    mainLayout->addWidget(pLabelNameSon);
    mainLayout->addWidget(pComboBoxSon);
    mainLayout->addLayout(HboxLayout);
    mainLayout->addLayout(HboxLayout2);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(verticalSpacer);

    mainLayout->addWidget(pOkButton);
    pDialog->setLayout(mainLayout);

    pDialog->setMinimumSize(200,200);

    QPushButton *pCancelButton = new QPushButton("Annuler");
    pCancelButton->setFont(QFont("Arial", 16));
    mainLayout->addWidget(pCancelButton);
    pDialog->connect(pCancelButton, &QPushButton::clicked, this, [&, pDialog]() //When you click on the cancel button
    {
        pDialog->reject();
    });
    pDialog->connect(pOkButton, &QPushButton::clicked, pDialog,[&, pComboBoxSon]()
    {
        m_sLienSon = pComboBoxSon->currentText();
        m_iDureeSon = pSpinBoxTime->value();
        m_bMuteSon = CheckBoxMute->checkState();
        pDialog->accept();

        //Preparation of the database
        QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(m_sDatabasePath);
        p_Base_De_Donnee->Connection_Base(&db,m_qson_pushButton_Son_clicked);
        QString sQuery1 = QString("UPDATE tson SET nomSon = '%1' , duree = %2 , muteBool = %3 WHERE idSon = 1")
                                            .arg(m_sLienSon)
                                            .arg(m_iDureeSon)
                                            .arg(m_bMuteSon);

        QSqlQuery query(sQuery1);
        query = p_Base_De_Donnee->Requete_Base(&db,m_qson_pushButton_Son_clicked,query);
        //Closing the connection
        db.close();
        db.removeDatabase("QSQLITE");
    });
    pDialog->exec();
}

///
/// \brief MainWindow::on_pushButtonMessageFinSequence_clicked
///
void MainWindow::on_pushButtonMessageFinSequence_clicked()
{
    QString Qs_on_pushButtonMessageFinSequence_clicked = "on_pushButtonMessageFinSequence_clicked";

    //Initialization of the dialog window
    QDialog *pDialog = new QDialog(this);
    pDialog->setMinimumSize(400, 200);
    pDialog->setWindowTitle("Message fin de séquence");
    QVBoxLayout *pMainLayout = new QVBoxLayout(pDialog);
    QLabel *pLabel = new QLabel("Message fin de séquence");
    pLabel->setFont(QFont("Arial", 16));
    QLineEdit *pMesage = new QLineEdit;
    pMesage->setFont(QFont("Arial", 16));
    QPushButton *pOkButton = new QPushButton("OK");
    pOkButton->setFont(QFont("Arial", 16));
    pMesage->setValidator(m_pValidatorName);
    pMainLayout->addWidget(pLabel);
    pMainLayout->addWidget(pMesage);
    pMainLayout->addWidget(pOkButton);
    pDialog->setLayout(pMainLayout);
    QPushButton *pCancelButton = new QPushButton("Annuler");
    pCancelButton->setFont(QFont("Arial", 16));
    pMainLayout->addWidget(pCancelButton);
    pDialog->connect(pCancelButton, &QPushButton::clicked, this, [&, pDialog]() //When you click on the cancel button
    {
        pDialog->reject();
    });
    pDialog->connect(pOkButton, &QPushButton::clicked, pDialog,[&, pMesage]() //When validation button was pressed
    {
        if (!pMesage->text().isEmpty()) //If the user has entered a name to the service
        {
            pDialog->accept();
            //Preparation of the database
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // Définir le type de la base de donnée
            db.setDatabaseName(m_sDatabasePath); // Assigner le chemin à la variable de la base de donné
            p_Base_De_Donnee->Connection_Base(&db,Qs_on_pushButtonMessageFinSequence_clicked);
            QString sQuery = QString("UPDATE tmessage_fin_de_sequence SET MessageFinService = '%1' WHERE IdMessage = 1")
                                            .arg(pMesage->text());

            QSqlQuery query;
            query = p_Base_De_Donnee->Requete_Base_Update(&db,Qs_on_pushButtonMessageFinSequence_clicked,query,sQuery);

            //Closing the connection
            db.close();
            db.removeDatabase("QSQLITE");
        }
        else //If the user has not entered a service name
        {
            QMessageBox::critical(this,tr("Erreur"), tr("Veuillez entrez un message de fin de service"));
        }
    });
    pDialog->exec();
}

///
/// \brief MainWindow::SequenceFinished
///
void MainWindow::SequenceFinished()
{
    QString name_SequenceFinished = "SequenceFinished";
    QString qs_MessageFin;

    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    p_Base_De_Donnee->Connection_Base(&db,name_SequenceFinished);

    QSqlQuery query("SELECT MessageFinService FROM tmessage_fin_de_sequence WHERE IdMessage = 1");
    query = p_Base_De_Donnee->Requete_Base(&db,name_SequenceFinished,query);

    while (query.next())
    {
        qs_MessageFin = query.value(0).toString();
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");

    m_pEngine->say(qs_MessageFin);

    //Initialization of the dialog window
    QDialog *pDialog = new QDialog(this);
    pDialog->setWindowTitle("Fin de service");
    QVBoxLayout *pMainLayout = new QVBoxLayout(pDialog);
    QLabel *pLabel = new QLabel(qs_MessageFin);
    pLabel->setFont(QFont("Arial", 16));
    QPushButton *pOkButton = new QPushButton("OK");
    pOkButton->setFont(QFont("Arial", 16));
    pMainLayout->addWidget(pLabel);
    pMainLayout->addWidget(pOkButton);
    pDialog->setLayout(pMainLayout);
    pDialog->setMinimumSize(400,200);
    pDialog->connect(pOkButton, &QPushButton::clicked, pDialog,[&]() //When validation button was pressed
    {
        pDialog->accept();
    });
    pDialog->exec();
}

void MainWindow::SequenceProgress()
{
    double temp_m_si = m_siNumeroActivite;
    double resultatStyleSheet = temp_m_si / m_qslActivite.length();
    QString styleSheet = QString("QPushButton {background-color: qlineargradient(spread:pad, x1:0, y1:%1, x2:0, y2:0 stop:0 rgba(146, 208, 79, 255), stop:0.495 rgba(146, 208, 79, 255), stop:0.505 rgba(255, 165, 0, 255), stop:1 rgba(255, 165, 0, 255));}")
            .arg(2-(resultatStyleSheet*2));
    ui->pushButtonServiceProgress->setStyleSheet(styleSheet);
}

///
/// \brief MainWindow::EndOfServiceStatistics enter statistics into the database
///
void MainWindow::EndOfServiceStatistics()
{
    QString Qs_EndOfServiceStatistics = "EndOfServiceStatistics";
    //Preparation of the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,Qs_EndOfServiceStatistics);

    QSqlQuery query;

    //Requete pour insérer une nouvelle execution

    query.prepare("INSERT INTO texecutionservice (dateExecService, fk_texecutionservice_IdService) VALUES(:date, :serviceId)");
    query.bindValue(":date", m_qsFormattedDate);
    query.bindValue(":serviceId", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    query = p_Base_De_Donnee->Requete_Base(&db,Qs_EndOfServiceStatistics,query);

    p_Base_De_Donnee->Connection_Base(&db,Qs_EndOfServiceStatistics);
    QSqlQuery querySelect;
    querySelect.prepare("SELECT idExecutionService, fk_texecutionservice_IdService FROM texecutionservice ");

    querySelect = p_Base_De_Donnee->Requete_Base(&db,Qs_EndOfServiceStatistics,querySelect);

    int idExecutionServiceTemp = 0;
    int IdServiceTemp = 0;
    while(querySelect.next())
    {
        idExecutionServiceTemp = querySelect.value(0).toInt();
        IdServiceTemp = querySelect.value(1).toInt();
    }
    p_Base_De_Donnee->Connection_Base(&db,Qs_EndOfServiceStatistics);

    QSqlQuery queryTemp;

    for(int i = 0; i < m_qlTimeCompleted.size(); i++)
    {
        p_Base_De_Donnee->Connection_Base(&db,Qs_EndOfServiceStatistics);
        QSqlQuery queryInsertStat;
        queryInsertStat.prepare("INSERT INTO tstatistique (timeActivity, fk_tstatistique_IdItem, fk_tstatistique_id_ExecutionService) VALUES (:timeAct, :itemId, :execService)");
        queryInsertStat.bindValue(":execService", QString::number(idExecutionServiceTemp));
        queryInsertStat.bindValue(":itemId", QString::number(m_qlIdItem.at(i)));
        queryInsertStat.bindValue(":timeAct", QString::number(m_qlTimeCompleted.at(i)));

        queryTemp = p_Base_De_Donnee->Requete_Base(&db,Qs_EndOfServiceStatistics,queryInsertStat);
        qDebug() << m_qlIdItem.at(i) << "   " << m_qlTimeCompleted.at(i);
    }
    //Closing the connection
    db.close();
    db.removeDatabase("QSQLITE");
}

///
/// \brief MainWindow::StatisticsDisplay
///
void MainWindow::StatisticsDisplay()
{
    QString Qs_StatisticsDisplay = "StatisticsDisplay";

    m_qldIdExecutionService.clear();

    ui->tableWidgetServicesStatistique->clearContents();
    ui->tableWidgetServicesStatistique->setItem(0,0,new QTableWidgetItem("Temp Total"));

    QFont fixedFont;
    fixedFont.setPointSize(25);

    //Preparation of the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_sDatabasePath);
    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryCount;

    //Requete connaitre le nombre d'excution pour un service

    queryCount.prepare("SELECT COUNT(idExecutionService) AS nbExec FROM texecutionservice WHERE fk_texecutionservice_IdService = :idDuService");
    queryCount.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryCount = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryCount);
    int nbExec = 0;
    while (queryCount.next())
    {
        nbExec = queryCount.value(0).toInt();
        int temp = 4 + nbExec;
        ui->tableWidgetServicesStatistique->setColumnCount(temp);
    }

    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryDate;

    //Requete pour connaitre les dates des executions

    queryDate.prepare("SELECT dateExecService FROM texecutionservice WHERE fk_texecutionservice_IdService = :idDuService ORDER BY idExecutionService DESC");
    queryDate.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryDate = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryDate);
    int tt = 0;
    while (queryDate.next())
    {
        ui->tableWidgetServicesStatistique->setHorizontalHeaderItem(4 + tt, new QTableWidgetItem(queryDate.value(0).toString()));
        tt++;
    }

    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryIdExec;

    //Requete connaitre le nombre d'excution pour un service

    queryIdExec.prepare("SELECT idExecutionService FROM texecutionservice WHERE fk_texecutionservice_IdService = :idDuService ORDER BY idExecutionService DESC");
    queryIdExec.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryIdExec = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryIdExec);
    while (queryIdExec.next())
    {
        m_qldIdExecutionService.append(queryIdExec.value(0).toDouble());
    }

    //Nb nombre item pour un service
    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryNbItem;

    //Nb nombre item pour un service

    queryNbItem.prepare("SELECT COUNT(NameItem) AS nbNameItem FROM titem WHERE fk_titem_tservice = :idDuService");
    queryNbItem.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryNbItem = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryNbItem);

    int nbItem = 0;
    while (queryNbItem.next())
    {
        nbItem = queryNbItem.value(0).toInt();
        int tempRow = 1 + nbItem;
        ui->tableWidgetServicesStatistique->setRowCount(tempRow);
    }

    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryDisplayActivity;

    //Requete pour insérer une nouvelle execution

    queryDisplayActivity.prepare("SELECT NameItem AS nbNameItem FROM titem WHERE fk_titem_tservice = :idDuService ORDER BY OrdreItem ASC");
    queryDisplayActivity.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryDisplayActivity = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryDisplayActivity);
    int i = 1;
    while (queryDisplayActivity.next())
    {
        ui->tableWidgetServicesStatistique->setItem(i,0,new QTableWidgetItem(queryDisplayActivity.value(0).toString()));
        i++;
    }

    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryTempsTotalMinMaxMoy;

    //Requete pour insérer TMax TMin TMoy pour le temps total

    queryTempsTotalMinMaxMoy.prepare("SELECT MAX(total_time) AS TMaxTotaux, MIN(total_time) AS TMinTotaux, AVG(total_time) AS TMoyTotaux FROM (SELECT SUM(timeActivity) AS total_time FROM tstatistique ts, texecutionservice te WHERE ts.fk_tstatistique_id_ExecutionService = te.idExecutionService AND te.fk_texecutionservice_IdService = :idDuService GROUP BY fk_tstatistique_id_ExecutionService ORDER BY fk_tstatistique_id_ExecutionService DESC)");
    queryTempsTotalMinMaxMoy.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryTempsTotalMinMaxMoy = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryTempsTotalMinMaxMoy);

    while (queryTempsTotalMinMaxMoy.next())
    {
        QString sTimeMax = QString("%1h %2m %3s").arg(queryTempsTotalMinMaxMoy.value(0).toInt()/3600).arg((queryTempsTotalMinMaxMoy.value(0).toInt()%3600)/60).arg(queryTempsTotalMinMaxMoy.value(0).toInt()%60);
        QString sTimeMin = QString("%1h %2m %3s").arg(queryTempsTotalMinMaxMoy.value(1).toInt()/3600).arg((queryTempsTotalMinMaxMoy.value(1).toInt()%3600)/60).arg(queryTempsTotalMinMaxMoy.value(1).toInt()%60);
        QString sTimeMoy = QString("%1h %2m %3s").arg(queryTempsTotalMinMaxMoy.value(2).toInt()/3600).arg((queryTempsTotalMinMaxMoy.value(2).toInt()%3600)/60).arg(queryTempsTotalMinMaxMoy.value(2).toInt()%60);
        ui->tableWidgetServicesStatistique->setItem(0,1,new QTableWidgetItem(sTimeMax));
        ui->tableWidgetServicesStatistique->setItem(0,2,new QTableWidgetItem(sTimeMin));
        ui->tableWidgetServicesStatistique->setItem(0,3,new QTableWidgetItem(sTimeMoy));
    }

    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryTempsActivityMinMaxMoy;

    //Requete pour insérer TMax TMin TMoy pour les activités

    queryTempsActivityMinMaxMoy.prepare("SELECT MAX(timeActivity) AS TMaxAct, MIN(timeActivity) AS TMinAct, AVG(timeActivity) AS TMoyAct FROM tstatistique ts, texecutionservice te, titem ti WHERE ts.fk_tstatistique_id_ExecutionService = te.idExecutionService AND ti.IdItem = ts.fk_tstatistique_IdItem AND te.fk_texecutionservice_IdService = :idDuService GROUP BY fk_tstatistique_IdItem ORDER BY fk_tstatistique_id_ExecutionService DESC, OrdreItem ASC");
    queryTempsActivityMinMaxMoy.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryTempsActivityMinMaxMoy = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryTempsActivityMinMaxMoy);

    int compteur = 1;
    while (queryTempsActivityMinMaxMoy.next())
    {
        QString sTimeMax = QString("%1h %2m %3s").arg(queryTempsActivityMinMaxMoy.value(0).toInt()/3600).arg((queryTempsActivityMinMaxMoy.value(0).toInt()%3600)/60).arg(queryTempsActivityMinMaxMoy.value(0).toInt()%60);
        QString sTimeMin = QString("%1h %2m %3s").arg(queryTempsActivityMinMaxMoy.value(1).toInt()/3600).arg((queryTempsActivityMinMaxMoy.value(1).toInt()%3600)/60).arg(queryTempsActivityMinMaxMoy.value(1).toInt()%60);
        QString sTimeMoy = QString("%1h %2m %3s").arg(queryTempsActivityMinMaxMoy.value(2).toInt()/3600).arg((queryTempsActivityMinMaxMoy.value(2).toInt()%3600)/60).arg(queryTempsActivityMinMaxMoy.value(2).toInt()%60);
        ui->tableWidgetServicesStatistique->setItem(compteur,1,new QTableWidgetItem(sTimeMax));
        ui->tableWidgetServicesStatistique->setItem(compteur,2,new QTableWidgetItem(sTimeMin));
        ui->tableWidgetServicesStatistique->setItem(compteur,3,new QTableWidgetItem(sTimeMoy));
        compteur++;
    }

    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryTempsTotalActivity;

    //Requete pour afficher le temps total de chaque execution

    queryTempsTotalActivity.prepare("SELECT SUM(timeActivity) as temp_total_activity FROM tstatistique ts, texecutionservice te, titem ti WHERE ts.fk_tstatistique_id_ExecutionService = te.idExecutionService AND ti.IdItem = ts.fk_tstatistique_IdItem AND te.fk_texecutionservice_IdService = :idDuService GROUP BY fk_tstatistique_id_ExecutionService ORDER BY fk_tstatistique_id_ExecutionService DESC, OrdreItem ASC");
    queryTempsTotalActivity.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryTempsTotalActivity = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryTempsTotalActivity);

    int tempSUMActivity = 0;
    int basePlace = 4;
    while (queryTempsTotalActivity.next())
    {
        QString sFullTimeActivity = QString("%1h %2m %3s").arg(queryTempsTotalActivity.value(0).toInt()/3600).arg((queryTempsTotalActivity.value(0).toInt()%3600)/60).arg(queryTempsTotalActivity.value(0).toInt()%60);
        ui->tableWidgetServicesStatistique->setItem(0, tempSUMActivity + basePlace,new QTableWidgetItem(sFullTimeActivity));
        tempSUMActivity++;
    }

    p_Base_De_Donnee->Connection_Base(&db,Qs_StatisticsDisplay);

    QSqlQuery queryTempsActivity;

    //Requete pour insérer le temps de chaque activité par execution

    queryTempsActivity.prepare("SELECT timeActivity FROM tstatistique ts, texecutionservice te, titem ti WHERE ts.fk_tstatistique_id_ExecutionService = te.idExecutionService AND ti.IdItem = ts.fk_tstatistique_IdItem AND te.fk_texecutionservice_IdService = :idDuService ORDER BY fk_tstatistique_id_ExecutionService DESC, OrdreItem ASC");
    queryTempsActivity.bindValue(":idDuService", QString::number(ui->comboBoxServicesStatistique->currentIndex()+1));

    queryTempsTotalActivity = p_Base_De_Donnee->Requete_Base(&db,Qs_StatisticsDisplay,queryTempsActivity);

    int compteurActivity = 0;
    int numExec = 0;
    while (queryTempsActivity.next())
    {
        if (nbItem == compteurActivity)
        {
            compteurActivity = 0;
            numExec++;
        }
        QString sTimeActivity = QString("%1h %2m %3s").arg(queryTempsActivity.value(0).toInt()/3600).arg((queryTempsActivity.value(0).toInt()%3600)/60).arg(queryTempsActivity.value(0).toInt()%60);
        ui->tableWidgetServicesStatistique->setItem(compteurActivity+1,numExec + basePlace,new QTableWidgetItem(sTimeActivity));
        compteurActivity++;
    }

    //Pour fixer la taille des champs
    ui->tableWidgetServicesStatistique->setFont(fixedFont);
    ui->tableWidgetServicesStatistique->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidgetServicesStatistique->horizontalHeader()->setFont(fixedFont);
    ResizeRow();
}

///
/// \brief MainWindow::on_pushButtonDeleteStatistique_clicked
///
void MainWindow::on_pushButtonDeleteStatistique_clicked()
{
    QString m_qson_pushButtonDeleteStatistique_clicked = "on_pushButtonDeleteStatistique_clicked";

    QList<QTableWidgetSelectionRange> selectionRanges = ui->tableWidgetServicesStatistique->selectedRanges();

    int nFirstColumn = 0;
    int nLastColumn;
    int nNumberOfColumn;
    if(!selectionRanges.isEmpty()) //If the user has selected the right lines
    {
        // Récupérer le numéro de ligne de la première case sélectionnée
        nFirstColumn = selectionRanges.first().leftColumn();
        // Récupérer le numéro de ligne de la dernière case sélectionnée
        nLastColumn = selectionRanges.last().rightColumn();
        nNumberOfColumn=nLastColumn-nFirstColumn;
        nNumberOfColumn++;
        //qDebug() << m_qldIdExecutionService.at(0);
    }
    else
    {
        QMessageBox::critical(this,tr("Erreur"), tr("Ligne vide"));
    }
    if(nFirstColumn < 4)
    {
        QMessageBox::critical(this,tr("Erreur"), tr("Vous ne pouvez pas supprimer cette ligne, elle ne fait pas partie des statistiques d'un service"));
    }
    else
    {
        //Preparation of the database
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(m_sDatabasePath);
        p_Base_De_Donnee->Connection_Base(&db,m_qson_pushButtonDeleteStatistique_clicked);

        QSqlQuery queryIdExec;

        //Requete pour enlerver le l'idExecutionService de la table texecutionservice

        queryIdExec.prepare("DELETE FROM 'texecutionservice' WHERE idExecutionService = :idExec");
        queryIdExec.bindValue(":idExec", m_qldIdExecutionService.at(nFirstColumn-4));

        queryIdExec = p_Base_De_Donnee->Requete_Base(&db,m_qson_pushButtonDeleteStatistique_clicked,queryIdExec);

        p_Base_De_Donnee->Connection_Base(&db,m_qson_pushButtonDeleteStatistique_clicked);

        QSqlQuery querytStat;

        //Requete pour enlerver le l'idExecutionService de la table texecutionservice

        querytStat.prepare("DELETE FROM 'tstatistique' WHERE fk_tstatistique_id_ExecutionService = :idExec");
        querytStat.bindValue(":idExec", m_qldIdExecutionService.at(nFirstColumn-4));

        querytStat = p_Base_De_Donnee->Requete_Base(&db,m_qson_pushButtonDeleteStatistique_clicked,querytStat);
    }
    StatisticsDisplay();
}
