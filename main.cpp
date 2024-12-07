#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QFont>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QDebug>
#include <QSpacerItem>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QApplication>
#include <QObject>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QButtonGroup>
#include <QPixmap>
#include <cstdlib>  // Для rand() и srand()
#include <ctime>
#include <cmath>

struct diagonal {           // структура только для диагональных элементов
    double value;
    int i;
};

struct not_diagonal {       // структура для всех остальных элементов
    double value;
    int i;
    int j;
};

// Объявление указателей для хранения матриц
diagonal* pack_diagonal1 = nullptr;
not_diagonal* pack_notdiagonal1 = nullptr;
diagonal* pack_diagonal2 = nullptr;
not_diagonal* pack_notdiagonal2 = nullptr;
diagonal* pack_diagonal3 = nullptr;
not_diagonal* pack_notdiagonal3 = nullptr;
diagonal* pack_diagonal4 = nullptr;
not_diagonal* pack_notdiagonal4 = nullptr;
diagonal* pack_diagonal5 = nullptr;
not_diagonal* pack_notdiagonal5 = nullptr;

int count_notdiagonal1 = 0; int count_diagonal1 = 0;
int count_notdiagonal2 = 0; int count_diagonal2 = 0;
int count_notdiagonal3 = 0; int count_diagonal3 = 0;
int count_notdiagonal4 = 0; int count_diagonal4 = 0;
int count_notdiagonal5 = 0; int count_diagonal5 = 0;


//Страница вывода матрицы
class ViewPage: public QWidget {
    Q_OBJECT

private:
    QLineEdit *dimensionInput; // Поле ввода для размерности

public:
    ViewPage(QStackedWidget *stackedWidget, int* matrixSize, bool* filledArray, QWidget *parent = nullptr) :
        QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Создаем горизонтальный макет для размещения лейбла и поля ввода рядом
        QHBoxLayout *inputLayout = new QHBoxLayout();
        QLabel *dimensionLabel = new QLabel("Размерность разреженной матрицы:", this);
        QFont dimensionFont;
        dimensionFont.setPointSize(15);
        dimensionLabel->setFont(dimensionFont);
        inputLayout->addWidget(dimensionLabel);

        QFont buttonFont;
        buttonFont.setPointSize(16); // Устанавливаем размер шрифта для кнопки

        layout->addLayout(inputLayout);
        layout->addStretch();

        // Создаем матрицу (например, 10x10)
        const int rows = 10;
        const int columns = 3;

        // Создаем QTableWidget
        QTableWidget *table = new QTableWidget();
        table->setRowCount(rows);
        table->setColumnCount(columns);

        // Заполняем таблицу данными
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                table->setItem(i, j, new QTableWidgetItem(QString::number(i * columns + j)));
            }
        }

        table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //Запрет редактирования ячеек

        // Настраиваем заголовки столбцов и строк
        QStringList headerLabels;
        headerLabels << QString("Номер строки");
        headerLabels << QString("Номер столбца");
        headerLabels << QString("Значение");
        table->setHorizontalHeaderLabels(headerLabels); //названия столбцов
        QStringList rowLabels;
        for (int i = 0; i < rows; ++i) {
            rowLabels << QString("%1").arg(i+1);
        }
        table->setVerticalHeaderLabels(rowLabels);      //номера строк

        // Создаем QScrollArea и устанавливаем ее виджетом
        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(table);

        // Добавляем в окно таблицу со скроллом
        layout->addWidget(scrollArea);

        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        buttonFont.setPointSize(16);

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(buttonFont);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(buttonFont);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        exitLayout->setContentsMargins(0, 10, 0, 0); // Отступы сверху 10 для визуального разделения
        exitLayout->setSpacing(10);

        layout->addLayout(exitLayout);
        setLayout(layout);

        connect(exitButton, &QPushButton::clicked, this, &ViewPage::exitApplication);
        connect(backButton, &QPushButton::clicked, this, &ViewPage::onBackButtonClicked);
    }

signals:
    void backButtonClicked(); // Сигнал для возврата

private slots:
    void exitApplication() {
        QApplication::quit(); // Завершение приложения
    }
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(0); // Эмитируем сигнал назад
    }

private:
    int selectedMatrixIndex;
    int* size;
    bool* filled;
    QStackedWidget *stackedWidget;
};


//ЗАПОЛНЕНИЕ С КЛАВИАТУРЫ: траница ввода с клавиатуры в таблицу с элементами
class InputKeyboard: public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int* count_diagonal;                  //кол-во диагональных элементов матрицы
    int* count_notdiagonal;               //кол-во не диагональных элементов матрицы
    diagonal* pack_diagonal;              //диагональная матрица
    not_diagonal* pack_notdiagonal;       //не диагональная матрица
    int selectedMatrixIndex;              //номер выбранной матрицы
    QTableWidget *table;                  //таблица значений матрицы
    QStackedWidget *stackedWidget;

