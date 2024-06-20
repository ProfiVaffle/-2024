#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <locale>
#include <codecvt>

using namespace std;

struct Toy {
    int id;
    string name;
    string category;
    double price;
    string currency;
    int quantity;
};

vector<Toy> toys;
int current_id = 1;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

template<typename T>
T getInput(const string& prompt, T minValue, T maxValue) {
    T input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (cin.fail() || input < minValue || input > maxValue) {
            cout << "Неправильне введення, спробуйте ще раз." << endl;
            clearInput();
        }
        else {
            clearInput();
            break;
        }
    }
    return input;
}

bool isValidNumber(const string& str) {
    for (char c : str) {
        if (!isdigit(c) && c != '.') {
            return false;
        }
    }
    return true;
}

double getPriceInput() {
    string input;
    while (true) {
        cout << "Введіть ціну іграшки (до 1,000,000): ";
        cin >> input;
        if (!isValidNumber(input)) {
            cout << "Неправильне введення, спробуйте ще раз." << endl;
            clearInput();
        }
        else {
            double price = stod(input);
            if (price < 0 || price > 1000000) {
                cout << "Неправильне введення, спробуйте ще раз." << endl;
                clearInput();
            }
            else {
                return price;
            }
        }
    }
}

string getCurrencyInput() {
    int choice;
    while (true) {
        cout << "Оберіть валюту: 1. Долари 2. Євро 3. Гривні: ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 3) {
            cout << "Неправильний вибір, спробуйте ще раз." << endl;
            clearInput();
        }
        else {
            break;
        }
    }

    switch (choice) {
    case 1: return "USD";
    case 2: return "EUR";
    case 3: return "UAH";
    default: return "UNKNOWN";
    }
}

int getQuantityInput() {
    string input;
    while (true) {
        cout << "Введіть кількість іграшок (до 1000): ";
        cin >> input;
        if (!isValidNumber(input)) {
            cout << "Неправильне введення, спробуйте ще раз." << endl;
            clearInput();
        }
        else {
            int quantity = stoi(input);
            if (quantity < 0 || quantity > 1000) {
                cout << "Неправильне введення, спробуйте ще раз." << endl;
                clearInput();
            }
            else {
                return quantity;
            }
        }
    }
}

void addToy() {
    string name, category;
    cout << "Введіть назву іграшки: ";
    cin >> name;
    cout << "Введіть категорію іграшки: ";
    cin >> category;

    for (auto& toy : toys) {
        if (toy.name == name && toy.category == category) {
            int additionalQuantity = getQuantityInput();
            toy.quantity += additionalQuantity;
            cout << "Кількість іграшки \"" << toy.name << "\" збільшено на " << additionalQuantity << "!" << endl;
            return;
        }
    }

    Toy newToy;
    newToy.id = current_id++;
    newToy.name = name;
    newToy.category = category;
    newToy.price = getPriceInput();
    newToy.currency = getCurrencyInput();
    newToy.quantity = getQuantityInput();
    toys.push_back(newToy);
    cout << "Іграшку успішно додано!" << endl;
}

void deleteToy() {
    int id = getInput<int>("Введіть ID іграшки для видалення: ", 1, current_id - 1);
    for (auto it = toys.begin(); it != toys.end(); ++it) {
        if (it->id == id) {
            cout << "Ви хочете видалити: " << it->name << " (" << it->category << ") Ціна: " << it->price << " " << it->currency << ", Кількість: " << it->quantity << endl;
            cout << "Ви хочете видалити всю іграшку або тільки певну кількість? (вся/кількість): ";
            string choice;
            cin >> choice;

            if (choice == "вся") {
                while (true) {
                    cout << "Ви впевнені, що хочете повністю видалити цю іграшку? (так/ні): ";
                    string confirmation;
                    cin >> confirmation;
                    if (confirmation == "так") {
                        double loss = it->price * it->quantity;
                        cout << "Ви видалили іграшку \"" << it->name << "\", втратили: " << loss << " " << it->currency << "." << endl;
                        toys.erase(it);
                        return;
                    }
                    else if (confirmation == "ні") {
                        cout << "Видалення скасовано." << endl;
                        return;
                    }
                    else {
                        cout << "Неправильне введення, спробуйте ще раз." << endl;
                    }
                }
            }
            else if (choice == "кількість") {
                int quantityToDelete = getInput<int>("Введіть кількість для видалення: ", 1, it->quantity);
                while (true) {
                    cout << "Ви впевнені, що хочете видалити " << quantityToDelete << " шт. цієї іграшки? (так/ні): ";
                    string confirmation;
                    cin >> confirmation;
                    if (confirmation == "так") {
                        double loss = it->price * quantityToDelete;
                        cout << "Ви видалили " << quantityToDelete << " шт. іграшки \"" << it->name << "\", втратили: " << loss << " " << it->currency << "." << endl;
                        it->quantity -= quantityToDelete;
                        if (it->quantity == 0) {
                            toys.erase(it);
                        }
                        return;
                    }
                    else if (confirmation == "ні") {
                        cout << "Видалення скасовано." << endl;
                        return;
                    }
                    else {
                        cout << "Неправильне введення, спробуйте ще раз." << endl;
                    }
                }
            }
            else {
                cout << "Неправильний вибір, видалення скасовано." << endl;
            }
        }
    }
    cout << "Іграшку не знайдено!" << endl;
}

