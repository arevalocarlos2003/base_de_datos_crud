#include <sqlite3.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

using namespace std;

int insert();
int read();
int del();
void menu();
//variables globales
sqlite3* db;

int main(){
    menu();
    //menu();
    return 0;
}

void menu(){
    int option;
    cout << endl << "Choose an option: " << setw(20) << "|| [1] Insert   " 
                                         << setw(20) << "|| [2] Read     " 
                                         << setw(20) << "|| [3] Delete   " 
                                         << setw(20) << "|| [4] ClearScr "
                                         << setw(20) << "|| [5] Exit     ||" << endl;
    cin >> option;
    switch (option){
        case 1:
            cout << "Insert [1]: " << endl;
            insert();
            break;
        case 2:
            cout << "Read [2]: " << endl;
            read();
            break;
        case 3:
            cout << "Delete [3]: " << endl;
            del();
            break;
        case 4:
            system("clear");
            menu();
            break;
        default:
            cout << "Closing program..." << endl;
            break;
    }
}

int insert(){
    int rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK){
        cerr << "Error al intentar abrir la base de datos" << sqlite3_errmsg << endl;
        return 1;
    }
    string email;
    string user;
    string pass;
    
    cout << "Inserte correo-e: " << endl << "$ ";
    cin >> email;
    
    cout << "Inserte usuario: " << endl << "$ ";
    cin >> user;

    cout << "Inserte password: " << endl << "$ ";
    cin >> pass;

    string insert = "insert into login ('email', 'user', 'pass') values ('"+ email +"', '" + user + "', '" + pass + "');";

    char* error_msg = nullptr;
    rc = sqlite3_exec(db, insert.c_str(), NULL, NULL, NULL);

    if (rc != SQLITE_OK){
        sqlite3_free(error_msg);
    }
    read();
    menu();
    sqlite3_close(db);
    return 0;
}

int read(){
    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK){
        cerr << "Error al intentar abrir la base de datos " << sqlite3_errmsg << endl;
        return 1;
    }

    sqlite3_stmt* stmt;
    const char* read = "select email, user, pass from login;";
    rc = sqlite3_prepare_v2(db, read, -1, &stmt, nullptr);

    if (rc != SQLITE_OK){
        cerr << "Error al intentar realizar la operacion " << sqlite3_errmsg << endl;
        return 1;
    }
    cout << left << "|" << setw(40) << "EMAIL"
                 << "|" << setw(40) << "NAME"
                 << "|" << setw(40) << "PASS"
                 << endl;
    for (int i = 0; i < 120; i++){
        cout << "-";
    }
    cout << endl;
    while(sqlite3_step(stmt) == SQLITE_ROW){
        const char* email = (const char*) sqlite3_column_text(stmt, 0);
        const char* name = (const char*) sqlite3_column_text(stmt, 1);
        const char* pass = (const char*) sqlite3_column_text(stmt, 2);
        
        cout << left << "|" << setw(40) << email << "|" << setw(40) << name << "|" << setw(40) << pass << endl;
        sleep(1);
    }
    char option_;
    cout << "Desea volver al menu?: " <<endl;
    cin >> option_;
    if (option_ == 'S' || option_ == 's'){
        menu();
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int del(){
    read();
    cout << "Datos actuales:" << endl;
    //configuracion general
    string cell, value;
    cout << "Escriba la celda para borrar la fila completa " << endl;
    cin>>cell;
    
    cout << "Escriba el valor de referencia para eliminar " << endl;
    cin>>value;

    int rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK){
        cerr << "Error al intentar abrir la base de datos " << sqlite3_errmsg << endl;
        return 1;
    }

    string Delete = "delete from login where " + cell + " = '" + value + "';";
    rc = sqlite3_exec(db, Delete.c_str(), NULL, NULL, NULL );
    //system("clear");
    read();
    return 0;
}