public:
    InputKeyboard(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, int selectedMatrixIndex, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray),
        selectedMatrixIndex(selectedMatrixIndex){

        count_diagonal = new int(0);
        count_notdiagonal = new int(0);

        QVBoxLayout *layout = new QVBoxLayout(this);
        QHBoxLayout *inputLayout = new QHBoxLayout();
        QLabel *dimensionLabel = new QLabel("Размерность разреженной матрицы:", this);

        QFont Font;
        Font.setPointSize(16);

        dimensionLabel->setFont(Font);
        inputLayout->addWidget(dimensionLabel);

        layout->addLayout(inputLayout);
        layout->addStretch();

        //ТАБЛИЦА
        const int rows = 10;
        const int columns = 3;
        table = new QTableWidget(rows, columns, this);
        QStringList headerLabels;
        headerLabels << QString("Номер строки") << QString("Номер столбца") << QString("Значение");
        table->setHorizontalHeaderLabels(headerLabels);

        QStringList rowLabels;
        for (int i = 0; i < rows; ++i) {
            rowLabels << QString("%1").arg(i + 1);
        }
        table->setVerticalHeaderLabels(rowLabels);

        //СКРОЛЛ
        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(table);

        layout->addWidget(scrollArea);

        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* saveButton = new QPushButton("Сохранить", this); // Adding Save button
        saveButton->setFont(Font);
        saveButton->setFixedSize(200, 50);
        exitLayout->addWidget(saveButton);

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        exitLayout->setContentsMargins(0, 10, 0, 0); // Отступы сверху 10 для визуального разделения
        exitLayout->setSpacing(10);
        layout->addLayout(exitLayout);

        setLayout(layout);

        //ПЕРЕХОДЫ ПО КНОПКАМ
        connect(backButton, &QPushButton::clicked, this, &InputKeyboard::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, &InputKeyboard::exitApplication);
        connect(saveButton, &QPushButton::clicked, this, &InputKeyboard::saveData);
    }

private slots:
    //СИГНАЛ ЗАВЕРШЕНИЯ
    void exitApplication() {
        QApplication::quit();
    }
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(2);
    }

    //ЗАПОЛНЕНИЕ МАТРИЦЫ ЗНАЧЕНИЯМИ ИЗ ТАБЛИЦЫ
    void saveData() {
        int n = size[selectedMatrixIndex - 1]; // Используем размерность выбранной матрицы
        int MAX_NOT_DIAGONAL = n * n * 0.15;

        // Выделение памяти для нужной матрицы в зависимости от индекса
        pack_diagonal = new diagonal[n];
        pack_notdiagonal = new not_diagonal[MAX_NOT_DIAGONAL];

        *count_diagonal = 0;
        *count_notdiagonal = 0;

        for (int row = 0; row < table->rowCount(); ++row) {
            QTableWidgetItem* itemRow = table->item(row, 0);
            QTableWidgetItem* itemCol = table->item(row, 1);
            QTableWidgetItem* itemValue = table->item(row, 2);

            if (itemRow && itemCol && itemValue) {
                bool okRow, okCol, okValue;
                int realRowNumber = itemRow->text().toInt(&okRow);
                int realColNumber = itemCol->text().toInt(&okCol);
                double value = itemValue->text().toDouble(&okValue);

                if (okRow && okCol && okValue && realRowNumber != 0 && realColNumber != 0) {
                    if (realRowNumber == realColNumber) {
                        if (*count_diagonal < n) {
                            pack_diagonal[*count_diagonal] = { value, realRowNumber };
                            (*count_diagonal)++;
                        }
                    } else {
                        if (*count_notdiagonal < MAX_NOT_DIAGONAL) {
                            pack_notdiagonal[*count_notdiagonal] = { value, realRowNumber, realColNumber };
                            (*count_notdiagonal)++;
                        }
                    }
                }
            }
        }
        qDebug() << "Сохранение данных для выбранной матрицы с индексом:" << selectedMatrixIndex;
        switch(selectedMatrixIndex){
        case 1:
            pack_diagonal1 = pack_diagonal;
            pack_notdiagonal1 = pack_notdiagonal;
            count_diagonal1 = *count_diagonal;
            count_notdiagonal1 = *count_notdiagonal;
            break;
        case 2:
            pack_diagonal2 = pack_diagonal;
            pack_notdiagonal2 = pack_notdiagonal;
            count_diagonal2 = *count_diagonal;
            count_notdiagonal2 = *count_notdiagonal;
            break;
        case 3:
            pack_diagonal3 = pack_diagonal;
            pack_notdiagonal3 = pack_notdiagonal;
            count_diagonal3 = *count_diagonal;
            count_notdiagonal3 = *count_notdiagonal;
            break;
        case 4:
            pack_diagonal4 = pack_diagonal;
            pack_notdiagonal4 = pack_notdiagonal;
            count_diagonal4 = *count_diagonal;
            count_notdiagonal4 = *count_notdiagonal;
            break;
        case 5:
            pack_diagonal5 = pack_diagonal;
            pack_notdiagonal5 = pack_notdiagonal;
            count_diagonal5 = *count_diagonal;
            count_notdiagonal5 = *count_notdiagonal;
            break;
        }

        printSavedData();
        stackedWidget->setCurrentIndex(0);
    }

    void printSavedData() {
        qDebug() << "Текущие значения счетчиков:";
        qDebug() << "count_diagonal1:" << count_diagonal1;
        qDebug() << "count_notdiagonal1:" << count_notdiagonal1;
        qDebug() << "count_diagonal2:" << count_diagonal2;
        qDebug() << "count_notdiagonal2:" << count_notdiagonal2;
        qDebug() << "count_diagonal3:" << count_diagonal3;
        qDebug() << "count_notdiagonal3:" << count_notdiagonal3;
        qDebug() << "count_diagonal4:" << count_diagonal4;
        qDebug() << "count_notdiagonal4:" << count_notdiagonal4;

        //----------------------------------------------------
        qDebug() << "Диагональные элементы (1):";
        for (int i = 0; i < count_diagonal1; ++i) {
            qDebug() << "Значение:" << pack_diagonal1[i].value << ", Индекс:" << pack_diagonal1[i].i;
        }
        qDebug() << "Недиагональные элементы (1):";
        for (int i = 0; i < count_notdiagonal1; ++i) {
            qDebug() << "Значение:" << pack_notdiagonal1[i].value << ", Индекс строки:" << pack_notdiagonal1[i].i << ", Индекс столбца:" << pack_notdiagonal1[i].j;
        }

        qDebug() << "Диагональные элементы (2):";
        for (int i = 0; i < count_diagonal2; ++i) {
            qDebug() << "Значение:" << pack_diagonal2[i].value << ", Индекс:" << pack_diagonal2[i].i;
        }
        qDebug() << "Недиагональные элементы (2):";
        for (int i = 0; i < count_notdiagonal2; ++i) {
            qDebug() << "Значение:" << pack_notdiagonal2[i].value << ", Индекс строки:" << pack_notdiagonal2[i].i << ", Индекс столбца:" << pack_notdiagonal2[i].j;
        }

        qDebug() << "Диагональные элементы (3):";
        for (int i = 0; i < count_diagonal3; ++i) {
            qDebug() << "Значение:" << pack_diagonal3[i].value << ", Индекс:" << pack_diagonal3[i].i;
        }
        qDebug() << "Недиагональные элементы (3):";
        for (int i = 0; i < count_notdiagonal3; ++i) {
            qDebug() << "Значение:" << pack_notdiagonal3[i].value << ", Индекс строки:" << pack_notdiagonal3[i].i << ", Индекс столбца:" << pack_notdiagonal3[i].j;
        }

        qDebug() << "Диагональные элементы (4):";
        for (int i = 0; i < count_diagonal4; ++i) {
            qDebug() << "Значение:" << pack_diagonal4[i].value << ", Индекс:" << pack_diagonal4[i].i;
        }
        qDebug() << "Недиагональные элементы (4):";
        for (int i = 0; i < count_notdiagonal4; ++i) {
            qDebug() << "Значение:" << pack_notdiagonal4[i].value << ", Индекс строки:" << pack_notdiagonal4[i].i << ", Индекс столбца:" << pack_notdiagonal4[i].j;
        }
    }
};


