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
    int* size;
    bool* filled;
    QTableWidget *table;
    QLineEdit *dimensionInput; // Поле ввода для размерности
    QStackedWidget *stackedWidget;

public:
    InputKeyboard(QStackedWidget *stackedWidget, int* matrixSize, bool* filledArray, QWidget *parent = nullptr) :
        QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {

        QVBoxLayout *layout = new QVBoxLayout(this);
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
        table = new QTableWidget(rows, columns, this);

        // Настраиваем заголовки столбцов и строк
        QStringList headerLabels;
        headerLabels << QString("Номер строки") << QString("Номер столбца") << QString("Значение");
        table->setHorizontalHeaderLabels(headerLabels);

        QStringList rowLabels;
        for (int i = 0; i < rows; ++i) {
            rowLabels << QString("%1").arg(i + 1);
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

        connect(backButton, &QPushButton::clicked, this, &InputKeyboard::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, &InputKeyboard::exitApplication);

    }

private slots:
    void exitApplication() {
        QApplication::quit(); // Завершение приложения
    }
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(2); // Эмитируем сигнал назад
    }
};


//ЗАПОЛНЕНИЕ С КЛАВИАТУРЫ: ввод размерности матрицы
class InputKeyboardPage : public QWidget {
    Q_OBJECT

private:
    int selectedMatrixIndex;
    int* size;
    bool* filled;
    QStackedWidget *stackedWidget;
    QLineEdit *dimensionInput; // Поле ввода для размерности

public:
    InputKeyboardPage(QStackedWidget *stackedWidget, int* matrixSize, bool* filledArray, QWidget *parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {

        QVBoxLayout *layout = new QVBoxLayout(this);
        QHBoxLayout *inputLayout = new QHBoxLayout();

        QLabel *dimensionLabel = new QLabel("Введите размерность разреженной матрицы:", this);
        QFont dimensionFont;
        dimensionFont.setPointSize(15);
        dimensionLabel->setFont(dimensionFont);
        inputLayout->addWidget(dimensionLabel);

        dimensionInput = new QLineEdit(this); // Здесь будет ввод размерности
        dimensionInput->setFixedWidth(200); // Увеличиваем фиксированную ширину для окна ввода
        dimensionInput->setFont(QFont("", 15)); // Устанавливаем размер шрифта для текста в QLineEdit
        inputLayout->addWidget(dimensionInput);

        QLabel *dynamicTextLabel = new QLabel("х ", this); // Инициализируем с "х"
        QFont xFont;
        xFont.setPointSize(15); // Устанавливаем размер шрифта для "х"
        dynamicTextLabel->setFont(xFont);
        inputLayout->addWidget(dynamicTextLabel);

        QPushButton *continueButton = new QPushButton("Продолжить", this);
        QFont buttonFont;
        buttonFont.setPointSize(16); // Устанавливаем размер шрифта для кнопки
        continueButton->setFixedSize(200, 50);
        continueButton->setFont(buttonFont);
        inputLayout->addWidget(continueButton);

        // Убираем промежутки между элементами
        inputLayout->setContentsMargins(0, 0, 0, 0); // Убираем отступы по краям
        inputLayout->setSpacing(5); // Устанавливаем небольшое расстояние (можно уменьшить)

        layout->addLayout(inputLayout);

        connect(dimensionInput, &QLineEdit::textChanged, this, [=](const QString &text) {
            dynamicTextLabel->setText("х " + text);
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

        exitLayout->setContentsMargins(0, 10, 0, 0); // Отступы сверху 10 для визуального разделения
        exitLayout->setSpacing(10);

        layout->addLayout(exitLayout);
        setLayout(layout);

        connect(continueButton, &QPushButton::clicked, this, &InputKeyboardPage::handleContinue);
        connect(backButton, &QPushButton::clicked, this, &InputKeyboardPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, &InputKeyboardPage::exitApplication);
    }
    void setSelectedMatrixIndex(int index) {
        selectedMatrixIndex = index; // Сохраняем индекс выбранной матрицы

        if (selectedMatrixIndex > 0 && selectedMatrixIndex <= 5) {
            dimensionInput->clear(); // Очищаем поле ввода при выборе новой матрицы
        }
        qDebug() << "Выбрана матрица с индексом:" << index;
    }

signals:
    void showNextPage();

private slots:
    void exitApplication() {
        QApplication::quit(); // Завершение приложения
    }

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
            }
            stackedWidget->setCurrentIndex(6); // Переключаемся на следующую страницу
        } else {
            // Здесь можно реализовать уведомление о некорректном вводе
            QMessageBox::warning(this, "Недопустимый ввод", "Пожалуйста, введите корректное число.");
        }
    }

    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(2); // Эмитируем сигнал назад
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
public:
    RandomInputPage(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(new QLabel("3Случайный ввод данных", this));

        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));


        QFont buttonFont;
        buttonFont.setPointSize(16);

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(buttonFont);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(buttonFont);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        layout->addLayout(exitLayout);
        setLayout(layout);
    }
    void setSelectedMatrixIndex(int index) {
        // Здесь сохраняйте индекс выбранной матрицы и используйте по необходимости
        // Например, запомнить, какую матрицу вводим
        // ...
        qDebug() << "Выбрана матрица с индексом:" << index;
    }
};


