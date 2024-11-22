#ifndef CBASE_DE_DONNEE_H
#define CBASE_DE_DONNEE_H

#include <QtGlobal>
#include <QtSql>

class CBase_De_Donnee
{
public:
    CBase_De_Donnee();
    void Connection_Base(QSqlDatabase *db, QString nom_Function_in);
    QSqlQuery Requete_Base(QSqlDatabase *db, QString nom_Function_in, QSqlQuery query_in);
    QSqlQuery Requete_Base_Update(QSqlDatabase *db, QString nom_Function_in, QSqlQuery query_in, QString update_in);

private:
    QString m_sDatabasePath;

};

#endif // CBASE_DE_DONNEE_H