//ЗАПОЛНЕНИЕ С КЛАВИАТУРЫ: ввод размерности матрицы
class InputKeyboardPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QStackedWidget *stackedWidget;
    QLineEdit *dimensionInput;            //поле ввода размерности

public:
    InputKeyboardPage(QStackedWidget *stackedWidget, int* matrixSize, bool* filledArray, QWidget *parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {

        QVBoxLayout *layout = new QVBoxLayout(this);
        QHBoxLayout *inputLayout = new QHBoxLayout();

        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        layout->addSpacing(70);
        QLabel *dimensionLabel = new QLabel("Введите размерность разреженной матрицы:", this);
        dimensionLabel->setFont(Fontstr);
        inputLayout->addWidget(dimensionLabel);

        //ВВОД РАЗМЕРНОСТИ
        dimensionInput = new QLineEdit(this);
        dimensionInput->setFixedWidth(100);
        dimensionInput->setFont(QFont("", 15));
        inputLayout->addWidget(dimensionInput);

        QLabel *dynamicTextLabel = new QLabel("х ", this);
        dynamicTextLabel->setFont(Font);
        inputLayout->addWidget(dynamicTextLabel);

        QPushButton *continueButton = new QPushButton("Продолжить", this);
        continueButton->setFixedSize(200, 50);
        continueButton->setFont(Font);
        inputLayout->addWidget(continueButton);

        inputLayout->setContentsMargins(0, 0, 0, 0);
        inputLayout->setSpacing(5);

        layout->addLayout(inputLayout);

        //ГЕНЕРАЦИЯ ТЕКСТА ИЗ ПОЛЯ ВВОДА
        connect(dimensionInput, &QLineEdit::textChanged, this, [=](const QString &text) {
            dynamicTextLabel->setText("х " + text);
        });

        layout->addStretch();

        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        exitLayout->setContentsMargins(0, 10, 0, 0); // Отступы сверху 10 для визуального разделения
        exitLayout->setSpacing(10);

        layout->addLayout(exitLayout);
        setLayout(layout);

        connect(continueButton, &QPushButton::clicked, this, &InputKeyboardPage::handleContinue);
        connect(backButton, &QPushButton::clicked, this, &InputKeyboardPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, &InputKeyboardPage::exitApplication);
    }
    //СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
    void setSelectedMatrixIndex(int index) {
        selectedMatrixIndex = index;

        //очищение поля ввода
        if (selectedMatrixIndex > 0 && selectedMatrixIndex <= 5) {
            dimensionInput->clear();
        }
        qDebug() << "Выбрана матрица с индексом:" << index;
    }

signals:
    void showNextPage();

private slots:
    //сигнал завершения
    void exitApplication() {
        QApplication::quit();
    }
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(2);
    }

    //ПЕРЕХОД К ВВОДУ ЗНАЧЕНИЙ С КЛАВИАТУРЫ
    void handleContinue() {
        bool ok;
        int dimension = dimensionInput->text().toInt(&ok); // Берём текст из поля и преобразуем в int

        if (ok) { // Проверяем, что преобразование прошло успешно
            if (selectedMatrixIndex > 0 && selectedMatrixIndex <= 5) {
                size[selectedMatrixIndex - 1] = dimension; // Сохраняем введённое значение в массив

                // Печатаем текущее состояние массива size через qDebug
                qDebug() << "Текущая размерность матриц:";
                for (int i = 0; i < 5; ++i) {
                    qDebug() << "Матрица" << (i + 1) << ":" << size[i];
                }

                InputKeyboard* keyboardPage = nullptr;
                keyboardPage = new InputKeyboard(stackedWidget, size, filled, selectedMatrixIndex, this);
                stackedWidget->addWidget(keyboardPage);
                stackedWidget->setCurrentWidget(keyboardPage);
            }
        }
    }
};