//ВЫБОР МАТРИЦЫ ДЛЯ ВВОДА
class InputPage : public QWidget {
    Q_OBJECT

public:
    InputPage(QStackedWidget *stackedWidget, int* matrixSize, bool* filledArray, QWidget *parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {
        // Основной вертикальный слой
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Заголовок страницы
        QLabel *titleLabel = new QLabel("Выберите матрицу, в которую нужно ввести значения:", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont titleFont;
        titleFont.setPointSize(18);
        titleLabel->setFont(titleFont);
        layout->addWidget(titleLabel);


        QVBoxLayout *checkBoxLayout = new QVBoxLayout();
        // Создаем чекбоксы для выбора матриц
        matrix1 = new QCheckBox("Матрица №1", this);
        matrix2 = new QCheckBox("Матрица №2", this);
        matrix3 = new QCheckBox("Матрица №3", this);
        matrix4 = new QCheckBox("Матрица №4", this);
        matrix5 = new QCheckBox("Матрица №5", this);


        // Увеличение размера текста в чекбоксах
        QFont checkBoxFont;
        checkBoxFont.setPointSize(15); // Устанавливаем размер шрифта
        matrix1->setFont(checkBoxFont);
        matrix2->setFont(checkBoxFont);
        matrix3->setFont(checkBoxFont);
        matrix4->setFont(checkBoxFont);
        matrix5->setFont(checkBoxFont);

        connect(matrix1, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);
        connect(matrix2, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);
        connect(matrix3, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);
        connect(matrix4, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);
        connect(matrix5, &QCheckBox::clicked, this, &InputPage::onMatrixCheckBoxClicked);

        // Добавляем чекбоксы в layout
        checkBoxLayout->addWidget(matrix1);
        checkBoxLayout->addWidget(matrix2);
        checkBoxLayout->addWidget(matrix3);
        checkBoxLayout->addWidget(matrix4);
        checkBoxLayout->addWidget(matrix5);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(checkBoxLayout);

        // Кнопка "Продолжить"
        QPushButton *continueButton = new QPushButton("Продолжить", this);
        QFont buttonFont;
        buttonFont.setPointSize(16); // Устанавливаем размер шрифта для кнопки
        continueButton->setFixedSize(290, 50);
        continueButton->setFont(buttonFont);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);

        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(buttonFont);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(buttonFont);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        layout->addLayout(exitLayout);

        connect(backButton, &QPushButton::clicked, this, &InputPage::onBackButtonClicked);
        connect(continueButton, &QPushButton::clicked, this, &InputPage::onContinueButtonClicked);
        connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

        // Устанавливаем основной layout
        setLayout(layout);
        layout->setSpacing(2); // Установка пространства между виджетами (по умолчанию больше)
        layout->setContentsMargins(5, 5, 5, 5);
    }

public slots: // Измените на public slots
    void setInputMethod(const QString &method) {
        inputMethod = method; // Сохраняем выбранный метод
    }

private slots:
    void onMatrixCheckBoxClicked() {
        // Если чекбокс был активирован, снимаем галочки с остальных
        QCheckBox* senderCheckBox = qobject_cast<QCheckBox*>(sender());
        if (senderCheckBox && senderCheckBox->isChecked()) {
            // Снимаем галочки с остальных чекбоксов
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

    void onContinueButtonClicked() {
        // Переход по методу ввода
        if (inputMethod == "Ввести с клавиатуры") {
            InputKeyboardPage *keyboardPage = qobject_cast<InputKeyboardPage*>(stackedWidget->widget(3));
            if (keyboardPage) {
                keyboardPage->setSelectedMatrixIndex(selectedMatrixIndex); // Передаем индекс
            }
            stackedWidget->setCurrentIndex(3); // Индекс страницы InputKeyboardPage
        } else if (inputMethod == "Ввести из текстового файла") {
            InputFilePage *filePage = qobject_cast<InputFilePage*>(stackedWidget->widget(4));
            if (filePage) {
                filePage->setSelectedMatrixIndex(selectedMatrixIndex); // Передаем индекс
            }
            stackedWidget->setCurrentIndex(4); // Индекс страницы InputFilePage
        } else if (inputMethod == "Случайный ввод") {
            RandomInputPage *randomPage = qobject_cast<RandomInputPage*>(stackedWidget->widget(5));
            if (randomPage) {
                randomPage->setSelectedMatrixIndex(selectedMatrixIndex); // Передаем индекс
            }
            stackedWidget->setCurrentIndex(5); // Индекс страницы RandomInputPage
        }
    }

    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(1); // Индекс вашей страницы NewMatrixPage
    }

private:
    QString inputMethod;
    int* size;
    bool* filled;
    int selectedMatrixIndex;
    QStackedWidget *stackedWidget;
    QCheckBox *matrix1;
    QCheckBox *matrix2;
    QCheckBox *matrix3;
    QCheckBox *matrix4;
    QCheckBox *matrix5;
};


//ВЫБОР ВВОДА МАТРИЦЫ
class NewMatrixPage : public QWidget {
    Q_OBJECT

public:
    NewMatrixPage(QStackedWidget *stackedWidget, int* matrixSize, bool* filledArray, QWidget *parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

        QVBoxLayout *buttonLayout = new QVBoxLayout();
        QPushButton *input1 = new QPushButton("Ввести с клавиатуры", this);
        QPushButton *input2 = new QPushButton("Ввести из текстового файла", this);
        QPushButton *input3 = new QPushButton("Случайный ввод", this);


        QSize buttonSize(290, 50);
        input1->setFixedSize(buttonSize);
        input2->setFixedSize(buttonSize);
        input3->setFixedSize(buttonSize);

        QFont buttonFont;
        buttonFont.setPointSize(15);
        input1->setFont(buttonFont);
        input2->setFont(buttonFont);
        input3->setFont(buttonFont);

        QLabel *titleLabel = new QLabel("Выбор способа ввода разреженной матрицы", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        QFont titleFont;
        titleFont.setPointSize(18);
        titleLabel->setFont(titleFont);
        titleLabel->setMinimumHeight(50);

        layout->addWidget(titleLabel);
        buttonLayout->addWidget(input1);
        buttonLayout->addWidget(input2);
        buttonLayout->addWidget(input3);
        buttonLayout->setAlignment(Qt::AlignCenter);
        buttonLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
        layout->addLayout(buttonLayout);
        layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

        QHBoxLayout *exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton *backButton = new QPushButton("Назад", this);
        backButton->setFont(buttonFont);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton *exitButton = new QPushButton("Выход", this);
        exitButton->setFont(buttonFont);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        layout->addLayout(exitLayout);
        setLayout(layout);

        connect(input1, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        connect(input2, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        connect(input3, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        // Подключение сигнала нажатия кнопки "Назад"
        connect(backButton, &QPushButton::clicked, this, &NewMatrixPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    }

signals:
    void backButtonClicked(); // Сигнал для возврата
    void inputMethodSelected(const QString &method);

private slots:
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(0); // Эмитируем сигнал назад
    }

    void onInputButtonClicked() {
        // Переключение на новый виджет ввода
        stackedWidget->setCurrentIndex(2); // Индекс новой страницы
    }

    void onInputMethodSelected() {
        QPushButton *button = qobject_cast<QPushButton*>(sender());
        if (button) {
            QString method = button->text(); // Получаем текст кнопки
            emit inputMethodSelected(method); // Эмитируем сигнал
            stackedWidget->setCurrentIndex(2); // Переход к InputPage
        }
    }
private:
    QStackedWidget *stackedWidget; // Указатель на QStackedWidget
    int* size;
    bool* filled;
};


//ГЛАВНАЯ СТРАНИЦА
class MainWindows : public QWidget {
    Q_OBJECT

public:
    MainWindows(int* matrixSize, bool* filledArray)
        : size(matrixSize), filled(filledArray) {
        setWindowTitle("MathMat");
        setFixedSize(1000, 600);

        // Главное представление с переключением страниц
        stackedWidget = new QStackedWidget(this);

        // Главная страница
        QWidget *mainPage = new QWidget();
        QVBoxLayout *mainLayout = new QVBoxLayout(mainPage);

        // Кнопки
        newButton = new QPushButton("Новая матрица");
        viewButton = new QPushButton("Просмотр матрицы");
        editButton = new QPushButton("Редактировать");
        delButton = new QPushButton("Удалить");
        addButton = new QPushButton("Сложение двух матриц");
        subButton = new QPushButton("Вычитание двух матриц");
        mulButton = new QPushButton("Умножение двух матриц");
        inverseButton = new QPushButton("Обратная матрица");
        quit = new QPushButton("Выход"); // Кнопка выход

        // Инициализация размеров кнопок
        QSize buttonSize(260, 50);
        newButton->setFixedSize(buttonSize);
        viewButton->setFixedSize(buttonSize);
        editButton->setFixedSize(buttonSize);
        delButton->setFixedSize(buttonSize);
        addButton->setFixedSize(buttonSize);
        subButton->setFixedSize(buttonSize);
        mulButton->setFixedSize(buttonSize);
        inverseButton->setFixedSize(buttonSize);
        QSize buttonSizeexit(200, 50);
        quit->setFixedSize(buttonSizeexit);

        //размер текста в кнопках
        QFont buttonFont;
        buttonFont.setPointSize(15); //размер шрифта
        newButton->setFont(buttonFont);
        viewButton->setFont(buttonFont);
        editButton->setFont(buttonFont);
        delButton->setFont(buttonFont);
        addButton->setFont(buttonFont);
        subButton->setFont(buttonFont);
        mulButton->setFont(buttonFont);
        inverseButton->setFont(buttonFont);
        quit->setFont(buttonFont);

        // Тексты на экране
        dataLabel = new QLabel("Работа с данными:");
        operationLabel = new QLabel("Выполнить операцию:");
        dataLabel->setAlignment(Qt::AlignCenter);
        operationLabel->setAlignment(Qt::AlignCenter);
        QFont dataFont;
        dataFont.setPointSize(18);
        dataLabel->setFont(dataFont);
        operationLabel->setFont(dataFont);

        // Размещение кнопок в макете
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

        // Добавление кнопки "Выход" в главную страницу
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
        RandomInputPage *randomInputPage = new RandomInputPage();
        InputKeyboard *inputKeyboard = new InputKeyboard(stackedWidget, size, filled);
        ViewPage * viewPage = new ViewPage(stackedWidget, size, filled);
        stackedWidget->addWidget(newMatrixPage);
        stackedWidget->addWidget(inputPage);
        stackedWidget->addWidget(inputKeyboardPage);  // Индекс 3
        stackedWidget->addWidget(inputFilePage);      // Индекс 4
        stackedWidget->addWidget(randomInputPage);
        stackedWidget->addWidget(inputKeyboard);
        stackedWidget->addWidget(viewPage);

        QVBoxLayout *centralLayout = new QVBoxLayout(this);
        centralLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        centralLayout->addWidget(stackedWidget);
        centralLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        setLayout(centralLayout);

        // Связываем кнопки с функциями
        connect(newButton, &QPushButton::clicked, this, &MainWindows::showNewMatrixPage);   //Переход к вводу
        connect(quit, &QPushButton::clicked, qApp, &QApplication::quit);                    //Выход из программы
        connect(editButton, &QPushButton::clicked, this, &MainWindows::showEditPage);       //Временно на редактировании ввод с клавиатуры
        connect(newMatrixPage, &NewMatrixPage::inputMethodSelected, inputPage, &InputPage::setInputMethod); //Подключение кнопки выбора ввода
        connect(viewButton, &QPushButton::clicked, this, &MainWindows::showViewPage);       //Подключение кнопки вывода
    }

private slots:
    void showNewMatrixPage() {
        stackedWidget->setCurrentIndex(1);  //Переход на окно ввода матрицы
    }
    void showMainPage() {
        stackedWidget->setCurrentIndex(0); // Переход на главную страницу
    }
    void showViewPage(){
        stackedWidget->setCurrentIndex(7);  //Переход на вывод матрицы
    }
    void showEditPage(){
        stackedWidget->setCurrentIndex(6);  //Перевод на редактирование
    }

private:
    int* size;
    bool* filled;
    QStackedWidget *stackedWidget;
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
