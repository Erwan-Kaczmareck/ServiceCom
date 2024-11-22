#include "cbase_de_donnee.h"


///
/// \brief CBase_De_Donnee::CBase_De_Donnee
///
CBase_De_Donnee::CBase_De_Donnee()
{
    //Pour l'ecole
    //D//kaczmarecke.SNIRW//dossier_Projet//2024-ARHM//Documents//ProjetARHM_2023//ServiceCOM//Sources//BDD_ServiceCom.db
    //"D://kaczmarecke.SNIRW//clone//BDD_ServiceCom.db";
    //Pour chez moi
    //"C://code//Qt//clone//BDD_ServiceCom.db";

    //this->m_sDatabasePath="../ServiceCOM/BDD_ServiceCom.db"

    #if _WIN32 //Window 32 and 64bit
        //this->m_sDatabasePath="D://kaczmarecke.SNIRW//clone//BDD_ServiceCom.db";

        this->m_sDatabasePath="../clone/BDD_ServiceCom.db";
    #elif __ANDROID__ //Android device
        this->m_sDatabasePath="/storage/emulated/0/ServiceCom/BDD_ServiceCom.db";
    #endif
    //this->m_sDatabasePath = "D://kaczmarecke.SNIRW//dossier_Projet//2024-ARHM//Documents//ProjetARHM_2023//ServiceCOM//ServiceCOM_-Test_de_factionnement_du_code//BDD_ServiceCom.db";
}

///
/// \brief CBase_De_Donnee::Connection_Base
/// \param db
/// \param nom_Fonction_in
///
void CBase_De_Donnee::Connection_Base(QSqlDatabase *db,QString nom_Fonction_in)
{

    db->setDatabaseName(this->m_sDatabasePath);
    if (!db->open()) //Try to open the database
    {
        qDebug() << "Error when opening the database, "<< nom_Fonction_in << " : " << db->lastError().text() << '\n';
        db->removeDatabase("QSQLITE");
    }
}

///
/// \brief CBase_De_Donnee::Requete_Base
/// \param db
/// \param nom_Function_in
/// \param query_in
/// \return query
///
QSqlQuery CBase_De_Donnee::Requete_Base(QSqlDatabase *db, QString nom_Function_in, QSqlQuery query_in)
{
    QSqlQuery query = query_in;
    if(!query.exec()) //Try to execute the query
    {
        qDebug() << "Error during the execution of the request, "<< nom_Function_in << ": " << query.lastError().text() << '\n';
        //Closing the connection
        db->close();
        db->removeDatabase("QSQLITE");
        return query;
    }
    return query;
}

///
/// \brief CBase_De_Donnee::CBase_De_Donnee::Requete_Base_Update
/// \param db
/// \param nom_Function_in
/// \param query_in
/// \param update_in
/// \return query
///
QSqlQuery CBase_De_Donnee::CBase_De_Donnee::Requete_Base_Update(QSqlDatabase *db, QString nom_Function_in, QSqlQuery query_in, QString update_in)
{
    QSqlQuery query = query_in;
    if(!query.exec(update_in)) //Try to execute the query
    {

        qDebug() << "Error during the execution of the request, " << nom_Function_in << "(order): " << query.lastError().text() << '\n';
        //Closing the connection
        db->close();
        db->removeDatabase("QSQLITE");
        return query;
    }
    return query;
}
