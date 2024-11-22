#include "cson.h"

CSon::CSon()
{
    #if _WIN32 //Window 32 and 64bit
    this->m_sSongPath="D://kaczmarecke.SNIRW//cSon";
    #elif __ANDROID__ //Android device
    this->m_sSongPath="/storage/emulated/0/ServiceCom";
    #endif
}

void CSon::JouerLeSon(QString lienSon, int dureeEnSeconde)
{

    //avoir accés au son choisi
    #if _WIN32 //Window 32 and 64bit

    mediaPlayer.setMedia(QUrl(m_sSongPath + "//" +lienSon));

    #elif __ANDROID__ //Android device
    mediaPlayer.setMedia(QUrl(m_sSongPath + "/" + lienSon));
    #endif

    QString lienDuSon = QUrl(m_sSongPath + "/" + lienSon).toString();

    qDebug() << lienDuSon;

    //lancement du son
    mediaPlayer.setVolume(100);
    mediaPlayer.play();

    //Création du timer et connection avec entre mediaPlayer et QTimer

    QTimer timer;
    timer.setSingleShot(true); //pas de répétition du timer
    timer.start(1000*dureeEnSeconde); // *1000 pour avoir le temps en seconde

    QObject::connect(&timer, &QTimer::timeout, &mediaPlayer, &QMediaPlayer::stop, Qt::DirectConnection);

}

QStringList CSon::RecuperationDesSons()
{
    m_qlSongOnTheDirectory.clear();

    QDir directory(m_sSongPath);
    QStringList filters;
    filters << "*.mp3" << "*.mp4" << "*.wav";
    directory.setNameFilters(filters);

    //Ligne pour obtenir les sons dans la
    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    int i = 0;

    while (i < fileInfoList.size())
    {
        m_qlSongOnTheDirectory.append(fileInfoList.at(i).fileName());
        i++;
    }

    return m_qlSongOnTheDirectory;
}