void editToy() {
    int id = getInput<int>("Введіть ID іграшки для редагування: ", 1, current_id - 1);
    for (auto& toy : toys) {
        if (toy.id == id) {
            cout << "Введіть нову назву іграшки: ";
            cin >> toy.name;
            cout << "Введіть нову категорію іграшки: ";
            cin >> toy.category;
            toy.price = getPriceInput();
            toy.currency = getCurrencyInput();
            toy.quantity = getQuantityInput();
            cout << "Іграшку успішно відредаговано!" << endl;
            return;
        }
    }
    cout << "Іграшку не знайдено!" << endl;
}

void listToys() {
    for (const auto& toy : toys) {
        cout << "ID: " << toy.id << ", Назва: " << toy.name << ", Категорія: " << toy.category
            << ", Ціна: " << toy.price << " " << toy.currency << ", Кількість: " << toy.quantity << endl;
    }
}

void searchToys() {
    string keyword;
    cout << "Введіть ключове слово для пошуку: ";
    cin >> keyword;
    bool found = false;
    for (const auto& toy : toys) {
        if (toy.name.find(keyword) != string::npos || toy.category.find(keyword) != string::npos) {
            cout << "ID: " << toy.id << ", Назва: " << toy.name << ", Категорія: " << toy.category
                << ", Ціна: " << toy.price << " " << toy.currency << ", Кількість: " << toy.quantity << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Іграшки не знайдено за ключовим словом \"" << keyword << "\"." << endl;
    }
}

void buyToy() {
    int id = getInput<int>("Введіть ID іграшки для покупки: ", 1, current_id - 1);
    int quantity = getInput<int>("Введіть кількість іграшок для покупки: ", 1, numeric_limits<int>::max());
    for (auto& toy : toys) {
        if (toy.id == id) {
            if (toy.quantity < quantity) {
                cout << "Недостатньо іграшок на складі!" << endl;
                return;
            }
            while (true) {
                cout << "Ви впевнені, що хочете купити " << quantity << " шт. цієї іграшки? (так/ні): ";
                string confirmation;
                cin >> confirmation;
                if (confirmation == "так") {
                    toy.quantity -= quantity;
                    double totalPrice = toy.price * quantity;
                    cout << "Ви купили " << quantity << " шт. іграшки \"" << toy.name << "\" на суму " << totalPrice << " " << toy.currency << "." << endl;
                    if (toy.quantity == 0) {
                        toys.erase(remove_if(toys.begin(), toys.end(), [&toy](const Toy& t) { return t.id == toy.id; }), toys.end());
                    }
                    return;
                }
                else if (confirmation == "ні") {
                    cout << "Покупку скасовано." << endl;
                    return;
                }
                else {
                    cout << "Неправильне введення, спробуйте ще раз." << endl;
                }
            }
        }
    }
    cout << "Іграшку не знайдено!" << endl;
}

int main() {
    setlocale(LC_ALL, "uk_UA.UTF-8");
    locale::global(locale("uk_UA.UTF-8"));
    wcout.imbue(locale("uk_UA.UTF-8"));

    while (true) {
        cout << "1. Додати іграшку\n2. Видалити іграшку\n3. Редагувати іграшку\n4. Переглянути список іграшок\n5. Пошук іграшки\n6. Купити іграшку\n7. Вихід\n";
        int choice = getInput<int>("Зробіть вибір: ", 1, 7);

        switch (choice) {
        case 1: addToy(); break;
        case 2: deleteToy(); break;
        case 3: editToy(); break;
        case 4: listToys(); break;
        case 5: searchToys(); break;
        case 6: buyToy(); break;
        case 7: return 0;
        default: cout << "Неправильний вибір, спробуйте ще раз." << endl; break;
        }
    }
    return 0;
}