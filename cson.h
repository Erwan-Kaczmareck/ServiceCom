#ifndef CSON_H
#define CSON_H

#include <QString>
#include <QMediaPlayer>
#include <QTextToSpeech>
#include <QUrl>
#include <QTimer>
#include <QDir>

class CSon
{
public:
    CSon();
    void JouerLeSon(QString lienSon, int dureeEnSeconde);
    QStringList RecuperationDesSons();

private:
    QString m_sSongPath;
    QMediaPlayer mediaPlayer;
    QStringList m_qlSongOnTheDirectory;
    QTextToSpeech *m_textToSpeech;

};

#endif // CSON_H
