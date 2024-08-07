#include "personal.h"

using namespace std;

int main()
{
	std::string connectionString =
		"host=localhost "
		"port=5432 "
		"dbname=PersonalCpp "
		"user=postgres "
		"password=89617479237k";

	int num;
	do
	{
		
		cout << "\n Выберите действие: \n" << endl <<
			"1.Добавить нового клиента \n"
			"2.Добавить телефон для существующего клиента \n"
			"3.Изменить данные о клиенте \n"
			"4.Удалить телефон у существующего клиента \n"
			"5.Удалить существующего клиента \n"
			"6.Найти клиента по его данным — имени, фамилии, email или номер. \n"
			"7.Выход \n \n"
			"> ";
		cin >> num;
		
		try
		{
			pqxx::connection conn(connectionString);

			PersonalDB::CreateTables(conn);

			if (num == 1)
			{
				PersonalDB::AddClient(conn);
			}

			else if (num == 2)
			{
				PersonalDB::AddNumber(conn);
			}

			else if (num == 3)
			{
				PersonalDB::ChangeInfo(conn);
			}

			else if (num == 4)
			{
				PersonalDB::DeleteNumber(conn);
			}

			else if (num == 5)
			{
				PersonalDB::DeleteClient(conn);
			}

			else if (num == 6)
			{
				PersonalDB::SearchClient(conn);
			}

		}
		catch (const std::exception& e)
		{
			std::cout << "Ошибка: " << e.what() << std::endl;
		};

		}

	while (num != 7);
}