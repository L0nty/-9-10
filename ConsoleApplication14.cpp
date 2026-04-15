// ConsoleApplication14.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <string>
#include <fstream>
using namespace std;


enum Gender { MALE, FEMALE };


struct Date {
    int day, month, year;
};


struct Child {
    string surname;
    Gender sex;
    double height;
    Date birthDate;
};

void printList(Child arr[], int n, string title) {
    cout << "\n--- " << title << " ---" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i].surname << " | "
            << (arr[i].sex == MALE ? "Мальчик" : "Девочка") << " | "
            << arr[i].height << " см | "
            << arr[i].birthDate.day << "." << arr[i].birthDate.month << "." << arr[i].birthDate.year << endl;
    }
}

void showAverageBoys(Child arr[], int n) {
    double sum = 0;
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].sex == MALE) {
            sum += arr[i].height;
            count++;
        }
    }
    if (count > 0) cout << "\nСредний рост мальчиков: " << sum / count << endl;
}

void showTopGirls(Child arr[], int n) {
    Child girls[20];
    int gCount = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].sex == FEMALE) {
            girls[gCount] = arr[i];
            gCount++;
        }
    }
    // Пузырек
    for (int i = 0; i < gCount - 1; i++) {
        for (int j = 0; j < gCount - i - 1; j++) {
            if (girls[j].height < girls[j + 1].height) {
                Child temp = girls[j];
                girls[j] = girls[j + 1];
                girls[j + 1] = temp;
            }
        }
    }
    int limit = (gCount < 5) ? gCount : 5;
    printList(girls, limit, "5 САМЫХ ВЫСОКИХ ДЕВОЧЕК");
}