//ЗАПОЛНЕНИЕ ЧЕРЕЗ ФАЙЛ
class InputFilePage : public QWidget {
    Q_OBJECT

private:
    int* size;
    bool* filled;
    int selectedMatrixIndex;
    QStackedWidget *stackedWidget;

public:
    InputFilePage(QStackedWidget *stackedWidget,  int* matrixSize, bool* filledArray, QWidget *parent = nullptr)
        :QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QHBoxLayout *inputLayout = new QHBoxLayout();
        QLabel *dimensionLabel = new QLabel("Выбор файла:", this);
        QFont dimensionFont;
        dimensionFont.setPointSize(18);
        dimensionLabel->setFont(dimensionFont);

        QFont buttonFont;
        buttonFont.setPointSize(16);

        QPushButton *choiceButton = new QPushButton("Выбрать", this);
        choiceButton->setFont(buttonFont);
        choiceButton->setFixedSize(200, 50);

        inputLayout->addWidget(dimensionLabel);
        inputLayout->addWidget(choiceButton);
        layout->addLayout(inputLayout);

        // Подключаем сигнал нажатия кнопки к слоту
        connect(choiceButton, &QPushButton::clicked, [=]() {
            // Открываем диалог выбора файла
            QString fileName = QFileDialog::getOpenFileName(this, "Выбрать файл");
        });

        layout->addStretch();

        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        buttonFont.setPointSize(16);

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(buttonFont);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(buttonFont);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        exitLayout->setContentsMargins(0, 10, 0, 0);
        exitLayout->setSpacing(10);

        layout->addLayout(exitLayout);
        setLayout(layout);

        connect(backButton, &QPushButton::clicked, this, &InputFilePage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, &InputFilePage::exitApplication);

    }
    void setSelectedMatrixIndex(int index) {
        qDebug() << "Выбрана матрица с индексом:" << index;
    }

private slots:
    void exitApplication() {
        QApplication::quit(); // Завершение приложения
    }
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(2); // Переход к выбору матрицы
    }
};


//ЗАПОЛНЕНИЕ СЛУЧАЙНЫМ ОБРАЗОМ
class RandomInputPage : public QWidget {
    Q_OBJECT
private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int* count_diagonal;                  //кол-во диагональных элементов матрицы
    int* count_notdiagonal;               //кол-во не диагональных элементов матрицы
    diagonal* pack_diagonal;              //диагональная матрица
    not_diagonal* pack_notdiagonal;       //не диагональная матрица
    int selectedMatrixIndex;              //номер выбранной матрицы
    QLineEdit *dimensionInput;            //поле ввода размерности
    QStackedWidget *stackedWidget;
    QString strn;                            //размерность

    double generate() {
        double value;
        if (rand() % 2 == 0) {                   // 50% шанс на целое число
            return rand() % 2001 - 1000;         // Генерация целого числа от -1000 до 1000
        } else {
            // Генерируем дробное число
            value = (rand() % 2001 - 1000) + (rand() / static_cast<double>(RAND_MAX));
            return round(value * 100.0) / 100.0; // Округление до двух знаков после запятой
        }
    }

public:
    RandomInputPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, int selectedMatrixIndex, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(selectedMatrixIndex){

        srand(static_cast<unsigned int>(time(0)));
        count_diagonal = new int(0);
        count_notdiagonal = new int(0);

        QVBoxLayout *layout = new QVBoxLayout(this);
        QHBoxLayout *inputLayout = new QHBoxLayout();
        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        layout->addSpacing(70);
        QLabel *dimensionLabel = new QLabel("Введите размерность разреженной матрицы:", this);
        dimensionLabel->setFont(Fontstr);
        inputLayout->addWidget(dimensionLabel);

        //ВВОД РАЗМЕРНОСТИ
        dimensionInput = new QLineEdit(this);
        dimensionInput->setFixedWidth(100);
        dimensionInput->setFont(QFont("", 15));
        inputLayout->addWidget(dimensionInput);

        QLabel *dynamicTextLabel = new QLabel("х ", this);
        dynamicTextLabel->setFont(Font);
        inputLayout->addWidget(dynamicTextLabel);

        QPushButton *continueButton = new QPushButton("Сохранить", this);
        continueButton->setFixedSize(200, 50);
        continueButton->setFont(Font);
        inputLayout->addWidget(continueButton);

        inputLayout->setContentsMargins(0, 0, 0, 0);
        inputLayout->setSpacing(5);
        layout->addLayout(inputLayout);

        //ГЕНЕРАЦИЯ ТЕКСТА ИЗ ПОЛЯ ВВОДА
        connect(dimensionInput, &QLineEdit::textChanged, this, [=](const QString &text) {
            dynamicTextLabel->setText("х " + text);
        });

        // ЧЕКБОКСЫ
        layout->addSpacing(80);
        QCheckBox *nonZeroValuesCheckbox = new QCheckBox("Ввод количества ненулевых значений", this);
        nonZeroValuesCheckbox->setFont(Font);
        layout->addWidget(nonZeroValuesCheckbox, 0, Qt::AlignLeft);

        //добавляем поле ввода после первого чекбокса
        QLineEdit *nonZeroValuesInput = new QLineEdit(this);
        nonZeroValuesInput->setFixedWidth(100);
        nonZeroValuesInput->setEnabled(false);
        layout->addWidget(nonZeroValuesInput, 0, Qt::AlignLeft);

        connect(nonZeroValuesCheckbox, &QCheckBox::toggled, this, [=](bool checked) {
            nonZeroValuesInput->setEnabled(checked); // Активируем или деактивируем второе поле ввода
            if (!checked) {
                nonZeroValuesInput->clear(); // Очищаем поле ввода, если чекбокс снят
            }
        });

        layout->addSpacing(20);
        QLabel *orLabel = new QLabel("ИЛИ", this);
        orLabel->setFont(Fontstr);
        layout->addWidget(orLabel, 0, Qt::AlignLeft);

        layout->addSpacing(25);
        QCheckBox *tenPercentCheckbox = new QCheckBox("Сгенерировать 10% ненулевых значений от размерности матрицы", this);
        tenPercentCheckbox->setFont(Font);
        layout->addWidget(tenPercentCheckbox, 0, Qt::AlignLeft);

        QButtonGroup *checkboxGroup = new QButtonGroup(this);
        checkboxGroup->setExclusive(true); // Обеспечить взаимное исключение
        checkboxGroup->addButton(nonZeroValuesCheckbox);
        checkboxGroup->addButton(tenPercentCheckbox);

        layout->addStretch();

        //КНОПКИ
        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton *generateButton = new QPushButton("Сгенерировать", this);
        generateButton->setFont(Font);
        generateButton->setFixedSize(200, 50);
        exitLayout->addWidget(generateButton);

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        exitLayout->setContentsMargins(0, 10, 0, 0); // Отступы сверху 10 для визуального разделения
        exitLayout->setSpacing(10);
        layout->addLayout(exitLayout);

        setLayout(layout);

        connect(continueButton, &QPushButton::clicked, this, [=]() {
            strn = dimensionInput->text();
            qDebug() << "Сохраненное значение:" << strn; // Выводим его в отладочное окно
        });

        connect(generateButton, &QPushButton::clicked, this, [=]() {
            int n = strn.toInt();

            // Выделение динамической памяти для матриц
            pack_diagonal = new diagonal[n]; // Выделяем память для диагональных элементов
            pack_notdiagonal = new not_diagonal[static_cast<int>(floor(n * n * 0.15))]; // 15% от n*n для недиагональных элементов

            if (nonZeroValuesCheckbox->isChecked() && !nonZeroValuesInput->text().isEmpty()) {
                int countValues = nonZeroValuesInput->text().toInt();
                generateMatrix(n, countValues);
            }
            else if (tenPercentCheckbox->isChecked()) {
                int countValues = static_cast<int>(floor(n * n * 0.1)); // 10% от n
                generateMatrix(n, countValues);
            }

            // Логирование результатов
            qDebug() << "Сгенерировано" << *count_diagonal << "диагональных и" << *count_notdiagonal << "недиагональных элементов.";
        });

        connect(backButton, &QPushButton::clicked, this, &RandomInputPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, &RandomInputPage::exitApplication);
    }

