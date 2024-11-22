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
    - increases the size of QLabel and QLineEdit
    - change the icon of the application
*********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextToSpeech>
#include <QTimeEdit>
#include <QScrollBar>
#include <QAction>
#include <QValidator>
#include <QRegularExpression>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QProgressBar>
#include <QDateTime>

#include "cbase_de_donnee.h"
#include "cson.h"

const QColor COLOR_RED = QColor(237, 28, 36);
const QColor COLOR_GREEN = QColor(146,208,79);
const QColor COLOR_ORANGE = QColor(255, 165, 0);


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void CloseButtonConfiguration();
    void SettingsButtonConfiguration();
    void StartButtonConfiguration();
    void ConfigComboBox();
    void InitialConfiguration();
    void UpdateActivity(int nIndexOfServices_in);
    void UpdateServices(int nSelectedServices_in);
    void StartServices();
    void TableView();
    void ColorRow(int nRow_in, QColor color_in);
    void ReadServices();
    void StopServices();
    void ResetServices();
    void ReorganizesIdActivity();
    void ReorganizesIdActivityId();
    void ReorganizesIdActivityOrder();
    void ReorganizesIdServices();
    void ChangeLastSelectedIndexService(int nIndex_in);
    int GetMaxIdActivity();
    int GetMaxIdService();
    int GetLastSelectedIndexService();
    QString GetPasswordOfArhm();
    QByteArray HashingAndSalling(QString sTextToHash, quint32 saltValue);

    void ButtonTopConfiguration();
    void ButtonBottomConfiguration();
    void SequenceFinished();
    void SequenceProgress();
    void EndOfServiceStatistics();
    void StatisticsDisplay();

private:
    Ui::MainWindow *ui;
    QTextToSpeech *m_pEngine;
    QValidator *m_pValidatorName;
    QTimer *m_pTimerNextSequence;
    QTimer *m_pTimerSeconds;
    QString m_sDatabasePath;
    bool m_bInitialConfigurationIsDone;
    bool m_bServiceIsStart;
    bool m_bUpdate;
    int m_nSecondsRemaining;
    int m_nService;

    //Ajout d'attribut

    CBase_De_Donnee *p_Base_De_Donnee;
    CSon *p_Son;

    short int m_siRepetition = 0;
    int m_Temp_Time;
    bool m_Verification_Temp_Minimal;

    int m_iNbActivite = 0;
    QStringList m_qslActivite;
    QList<int> m_qlTime;
    QList<int> m_qlTimeReset;
    QList<int> m_qlTimeMin;
    short int m_siNumeroActivite = -1;
    QList<int> m_qlTimeCompleted;
    QList<int> m_qlIndexTask;
    QList<int> m_qlIdItem;

    QString m_sLienSon;
    int m_iDureeSon;
    bool m_bMuteSon;

    bool m_bCheckElapseTime = false;
    bool m_bCheckRepetition = false;

    QDateTime m_qdtDateTime;
    QString m_qsFormattedDate;

    QList<double> m_qldIdExecutionService;

    int m_iRepeatActivitie = 60;
    bool m_bRepeatActivitie = false;

private slots:
    //Activity button
    void on_pushButtonAddActivity_clicked();
    void on_pushButtonModifyActivity_clicked();
    void on_pushButtonDeleteActivity_clicked();
    //Service button
    void on_pushButtonAddService_clicked();
    void on_pushButtonDeleteService_clicked();
    //Other button
    void on_pushButtonStartService_clicked();
    void on_pushButtonGoToMainWindow_clicked();
    void on_pushButtonSettings_clicked();
    //Index change
    void on_comboBoxServicesAdmin_currentIndexChanged(int nIndex_in);
    void on_comboBoxServices_currentIndexChanged(int nIndex_in);
    void ComboboxIndexChange(int nIndex_in);
    //Timer
    void Timer();
    void ElapsedTime();
    //Other
    void ResizeRow();

    void on_pushButton_MoveTopActivity_clicked();
    void on_pushButton_MoveBotActivity_clicked();
    void on_pushButton_Son_clicked();
    void on_pushButtonMessageFinSequence_clicked();
    void on_comboBoxServicesStatistique_currentIndexChanged(int index);
    void on_pushButtonGoToMainWindow_2_clicked();
    void on_pushButtonDeleteStatistique_clicked();
};

#endif // MAINWINDOW_H