void sortBySurname(Child arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].surname > arr[j + 1].surname) {
                Child temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void findAndEdit(Child arr[], int n) {
    string searchName;
    cout << "\nВведите ФАМИЛИЮ для поиска: ";
    cin >> searchName;

    bool found = false;
    for (int i = 0; i < n; i++) {
        if (arr[i].surname == searchName) {
            found = true;
            cout << "Ученик найден! Что хотите изменить?\n";
            cout << "1 - Фамилию\n2 - Рост\n3 - Дата рождения\nВаш выбор: ";
            int choice;
            cin >> choice;

            if (choice == 1) {
                cout << "Введите новую фамилию: "; cin >> arr[i].surname;
            }
            else if (choice == 2) {
                cout << "Введите новый рост: "; cin >> arr[i].height;
            }
            else if (choice == 3) {
                cout << "Введите день, месяц и год через пробел: ";
                cin >> arr[i].birthDate.day >> arr[i].birthDate.month >> arr[i].birthDate.year;
            }
            cout << "Данные обновлены.\n";
            break;
        }
    }
    if (!found) {
        cout << "Ошибка: Ученик с фамилией '" << searchName << "' не найден в списке." << endl;
    }
}

//Запись в бинар
void writeChildToBinaryFile(const char* filename, Child data[], int size) {
    ofstream out(filename, ios::binary | ios::out);
    if (!out.is_open()) {
        cout << "Ошибка" << endl;
        return;
    }

    for (int i = 0; i < size; i++) {
        //фамилия
        int surnameLen = data[i].surname.length();
        out.write((char*)&surnameLen, sizeof(surnameLen));
        out.write(data[i].surname.c_str(), surnameLen);

        //пол
        int gender = data[i].sex;
        out.write((char*)&gender, sizeof(gender));

        //рост
        out.write((char*)&data[i].height, sizeof(data[i].height));

        //дата 
        out.write((char*)&data[i].birthDate.day, sizeof(data[i].birthDate.day));
        out.write((char*)&data[i].birthDate.month, sizeof(data[i].birthDate.month));
        out.write((char*)&data[i].birthDate.year, sizeof(data[i].birthDate.year));
    }

    out.close();
    cout << "Данные записаны " << filename << "успех" << endl;
}
        //чтение из бинара
void readChildFromBinaryFile(const char* filename, Child data[], int maxSize) {
    ifstream in(filename, ios::binary | ios::in);
    if (!in.is_open()) {
        cout << "Ошибка" << endl;
        return;
    }

    int i = 0;
    while (i < maxSize && !in.eof()) {
        //фамилия
        int surnameLen;
        if (in.read((char*)&surnameLen, sizeof(surnameLen)).eof()) break;
        char* surnameBuffer = new char[surnameLen + 1];
        in.read(surnameBuffer, surnameLen);
        surnameBuffer[surnameLen] = '\0';
        data[i].surname = surnameBuffer;
        delete[] surnameBuffer;

        //пол
        int gender;
        in.read((char*)&gender, sizeof(gender));
        data[i].sex = static_cast<Gender>(gender);

        //рост
        in.read((char*)&data[i].height, sizeof(data[i].height));

        // дата 
        in.read((char*)&data[i].birthDate.day, sizeof(data[i].birthDate.day));
        in.read((char*)&data[i].birthDate.month, sizeof(data[i].birthDate.month));
        in.read((char*)&data[i].birthDate.year, sizeof(data[i].birthDate.year));

        i++;
    }

    in.close();
    cout << " " << i << "  " << filename << endl;
}



int main() {
    
    setlocale(LC_ALL, "Russian");
    system("chcp 1251 > nul");

    //10 практика, чтение из файла
    cout << "Считывание файла" << endl;
    ifstream fin("nameChild.txt");
    if (!fin.is_open()) {
        cout << "Ошибка: файл не найден" << endl;
        return 1;
    }

    string name;
    int number;
    cout << "Числа из файла:" << endl;
    while (fin >> name >> number) {
        cout << number << endl;
    }
    fin.close();
    cout << endl;

    const int N = 20;
    Child group[N] = {
        {"Иванов", MALE, 150.0, {10, 5, 2012}}, {"Смирнова", FEMALE, 140.0, {15, 5, 2012}},
        {"Петров", MALE, 155.0, {20, 1, 2011}}, {"Кузнецова", FEMALE, 160.0, {5, 3, 2012}},
        {"Попов", MALE, 148.0, {12, 5, 2012}}, {"Васильева", FEMALE, 152.0, {7, 8, 2012}},
        {"Соколов", MALE, 162.0, {19, 11, 2011}}, {"Михайлова", FEMALE, 158.0, {25, 12, 2012}},
        {"Новиков", MALE, 142.0, {14, 2, 2013}}, {"Федорова", FEMALE, 147.0, {2, 6, 2012}},
        {"Морозов", MALE, 159.0, {11, 12, 2011}}, {"Волкова", FEMALE, 165.0, {20, 4, 2012}},
        {"Алексеев", MALE, 151.0, {5, 6, 2012}}, {"Лебедева", FEMALE, 149.0, {29, 9, 2012}},
        {"Семенов", MALE, 157.0, {17, 5, 2011}}, {"Егорова", FEMALE, 153.0, {10, 7, 2012}},
        {"Павлов", MALE, 146.0, {22, 5, 2013}}, {"Козлова", FEMALE, 156.0, {1, 3, 2012}},
        {"Степанов", MALE, 163.0, {8, 11, 2011}}, {"Николаева", FEMALE, 161.0, {13, 2, 2012}}
    
    
    
    };

    showAverageBoys(group, N);
    showTopGirls(group, N);

    sortBySurname(group, N);
    printList(group, N, "СПИСОК ПО АЛФАВИТУ");

   
    findAndEdit(group, N);

    
    printList(group, N, "ОБНОВЛЕННЫЙ СПИСОК");
    writeChildToBinaryFile("nameChild2", group, N);
    Child readGroup[N];
    readChildFromBinaryFile("nameChild2", readGroup, N);
    printList(readGroup, N, "ПРОЧИТАННЫЕ ДАННЫЕ ИЗ БИНАРНОГО ФАЙЛА");
    

    return 0;
    
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