    void generateMatrix(int n, int countValues) {
        *count_diagonal = 0;
        *count_notdiagonal = 0;
        bool* occupied = new bool[n * n]();

        int random_number = rand() % (countValues + 1);
        int diag_el, notdiag_el;
        if (random_number < (n / 10)) {
            diag_el = random_number;
            notdiag_el = countValues - random_number;
        }
        else {
            notdiag_el = random_number;
            diag_el = countValues - random_number;
        }

        while (*count_diagonal < diag_el || *count_notdiagonal < notdiag_el) {
            int i = rand() % n + 1;
            int j = rand() % n + 1;
            double value = generate();

            if (i == j) {
                if (*count_diagonal < diag_el && !occupied[(i - 1) * n + (j - 1)]) {
                    pack_diagonal[*count_diagonal].value = value;
                    pack_diagonal[*count_diagonal].i = i;
                    occupied[(i - 1) * n + (j - 1)] = true;
                    (*count_diagonal)++;
                }
            } else {
                int j = rand() % n;
                if (*count_notdiagonal < notdiag_el && !occupied[(i - 1) * n + (j - 1)]) {
                    pack_notdiagonal[*count_notdiagonal].value = value;
                    pack_notdiagonal[*count_notdiagonal].i = i;
                    pack_notdiagonal[*count_notdiagonal].j = j;
                    occupied[i * n + j] = true;
                    (*count_notdiagonal)++;
                }
            }
        }
        delete[] occupied; // Освобождение выделенной памяти

        qDebug() << "Сохранение данных для выбранной матрицы с индексом:" << selectedMatrixIndex;
        switch(selectedMatrixIndex){
        case 1:
            pack_diagonal1 = pack_diagonal;
            pack_notdiagonal1 = pack_notdiagonal;
            count_diagonal1 = *count_diagonal;
            count_notdiagonal1 = *count_notdiagonal;
            break;
        case 2:
            pack_diagonal2 = pack_diagonal;
            pack_notdiagonal2 = pack_notdiagonal;
            count_diagonal2 = *count_diagonal;
            count_notdiagonal2 = *count_notdiagonal;
            break;
        case 3:
            pack_diagonal3 = pack_diagonal;
            pack_notdiagonal3 = pack_notdiagonal;
            count_diagonal3 = *count_diagonal;
            count_notdiagonal3 = *count_notdiagonal;
            break;
        case 4:
            pack_diagonal4 = pack_diagonal;
            pack_notdiagonal4 = pack_notdiagonal;
            count_diagonal4 = *count_diagonal;
            count_notdiagonal4 = *count_notdiagonal;
            break;
        case 5:
            pack_diagonal5 = pack_diagonal;
            pack_notdiagonal5 = pack_notdiagonal;
            count_diagonal5 = *count_diagonal;
            count_notdiagonal5 = *count_notdiagonal;
            break;
        }

        printSavedData();
        stackedWidget->setCurrentIndex(0);
    }

