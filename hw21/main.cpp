//
//  main.cpp
//  hw21
//
//  Created by Valeria Dudinova on 06.12.2024.
//

#include <iostream>
using namespace std;

// Клас, який імітує unique_ptr без використання динамічної пам'яті
template <typename T>
class MyUniquePtr
{
private:
    T* ptr;

public:
    explicit MyUniquePtr(T* p = nullptr) : ptr(p) {}

    ~MyUniquePtr() {
        delete ptr;
    }

    MyUniquePtr(MyUniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr; // Обнуляємо вказівник іншого об'єкта
    }

    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {  // Перевірка на самоприсвоєння
            delete ptr;  // Видалення старого об'єкта
            ptr = other.ptr;  // Переміщаємо вказівник
            other.ptr = nullptr;  // Обнуляємо вказівник іншого об'єкта
        }
        return *this;
    }

    // Метод для отримання вказівника на об'єкт
    T* get() const {
        return ptr;
    }

    // Операція розіменування для доступу до об'єкта
    T& operator*() const {
        return *ptr;
    }

    // Операція -> для доступу до членів об'єкта
    T* operator->() const {
        return ptr;
    }

    // Унікальний вказівник не можна копіювати
    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;
};

template <typename T>
class MySharedPtr {
private:
    T* ptr;
    unsigned* count;
public:
    explicit MySharedPtr(T* p = nullptr) : ptr(p), count(new unsigned(1)) {}

    ~MySharedPtr() {
        if (--(*count) == 0) {
            delete ptr;
            delete count;
        }
    }

    MySharedPtr(const MySharedPtr& other) : ptr(other.ptr), count(other.count) {
        ++(*count);
    }

    MySharedPtr& operator=(const MySharedPtr& other) {
        if (this != &other) {
            if (--(*count) == 0) {
                delete ptr;
                delete count;
            }
            ptr = other.ptr;
            count = other.count;
            ++(*count);  // Збільшуємо лічильник посилань
        }
        return *this;
    }

    // Метод для отримання вказівника на об'єкт
    T* get() const {
        return ptr;
    }

    // Операція розіменування для доступу до об'єкта
    T& operator*() const {
        return *ptr;
    }

    // Операція -> для доступу до членів об'єкта
    T* operator->() const {
        return ptr;
    }
};

// Клас для тестування умних вказівників
class Test
{
public:
    void display()
    {
        cout << "Test object function called" << endl;
    }
};

int main() {
    // Тестування MyUniquePtr
    {
        MyUniquePtr<Test> uniquePtr1(new Test());  // Створюємо об'єкт і передаємо його в унікальний вказівник
        uniquePtr1->display();  // Викликаємо метод через вказівник

        // Переміщаємо унікальний вказівник
        MyUniquePtr<Test> uniquePtr2 = move(uniquePtr1);
        uniquePtr2->display();  // Викликаємо метод через новий вказівник

        // uniquePtr1 більше не доступний для використання
    }

    {
        MySharedPtr<Test> sharedPtr1(new Test());  // Створюємо об'єкт і передаємо його в shared_ptr
        sharedPtr1->display();

        {
            MySharedPtr<Test> sharedPtr2 = sharedPtr1;
            sharedPtr2->display();
        }

        // sharedPtr1 все ще існує, оскільки лічильник посилань > 0
    }

    return 0;
}

