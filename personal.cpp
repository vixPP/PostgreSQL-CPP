#include "personal.h"
#include <iostream>

using namespace std;


void PersonalDB::CreateTables(pqxx::connection& conn)
{
	pqxx::transaction t{ conn };

	std::string query = "CREATE TABLE IF NOT EXISTS clients \n"
		"(id SERIAL PRIMARY KEY, \n"
		"name TEXT, \n"
		"surname TEXT, \n"
		"email TEXT); \n \n";

	t.exec(query);

	query = "CREATE TABLE IF NOT EXISTS phone_numbers \n"
		"(id SERIAL PRIMARY KEY, \n"
		"client_id SERIAL REFERENCES clients(id), \n"
		"phone_number TEXT); \n \n";

	t.exec(query);

	t.commit();
}

void PersonalDB::AddClient(pqxx::connection& conn)
{
	std::cout << "Введите ID пользователя: \n"
		"> ";
	string NewID;
	cin >> NewID;

	std::cout << "Введите имя пользователя: \n"
		"> ";
	string NewName;
	cin >> NewName;

	cout << "Введите фамилию пользователя: \n"
		"> ";
	string NewSurname;
	cin >> NewSurname;

	cout << "Введите email пользователя: \n"
		"> ";
	string NewEmail;
	cin >> NewEmail;

	pqxx::work t{ conn };
	
	t.exec("INSERT INTO clients (id, name, surname, email) "
		"VALUES('" + t.esc(NewID) + "','" + t.esc(NewName) + "', '" + t.esc(NewSurname) + " ', ' " + t.esc(NewEmail) + "')");

	t.commit();

	cout << "\n Клиент успешно добавлен! \n";
};

void PersonalDB::AddNumber(pqxx::connection& conn)
{
	cout << "Введите id клиента, которому требуется добавления номера \n"
		"> ";
	int clientID = 0;
	cin >> clientID;

	cout << "Введите номер, который хотите добавить \n"
		"> ";

	string NewNumber;
	cin >> NewNumber;

	pqxx::work t{ conn };
	t.exec("INSERT INTO phone_numbers(id, client_id, phone_number) "
		"VALUES (DEFAULT, " + t.quote(clientID) + ", " + t.quote(NewNumber) + ")");


	t.commit();

	cout << "\n Номер успешно добавлен! \n";
};


void PersonalDB::ChangeInfo(pqxx::connection& conn)
{
	cout << "Введите id клиента, которому требуется изменения: \n"
		"> ";
	int clientID = 0;
	cin >> clientID;

	if (cin.fail())
	{
		cerr << "Ошибка: неверный формат ID\n";
		return;
	}

	cout << "Выберите, что Вы хотите изменить:\n"
		<< "1. Имя\n"
		<< "2. Фамилия\n"
		<< "3. Почта\n"
		<< "4. Номер\n>";
	int columnChoice = 0;
	cin >> columnChoice;

	if (cin.fail() || columnChoice < 1 || columnChoice > 4)
	{
		cerr << "Ошибка: неверный выбор опции\n";
		return;
	}

	string column;
	switch (columnChoice)
	{
	case 1:
		column = "name";
		break;
	case 2:
		column = "surname";
		break;
	case 3:
		column = "email";
		break;
	case 4:
		column = "phone_number";
		break;
	}

	cout << "Введите новое значение:\n> ";
	string newValue;
	cin >> newValue; 

	try 
	{
		pqxx::work txn(conn);
		if (column == "phone_number") 
		{
			txn.exec("UPDATE phone_numbers SET " + column + " = '" + newValue + "' WHERE client_id = " + to_string(clientID));
		}
		else 
		{
			txn.exec("UPDATE clients SET " + column + " = '" + newValue + "' WHERE id = " + to_string(clientID));
		}
		txn.commit();
		cout << "\n Информация успешно изменена! \n";
	}
	catch (const std::exception& e) {
		cerr << "Ошибка: " << e.what() << endl;
	}

}

void PersonalDB::DeleteNumber(pqxx::connection& conn)
{
	cout << "Введите id клиента, у которого нужно удалить номер: \n" <<
		"> ";
	int clientID = 0;
	cin >> clientID;

	if (cin.fail())
	{
		cerr << "Ошибка: неверный формат ID\n";
		return;
	}

	pqxx::work t(conn);

	string deleteQuery = "DELETE FROM phone_numbers WHERE id = ";
	deleteQuery += to_string(clientID);

	t.exec(deleteQuery);

	t.commit();

	cout << "Номер успешно удален!" << endl;
}

void PersonalDB::DeleteClient(pqxx::connection& conn)
{
	cout << "Введите id клиента, которого хотите удалить: ";
	int clientID = 0;
	cin >> clientID;

	if (cin.fail())
	{
		cerr << "Ошибка: неверный формат ID\n";
		return;
	}

	pqxx::work t(conn);

	string deleteQuery = "DELETE FROM clients WHERE id = " + t.quote(clientID);

	t.exec(deleteQuery);

	t.commit();

	cout << "Клиент удален!" << endl;
}

void PersonalDB::SearchClient(pqxx::connection& conn)
{
	cout << "Выберите действие: \n"
		"1. Искать по имени, фамилии и email \n"
		"2. Искать по номеру телефона \n"
		"> ";
	int N = 0;
	cin >> N;

	pqxx::work t{ conn };

	if (N == 1)
	{

		cout << "Введите имя: \n";
		string SName;
		cin >> SName;

		cout << "Введите фамилию: \n";
		string SSurname;
		cin >> SSurname;

		cout << "Введите почту: \n";
		string Smail;
		cin >> Smail;
		
		
		auto results = t.query<int, string, string, string>(
			"SELECT * "
			"FROM clients "
			"WHERE name = '" + SName + "' OR surname = '" + SSurname +
			"' OR email = '" + Smail + "'");

		cout << "\n Найденный клиент: " << endl;

			for (tuple<int, string, string, string> value : results)
			{
				cout << "id = " << get<0>(value) << endl;
				cout << "Имя = " << get<1>(value) << endl;
				cout << "Фамилия = " << get<2>(value) << endl;
				cout << "email = " << get<3>(value) << endl;
			}
		
		 
	}
	
	else if (N == 2)
	{
		cout << "Введите номер клиента, которого хотите найти: \n > ";
		string SNumber;
		cin >> SNumber;

		auto results = t.query<int, string, string, string>(
			"SELECT c.id, c.name, c.surname, c.email "
			"FROM clients c "
			"INNER JOIN phone_numbers n ON c.id = n.client_id "
			"WHERE n.phone_number = '" + SNumber + "'");

		for (tuple<int, string, string, string> value : results)
		{
			//cout << "Найденный клиент: " << endl;
			cout << "id = " << get<0>(value) << endl;
			cout << "Имя = " << get<1>(value) << endl;
			cout << "Фамилия = " << get<2>(value) << endl;
			cout << "email = " << get<3>(value) << endl;
			cout << "______________________ " << endl;
		}
	}

}