    void printSavedData() {
        qDebug() << "Текущие значения счетчиков:";
        qDebug() << "count_diagonal1:" << count_diagonal1;
        qDebug() << "count_notdiagonal1:" << count_notdiagonal1;
        qDebug() << "count_diagonal2:" << count_diagonal2;
        qDebug() << "count_notdiagonal2:" << count_notdiagonal2;
        qDebug() << "count_diagonal3:" << count_diagonal3;
        qDebug() << "count_notdiagonal3:" << count_notdiagonal3;
        qDebug() << "count_diagonal4:" << count_diagonal4;
        qDebug() << "count_notdiagonal4:" << count_notdiagonal4;

        //----------------------------------------------------
        qDebug() << "Диагональные элементы (1):";
        for (int i = 0; i < count_diagonal1; ++i) {
            qDebug() << "Значение:" << pack_diagonal1[i].value << ", Индекс:" << pack_diagonal1[i].i;
        }
        qDebug() << "Недиагональные элементы (1):";
        for (int i = 0; i < count_notdiagonal1; ++i) {
            qDebug() << "Значение:" << pack_notdiagonal1[i].value << ", Индекс строки:" << pack_notdiagonal1[i].i << ", Индекс столбца:" << pack_notdiagonal1[i].j;
        }

        qDebug() << "Диагональные элементы (2):";
        for (int i = 0; i < count_diagonal2; ++i) {
            qDebug() << "Значение:" << pack_diagonal2[i].value << ", Индекс:" << pack_diagonal2[i].i;
        }
        qDebug() << "Недиагональные элементы (2):";
        for (int i = 0; i < count_notdiagonal2; ++i) {
            qDebug() << "Значение:" << pack_notdiagonal2[i].value << ", Индекс строки:" << pack_notdiagonal2[i].i << ", Индекс столбца:" << pack_notdiagonal2[i].j;
        }

        qDebug() << "Диагональные элементы (3):";
        for (int i = 0; i < count_diagonal3; ++i) {
            qDebug() << "Значение:" << pack_diagonal3[i].value << ", Индекс:" << pack_diagonal3[i].i;
        }
        qDebug() << "Недиагональные элементы (3):";
        for (int i = 0; i < count_notdiagonal3; ++i) {
            qDebug() << "Значение:" << pack_notdiagonal3[i].value << ", Индекс строки:" << pack_notdiagonal3[i].i << ", Индекс столбца:" << pack_notdiagonal3[i].j;
        }

        qDebug() << "Диагональные элементы (4):";
        for (int i = 0; i < count_diagonal4; ++i) {
            qDebug() << "Значение:" << pack_diagonal4[i].value << ", Индекс:" << pack_diagonal4[i].i;
        }
        qDebug() << "Недиагональные элементы (4):";
        for (int i = 0; i < count_notdiagonal4; ++i) {
            qDebug() << "Значение:" << pack_notdiagonal4[i].value << ", Индекс строки:" << pack_notdiagonal4[i].i << ", Индекс столбца:" << pack_notdiagonal4[i].j;
        }
    }

    //СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
    void setSelectedMatrixIndex(int index) {
        selectedMatrixIndex = index;

        //очищение поля ввода
        if (selectedMatrixIndex > 0 && selectedMatrixIndex <= 5) {
            dimensionInput->clear();
        }
        qDebug() << "Выбрана матрица с индексом:" << index;
    }

signals:
    void showNextPage();

private slots:
    //сигнал завершения
    void exitApplication() {
        QApplication::quit();
    }
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(2);
    }
};


//ВЫБОР МАТРИЦЫ ДЛЯ ВВОДА
class InputPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QStackedWidget *stackedWidget;
    QLineEdit *dimensionInput;            //поле ввода размерности
    QString inputMethod;                  //выбранный метод ввода
    QCheckBox *matrix1;
    QCheckBox *matrix2;
    QCheckBox *matrix3;
    QCheckBox *matrix4;
    QCheckBox *matrix5;

public:
    InputPage(QStackedWidget *stackedWidget, int* matrixSize, bool* filledArray, QWidget *parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {

        QVBoxLayout *layout = new QVBoxLayout(this);

        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        QLabel *titleLabel = new QLabel("Выберите матрицу, в которую нужно ввести значения:", this);
        titleLabel->setAlignment(Qt::AlignCenter);;
        titleLabel->setFont(Fontstr);
        layout->addWidget(titleLabel);

        //ЧЕКБОКСЫ ВЫБОРА МАТРИЦЫ
        QVBoxLayout *checkBoxLayout = new QVBoxLayout();
        matrix1 = new QCheckBox("Матрица №1", this);
        matrix2 = new QCheckBox("Матрица №2", this);
        matrix3 = new QCheckBox("Матрица №3", this);
        matrix4 = new QCheckBox("Матрица №4", this);
        matrix5 = new QCheckBox("Матрица №5", this);

        //РАЗМЕР ТЕКСТА
        matrix1->setFont(Font);
        matrix2->setFont(Font);
        matrix3->setFont(Font);
        matrix4->setFont(Font);
        matrix5->setFont(Font);

        connect(matrix1, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);
        connect(matrix2, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);
        connect(matrix3, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);
        connect(matrix4, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);
        connect(matrix5, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);

        checkBoxLayout->addWidget(matrix1);
        checkBoxLayout->addWidget(matrix2);
        checkBoxLayout->addWidget(matrix3);
        checkBoxLayout->addWidget(matrix4);
        checkBoxLayout->addWidget(matrix5);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(checkBoxLayout);

        // Кнопка "Продолжить"
        QPushButton *continueButton = new QPushButton("Продолжить", this);
        continueButton->setFixedSize(290, 50);
        continueButton->setFont(Font);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);

        //КНОПКИ
        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        layout->addLayout(exitLayout);
        setLayout(layout);
        layout->setSpacing(2);
        layout->setContentsMargins(5, 5, 5, 5);

        //ПЕРЕХОДЫ НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(backButton, &QPushButton::clicked, this, &InputPage::onBackButtonClicked);
        connect(continueButton, &QPushButton::clicked, this, &InputPage::onContinueButtonClicked);
        connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    }

public slots:
    //СОХРАНЕНИЕ ВЫБРАННОГО МЕТОДА ВВОДА МАТРИЦЫ
    void setInputMethod(const QString &method) {
        inputMethod = method;
    }

private slots:
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(1);
    }

    //РАБОТА С ЧЕКБОКСОМ И СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
    void onMatrixCheckBoxClicked() {
        QCheckBox* senderCheckBox = qobject_cast<QCheckBox*>(sender());
        if (senderCheckBox && senderCheckBox->isChecked()) {
            selectedMatrixIndex = (senderCheckBox == matrix1) ? 1 :
                                      (senderCheckBox == matrix2) ? 2 :
                                      (senderCheckBox == matrix3) ? 3 :
                                      (senderCheckBox == matrix4) ? 4 :
                                      (senderCheckBox == matrix5) ? 5 : -1;

            matrix1->setChecked(senderCheckBox == matrix1);
            matrix2->setChecked(senderCheckBox == matrix2);
            matrix3->setChecked(senderCheckBox == matrix3);
            matrix4->setChecked(senderCheckBox == matrix4);
            matrix5->setChecked(senderCheckBox == matrix5);
        }
    }

    //ПЕРЕХОДЫ НА СТРАНИЦЫ ВВОДА МАТРИЦЫ
    void onContinueButtonClicked() {
        if (inputMethod == "Ввести с клавиатуры") {
            InputKeyboardPage* keyboardPage = new InputKeyboardPage(stackedWidget, size, filled, this);
            keyboardPage->setSelectedMatrixIndex(selectedMatrixIndex);

            stackedWidget->addWidget(keyboardPage);
            stackedWidget->setCurrentWidget(keyboardPage);

        } else if (inputMethod == "Ввести из текстового файла") {
            InputFilePage *filePage = qobject_cast<InputFilePage*>(stackedWidget->widget(4));
            if (filePage) {
                filePage->setSelectedMatrixIndex(selectedMatrixIndex);
            }
            stackedWidget->setCurrentIndex(4);
        } else if (inputMethod == "Случайный ввод") {
            RandomInputPage *randomPage = qobject_cast<RandomInputPage*>(stackedWidget->widget(5));
            if (randomPage) {
                randomPage->setSelectedMatrixIndex(selectedMatrixIndex);
            }
            stackedWidget->setCurrentIndex(5);
        }
    }
};


//ВЫБОР ВВОДА МАТРИЦЫ
class NewMatrixPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    QStackedWidget *stackedWidget;

public:
    NewMatrixPage(QStackedWidget *stackedWidget, int* matrixSize, bool* filledArray, QWidget *parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

        //КНОПКИ
        QVBoxLayout *buttonLayout = new QVBoxLayout();
        QPushButton *input1 = new QPushButton("Ввести с клавиатуры", this);
        QPushButton *input2 = new QPushButton("Ввести из текстового файла", this);
        QPushButton *input3 = new QPushButton("Случайный ввод", this);

        //РАЗМЕР КНОПОК
        QSize buttonSize(290, 50);
        input1->setFixedSize(buttonSize);
        input2->setFixedSize(buttonSize);
        input3->setFixedSize(buttonSize);

        //РАЗМЕР ТЕКСТА КНОПОК
        QFont Font;
        Font.setPointSize(15);
        input1->setFont(Font);
        input2->setFont(Font);
        input3->setFont(Font);

        QFont Fontstr;
        Fontstr.setPointSize(18);
        QLabel *titleLabel = new QLabel("Выбор способа ввода разреженной матрицы", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setFont(Fontstr);
        titleLabel->setMinimumHeight(50);

        layout->addWidget(titleLabel);
        buttonLayout->addWidget(input1);
        buttonLayout->addWidget(input2);
        buttonLayout->addWidget(input3);
        buttonLayout->setAlignment(Qt::AlignCenter);
        buttonLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
        layout->addLayout(buttonLayout);
        layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

        //КНОПКИ
        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        layout->addLayout(exitLayout);
        setLayout(layout);

        //ПЕРЕХОД НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(input1, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        connect(input2, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        connect(input3, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        connect(backButton, &QPushButton::clicked, this, &NewMatrixPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    }

signals:
    void backButtonClicked();  //сигнал назад
    void inputMethodSelected(const QString &method);

private slots:
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(0);
    }
    //ПЕРЕХОД  К ВЫБОРУ МАТРИЦЫ
    void onInputMethodSelected() {
        QPushButton *button = qobject_cast<QPushButton*>(sender());
        if (button) {
            QString method = button->text();
            emit inputMethodSelected(method);
            stackedWidget->setCurrentIndex(2);
        }
    }
};


//ГЛАВНАЯ СТРАНИЦА
class MainWindows : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    QStackedWidget *stackedWidget;
    //кнопки
    QPushButton *newButton;
    QPushButton *viewButton;
    QPushButton *editButton;
    QPushButton *delButton;
    QPushButton *addButton;
    QPushButton *subButton;
    QPushButton *mulButton;
    QPushButton *inverseButton;
    QPushButton *quit;
    QLabel *dataLabel;
    QLabel *operationLabel;
    QGridLayout *buttonLayout;

public:
    MainWindows(int* matrixSize, bool* filledArray): size(matrixSize), filled(filledArray) {

        setWindowTitle("MathMat");
        setFixedSize(1000, 600);

        //главное представление с переключением страниц
        stackedWidget = new QStackedWidget(this);

        //ГЛАВНАЯ СТРАНИЦА
        QWidget *mainPage = new QWidget();
        QVBoxLayout *mainLayout = new QVBoxLayout(mainPage);

        //КНОПКИ
        newButton = new QPushButton("Новая матрица");
        viewButton = new QPushButton("Просмотр матрицы");
        editButton = new QPushButton("Редактировать");
        delButton = new QPushButton("Удалить");
        addButton = new QPushButton("Сложение двух матриц");
        subButton = new QPushButton("Вычитание двух матриц");
        mulButton = new QPushButton("Умножение двух матриц");
        inverseButton = new QPushButton("Обратная матрица");
        quit = new QPushButton("Выход");

        //РАЗМЕР КНОПОК
        QSize buttonSize(260, 50);
        QSize buttonSizeexit(200, 50);
        newButton->setFixedSize(buttonSize);
        viewButton->setFixedSize(buttonSize);
        editButton->setFixedSize(buttonSize);
        delButton->setFixedSize(buttonSize);
        addButton->setFixedSize(buttonSize);
        subButton->setFixedSize(buttonSize);
        mulButton->setFixedSize(buttonSize);
        inverseButton->setFixedSize(buttonSize);
        quit->setFixedSize(buttonSizeexit);

        //РАЗМЕР ТЕКСТА
        QFont Font;
        Font.setPointSize(15);
        newButton->setFont(Font);
        viewButton->setFont(Font);
        editButton->setFont(Font);
        delButton->setFont(Font);
        addButton->setFont(Font);
        subButton->setFont(Font);
        mulButton->setFont(Font);
        inverseButton->setFont(Font);
        quit->setFont(Font);

        QFont Fontstr;
        Fontstr.setPointSize(18);
        dataLabel = new QLabel("Работа с данными:");
        operationLabel = new QLabel("Выполнить операцию:");
        dataLabel->setAlignment(Qt::AlignCenter);
        operationLabel->setAlignment(Qt::AlignCenter);
        dataLabel->setFont(Fontstr);
        operationLabel->setFont(Fontstr);

        //размещение кнопок в макете
        buttonLayout = new QGridLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->addItem(new QSpacerItem(20, 150, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 0);
        buttonLayout->addWidget(dataLabel, 1, 0);
        buttonLayout->addWidget(newButton, 2, 0);
        buttonLayout->addWidget(viewButton, 3, 0);
        buttonLayout->addWidget(editButton, 4, 0);
        buttonLayout->addWidget(delButton, 5, 0);
        buttonLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), 6, 0);
        buttonLayout->addWidget(operationLabel, 1, 1);
        buttonLayout->addWidget(addButton, 2, 1);
        buttonLayout->addWidget(subButton, 3, 1);
        buttonLayout->addWidget(mulButton, 4, 1);
        buttonLayout->addWidget(inverseButton, 5, 1);
        buttonLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), 7, 0);

        mainLayout->addLayout(buttonLayout);

        //добавление кнопки "Выход" в главную страницу
        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        exitLayout->addWidget(quit);

        QSpacerItem *spacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        mainLayout->addItem(spacerItem);
        mainLayout->addLayout(exitLayout);

        //СТРАНИЦЫ
        stackedWidget->addWidget(mainPage);
        NewMatrixPage *newMatrixPage = new NewMatrixPage(stackedWidget, size, filled);
        InputPage *inputPage = new InputPage(stackedWidget, size, filled);
        InputKeyboardPage *inputKeyboardPage = new InputKeyboardPage(stackedWidget, size, filled);
        InputFilePage *inputFilePage = new InputFilePage(stackedWidget, size, filled);
        RandomInputPage *randomInputPage = new RandomInputPage(stackedWidget, size, filled, 1);
        InputKeyboard* inputKeyboard = new InputKeyboard(stackedWidget, size, filled, 1);
        ViewPage * viewPage = new ViewPage(stackedWidget, size, filled);
        stackedWidget->addWidget(newMatrixPage);
        stackedWidget->addWidget(inputPage);
        stackedWidget->addWidget(inputKeyboardPage);
        stackedWidget->addWidget(inputFilePage);
        stackedWidget->addWidget(randomInputPage);
        stackedWidget->addWidget(inputKeyboard);
        stackedWidget->addWidget(viewPage);

        QVBoxLayout *centralLayout = new QVBoxLayout(this);
        centralLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        centralLayout->addWidget(stackedWidget);
        centralLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        setLayout(centralLayout);

        //ПЕРЕХОД НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(newButton, &QPushButton::clicked, this, &MainWindows::showNewMatrixPage);
        connect(editButton, &QPushButton::clicked, this, &MainWindows::showEditPage);
        connect(newMatrixPage, &NewMatrixPage::inputMethodSelected, inputPage, &InputPage::setInputMethod);
        connect(viewButton, &QPushButton::clicked, this, &MainWindows::showViewPage);
        connect(quit, &QPushButton::clicked, qApp, &QApplication::quit);
    }

private slots:
    void showNewMatrixPage() {
        stackedWidget->setCurrentIndex(1);  //переход на окно ввода матрицы
    }
    void showMainPage() {
        stackedWidget->setCurrentIndex(0); //переход на главную страницу
    }
    void showViewPage(){
        stackedWidget->setCurrentIndex(7);  //переход на вывод матрицы
    }
    void showEditPage(){
        stackedWidget->setCurrentIndex(6);  //перевод на редактирование
    }
};



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "MathMat_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    int* size = new int[6];             //матрица размерности матриц
    bool* filled = new bool[6];         //матрица заполненности матриц
    for (int i = 0; i < 6; i++) {
        size[i] = 0;
        filled[i] = false;
    }
    MainWindows w(size, filled);
    w.show();
    return a.exec();
}

#include "main.moc"
