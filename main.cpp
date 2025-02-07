#include "mainwindow.h"
#include <QObject>
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
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QScrollArea>
#include <QFileDialog>
#include <QButtonGroup>
#include <QPixmap>
#include <cstdlib>
#include <ctime>
#include <QMessageBox>
#include <QIntValidator>
#include <QComboBox>
#include <QTimer>
#include <QTableWidgetItem>
#include <QCoreApplication>
#include <QDialog>
#include <QProgressBar>
#include <QFuture>
#include <QtConcurrent>
#include <QFile>
#include <QMouseEvent>
#include <QPalette>
#include <QMainWindow>
#include <QRegularExpression>

class MainWindows;

//структура для диагональных элементов
struct diagonal {
    double value;
    int i;
};

//структура для не диагональных элементов
struct not_diagonal {
    double value;
    int i;
    int j;
};

//указатели для хранения массивов диагональных и не диагональных элементов
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
diagonal* pack_diagonal6 = nullptr;
not_diagonal* pack_notdiagonal6 = nullptr;

//счетчики для хранения количества недиагональных и диагональных элементов
int count_notdiagonal1 = 0; int count_diagonal1 = 0;
int count_notdiagonal2 = 0; int count_diagonal2 = 0;
int count_notdiagonal3 = 0; int count_diagonal3 = 0;
int count_notdiagonal4 = 0; int count_diagonal4 = 0;
int count_notdiagonal5 = 0; int count_diagonal5 = 0;
int count_notdiagonal6 = 0; int count_diagonal6 = 0;
bool Obr = false; //переменная для отслеживания выбрана ли операция обратной матрицы

//СОРТИРОВКА ДИАГОНАЛЬНЫХ И НЕ ДИАГОНАЛЬНЫХ МАССИВОВ
template<typename T, typename Comparator>
void merge(T* Pack, int left, int mid, int right, Comparator compare) {  //функция для слияния двух отсортированных подмассивов
    int n1 = mid - left + 1;           //размер подмассива L
    int n2 = right - mid;              //размер подмассива R

    //временные массивы для подмассивов L и R
    T* L = new T[n1];
    T* R = new T[n2];

    //копирование данных в временные массивы L и R
    for (int i = 0; i < n1; i++) L[i] = Pack[left + i];
    for (int j = 0; j < n2; j++) R[j] = Pack[mid + 1 + j];

    //указатели для текущего индекса подмассивов и общего массива
    int i = 0, j = 0, k = left;

    //слияние подмассивов L и R обратно в массив Pack
    while (i < n1 && j < n2) {
        //сравниваем элементы и добавляем меньший в основной массив
        if (compare(L[i], R[j])) {
            Pack[k] = L[i];
            i++;
        }
        else {
            Pack[k] = R[j];
            j++;
        }
        k++;
    }
    //копирование оставшихся элементов из L, если они есть
    while (i < n1) {
        Pack[k] = L[i];
        i++;
        k++;
    }
    //копирование оставшихся элементов из R, если они есть
    while (j < n2) {
        Pack[k] = R[j];
        j++;
        k++;
    }
    delete[] L;
    delete[] R;
}

//функция сортировки слиянием
template<typename T, typename Comparator>
void mergeSort(T* Pack, int left, int right, Comparator compare) {
    //проверяем, есть ли необходимость продолжать сортировку
    if (left < right) {
        int mid = left + (right - left) / 2;          //средний элемент
        //рекурсивная сортировка первой и второй половины массива
        mergeSort(Pack, left, mid, compare);
        mergeSort(Pack, mid + 1, right, compare);
        merge(Pack, left, mid, right, compare);      //слияние отсортированных половин
    }
}

//функция для сравнения объектов типа diagonal
static bool compareDiagonal(const diagonal& d1, const diagonal& d2) {
    return d1.i < d2.i;   //сравнение по полю i
}

//функция для сравнения объектов типа diagonal
static bool compareNotDiagonal(const not_diagonal& nd1, const not_diagonal& nd2) {
    //сравнение сначала по полю i, затем по полю j, если i равны
    return (nd1.i < nd2.i) || (nd1.i == nd2.i && nd1.j < nd2.j);
}

void performSorting(int matrixIndex) {
    switch (matrixIndex) {
    case 1:
        mergeSort(pack_diagonal1, 0, count_diagonal1 - 1, compareDiagonal);
        mergeSort(pack_notdiagonal1, 0, count_notdiagonal1 - 1, compareNotDiagonal);
        break;
    case 2:
        mergeSort(pack_diagonal2, 0, count_diagonal2 - 1, compareDiagonal);
        mergeSort(pack_notdiagonal2, 0, count_notdiagonal2 - 1, compareNotDiagonal);
        break;
    case 3:
        mergeSort(pack_diagonal3, 0, count_diagonal3 - 1, compareDiagonal);
        mergeSort(pack_notdiagonal3, 0, count_notdiagonal3 - 1, compareNotDiagonal);
        break;
    case 4:
        mergeSort(pack_diagonal4, 0, count_diagonal4 - 1, compareDiagonal);
        mergeSort(pack_notdiagonal4, 0, count_notdiagonal4 - 1, compareNotDiagonal);
        break;
    case 5:
        mergeSort(pack_diagonal5, 0, count_diagonal5 - 1, compareDiagonal);
        mergeSort(pack_notdiagonal5, 0, count_notdiagonal5 - 1, compareNotDiagonal);
        break;
    case 6:
        mergeSort(pack_diagonal6, 0, count_diagonal6 - 1, compareDiagonal);
        mergeSort(pack_notdiagonal6, 0, count_notdiagonal6 - 1, compareNotDiagonal);
        break;
    default:
        break; // Handle any unexpected input, if necessary
    }
}

//стиль кнопок
void setupButton(QPushButton* button, const QString& backgroundColor, const QString& borderColor,
                     const QString& hoverBackgroundColor, const QString& textColor,
                     const QString& hoverTextColor) {
    button->setStyleSheet(
        QString("QPushButton {"
                "   background-color: %1;"
                "   color: %2;"
                "   border: 1px solid %3;"
                "}"
                "QPushButton:hover {"
                "   background-color: %4;"
                "   color: %5;"
                "}")
        .arg(backgroundColor)        //цвет фона кнопки
        .arg(textColor)              //цвет текста кнопки
        .arg(borderColor)            //цвет обводки
        .arg(hoverBackgroundColor)   //цвет фона при наведении
        .arg(hoverTextColor)         //цвет текста при наведении
    );
}

//стиль таблицы
void setupTableStyle(QTableWidget* table) {
    table->setStyleSheet(
        "QTableWidget { background-color: #F0FFFF; border: 1px solid #A3C1DA; gridline-color: #A3C1DA; } "
        "QHeaderView::section { background-color: #A3C1DA; color: #001a33; border: none; }"
        "QTableWidget::item { border: none; padding: 5px; color: #001a33; } " // Цвет текста для обычных элементов
        "QTableWidget::item:alternate { background-color: #E0F7FA; color: #001a33; } "
        "QTableWidget::item:selected { background-color: #A3C1DA; color: #001a33; }"
    );
}

//стиль скролла
void setupScrollAreaStyles(QScrollArea* scrollArea) {
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #F0FFFF; } "
        //стиль вертикальной прокрутки
        "QScrollBar:vertical { background: #A3C1DA; width: 15px; margin: 22px 0; } "
        "QScrollBar::handle:vertical { background: #003366; min-height: 30px; border: none; } "
        //стиль кнопок прокрутки для вертикального скролла
        "QScrollBar::add-line:vertical { background: #c8d9e8; height: 35px; subcontrol-origin: margin; subcontrol-position: bottom; } "
        "QScrollBar::sub-line:vertical { background: #c8d9e8; height: 35px; subcontrol-origin: margin; subcontrol-position: top; } "
        //стиль для области полосы прокрутки, не занятой тянущейся частью
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: #F0FFFF; } "
        //стиль для тянущейся части вертикальной полосы прокрутки при наведении
        "QScrollBar::handle:vertical:hover { background: #004080; } "
        "QScrollBar::add-line:vertical:hover, QScrollBar::sub-line:vertical:hover { background: #A3C1DA; } "
        //стиль кнопок прокрутки для горизонтального скролла
        "QScrollBar:horizontal { background: #A3C1DA; height: 15px; margin: 0 22px; } "
        "QScrollBar::handle:horizontal { background: #003366; min-width: 30px; border: none; } "
        //стиль кнопок прокрутки влево и вправо для горизонтальной полосы прокрутки
        "QScrollBar::add-line:horizontal { background: #c8d9e8; width: 35px; subcontrol-origin: margin; subcontrol-position: right; } "
        "QScrollBar::sub-line:horizontal { background: #c8d9e8; width: 35px; subcontrol-origin: margin; subcontrol-position: left; } "
        //стиль для области полосы прокрутки, не занятой тянущейся частью горизонтальной полосы
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: #F0FFFF; } "
        //стиль для тянущейся части горизонтальной полосы прокрутки при наведении
        "QScrollBar::handle:horizontal:hover { background: #004080; } "
        "QScrollBar::add-line:horizontal:hover, QScrollBar::sub-line:horizontal:hover { background: #A3C1DA; }"
    );
}

//стиль на массив чекбоксов
void applyCheckBoxStyle(QCheckBox* checkboxes[], int count) {
    QString styleSheet = R"(
        QCheckBox {
            color: #001a33; /* Цвет текста */
        }
        QCheckBox::indicator {
            background-color: #F0FFFF; /* Цвет фона квадратика */
            border: 1px solid #001a33; /* Цвет ободка квадратика */
            width: 10px; /* Ширина квадратика */
            height: 10px; /* Высота квадратика */
        }
        QCheckBox::indicator:checked {
            background-color: #003366; /* Цвет фона при отметке */
            border: 1px solid #001a33; /* Цвет ободка при отметке */
        }
        QCheckBox:disabled {
            color: #4574a1; /* Цвет текста, когда чекбокс отключен */
            background-color: #F0FFFF; /* Цвет фона текста при отключенном чекбоксе */
        }
        QCheckBox::indicator:disabled {
            background-color: #F0FFFF; /* Цвет фона квадратика, когда чекбокс отключен */
            border: 1px solid #4574a1; /* Цвет ободка квадратика, когда чекбокс отключен */
        }
    )";

    for (int i = 0; i < count; ++i) {
        checkboxes[i]->setStyleSheet(styleSheet);
    }
}

//стиль окон ввода
void applyLineEditStyle(QLineEdit* lineEdit) {
    lineEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: #fafffd;
            border: 1px solid #003366; /* Цвет обводки при активном состоянии */
            font-size: 20px;
            color: #001a33;
        }
        QLineEdit:disabled {
            background-color: #F0FFFF;
            border: 1px solid #A3C1DA; /* Цвет обводки при неактивном состоянии */
            font-size: 20px;
            color: #001a33;
        }
    )");
}

//кнопка "выход" Подтверждение
void showExitConfirmation(QWidget* parent) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle("Подтверждение");
    msgBox.setText("Вы точно хотите выйти из  программы?");
    msgBox.setIcon(QMessageBox::NoIcon);
    QPushButton* yesButton = msgBox.addButton("Да", QMessageBox::AcceptRole);
    QPushButton* noButton = msgBox.addButton("Нет", QMessageBox::RejectRole);
    msgBox.setDefaultButton(noButton);
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #F0FFFF; color: #001a33;  border: 1px solid #A3C1DA; font-size: 16px;} "  // цвет фона и текста
        "QLabel { color: #001a33; font-size: 16px; } "
        "QPushButton { background-color: #F0FFFF; color: #001a33;  border: 1px solid #A3C1DA; min-width: 90px; min-height: 30px; font-size: 16px;} "  // цвет фона и текста кнопок
        "QPushButton:hover { background-color: #A3C1DA; color: #001a33;  border: 1px solid #A3C1DA; font-size: 16px;}"             // цвет фона кнопок при наведении
    );
    msgBox.setFixedSize(400, 150);
    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        QApplication::quit();
    }
    else {
        return;
    }
}

//окно предупреждения
void showMessageBox(const QString& title, const QString& message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.addButton(QMessageBox::Ok);
    msgBox.setModal(true);
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #F0FFFF; color: #001a33;  border: 1px solid #A3C1DA; font-size: 16px;} "  // цвет фона и текста
        "QLabel { color: #001a33; font-size: 16px; } "
        "QPushButton { background-color: #F0FFFF; color: #001a33;  border: 1px solid #A3C1DA; min-width: 90px; min-height: 30px; font-size: 16px;} "  // цвет фона и текста кнопок
        "QPushButton:hover { background-color: #A3C1DA; color: #001a33;  border: 1px solid #A3C1DA; font-size: 16px;}"             // цвет фона кнопок при наведении
    );
    msgBox.setFixedSize(400, 150);
    msgBox.exec();
}

//окно вопроса
bool showBackWarning(const QString &title, const QString &message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::NoIcon);
    QPushButton* yesButton = msgBox.addButton("Да", QMessageBox::AcceptRole);
    QPushButton* noButton = msgBox.addButton("Нет", QMessageBox::RejectRole);
    msgBox.setModal(true);
    msgBox.setStyleSheet("QMessageBox { background-color: #F0FFFF; }");
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #F0FFFF; color: #001a33;  border: 1px solid #A3C1DA; font-size: 16px;} "  // цвет фона и текста
        "QLabel { color: #001a33; font-size: 16px; } "
        "QPushButton { background-color: #F0FFFF; color: #001a33;  border: 1px solid #A3C1DA; min-width: 90px; min-height: 30px; font-size: 16px;} "  // цвет фона и текста кнопок
        "QPushButton:hover { background-color: #A3C1DA; color: #001a33;  border: 1px solid #A3C1DA; font-size: 16px;}"             // цвет фона кнопок при наведении
    );
    msgBox.exec();
    msgBox.setFixedSize(400, 150);

    //определяем, какую кнопку нажали и возвращаем результат
    return (msgBox.clickedButton() == yesButton);
}

//---------------------------------------------------------------------------------------------------------------

//СТРАНИЦА ВХОДА
class LandingPage : public QWidget {
    Q_OBJECT

public:
    explicit LandingPage(QWidget* parent = nullptr) : QWidget(parent) {

        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(240, 255, 255)); // Очень светло-голубой цвет
        this->setPalette(palette);

        setWindowTitle("MathMat");
        setFixedSize(1000, 600);

        QFont fontTitle;
        fontTitle.setPointSize(90);
        QFont fontSubtitle;
        fontSubtitle.setPointSize(18);
        QFont fontButton;
        fontButton.setPointSize(15);

        //заголовок
        QLabel* titleLabel = new QLabel("MathMat", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("QLabel { color: #001a33; }");
        titleLabel->setFont(fontTitle);

        QLabel* strTitle = new QLabel("Работа с разреженными матрицами.", this);
        strTitle->setAlignment(Qt::AlignCenter);
        strTitle->setStyleSheet("QLabel { color: #001a33; }");
        strTitle->setFont(fontSubtitle);

        //кнопка "продолжить"
        QPushButton* continueButton = new QPushButton("Начать работу", this);
        continueButton->setFont(fontButton);
        continueButton->setFixedSize(300, 50);
        setupButton(continueButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");

        //создание вертикального макета
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addSpacerItem(new QSpacerItem(0, 200, QSizePolicy::Minimum, QSizePolicy::Expanding)); // Пустое пространство сверху
        layout->addWidget(titleLabel);
        layout->addWidget(strTitle);
        layout->addSpacing(50);
        layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Fixed)); // Отсту
        layout->addWidget(continueButton);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);
        layout->addSpacerItem(new QSpacerItem(0, 200, QSizePolicy::Minimum, QSizePolicy::Expanding));

        setLayout(layout);

        connect(continueButton, &QPushButton::clicked, this, &LandingPage::onContinueClicked);
    }

signals:
    void continueToMain();

private slots:
    void onContinueClicked() {
        emit continueToMain();
    }

};

//ОКНО ЗАГРУЗКИ
class LoadingDialog : public QDialog {
    Q_OBJECT

private:
    QLabel* messageLabel;
    QLabel* infoLabel;
    QProgressBar* progressBar;

public:
    explicit LoadingDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Загрузка...");
        setStyleSheet("background-color: #F0FFFF;");
        setModal(true);
        resize(400, 150);
        setStyleSheet("background-color: #F0FFFF;");
        QVBoxLayout *layout = new QVBoxLayout(this);
        progressBar = new QProgressBar(this);
        progressBar->setRange(0, 0);
        progressBar->setStyleSheet(
                "QProgressBar { "
                "background-color: #F0FFFF; "
                "border: 2px solid #A3C1DA; "
                "border-radius: 5px; "
                "} "
                "QProgressBar::chunk { "
                "background-color: #A3C1DA; "
                "border-radius: 5px; "
                "}"
            );
        messageLabel = new QLabel("Пожалуйста, подождите...", this);  //текст по умолчанию
        messageLabel->setStyleSheet("color: #001a33; font-size: 18px;");
        infoLabel = new QLabel("Для отмены закройте окно", this); //текст для возможности отмены
        infoLabel->setStyleSheet("color: #001a33; font-size: 16px;");
        layout->addWidget(messageLabel);
        progressBar->setFixedHeight(20);
        layout->addWidget(progressBar);
        layout->addWidget(infoLabel);
        setLayout(layout);
    }

    void setMessage(const QString &message) {
        messageLabel->setText(message);
    }

protected:
    void closeEvent(QCloseEvent *event) override {
        emit loadingCancelled();
        event->accept();              //закрытие диалога
    }

signals:
    void loadingCancelled();         //сигнал, который будет послан при закрытии

};

//-----------------------------------------------------------------------------------------

//ВЫВОД ФИЗИЧЕСКОГО РАСПОЛОЖЕнИЯ ЭЛЕМЕНТОВ МАТРИЦЫ
class LocationPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QTableWidget* table;                  //таблица значений матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLabel* errorLabel;
    QTimer* timer;                        //таймер для заполнения
    QStringList rowLabels;                //для нумерации строк таблицы
    int currentIndex;                     //счётчик строк
    int i = 0;                            //индекс для диагональных
    int j = 0;                            //индекс для недиагональных
    int tableRowIndex = 0;

    //начало таймера
    void fillTable() {
        currentIndex = 0;
        rowLabels.clear();
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &LocationPage::addNextEntry);
        timer->start(1);
    }

    //переход к выводу таблицы с элементами матрицы
    void addNextEntry() {
        switch (selectedMatrixIndex) {
        case 1:
            fillMatrix(pack_diagonal1, count_diagonal1, pack_notdiagonal1, count_notdiagonal1);
            break;
        case 2:
            fillMatrix(pack_diagonal2, count_diagonal2, pack_notdiagonal2, count_notdiagonal2);
            break;
        case 3:
            fillMatrix(pack_diagonal3, count_diagonal3, pack_notdiagonal3, count_notdiagonal3);
            break;
        case 4:
            fillMatrix(pack_diagonal4, count_diagonal4, pack_notdiagonal4, count_notdiagonal4);
            break;
        case 5:
            fillMatrix(pack_diagonal5, count_diagonal5, pack_notdiagonal5, count_notdiagonal5);
            break;
        }
    }

    //вывод таблицы с элементами мтарицы
    void fillMatrix(diagonal* pack_diagonal, int count_diagonal, not_diagonal* pack_notdiagonal, int count_notdiagonal) {
        if (currentIndex < count_diagonal + count_notdiagonal) {

            int diagRow = pack_diagonal[i].i;
            int diagCol = pack_diagonal[i].i;

            int notDiagRow = pack_notdiagonal[j].i;
            int notDiagCol = pack_notdiagonal[j].j;

            //заполнение таблицы по возрастанию индексов
            if (i < count_diagonal && j < count_notdiagonal) {
                if (diagRow < notDiagRow || (diagRow == notDiagRow && diagCol < notDiagCol)) {
                    updateTableWithDiagonalValues(diagRow, diagCol, pack_diagonal, i);
                    ++i;
                } else {
                    updateTableWithNonDiagonalValues(notDiagRow, notDiagCol, pack_notdiagonal, j);
                    ++j;
                }
                tableRowIndex++;
            }

            if (!(i < count_diagonal && j < count_notdiagonal)) {
                //заполнение оставшихся диагональных элементов
                if (i < count_diagonal) {
                    updateTableWithDiagonalValues(pack_diagonal[i].i, pack_diagonal[i].i, pack_diagonal, i);
                    ++i;
                    ++tableRowIndex;
                }

                //заполнение оставшихся недиагональных элементов
                if (j < count_notdiagonal) {
                    updateTableWithNonDiagonalValues(pack_notdiagonal[j].i, pack_notdiagonal[j].j, pack_notdiagonal, j);
                    ++j;
                    ++tableRowIndex;
                }
            }

            currentIndex = tableRowIndex;

        } else {
            timer->stop();      //остановка таймера, когда все элементы заполнены
            delete timer;       //освобождение памяти
            table->setVerticalHeaderLabels(rowLabels);
        }
    }

    //добавление строки с диагональным элементом
    void updateTableWithDiagonalValues(int row, int col, diagonal* pack_diagonal, int i) {
        table->setItem(tableRowIndex, 0, new QTableWidgetItem(QString::number(row)));
        table->setItem(tableRowIndex, 1, new QTableWidgetItem(QString::number(col)));
        table->setItem(tableRowIndex, 2, new QTableWidgetItem(QString::number(pack_diagonal[i].value)));
        table->setItem(tableRowIndex, 3, new QTableWidgetItem(QString::number(reinterpret_cast<quintptr>(&pack_diagonal[i].value), 16)));

        rowLabels << QString::number(tableRowIndex + 1);
    }

    //добавление строки с не диагональным элементом
    void updateTableWithNonDiagonalValues(int row, int col, not_diagonal* pack_notdiagonal, int i) {
        table->setItem(tableRowIndex, 0, new QTableWidgetItem(QString::number(row)));
        table->setItem(tableRowIndex, 1, new QTableWidgetItem(QString::number(col)));
        table->setItem(tableRowIndex, 2, new QTableWidgetItem(QString::number(pack_notdiagonal[i].value)));
        table->setItem(tableRowIndex, 3, new QTableWidgetItem(QString::number(reinterpret_cast<quintptr>(&pack_notdiagonal[i].value), 16)));

        rowLabels << QString::number(tableRowIndex + 1);
    }

public:
    LocationPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, int selectedMatrixIndex, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(selectedMatrixIndex) {

        QFont Font;
        Font.setPointSize(18);
        QFont Fontstr;
        Fontstr.setPointSize(15);

        QVBoxLayout* layout = new QVBoxLayout(this);
        QLabel* dimensionLabel = new QLabel("Физическое расположение элементов в разреженной матрице:", this);
        dimensionLabel->setFont(Font);
        dimensionLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(dimensionLabel);

        //ТАБЛИЦА
        int sizen = size[selectedMatrixIndex - 1];
        int rows = sizen * sizen * 0.15;
        const int columns = 4;
        table = new QTableWidget(rows, columns, this);
        QStringList headerLabels;
        headerLabels << QString("Номер строки") << QString("Номер столбца") << QString("Значение") << QString("Адрес элемента в ОП");
        table->setHorizontalHeaderLabels(headerLabels);

        setupTableStyle(table);
        table->setColumnWidth(0, 100);
        table->setColumnWidth(1, 100);
        table->setColumnWidth(2, 100);
        table->setColumnWidth(3, 200);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers); //запрет редактирования таблицы

        //СКРОЛЛ
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(table);
        scrollArea->setFixedHeight(250);
        scrollArea->setFixedWidth(500);
        setupScrollAreaStyles(scrollArea);
        layout->addWidget(scrollArea);

        errorLabel = new QLabel("", this);
        errorLabel->setStyleSheet("color: red;");
        errorLabel->setWordWrap(true);
        errorLabel->setFixedHeight(40);
        layout->addWidget(errorLabel);

        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Fontstr);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);
        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Fontstr);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        exitLayout->setContentsMargins(0, 20, 0, 0); // Отступы сверху 10 для визуального разделения
        exitLayout->setSpacing(10);
        layout->addLayout(exitLayout);

        setLayout(layout);

        //ПЕРЕХОДЫ ПО КНОПКАМ
        connect(backButton, &QPushButton::clicked, this, &LocationPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

    //передача выбранной матрицы
    void setSelectedMatrixIndex(int index) {
        selectedMatrixIndex = index;
        fillTable();
    }

private slots:
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(7);
    }
};


//РЕДАКТИРОВАНИЕ ВЫБРАННОЙ МАТРИЦЫ
class EditPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QTableWidget* table;                  //таблица значений матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLabel* errorLabel;
    QTimer* timer;                        //таймер для заполнения
    QStringList rowLabels;                //для нумерации строк таблицы
    int currentIndex;                     //
    int i = 0;                            //индекс для диагональных
    int j = 0;                            //индекс для недиагональных
    int tableRowIndex = 0;                //строка
    bool saveButton = false;

    //функция ввода из таблицы в матрицу значений
    void EnterMatrix(bool& hasNaturalError, bool& hasRationalError, bool& hasIndexError, diagonal*& pack_diagonal, not_diagonal*& pack_notdiagonal, int& count_diagonal, int& count_notdiagonal, int& n) {
        delete[] pack_diagonal;
        delete[] pack_notdiagonal;
        pack_diagonal = nullptr;
        pack_notdiagonal = nullptr;

        filled[selectedMatrixIndex - 1] = true;
        int MAX_NOT_DIAGONAL = n * n * 0.15;

        pack_diagonal = new diagonal[n];
        pack_notdiagonal = new not_diagonal[MAX_NOT_DIAGONAL];
        count_diagonal = 0;
        count_notdiagonal = 0;

        hasNaturalError = false;
        hasRationalError = false;
        hasIndexError = false;

        for (int row = 0; row < table->rowCount(); ++row) {
            QTableWidgetItem* itemRow = table->item(row, 0);
            QTableWidgetItem* itemCol = table->item(row, 1);
            QTableWidgetItem* itemValue = table->item(row, 2);

            if (itemRow && itemCol && itemValue) {
                bool okRow, okCol, okValue;
                int realRowNumber = itemRow->text().toInt(&okRow);
                int realColNumber = itemCol->text().toInt(&okCol);
                double value = itemValue->text().toDouble(&okValue);

                if (!okRow || !okCol || realRowNumber <= 0 || realColNumber <= 0) {
                    hasNaturalError = true;
                    continue;
                }
                if (!okValue || value == 0) {
                    hasRationalError = true;
                    continue;
                }
                if (realRowNumber > size[selectedMatrixIndex-1] || realColNumber > size[selectedMatrixIndex-1]){
                    hasIndexError = true;
                    continue;
                }

                if (realRowNumber == realColNumber) {
                    if (count_diagonal < n) {
                        pack_diagonal[count_diagonal] = { value, realRowNumber };
                        (count_diagonal)++;
                    }
                }
                else {
                    if (count_notdiagonal < MAX_NOT_DIAGONAL) {
                        pack_notdiagonal[count_notdiagonal] = { value, realRowNumber, realColNumber };
                        (count_notdiagonal)++;
                    }
                }
            }
        }
    }

    //начало таймера
    void fillTable() {
        currentIndex = 0;
        rowLabels.clear();
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &EditPage::addNextEntry);
        timer->start(1);
    }

    //переход к выводу таблицы с элементами матрицы
    void addNextEntry() {
        switch (selectedMatrixIndex) {
        case 1:
            fillMatrix(pack_diagonal1, count_diagonal1, pack_notdiagonal1, count_notdiagonal1);
            break;
        case 2:
            fillMatrix(pack_diagonal2, count_diagonal2, pack_notdiagonal2, count_notdiagonal2);
            break;
        case 3:
            fillMatrix(pack_diagonal3, count_diagonal3, pack_notdiagonal3, count_notdiagonal3);
            break;
        case 4:
            fillMatrix(pack_diagonal4, count_diagonal4, pack_notdiagonal4, count_notdiagonal4);
            break;
        case 5:
            fillMatrix(pack_diagonal5, count_diagonal5, pack_notdiagonal5, count_notdiagonal5);
            break;
        }
    }

    //вывод таблицы с элементами мтарицы
    void fillMatrix(diagonal* pack_diagonal, int count_diagonal, not_diagonal* pack_notdiagonal, int count_notdiagonal) {
        if (currentIndex < count_diagonal + count_notdiagonal) {

            //заполнение таблицы по возрастанию индексов
            if (i < count_diagonal && j < count_notdiagonal) {
                int diagRow = pack_diagonal[i].i;
                int diagCol = pack_diagonal[i].i;
                double diagValue = pack_diagonal[i].value;

                int notDiagRow = pack_notdiagonal[j].i;
                int notDiagCol = pack_notdiagonal[j].j;
                double notDiagValue = pack_notdiagonal[j].value;

                if (diagRow < notDiagRow || (diagRow == notDiagRow && diagCol < notDiagCol)) {
                    updateTableWithDiagonalValues(diagRow, diagCol, diagValue);
                    ++i;
                } else {
                    updateTableWithDiagonalValues(notDiagRow, notDiagCol, notDiagValue);
                    ++j;
                }
                tableRowIndex++;
            }

            if (!(i < count_diagonal && j < count_notdiagonal)) {
                //заполнение оставшихся диагональных элементов
                if (i < count_diagonal) {
                    updateTableWithDiagonalValues(pack_diagonal[i].i, pack_diagonal[i].i, pack_diagonal[i].value);
                    ++i;
                    ++tableRowIndex;
                }

                //заполнение оставшихся недиагональных элементов
                if (j < count_notdiagonal) {
                    updateTableWithNonDiagonalValues(pack_notdiagonal[j].i, pack_notdiagonal[j].j, pack_notdiagonal[j].value);
                    ++j;
                    ++tableRowIndex;
                }
            }

            currentIndex = tableRowIndex;

        } else {
            saveButton = true;
            timer->stop();         //остановка таймера, когда все элементы заполнены
            delete timer;          //освобождение памяти
            table->setVerticalHeaderLabels(rowLabels);
        }
    }

    //добавление строки с диагональным элементом
    void updateTableWithDiagonalValues(int row, int col, double value) {
        table->setItem(tableRowIndex, 0, new QTableWidgetItem(QString::number(row)));
        table->setItem(tableRowIndex, 1, new QTableWidgetItem(QString::number(col)));
        table->setItem(tableRowIndex, 2, new QTableWidgetItem(QString::number(value)));

        rowLabels << QString::number(tableRowIndex + 1); // Add row label, incrementing by 1 for display
    }

    //добавление строки с не диагональным элементом
    void updateTableWithNonDiagonalValues(int row, int col, double value) {
        table->setItem(tableRowIndex, 0, new QTableWidgetItem(QString::number(row)));
        table->setItem(tableRowIndex, 1, new QTableWidgetItem(QString::number(col)));
        table->setItem(tableRowIndex, 2, new QTableWidgetItem(QString::number(value)));

        rowLabels << QString::number(tableRowIndex + 1); // Add row label, incrementing by 1 for display
    }

public:
    EditPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, int selectedMatrixIndex, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(selectedMatrixIndex) {

        QFont Font;
        Font.setPointSize(18);
        QFont Fontstr;
        Fontstr.setPointSize(15);

        QVBoxLayout* layout = new QVBoxLayout(this);
        QLabel* dimensionLabel = new QLabel("Редактирование разреженной матрицы:", this);
        dimensionLabel->setFont(Font);
        dimensionLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(dimensionLabel);

        //ТАБЛИЦА
        int sizen = size[selectedMatrixIndex - 1];
        int rows = sizen * sizen * 0.15;
        const int columns = 3;
        table = new QTableWidget(rows, columns, this);
        QStringList headerLabels;
        headerLabels << QString("Номер строки") << QString("Номер столбца") << QString("Значение");
        table->setHorizontalHeaderLabels(headerLabels);

        setupTableStyle(table);
        table->setColumnWidth(0, 100);
        table->setColumnWidth(1, 100);
        table->setColumnWidth(2, 100);

        //СКРОЛЛ
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(table);
        scrollArea->setFixedHeight(250);
        scrollArea->setFixedWidth(345);
        setupScrollAreaStyles(scrollArea);
        layout->addWidget(scrollArea);

        errorLabel = new QLabel("", this);
        errorLabel->setStyleSheet("color: red;");
        errorLabel->setWordWrap(true);
        errorLabel->setFixedHeight(40);
        layout->addWidget(errorLabel);

        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* saveButton = new QPushButton("Сохранить", this);
        saveButton->setFont(Fontstr);
        saveButton->setFixedSize(200, 50);
        exitLayout->addWidget(saveButton);

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Fontstr);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);
        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Fontstr);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(saveButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        exitLayout->setContentsMargins(0, 20, 0, 0);
        exitLayout->setSpacing(10);
        layout->addLayout(exitLayout);

        setLayout(layout);

        //ПЕРЕХОДЫ ПО КНОПКАМ
        connect(backButton, &QPushButton::clicked, this, &EditPage::onBackButtonClicked);
        connect(saveButton, &QPushButton::clicked, this, &EditPage::saveData);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

public slots:
    //передача выбранной матрицы
    void setSelectedMatrixIndex(int index) {
        fillTable();
    }

private slots:
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        if (showBackWarning("Предупреждение", "Сохранить ввод, прежде чем вернуться назад?")) {
            saveData();
        } else {
            selectedMatrixIndex == -1;
            stackedWidget->setCurrentIndex(12);
        }
    }

    //ЗАПОЛНЕНИЕ МАТРИЦЫ ЗНАЧЕНИЯМИ ИЗ ТАБЛИЦЫ
    void saveData() {
        if (!saveButton){
            showMessageBox("Предупреждение", "Дождитесь полного вывода всей таблицы.");
            return;
        }else {
            errorLabel->clear();
            bool hasNaturalError = false;
            bool hasRationalError = false;
            bool hasIndexError = false;
            bool isTableEmpty = true;

            for (int row = 0; row < table->rowCount(); ++row) {
                QTableWidgetItem* itemRow = table->item(row, 0);
                QTableWidgetItem* itemCol = table->item(row, 1);
                QTableWidgetItem* itemValue = table->item(row, 2);

                //проверка на наличие хотя бы одного заполненного элемента
                if (itemRow && !itemRow->text().isEmpty() &&
                    itemCol && !itemCol->text().isEmpty() &&
                    itemValue && !itemValue->text().isEmpty()) {
                    isTableEmpty = false;
                    break;
                }
            }

            //если таблица пустая
            if (isTableEmpty) {
                showMessageBox("Предупреждение", "Заполните таблицу перед сохранением.");
                return;
            }

            for (int row = 0; row < table->rowCount(); ++row) {
                QTableWidgetItem* itemRow = table->item(row, 0);
                QTableWidgetItem* itemCol = table->item(row, 1);
                QTableWidgetItem* itemValue = table->item(row, 2);

                if (itemRow && !itemRow->text().isEmpty() &&
                    itemCol && !itemCol->text().isEmpty() &&
                    itemValue && !itemValue->text().isEmpty()){
                    bool okRow, okCol, okValue;
                    int realRowNumber = itemRow->text().toInt(&okRow);
                    int realColNumber = itemCol->text().toInt(&okCol);
                    double value = itemValue->text().toDouble(&okValue);

                    if (!okRow || !okCol || realRowNumber <= 0 || realColNumber <= 0) {
                        hasNaturalError = true;
                    }
                    if (!okValue || value == 0) {
                        hasRationalError = true;
                    }
                    if (realRowNumber > size[selectedMatrixIndex-1] || realColNumber > size[selectedMatrixIndex-1]){
                        hasIndexError = true;
                    }
                }
            }

            QString errorMessage;
            if (hasNaturalError) {
                errorMessage.append("&nbsp;&nbsp;&nbsp;&nbsp;!!! Номер строки/столбца должен быть натуральным.<br>");
            }
            if (hasRationalError) {
                errorMessage.append("&nbsp;&nbsp;&nbsp;&nbsp;!!! Значение должно быть рациональным числом, кроме нуля, записанное через точку ('.').<br>");
            }
            if (hasIndexError) {
                errorMessage.append("&nbsp;&nbsp;&nbsp;&nbsp;!!! Индекс не должен быть больше размерности разреженной матрицы.<br>");
            }
            errorLabel->setText(errorMessage);
            if (errorMessage.isEmpty()) {
                if (showBackWarning("Предупреждение", "Вы точно хотите завершить ввод?")) {
                    switch (selectedMatrixIndex) {
                    case 1: EnterMatrix(hasNaturalError, hasRationalError, hasIndexError, pack_diagonal1, pack_notdiagonal1, count_diagonal1, count_notdiagonal1, size[0]); break;
                    case 2: EnterMatrix(hasNaturalError, hasRationalError, hasIndexError, pack_diagonal2, pack_notdiagonal2, count_diagonal2, count_notdiagonal2, size[1]); break;
                    case 3: EnterMatrix(hasNaturalError, hasRationalError, hasIndexError, pack_diagonal3, pack_notdiagonal3, count_diagonal3, count_notdiagonal3, size[2]); break;
                    case 4: EnterMatrix(hasNaturalError, hasRationalError, hasIndexError, pack_diagonal4, pack_notdiagonal4, count_diagonal4, count_notdiagonal4, size[3]); break;
                    case 5: EnterMatrix(hasNaturalError, hasRationalError, hasIndexError, pack_diagonal5, pack_notdiagonal5, count_diagonal5, count_notdiagonal5, size[4]); break;
                    }
                    performSorting(selectedMatrixIndex);
                    stackedWidget->setCurrentIndex(0);
                    //если нет, то просто остаемся на странице
                } else {
                    return;
                }
            }
        }
    }
};


//ВЫБОР МАТРИЦЫ ДЛЯ РЕДАКТИРОВАНИЯ
class EditInputPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex = -1;              //номер выбранной матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLineEdit* dimensionInput;            //поле ввода размерности
    QString inputMethod;                  //выбранный метод ввода
    //чекбоксы выбора матрицы
    QCheckBox* matrix1;
    QCheckBox* matrix2;
    QCheckBox* matrix3;
    QCheckBox* matrix4;
    QCheckBox* matrix5;

    //проверка заполненности матрицы
    bool isMatrixFilled(int index) const {
        return (index >= 0 && index < 5) ? filled[index] : false;
    }

    //обновление состояния выбора матрицы
    void updateCheckBoxStates() {
        matrix1->setEnabled(isMatrixFilled(0));
        matrix2->setEnabled(isMatrixFilled(1));
        matrix3->setEnabled(isMatrixFilled(2));
        matrix4->setEnabled(isMatrixFilled(3));
        matrix5->setEnabled(isMatrixFilled(4));

        matrix1->setChecked(false);
        matrix2->setChecked(false);
        matrix3->setChecked(false);
        matrix4->setChecked(false);
        matrix5->setChecked(false);
    }

public:
    EditInputPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, int selectedMatrixIndex, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        QLabel* titleLabel = new QLabel("Выберите разреженную матрицу для редактирования:", this);
        titleLabel->setAlignment(Qt::AlignCenter);;
        titleLabel->setFont(Fontstr);
        titleLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(titleLabel);

        //ЧЕКБОКСЫ ВЫБОРА МАТРИЦЫ
        QVBoxLayout* checkBoxLayout = new QVBoxLayout();
        matrix1 = new QCheckBox("Матрица №1", this);
        matrix2 = new QCheckBox("Матрица №2", this);
        matrix3 = new QCheckBox("Матрица №3", this);
        matrix4 = new QCheckBox("Матрица №4", this);
        matrix5 = new QCheckBox("Матрица №5", this);

        QCheckBox* matrices[] = { matrix1, matrix2, matrix3, matrix4, matrix5 };
        applyCheckBoxStyle(matrices, sizeof(matrices) / sizeof(matrices[0]));  //стиль

        //РАЗМЕР ТЕКСТА
        matrix1->setFont(Font);
        matrix2->setFont(Font);
        matrix3->setFont(Font);
        matrix4->setFont(Font);
        matrix5->setFont(Font);

        updateCheckBoxStates();
        connect(matrix1, &QCheckBox::clicked, this, &EditInputPage::onMatrixCheckBoxClicked);
        connect(matrix2, &QCheckBox::clicked, this, &EditInputPage::onMatrixCheckBoxClicked);
        connect(matrix3, &QCheckBox::clicked, this, &EditInputPage::onMatrixCheckBoxClicked);
        connect(matrix4, &QCheckBox::clicked, this, &EditInputPage::onMatrixCheckBoxClicked);
        connect(matrix5, &QCheckBox::clicked, this, &EditInputPage::onMatrixCheckBoxClicked);

        checkBoxLayout->addWidget(matrix1);
        checkBoxLayout->addWidget(matrix2);
        checkBoxLayout->addWidget(matrix3);
        checkBoxLayout->addWidget(matrix4);
        checkBoxLayout->addWidget(matrix5);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(checkBoxLayout);

        //КНОПКИ
        QPushButton* continueButton = new QPushButton("Продолжить", this);
        continueButton->setFixedSize(290, 50);
        continueButton->setFont(Font);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);

        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(continueButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        layout->addLayout(exitLayout);
        setLayout(layout);
        layout->setSpacing(2);
        layout->setContentsMargins(5, 5, 5, 5);

        //ПЕРЕХОДЫ НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(backButton, &QPushButton::clicked, this, &EditInputPage::onBackButtonClicked);
        connect(continueButton, &QPushButton::clicked, this, &EditInputPage::onContinueButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }


protected:
    void showEvent(QShowEvent* event) override {
        QWidget::showEvent(event);
        updateCheckBoxStates();
    }

signals:
    void matrixSelected(int index);

public slots:
    //СОХРАНЕНИЕ ВЫБРАННОГО МЕТОДА ВВОДА МАТРИЦЫ
    void setInputMethod(const QString& method) {
        inputMethod = method;
    }

private slots:
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        selectedMatrixIndex = -1;
        stackedWidget->setCurrentIndex(0);
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

    //ПЕРЕХОДЫ НА СТРАНИЦУ РЕДАКТИРОВАНИЯ МАТРИЦЫ
    void onContinueButtonClicked() {
        if (selectedMatrixIndex == -1) {
            showMessageBox("Ошибка", "Выберите разреженную матрицу для редактирования.");
            return;
        }

        EditPage* editPage = new EditPage(stackedWidget, size, filled, selectedMatrixIndex, this);
        editPage->setSelectedMatrixIndex(selectedMatrixIndex);

        stackedWidget->addWidget(editPage);
        stackedWidget->setCurrentWidget(editPage);
    }
};


//СОХРАНИЕ РЕЗУЛЬТАТА ОПЕРАЦИИ
class SaveMatrixOperat : public QWidget {
    Q_OBJECT
private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int* count_diagonal;                  //кол-во диагональных элементов матрицы
    int* count_notdiagonal;               //кол-во не диагональных элементов матрицы
    diagonal* pack_diagonal;              //диагональная матрица
    not_diagonal* pack_notdiagonal;       //не диагональная матрица
    int selectedMatrixIndex = 0;          //номер выбранной матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QString filePath;                     //файл
    QComboBox* matrixSelector;            //бокс для выпадающего списка выбора матрицы
    QCheckBox* nonZeroValuesCheckbox;     //перавый чекбокс
    QCheckBox* tenPercentCheckbox;        //второй чекбокс
    QPushButton* choiceButton;            //кнопка "выбрать"
    LoadingDialog *loadingDialog = nullptr;  //страница загрузки
    QFuture<void> future;                 //хранение фоновой задачи
    bool cancelGeneration = false;        //флаг для отмены генерации

    //функция сохранения результата операции в матрицу
    void save_matrices(diagonal*& pack_diagonal, not_diagonal*& pack_notdiagonal, int& count_notdiagonal, int& count_diagonal) {
        delete[] pack_diagonal;
        delete[] pack_notdiagonal;
        count_diagonal = 0;
        count_notdiagonal = 0;
        pack_diagonal = new diagonal[count_diagonal6];
        pack_notdiagonal = new not_diagonal[count_notdiagonal6];

        for (int i = 0; i < count_diagonal6; i++) {
            pack_diagonal[count_diagonal].i = pack_diagonal6[i].i;
            pack_diagonal[count_diagonal].value = pack_diagonal6[i].value;
            count_diagonal++;
        }
        for (int i = 0; i < count_notdiagonal6; i++) {
            pack_notdiagonal[count_notdiagonal].i = pack_notdiagonal6[i].i;
            pack_notdiagonal[count_notdiagonal].j = pack_notdiagonal6[i].j;
            pack_notdiagonal[count_notdiagonal].value = pack_notdiagonal6[i].value;
            count_notdiagonal++;
        }
    }

public:
    SaveMatrixOperat(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        layout->addSpacing(70);
        QLabel* dimensionLabel = new QLabel("Выбор способа сохранения:", this);
        dimensionLabel->setFont(Fontstr);
        dimensionLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(dimensionLabel);

        layout->addSpacing(60);
        nonZeroValuesCheckbox = new QCheckBox("Сохранить в списке разреженных матриц", this);
        nonZeroValuesCheckbox->setFont(Font);
        dimensionLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(nonZeroValuesCheckbox, 0, Qt::AlignLeft);

        //ВЫПАДАЮЩИЙ СПИСОК
        matrixSelector = new QComboBox(this);
        matrixSelector->setFixedSize(200, 50);
        matrixSelector->setFont(Font);
        matrixSelector->setStyleSheet(R"(
            QComboBox {
                background-color: #fafffd;
                color: #001a33;
                border: 1px solid #003366;
            }
            QComboBox:!enabled {
                background-color: #F0FFFF;
                color: #A3C1DA;
                border: 1px solid #A3C1DA;
            }
            QComboBox QAbstractItemView {
                background-color: #fafffd;
                color: #001a33;
                border: 1px solid #003366;
            }
        )");
        matrixSelector->addItem("Выбрать");
        matrixSelector->addItem("Matrix №1");
        matrixSelector->addItem("Matrix №2");
        matrixSelector->addItem("Matrix №3");
        matrixSelector->addItem("Matrix №4");
        matrixSelector->addItem("Matrix №5");
        matrixSelector->setEnabled(false);
        layout->addWidget(matrixSelector, 0, Qt::AlignLeft);

        layout->addSpacing(20);
        QLabel* orLabel = new QLabel("ИЛИ", this);
        orLabel->setFont(Fontstr);
        orLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(orLabel, 0, Qt::AlignLeft);

        //КНОПКА "ВЫБРАТЬ" ДЛЯ СОХРАНЕНИЯ В ФАЙЛ
        layout->addSpacing(20);
        tenPercentCheckbox = new QCheckBox("Сохранить в файл", this);
        tenPercentCheckbox->setFont(Font);
        tenPercentCheckbox->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(tenPercentCheckbox, 0, Qt::AlignLeft);

        QCheckBox* matrices[] = { tenPercentCheckbox, nonZeroValuesCheckbox};
        applyCheckBoxStyle(matrices, sizeof(matrices) / sizeof(matrices[0]));  //стиль

        choiceButton = new QPushButton("Выбрать", this);
        choiceButton->setFont(Font);
        choiceButton->setFixedSize(200, 50);
        choiceButton->setEnabled(false);
        layout->addWidget(choiceButton, 0, Qt::AlignLeft);

        QButtonGroup* checkboxGroup = new QButtonGroup(this);
        checkboxGroup->setExclusive(true);
        checkboxGroup->addButton(nonZeroValuesCheckbox);
        checkboxGroup->addButton(tenPercentCheckbox);
        layout->addStretch();

        //КНОПКИ
        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* saveButton = new QPushButton("Сохранить", this);
        saveButton->setFont(Font);
        saveButton->setFixedSize(200, 50);
        exitLayout->addWidget(saveButton);

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(choiceButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(saveButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        exitLayout->setContentsMargins(0, 10, 0, 0);
        exitLayout->setSpacing(10);
        layout->addLayout(exitLayout);
        setLayout(layout);

        connect(tenPercentCheckbox, &QCheckBox::toggled, this, &SaveMatrixOperat::updateChoiceButtonState);
        connect(nonZeroValuesCheckbox, &QCheckBox::toggled, this, &SaveMatrixOperat::updateMatrixSelectorState);
        connect(choiceButton, &QPushButton::clicked, this, [=]() {
            filePath = QFileDialog::getSaveFileName(this, "Выбрать файл");
        });
        connect(saveButton, &QPushButton::clicked, this, &SaveMatrixOperat::onSaveButtonClicked);
        connect(backButton, &QPushButton::clicked, this, &SaveMatrixOperat::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

    void resetSelection() {
        matrixSelector->setCurrentIndex(0);
    }

signals:
    void showNextPage(int selectedIndex);

private slots:
    //НАЗАД
    void onBackButtonClicked() {
        if (showBackWarning("Предупреждение", "Вы точно хотите вернуться назад, не сохранив результат?")) {
            stackedWidget->setCurrentIndex(9);
        } else {
            return;
        }
    }

    void updateChoiceButtonState(bool checked) {
        choiceButton->setEnabled(checked);
    }
    void updateMatrixSelectorState(bool checked) {
        matrixSelector->setEnabled(checked);
    }
    //кнопка "сохранить"
    void onSaveButtonClicked() {
        int selectedMatrixIndex = matrixSelector->currentIndex();

        //ПРЕЖУПРЕЖДЕНИЕ ЕСЛИ НЕ ВЫБРАН ПУТЬ СОХРАНЕНИЯ
        if ((!tenPercentCheckbox->isChecked() && !nonZeroValuesCheckbox->isChecked())
            || (tenPercentCheckbox->isChecked() && filePath.isEmpty())
            || (nonZeroValuesCheckbox->isChecked() && selectedMatrixIndex == 0)) {

            showMessageBox("Предупреждение", "Пожалуйста, выберите разреженную матрицу для сохранения ненулевых значений.");
            return;
        }

        if (future.isRunning()) {
            showMessageBox("Предупреждение", "Обработка сохранения уже выполняется. Пожалуйста, дождитесь завершения.");
            return;
        }

        cancelGeneration = false;
        loadingDialog = new LoadingDialog(this);
        loadingDialog->setMessage("Сохранение разреженной матрицы...");
        loadingDialog->show();

        connect(loadingDialog, &LoadingDialog::loadingCancelled, this, [&]() {
            cancelGeneration = true;                      //флаг для отмены генерации
            loadingDialog->accept();                      //закрытие диалога загрузки
            showMessageBox("Предупреждение", "Сохрание разреженной матрицы отменено.");
            return;
        });

        QCoreApplication::processEvents();

        future = QtConcurrent::run([=]() {
            if (cancelGeneration) return;
            if (tenPercentCheckbox->isChecked() && !filePath.isEmpty()) {
                QFile file(filePath);
                if (cancelGeneration) return;
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    QString data;

                    //запоминаем размер
                    data.append(QString::number(size[5]) + "\n");

                    //записываем диагональные элементы
                    for (int i = 0; i < count_diagonal6; i++) {
                        if (cancelGeneration) {
                            file.close();
                            return; //прекращаем выполнение
                        }
                        data.append(QString("%1 %2 %3\n")
                            .arg(QString::number(pack_diagonal6[i].value, 'g', 15))
                            .arg(pack_diagonal6[i].i)
                            .arg(pack_diagonal6[i].i));
                    }

                    //записываем не диагональные элементы
                    for (int i = 0; i < count_notdiagonal6; i++) {
                        if (cancelGeneration) {
                            file.close();
                            return; //прекращаем выполнение
                        }
                        data.append(QString("%1 %2 %3\n")
                            .arg(QString::number(pack_notdiagonal6[i].value, 'g', 15))
                            .arg(pack_notdiagonal6[i].i)
                            .arg(pack_notdiagonal6[i].j));
                    }

                    if (cancelGeneration) return;
                    out << data;
                    file.close();
                }
                else {
                }
            }
            //СОХРАНЕНИЕ В МАТРИЦУ
            else if (nonZeroValuesCheckbox->isChecked() && selectedMatrixIndex > 0) {
                if (cancelGeneration) return;
                emit showNextPage(selectedMatrixIndex);
                filled[selectedMatrixIndex - 1] = true;
                size[selectedMatrixIndex - 1] = size[5];

                switch (selectedMatrixIndex) {
                case 1: save_matrices(pack_diagonal1, pack_notdiagonal1, count_notdiagonal1, count_diagonal1); break;
                case 2: save_matrices(pack_diagonal2, pack_notdiagonal2, count_notdiagonal2, count_diagonal2); break;
                case 3: save_matrices(pack_diagonal3, pack_notdiagonal3, count_notdiagonal3, count_diagonal3); break;
                case 4: save_matrices(pack_diagonal4, pack_notdiagonal4, count_notdiagonal4, count_diagonal4); break;
                case 5: save_matrices(pack_diagonal5, pack_notdiagonal5, count_notdiagonal5, count_diagonal5); break;
                }
            }

            QMetaObject::invokeMethod(this, [=]() {
                if (!cancelGeneration) {
                    loadingDialog->accept();                 //закрытие диалога загрузки
                    showMessageBox("Сохранение завершено", "Разреженная матрица сохранена.");
                    stackedWidget->setCurrentIndex(0);      //переход на главную страницу
                }
            }, Qt::QueuedConnection);
        });

        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() {
            if (!future.isRunning() && !cancelGeneration) {
                timer->stop();                            //остановка таймера
                loadingDialog->accept();                  //закрытие диалога загрузки
                //удаление вспомогательной матрицы для операций
                size[5] = 0;
                delete[] pack_diagonal6;
                pack_diagonal6 = nullptr;
                count_diagonal6 = 0;
                delete[] pack_notdiagonal6;
                pack_notdiagonal6 = nullptr;
                count_notdiagonal6 = 0;
                stackedWidget->setCurrentIndex(0);       //переход на главную страницу
            }
        });
        timer->start(100);
    }

};


//ВЫВОД МАТРИЦЫ ПОСЛЕ ОПЕРАЦИИ
class OperatViewPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QTableWidget* matrixTable;            //таблица для вывода матрицы
    QTimer* updateTimer;                  //таймер для заполнения
    int currentRow;                       //текущая строка, которую нужно заполнить
    int currentCol;                       //текущий столбец, который нужно заполнить

    //начало таймера
    void setupUpdateTimer() {
        updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &OperatViewPage::fillNextTableCell);
        currentRow = 0;
        currentCol = 0;
        updateTimer->start(1);
    }

    //переход к выводу таблицы с элементами матрицы
    void fillNextTableCell() {
        int cellsToFill = 10;     //количество ячеек, которые заполняем за один вызов
        for (int i = 0; i < cellsToFill && currentRow < size[5]; i++) {
            if (currentCol >= size[5]) {
                currentCol = 0;
                currentRow++;
            }
            if (currentRow >= size[5]) {
                updateTimer->stop();
                return;
            }
            fillCell(currentRow, currentCol);
            currentCol++;
        }
    }

    //вывод таблицы с элементами мтарицы
    void fillCell(int row, int col) {
        bool foundValue = false;

        //заполнение диагональных значений
        if (pack_diagonal6 != nullptr) {
            diagonal key = { .i = row + 1 };
            diagonal* it = std::lower_bound(pack_diagonal6, pack_diagonal6 + count_diagonal6, key, [](const diagonal& a, const diagonal& b) {
                return a.i < b.i;
            });

            //проверяем, подходит ли найденный элемент
            if (it != pack_diagonal6 + count_diagonal6 && it->i == key.i) {
                if (row == col) {
                    QString valueStr = QString::number(it->value, 'g', 15);
                    matrixTable->setItem(row, col, new QTableWidgetItem(valueStr));
                    foundValue = true;
                }
            }
        }

        //заполнение недиагональных значений
        if (!foundValue && pack_notdiagonal6 != nullptr) {
            not_diagonal key = { .i = row + 1, .j = col + 1 };
            not_diagonal* it = std::lower_bound(pack_notdiagonal6, pack_notdiagonal6 + count_notdiagonal6, key, [](const not_diagonal& a, const not_diagonal& b) {
                return std::tie(a.i, a.j) < std::tie(b.i, b.j);
            });

            if (it != pack_notdiagonal6 + count_notdiagonal6 && it->i == key.i && it->j == key.j) {
                QString valueStr = QString::number(it->value, 'g', 15);
                matrixTable->setItem(row, col, new QTableWidgetItem(valueStr));
                foundValue = true;
            }
        }

        if (!foundValue) {
            matrixTable->setItem(row, col, new QTableWidgetItem("0"));
        }
    }

public:
    OperatViewPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr) :
        QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        matrixTable = new QTableWidget(this);           //ТАБЛИЦА

        QFont buttonFont;
        buttonFont.setPointSize(16);

        //СКРОЛЛ
        QScrollArea* scrollArea = new QScrollArea(this);
        scrollArea->setWidget(matrixTable);
        scrollArea->setWidgetResizable(true);
        setupScrollAreaStyles(scrollArea);
        layout->addWidget(scrollArea);

        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        //КНОПКИ
        QPushButton* fileButton = new QPushButton("Сохранить", this);
        fileButton->setFont(buttonFont);
        fileButton->setFixedSize(200, 50);
        exitLayout->addWidget(fileButton);

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(buttonFont);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(buttonFont);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(fileButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        exitLayout->setContentsMargins(0, 10, 0, 0); // Отступы сверху 10 для визуального разделения
        exitLayout->setSpacing(10);

        layout->addLayout(exitLayout);
        setLayout(layout);

        connect(fileButton, &QPushButton::clicked, this, &OperatViewPage::printSavedData);
        connect(backButton, &QPushButton::clicked, this, &OperatViewPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

    //функция, которая вызывалась в конце предыдущей странице
    void setU() {
        updateMatrixTable();
    }

    //ТАБЛИЦА
    void updateMatrixTable() {
        int n = size[5];
        matrixTable->setRowCount(n);
        matrixTable->setColumnCount(n);

        setupTableStyle(matrixTable);
        matrixTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        matrixTable->clearContents();
        matrixTable->setUpdatesEnabled(false);
        setupUpdateTimer();
        matrixTable->setUpdatesEnabled(true);
    }

signals:
    void backButtonClicked(); // Сигнал для возврата

private slots:
    //кнопка "сохранить"
    void printSavedData() {
        stackedWidget->setCurrentIndex(11);          //переход на страницу сохранения матрицы
    }

    //кнопка "назад"
    void onBackButtonClicked() {
        //окно предупреждения
        if (showBackWarning("Предупреждение", "Сохранить результат?")) {
            stackedWidget->setCurrentIndex(11);
        } else {
            //Удаление вспомогательной матрицы для операций
            size[5] = 0;
            if (pack_diagonal6 != nullptr) {
                delete[] pack_diagonal6;
                pack_diagonal6 = nullptr;
            }
            count_diagonal6 = 0;

            if (pack_notdiagonal6 != nullptr) {
                delete[] pack_notdiagonal6;
                pack_notdiagonal6 = nullptr;
            }
            count_notdiagonal6 = 0;
            if (Obr) {
                Obr = false;
                stackedWidget->setCurrentIndex(13);
            }else{
                stackedWidget->setCurrentIndex(9);        //переход назад на выбор матрицы для операций
            }
        }
    }
};


//ВЫБОР МАТРИЦ ДЛЯ ОПЕРАЦИЙ И САМИ ОПЕРАЦИИ
class OperationMatrixInput : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndexLeft;          //номер выбранной матрицы в левом столбце
    int selectedMatrixIndexRight;         //номер выбранной матрицы в правом столбце
    int operation;                        //переменная для хранения выбранной операции
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLineEdit* dimensionInput;            //поле ввода размерности
    QString inputMethod;                  //выбранный метод ввода
    //чекбоксы для выбора первой матрицы для операции
    QCheckBox* matrix1Left; QCheckBox* matrix2Left;
    QCheckBox* matrix3Left; QCheckBox* matrix4Left; QCheckBox* matrix5Left;

    //чекбоксы для выбора второй матрицы для операции
    QCheckBox* matrix1Right; QCheckBox* matrix2Right;
    QCheckBox* matrix3Right; QCheckBox* matrix4Right; QCheckBox* matrix5Right;

    //проверка заполненности матрицы
    bool isMatrixFilled(int index) const {
        return (index >= 0 && index < 5) ? filled[index] : false;
    }

    //обновление состояния выбора матрицы
    void updateCheckBoxStates() {
        matrix1Left->setEnabled(isMatrixFilled(0));
        matrix2Left->setEnabled(isMatrixFilled(1));
        matrix3Left->setEnabled(isMatrixFilled(2));
        matrix4Left->setEnabled(isMatrixFilled(3));
        matrix5Left->setEnabled(isMatrixFilled(4));
        matrix1Left->setChecked(false);
        matrix2Left->setChecked(false);
        matrix3Left->setChecked(false);
        matrix4Left->setChecked(false);
        matrix5Left->setChecked(false);
        matrix1Right->setEnabled(isMatrixFilled(0));
        matrix2Right->setEnabled(isMatrixFilled(1));
        matrix3Right->setEnabled(isMatrixFilled(2));
        matrix4Right->setEnabled(isMatrixFilled(3));
        matrix5Right->setEnabled(isMatrixFilled(4));
        matrix1Right->setChecked(false);
        matrix2Right->setChecked(false);
        matrix3Right->setChecked(false);
        matrix4Right->setChecked(false);
        matrix5Right->setChecked(false);
    }

    //объединение чекбоксов в строки
    void addCheckBoxRow(QCheckBox* leftCheckBox, QCheckBox* rightCheckBox, QVBoxLayout* checkBoxLayout) {
        QHBoxLayout* checkBoxRow = new QHBoxLayout();
        checkBoxRow->addWidget(leftCheckBox);
        checkBoxRow->addSpacing(180);
        checkBoxRow->addWidget(rightCheckBox);
        checkBoxLayout->addLayout(checkBoxRow);
    }

    //размер текста
    void setCheckBoxFont(const std::initializer_list<QCheckBox*>& checkBoxes, const QFont& font) {
        for (auto checkBox : checkBoxes) {
            checkBox->setFont(font);
        }
    }

    //сигналы для чекбоксов
    void connectCheckBoxSignals() {
        for (auto checkBox : { matrix1Left, matrix2Left, matrix3Left, matrix4Left, matrix5Left,
                              matrix1Right, matrix2Right, matrix3Right, matrix4Right, matrix5Right }) {
            connect(checkBox, &QCheckBox::clicked, this, &OperationMatrixInput::onMatrixCheckBoxClicked);
        }
    }

public:
    OperationMatrixInput(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), operation(0), selectedMatrixIndexLeft(-1), selectedMatrixIndexRight(-1) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(16);

        QLabel* titleLabel1 = new QLabel("Первая матрица:", this);
        titleLabel1->setStyleSheet("QLabel { color: #001a33; }");
        titleLabel1->setFont(Fontstr);

        QLabel* titleLabel2 = new QLabel("Вторая матрица:", this);
        titleLabel2->setStyleSheet("QLabel { color: #001a33; }");
        titleLabel2->setFont(Fontstr);

        QHBoxLayout* titleLayout = new QHBoxLayout();
        titleLayout->addWidget(titleLabel1);
        titleLayout->addSpacing(180);
        titleLayout->addWidget(titleLabel2);
        titleLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(titleLayout);

        //ЧЕКБОКСЫ ВЫБОРА МАТРИЦЫ
        QVBoxLayout* checkBoxLayout = new QVBoxLayout();
        QHBoxLayout* checkBoxRow;

        //чекбоксы слева
        matrix1Left = new QCheckBox("Матрица №1", this);
        matrix2Left = new QCheckBox("Матрица №2", this);
        matrix3Left = new QCheckBox("Матрица №3", this);
        matrix4Left = new QCheckBox("Матрица №4", this);
        matrix5Left = new QCheckBox("Матрица №5", this);

        //чекбоксы справа
        matrix1Right = new QCheckBox("Матрица №1", this);
        matrix2Right = new QCheckBox("Матрица №2", this);
        matrix3Right = new QCheckBox("Матрица №3", this);
        matrix4Right = new QCheckBox("Матрица №4", this);
        matrix5Right = new QCheckBox("Матрица №5", this);

        QCheckBox* leftMatrices[] = { matrix1Left, matrix2Left, matrix3Left, matrix4Left, matrix5Left };
        QCheckBox* rightMatrices[] = { matrix1Right, matrix2Right, matrix3Right, matrix4Right, matrix5Right };
        //стиль к чекбоксам слева и справа
        applyCheckBoxStyle(leftMatrices, sizeof(leftMatrices) / sizeof(leftMatrices[0]));
        applyCheckBoxStyle(rightMatrices, sizeof(rightMatrices) / sizeof(rightMatrices[0]));

        //размер текста чекбоксов
        setCheckBoxFont({ matrix1Left, matrix2Left, matrix3Left, matrix4Left, matrix5Left,
                         matrix1Right, matrix2Right, matrix3Right, matrix4Right, matrix5Right }, Font);
        updateCheckBoxStates();

        //подключение сигналов для чекбоксов
        connectCheckBoxSignals();

        //объединение чекбоксов в строки
        addCheckBoxRow(matrix1Left, matrix1Right, checkBoxLayout);
        addCheckBoxRow(matrix2Left, matrix2Right, checkBoxLayout);
        addCheckBoxRow(matrix3Left, matrix3Right, checkBoxLayout);
        addCheckBoxRow(matrix4Left, matrix4Right, checkBoxLayout);
        addCheckBoxRow(matrix5Left, matrix5Right, checkBoxLayout);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(checkBoxLayout);

        //кнопка "Выполнить операцию"
        QPushButton* continueButton = new QPushButton("Выполнить операцию", this);
        continueButton->setFixedSize(290, 50);
        continueButton->setFont(Font);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);

        //КНОПКИ
        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(continueButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        layout->addLayout(exitLayout);
        setLayout(layout);
        layout->setSpacing(2);
        layout->setContentsMargins(5, 5, 5, 5);

        connect(backButton, &QPushButton::clicked, this, &OperationMatrixInput::onBackButtonClicked);
        connect(continueButton, &QPushButton::clicked, this, &OperationMatrixInput::onContinueClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

    void setOperation(int operation) {
        this->operation = operation; //установка выбранной операции
    }


protected:
    void showEvent(QShowEvent* event) override {
        QWidget::showEvent(event);
        updateCheckBoxStates();
    }

signals:
    void matrixSelected(int index);

private:
    //округление и проверка на ноль значения
    bool format_value(double value, double& formatted_value) {
        if (static_cast<int>(value) == value) {
            formatted_value = static_cast<int>(value);
        } else {
            formatted_value = static_cast<double>(std::round(value * 100.0) / 100.0);
        }

        if (formatted_value == 0.00 || formatted_value == 0 || value == 0) {
            return false;
        }
        return true;
    }

    //ОКНО ПРЕДУПРЕЖДЕНИЕ КОЭФ. ЗАПОЛНЕННОСТИ > 15%
    void show_warning() {
        showMessageBox("Ошибка", "Количество ненулевых значений в результате операции сложения разреженных матриц составляет больше 15%. Результат удалён.");

        //Удаление вспомогательной матрицы для операций
        size[5] = 0;
        if (pack_diagonal6 != nullptr) {
            delete[] pack_diagonal6;
            pack_diagonal6 = nullptr;
        }
        count_diagonal6 = 0;

        if (pack_notdiagonal6 != nullptr) {
            delete[] pack_notdiagonal6;
            pack_notdiagonal6 = nullptr;
        }
        count_notdiagonal6 = 0;
    }

    //СЛОЖЕНИЕ
    void add_matrices(int n, diagonal* pack_diagonal1, not_diagonal* pack_notdiagonal1, diagonal* pack_diagonal2, not_diagonal* pack_notdiagonal2,
                      int& size6, int count_notdiagonal1, int count_notdiagonal2, int count_diagonal1, int count_diagonal2, bool& success, bool& nulls) {

        size6 = n;
        int maxsize_notdiagonal = static_cast<int>(floor(n * n * 0.15));
        pack_diagonal6 = new diagonal[n];
        pack_notdiagonal6 = new not_diagonal[maxsize_notdiagonal];
        count_diagonal6 = 0;
        count_notdiagonal6 = 0;

        //сложение диагональных элементов
        int d1 = 0, d2 = 0;
        for (int i = 0; i < n; i++) {
            double sum = 0;
            bool found = false;

            if (d1 < count_diagonal1 && pack_diagonal1[d1].i == i + 1) {
                sum += pack_diagonal1[d1].value;
                d1++;
                found = true;
            }

            if (d2 < count_diagonal2 && pack_diagonal2[d2].i == i + 1) {
                sum += pack_diagonal2[d2].value;
                d2++;
                found = true;
            }

            if (found && sum != 0) {
                double formatted_value;
                if (format_value(sum, formatted_value)) {
                    pack_diagonal6[count_diagonal6].value = formatted_value;
                    pack_diagonal6[count_diagonal6].i = i + 1;
                    count_diagonal6++;
                    nulls = false;
                }
            }
        }

        d1 = 0; d2 = 0;
        double value = 0;

        //сложение недиагональных элементов
        while (d1 < count_notdiagonal1 && d2 < count_notdiagonal2) {
            if (pack_notdiagonal1[d1].i == pack_notdiagonal2[d2].i && pack_notdiagonal1[d1].j == pack_notdiagonal2[d2].j) {
                if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                    show_warning();
                    success = false;
                    return;
                }
                else {
                    double value = pack_notdiagonal1[d1].value + pack_notdiagonal2[d2].value;
                    double formatted_value;
                    if (format_value(value, formatted_value)) {
                        pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal1[d1].i;
                        pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal1[d1].j;
                        pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                        count_notdiagonal6++;
                        nulls = false;
                    }
                    d1++;
                    d2++;
                }
            }
            else if (pack_notdiagonal1[d1].i < pack_notdiagonal2[d2].i ||
                     (pack_notdiagonal1[d1].i == pack_notdiagonal2[d2].i && pack_notdiagonal1[d1].j < pack_notdiagonal2[d2].j)) {
                if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                    show_warning();
                    success = false;
                    return;
                }
                else {
                    double value = pack_notdiagonal1[d1].value;
                    double formatted_value;
                    if (format_value(value, formatted_value)) {
                        pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal1[d1].i;
                        pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal1[d1].j;
                        pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                        count_notdiagonal6++;
                        d1++;
                        nulls = false;
                    }
                }
            }
            else {
                if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                    show_warning();
                    success = false;
                    return;
                }
                else {
                    double value = pack_notdiagonal2[d2].value;
                    double formatted_value;
                    if (format_value(value, formatted_value)) {
                        pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal2[d2].i;
                        pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal2[d2].j;
                        pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                        count_notdiagonal6++;
                        d2++;
                        nulls = false;
                    }
                }
            }
        }

        //добавление оставшихся недиагональных элементов из первой матрицы
        while (d1 < count_notdiagonal1) {
            if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                show_warning();
                success = false;
                return;
            }
            else {
                double value = pack_notdiagonal1[d1].value;
                double formatted_value;
                if (format_value(value, formatted_value)) {
                    pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal1[d1].i;
                    pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal1[d1].j;
                    pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                    count_notdiagonal6++;
                    d1++;
                    nulls = false;
                }
            }
        }

        //добавление оставшихся недиагональных элементов из второй матрицы
        while (d2 < count_notdiagonal2) {
            if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                show_warning();
                success = false;
                return;
            }
            else {
                double value = pack_notdiagonal2[d2].value;
                double formatted_value;
                if (format_value(value, formatted_value)) {
                    pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal2[d2].i;
                    pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal2[d2].j;
                    pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                    count_notdiagonal6++;
                    d2++;
                    nulls = false;
                }
            }
        }
        success = true;
    }

    //ВЫЧИТАНИЕ
    void sub_matrices(int n, diagonal* pack_diagonal1, not_diagonal* pack_notdiagonal1, diagonal* pack_diagonal2, not_diagonal* pack_notdiagonal2,
                      int& size6, int count_notdiagonal1, int count_notdiagonal2, int count_diagonal1, int count_diagonal2, bool& success, bool& nulls) {

        size6 = n;
        int maxsize_notdiagonal = static_cast<int>(floor(n * n * 0.15));
        pack_diagonal6 = new diagonal[n];
        pack_notdiagonal6 = new not_diagonal[maxsize_notdiagonal];
        int d1 = 0, d2 = 0;

        //вычитание диагональных элементов
        for (int i = 0; i < n; i++) {
            double difference = 0;
            bool found = false;

            if (d1 < count_diagonal1 && pack_diagonal1[d1].i == i + 1) {
                difference += pack_diagonal1[d1].value;
                d1++;
                found = true;
            }
            if (d2 < count_diagonal2 && pack_diagonal2[d2].i == i + 1) {
                difference -= pack_diagonal2[d2].value;
                d2++;
                found = true;
            }
            if (found) {
                double formatted_value;
                if (format_value(difference, formatted_value)) {
                    pack_diagonal6[count_diagonal6].value = formatted_value;
                    pack_diagonal6[count_diagonal6].i = i + 1;
                    count_diagonal6++;
                    nulls = false;
                }
            }
        }
        d1 = 0, d2 = 0;
        //вычитание не диагональных элементов
        while (d1 < count_notdiagonal1 && d2 < count_notdiagonal2) {
            if (pack_notdiagonal1[d1].i == pack_notdiagonal2[d2].i && pack_notdiagonal1[d1].j == pack_notdiagonal2[d2].j) {
                if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                    show_warning();
                    success = false;
                    return;
                }
                else {
                    double value = pack_notdiagonal1[d1].value - pack_notdiagonal2[d2].value;
                    double formatted_value;
                    if (format_value(value, formatted_value)) {
                        pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal1[d1].i;
                        pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal1[d1].j;
                        pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                        count_notdiagonal6++;
                        nulls = false;
                    }
                    d1++;
                    d2++;
                }
            }
            else if (pack_notdiagonal1[d1].i < pack_notdiagonal2[d2].i || (pack_notdiagonal1[d1].i == pack_notdiagonal2[d2].i && pack_notdiagonal1[d1].j < pack_notdiagonal2[d2].j)) {

                if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                    show_warning();
                    success = false;
                    return;
                }
                else {
                    double value = pack_notdiagonal1[d1].value;
                    double formatted_value;
                    if (format_value(value, formatted_value)) {
                        pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal1[d1].i;
                        pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal1[d1].j;
                        pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                        count_notdiagonal6++;
                        d1++;
                        nulls = false;
                    }
                }
            }
            else {
                if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                    show_warning();
                    success = false;
                    return;
                }
                else {
                    double value = pack_notdiagonal2[d2].value * (-1);
                    double formatted_value;
                    if (format_value(value, formatted_value)) {
                        pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal2[d2].i;
                        pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal2[d2].j;
                        pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                        count_notdiagonal6++;
                        d2++;
                        nulls = false;
                    }
                }
            }
        }
        while (d1 < count_notdiagonal1) {
            if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                show_warning();
                success = false;
                return;
            }
            else {
                double value = pack_notdiagonal2[d1].value;
                double formatted_value;
                if (format_value(value, formatted_value)) {
                    pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal1[d1].i;
                    pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal1[d1].j;
                    pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                    count_notdiagonal6++;
                    d1++;
                    nulls = false;
                }
            }
        }
        while (d2 < count_notdiagonal2) {
            if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                show_warning();
                success = false;
                return;
            }
            else {
                double value = pack_notdiagonal2[d2].value * (-1);
                double formatted_value;
                if (format_value(value, formatted_value)) {
                    pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal2[d2].i;
                    pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal2[d2].j;
                    pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                    count_notdiagonal6++;
                    d2++;
                    nulls = false;
                }
            }
        }
        success = true;
    }

    //УМНОЖЕНИЕ
    void mul_matrices(int n, diagonal* pack_diagonal1, not_diagonal* pack_notdiagonal1, diagonal* pack_diagonal2, not_diagonal* pack_notdiagonal2,
                      int& size6, int count_notdiagonal1, int count_notdiagonal2, int count_diagonal1, int count_diagonal2, bool& success, bool& nulls)
    {
        size6 = n;
        int maxsize_notdiagonal = static_cast<int>(floor(n * n * 0.15));
        pack_diagonal6 = new diagonal[n];
        pack_notdiagonal6 = new not_diagonal[maxsize_notdiagonal];
        count_notdiagonal6 = 0;
        int d1 = 0, d2 = 0;

        //умножение диагональных элементов
        while (d1 < count_diagonal1 && d2 < count_diagonal2) {
            if (pack_diagonal1[d1].i == pack_diagonal2[d2].i) {
                double value = pack_diagonal1[d1].value * pack_diagonal2[d2].value;
                double formatted_value;
                if (format_value(value, formatted_value)) {
                    pack_diagonal6[count_diagonal6].value = formatted_value;
                    pack_diagonal6[count_diagonal6].i = pack_diagonal1[d1].i;
                    count_diagonal6++;
                    nulls = false;
                }

                d1++;
                d2++;
            }
            else if (pack_diagonal1[d1].i < pack_diagonal2[d2].i) {
                d1++;
            }
            else if (pack_diagonal1[d1].i > pack_diagonal2[d2].i) {
                d2++;
            }
        }

        //умножение недиагональных элементов
        d1 = 0, d2 = 0;
        while (d1 < count_notdiagonal1 && d2 < count_notdiagonal2) {
            if (pack_notdiagonal1[d1].i == pack_notdiagonal2[d2].i && pack_notdiagonal1[d1].j == pack_notdiagonal2[d2].j) {
                if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                    show_warning();
                    success = false;
                    return;
                }
                else {
                    double value = pack_notdiagonal1[d1].value * pack_notdiagonal2[d2].value;
                    double formatted_value;
                    if (format_value(value, formatted_value)) {
                        pack_notdiagonal6[count_notdiagonal6].value = formatted_value;
                        pack_notdiagonal6[count_notdiagonal6].i = pack_notdiagonal1[d1].i;
                        pack_notdiagonal6[count_notdiagonal6].j = pack_notdiagonal1[d1].j;
                        count_notdiagonal6++;
                        nulls = false;
                    }
                    d1++;
                    d2++;
                }
            }
            else if (count_notdiagonal6 + count_diagonal6 > maxsize_notdiagonal) {
                show_warning();
                success = false;
                return;
            }
            else if (pack_notdiagonal1[d1].i < pack_notdiagonal2[d2].i ||
                     (pack_notdiagonal1[d1].i == pack_notdiagonal2[d2].i && pack_notdiagonal1[d1].j < pack_notdiagonal2[d2].j)) {
                d1++;
            }
            else d2++;
        }
        success = true;
    }


private slots:
    //НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(0);
    }

    void onContinueClicked() {
        if (selectedMatrixIndexLeft == -1 || selectedMatrixIndexRight == -1) {
            showMessageBox("Ошибка", "Выберите две разреженные матрицы для операции.");
            stackedWidget->setCurrentIndex(9);
        }
        //ОКНО ПРЕДУПРЕЖДЕНИЯ: если размерность матриц неодинаковая
        else if (size[selectedMatrixIndexLeft - 1] != size[selectedMatrixIndexRight - 1]) {
            showMessageBox("Ошибка", "Разная размерность выбранных разреженных матриц. Операция невозможна.");
            return;
        }
        bool additionSuccessful = false;    //для отслеживания когда коэф. ненулевых эл. > 15%
        bool nulls = true;

        //переход на операции
        switch (operation) {
        case 1: //СЛОЖЕНИЕ
            switch (selectedMatrixIndexLeft) {
            case 1:
                switch (selectedMatrixIndexRight) {
                case 1: add_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal1, count_notdiagonal1, count_diagonal1, count_diagonal1, additionSuccessful, nulls); break;
                case 2: add_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal1, count_notdiagonal2, count_diagonal1, count_diagonal2, additionSuccessful, nulls); break;
                case 3: add_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal1, count_notdiagonal3, count_diagonal1, count_diagonal3, additionSuccessful, nulls); break;
                case 4: add_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal1, count_notdiagonal4, count_diagonal1, count_diagonal4, additionSuccessful, nulls); break;
                case 5: add_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal1, count_notdiagonal5, count_diagonal1, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 2:
                switch (selectedMatrixIndexRight) {
                case 1: add_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal2, count_notdiagonal1, count_diagonal2, count_diagonal1, additionSuccessful, nulls); break;
                case 2: add_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal2, count_notdiagonal2, count_diagonal2, count_diagonal2, additionSuccessful, nulls); break;
                case 3: add_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal2, count_notdiagonal3, count_diagonal2, count_diagonal3, additionSuccessful, nulls); break;
                case 4: add_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal2, count_notdiagonal4, count_diagonal2, count_diagonal4, additionSuccessful, nulls); break;
                case 5: add_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal2, count_notdiagonal5, count_diagonal2, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 3:
                switch (selectedMatrixIndexRight) {
                case 1: add_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal3, count_notdiagonal1, count_diagonal3, count_diagonal1, additionSuccessful, nulls); break;
                case 2: add_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal3, count_notdiagonal2, count_diagonal3, count_diagonal2, additionSuccessful, nulls); break;
                case 3: add_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal3, count_notdiagonal3, count_diagonal3, count_diagonal3, additionSuccessful, nulls); break;
                case 4: add_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal3, count_notdiagonal4, count_diagonal3, count_diagonal4, additionSuccessful, nulls); break;
                case 5: add_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal3, count_notdiagonal5, count_diagonal3, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 4:
                switch (selectedMatrixIndexRight) {
                case 1: add_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal4, count_notdiagonal1, count_diagonal4, count_diagonal1, additionSuccessful, nulls); break;
                case 2: add_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal4, count_notdiagonal2, count_diagonal4, count_diagonal2, additionSuccessful, nulls); break;
                case 3: add_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal4, count_notdiagonal3, count_diagonal4, count_diagonal3, additionSuccessful, nulls); break;
                case 4: add_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal4, count_notdiagonal4, count_diagonal4, count_diagonal4, additionSuccessful, nulls); break;
                case 5: add_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal4, count_notdiagonal5, count_diagonal4, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 5:
                switch (selectedMatrixIndexRight) {
                case 1: add_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal5, count_notdiagonal1, count_diagonal5, count_diagonal1, additionSuccessful, nulls); break;
                case 2: add_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal5, count_notdiagonal2, count_diagonal5, count_diagonal2, additionSuccessful, nulls); break;
                case 3: add_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal5, count_notdiagonal3, count_diagonal5, count_diagonal3, additionSuccessful, nulls); break;
                case 4: add_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal5, count_notdiagonal4, count_diagonal5, count_diagonal4, additionSuccessful, nulls); break;
                case 5: add_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal5, count_notdiagonal5, count_diagonal5, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            }
            //если коэф. < 15%
            if (additionSuccessful) {
                if (nulls){
                    showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён.");
                    stackedWidget->setCurrentIndex(9);
                }else{
                    selectedMatrixIndexLeft = -1;
                    selectedMatrixIndexRight = -1;
                    OperatViewPage* operView = new OperatViewPage(stackedWidget, size, filled, this);
                    operView->setU();
                    stackedWidget->addWidget(operView);
                    stackedWidget->setCurrentWidget(operView);
                }
            }
            else {
                stackedWidget->setCurrentIndex(9);   //>15%
            }
            break;

        case 2: //ВЫЧИТЕНИЕ
            switch (selectedMatrixIndexLeft) {
            case 1:
                switch (selectedMatrixIndexRight) {
                case 1: showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён."); break;
                case 2: sub_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal1, count_notdiagonal2, count_diagonal1, count_diagonal2, additionSuccessful, nulls); break;
                case 3: sub_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal1, count_notdiagonal3, count_diagonal1, count_diagonal3, additionSuccessful, nulls); break;
                case 4: sub_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal1, count_notdiagonal4, count_diagonal1, count_diagonal4, additionSuccessful, nulls); break;
                case 5: sub_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal1, count_notdiagonal5, count_diagonal1, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 2:
                switch (selectedMatrixIndexRight) {
                case 1: sub_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal2, count_notdiagonal1, count_diagonal2, count_diagonal1, additionSuccessful, nulls); break;
                case 2: showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён."); break;
                case 3: sub_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal2, count_notdiagonal3, count_diagonal2, count_diagonal3, additionSuccessful, nulls); break;
                case 4: sub_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal2, count_notdiagonal4, count_diagonal2, count_diagonal4, additionSuccessful, nulls); break;
                case 5: sub_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal2, count_notdiagonal5, count_diagonal2, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 3:
                switch (selectedMatrixIndexRight) {
                case 1: sub_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal3, count_notdiagonal1, count_diagonal3, count_diagonal1, additionSuccessful, nulls); break;
                case 2: sub_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal3, count_notdiagonal2, count_diagonal3, count_diagonal2, additionSuccessful, nulls); break;
                case 3: showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён."); break;
                case 4: sub_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal3, count_notdiagonal4, count_diagonal3, count_diagonal4, additionSuccessful, nulls); break;
                case 5: sub_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal3, count_notdiagonal5, count_diagonal3, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 4:
                switch (selectedMatrixIndexRight) {
                case 1: sub_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal4, count_notdiagonal1, count_diagonal4, count_diagonal1, additionSuccessful, nulls); break;
                case 2: sub_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal4, count_notdiagonal2, count_diagonal4, count_diagonal2, additionSuccessful, nulls); break;
                case 3: sub_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal4, count_notdiagonal3, count_diagonal4, count_diagonal3, additionSuccessful, nulls); break;
                case 4: showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён."); break;
                case 5: sub_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal4, count_notdiagonal5, count_diagonal4, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 5:
                switch (selectedMatrixIndexRight) {
                case 1: sub_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal5, count_notdiagonal1, count_diagonal5, count_diagonal1, additionSuccessful, nulls); break;
                case 2: sub_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal5, count_notdiagonal2, count_diagonal5, count_diagonal2, additionSuccessful, nulls); break;
                case 3: sub_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal5, count_notdiagonal3, count_diagonal5, count_diagonal3, additionSuccessful, nulls); break;
                case 4: sub_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal5, count_notdiagonal4, count_diagonal5, count_diagonal4, additionSuccessful, nulls); break;
                case 5: showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён."); break;
                }
                break;
            }
            if (additionSuccessful) {
                if (nulls){
                    showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён.");
                    stackedWidget->setCurrentIndex(9);
                }else{
                    selectedMatrixIndexLeft = -1;
                    selectedMatrixIndexRight = -1;
                    OperatViewPage* operView = new OperatViewPage(stackedWidget, size, filled, this);
                    operView->setU();
                    stackedWidget->addWidget(operView);
                    stackedWidget->setCurrentWidget(operView);
                }
            }
            else {
                stackedWidget->setCurrentIndex(9);
            }
            break;

        case 3: //УМНОЖЕНИЕ
            switch (selectedMatrixIndexLeft) {
            case 1:
                switch (selectedMatrixIndexRight) {
                case 1: mul_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal1, count_notdiagonal1, count_diagonal1, count_diagonal1, additionSuccessful, nulls); break;
                case 2: mul_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal1, count_notdiagonal2, count_diagonal1, count_diagonal2, additionSuccessful, nulls); break;
                case 3: mul_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal1, count_notdiagonal3, count_diagonal1, count_diagonal3, additionSuccessful, nulls); break;
                case 4: mul_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal1, count_notdiagonal4, count_diagonal1, count_diagonal4, additionSuccessful, nulls); break;
                case 5: mul_matrices(size[0], pack_diagonal1, pack_notdiagonal1, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal1, count_notdiagonal5, count_diagonal1, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 2:
                switch (selectedMatrixIndexRight) {
                case 1: mul_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal2, count_notdiagonal1, count_diagonal2, count_diagonal1, additionSuccessful, nulls); break;
                case 2: mul_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal2, count_notdiagonal2, count_diagonal2, count_diagonal2, additionSuccessful, nulls); break;
                case 3: mul_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal2, count_notdiagonal3, count_diagonal2, count_diagonal3, additionSuccessful, nulls); break;
                case 4: mul_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal2, count_notdiagonal4, count_diagonal2, count_diagonal4, additionSuccessful, nulls); break;
                case 5: mul_matrices(size[1], pack_diagonal2, pack_notdiagonal2, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal2, count_notdiagonal5, count_diagonal2, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 3:
                switch (selectedMatrixIndexRight) {
                case 1: mul_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal3, count_notdiagonal1, count_diagonal3, count_diagonal1, additionSuccessful, nulls); break;
                case 2: mul_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal3, count_notdiagonal2, count_diagonal3, count_diagonal2, additionSuccessful, nulls); break;
                case 3: mul_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal3, count_notdiagonal3, count_diagonal3, count_diagonal3, additionSuccessful, nulls); break;
                case 4: mul_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal3, count_notdiagonal4, count_diagonal3, count_diagonal4, additionSuccessful, nulls); break;
                case 5: mul_matrices(size[2], pack_diagonal3, pack_notdiagonal3, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal3, count_notdiagonal5, count_diagonal3, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 4:
                switch (selectedMatrixIndexRight) {
                case 1: mul_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal4, count_notdiagonal1, count_diagonal4, count_diagonal1, additionSuccessful, nulls); break;
                case 2: mul_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal4, count_notdiagonal2, count_diagonal4, count_diagonal2, additionSuccessful, nulls); break;
                case 3: mul_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal4, count_notdiagonal3, count_diagonal4, count_diagonal3, additionSuccessful, nulls); break;
                case 4: mul_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal4, count_notdiagonal4, count_diagonal4, count_diagonal4, additionSuccessful, nulls); break;
                case 5: mul_matrices(size[3], pack_diagonal4, pack_notdiagonal4, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal4, count_notdiagonal5, count_diagonal4, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            case 5:
                switch (selectedMatrixIndexRight) {
                case 1: mul_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal1, pack_notdiagonal1,
                                 size[5], count_notdiagonal5, count_notdiagonal1, count_diagonal5, count_diagonal1, additionSuccessful, nulls); break;
                case 2: mul_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal2, pack_notdiagonal2,
                                 size[5], count_notdiagonal5, count_notdiagonal2, count_diagonal5, count_diagonal2, additionSuccessful, nulls); break;
                case 3: mul_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal3, pack_notdiagonal3,
                                 size[5], count_notdiagonal5, count_notdiagonal3, count_diagonal5, count_diagonal3, additionSuccessful, nulls); break;
                case 4: mul_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal4, pack_notdiagonal4,
                                 size[5], count_notdiagonal5, count_notdiagonal4, count_diagonal5, count_diagonal4, additionSuccessful, nulls); break;
                case 5: mul_matrices(size[4], pack_diagonal5, pack_notdiagonal5, pack_diagonal5, pack_notdiagonal5,
                                 size[5], count_notdiagonal5, count_notdiagonal5, count_diagonal5, count_diagonal5, additionSuccessful, nulls); break;
                }
                break;
            }
            if (additionSuccessful) {
                if (nulls){
                    showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён.");
                    stackedWidget->setCurrentIndex(9);
                }else{
                    selectedMatrixIndexLeft = -1;
                    selectedMatrixIndexRight = -1;
                    OperatViewPage* operView = new OperatViewPage(stackedWidget, size, filled, this);
                    operView->setU();
                    stackedWidget->addWidget(operView);
                    stackedWidget->setCurrentWidget(operView);
                }
            }
            else {
                stackedWidget->setCurrentIndex(9);
            }
            break;
        }

    }

    //СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
    void onMatrixCheckBoxClicked() {
        QCheckBox* senderCheckBox = qobject_cast<QCheckBox*>(sender());

        if (senderCheckBox && senderCheckBox->isChecked()) {
            // Сброс всех чекбоксов в левом столбце
            if (senderCheckBox == matrix1Left || senderCheckBox == matrix2Left || senderCheckBox == matrix3Left || senderCheckBox == matrix4Left || senderCheckBox == matrix5Left) {
                selectedMatrixIndexLeft = (senderCheckBox == matrix1Left) ? 1 :
                                              (senderCheckBox == matrix2Left) ? 2 :
                                              (senderCheckBox == matrix3Left) ? 3 :
                                              (senderCheckBox == matrix4Left) ? 4 :
                                              (senderCheckBox == matrix5Left) ? 5 : -1;

                // Сброс остальных чекбоксов слева
                matrix1Left->setChecked(senderCheckBox == matrix1Left);
                matrix2Left->setChecked(senderCheckBox == matrix2Left);
                matrix3Left->setChecked(senderCheckBox == matrix3Left);
                matrix4Left->setChecked(senderCheckBox == matrix4Left);
                matrix5Left->setChecked(senderCheckBox == matrix5Left);
            }

            // Сброс всех чекбоксов в правом столбце
            else if (senderCheckBox == matrix1Right || senderCheckBox == matrix2Right || senderCheckBox == matrix3Right || senderCheckBox == matrix4Right || senderCheckBox == matrix5Right) {
                selectedMatrixIndexRight = (senderCheckBox == matrix1Right) ? 1 :
                                               (senderCheckBox == matrix2Right) ? 2 :
                                               (senderCheckBox == matrix3Right) ? 3 :
                                               (senderCheckBox == matrix4Right) ? 4 :
                                               (senderCheckBox == matrix5Right) ? 5 : -1;

                // Сброс остальных чекбоксов справа
                matrix1Right->setChecked(senderCheckBox == matrix1Right);
                matrix2Right->setChecked(senderCheckBox == matrix2Right);
                matrix3Right->setChecked(senderCheckBox == matrix3Right);
                matrix4Right->setChecked(senderCheckBox == matrix4Right);
                matrix5Right->setChecked(senderCheckBox == matrix5Right);
            }
        }
    }

};


//ВЫБОР МАТРИЦЫ ДЛЯ ОПЕРАЦИИ И ОБРАТНАЯ МАТРИЦА
class Iterpre_Matrix : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;          //номер выбранной матрицы в левом столбце
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLineEdit* dimensionInput;            //поле ввода размерности
    QString inputMethod;                  //выбранный метод ввода
    //чекбоксы для выбора первой матрицы для операции
    QCheckBox* matrix1;
    QCheckBox* matrix2;
    QCheckBox* matrix3;
    QCheckBox* matrix4;
    QCheckBox* matrix5;

    //проверка заполненности матрицы
    bool isMatrixFilled(int index) const {
        return (index >= 0 && index < 5) ? filled[index] : false;
    }

    //обновление состояния выбора матрицы
    void updateCheckBoxStates() {
        matrix1->setEnabled(isMatrixFilled(0));
        matrix2->setEnabled(isMatrixFilled(1));
        matrix3->setEnabled(isMatrixFilled(2));
        matrix4->setEnabled(isMatrixFilled(3));
        matrix5->setEnabled(isMatrixFilled(4));

        matrix1->setChecked(false);
        matrix2->setChecked(false);
        matrix3->setChecked(false);
        matrix4->setChecked(false);
        matrix5->setChecked(false);
    }

public:
    Iterpre_Matrix(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(17);

        QLabel* titleLabel1 = new QLabel("Матрица для обратной разреженной матрицы:", this);
        titleLabel1->setStyleSheet("QLabel { color: #001a33; }");
        titleLabel1->setFont(Fontstr);

        QHBoxLayout* titleLayout = new QHBoxLayout();
        titleLayout->addWidget(titleLabel1);
        titleLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(titleLayout);

        //ЧЕКБОКСЫ ВЫБОРА МАТРИЦЫ
        QVBoxLayout* checkBoxLayout = new QVBoxLayout();
        matrix1 = new QCheckBox("Матрица №1", this);
        matrix2 = new QCheckBox("Матрица №2", this);
        matrix3 = new QCheckBox("Матрица №3", this);
        matrix4 = new QCheckBox("Матрица №4", this);
        matrix5 = new QCheckBox("Матрица №5", this);

        QCheckBox* matrices[] = { matrix1, matrix2, matrix3, matrix4, matrix5 };
        applyCheckBoxStyle(matrices, sizeof(matrices) / sizeof(matrices[0]));  //стиль

        //РАЗМЕР ТЕКСТА
        matrix1->setFont(Font);
        matrix2->setFont(Font);
        matrix3->setFont(Font);
        matrix4->setFont(Font);
        matrix5->setFont(Font);

        updateCheckBoxStates();

        connect(matrix1, &QCheckBox::clicked, this, &Iterpre_Matrix::onMatrixCheckBoxClicked);
        connect(matrix2, &QCheckBox::clicked, this, &Iterpre_Matrix::onMatrixCheckBoxClicked);
        connect(matrix3, &QCheckBox::clicked, this, &Iterpre_Matrix::onMatrixCheckBoxClicked);
        connect(matrix4, &QCheckBox::clicked, this, &Iterpre_Matrix::onMatrixCheckBoxClicked);
        connect(matrix5, &QCheckBox::clicked, this, &Iterpre_Matrix::onMatrixCheckBoxClicked);

        checkBoxLayout->addWidget(matrix1);
        checkBoxLayout->addWidget(matrix2);
        checkBoxLayout->addWidget(matrix3);
        checkBoxLayout->addWidget(matrix4);
        checkBoxLayout->addWidget(matrix5);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(checkBoxLayout);

        //кнопка "Выполнить операцию"
        QPushButton* continueButton = new QPushButton("Выполнить операцию", this);
        continueButton->setFixedSize(290, 50);
        continueButton->setFont(Font);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);

        //КНОПКИ
        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(continueButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        layout->addLayout(exitLayout);
        setLayout(layout);
        layout->setSpacing(2);
        layout->setContentsMargins(5, 5, 5, 5);

        connect(backButton, &QPushButton::clicked, this, &Iterpre_Matrix::onBackButtonClicked);
        connect(continueButton, &QPushButton::clicked, this, &Iterpre_Matrix::onContinueClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

protected:
    void showEvent(QShowEvent* event) override {
        QWidget::showEvent(event);
        updateCheckBoxStates();
    }

signals:
    void matrixSelected(int index);

private:

    //ОКНО ПРЕДУПРЕЖДЕНИЯ: если при операции получилась пустая мтарица
    void oper_warning() {
        showMessageBox("Ошибка", "Результат операции - пустая матрица. Результат удалён.");
        //Удаление вспомогательной матрицы для операций
        size[5] = 0;
        if (pack_diagonal6 != nullptr) {
            delete[] pack_diagonal6;
            pack_diagonal6 = nullptr;
        }
        count_diagonal6 = 0;

        if (pack_notdiagonal6 != nullptr) {
            delete[] pack_notdiagonal6;
            pack_notdiagonal6 = nullptr;
        }
        count_notdiagonal6 = 0;
    }

    //ОКНО ПРЕДУПРЕЖДЕНИЯ: если Обратная матрица не существует
    void not_exist(){
        showMessageBox("Ошибка", "Обратная разреженная матрица не существует. Результат удалён.");
        //Удаление вспомогательной матрицы для операций
        size[5] = 0;
        if (pack_diagonal6 != nullptr) {
            delete[] pack_diagonal6;
            pack_diagonal6 = nullptr;
        }
        count_diagonal6 = 0;

        if (pack_notdiagonal6 != nullptr) {
            delete[] pack_notdiagonal6;
            pack_notdiagonal6 = nullptr;
        }
        count_notdiagonal6 = 0;
    }

    //ОБРАТНАЯ МАТРИЦА
    void interp_matrices(int n, diagonal* pack_diagonal, not_diagonal* pack_notdiagonal,
                         int& size6, int count_notdiagonal, int count_diagonal, bool& success, bool& nulls)
    {
        size6 = n;
        int maxsize_notdiagonal = static_cast<int>(floor(n * n * 0.15));
        pack_diagonal6 = new diagonal[n];
        pack_notdiagonal6 = new not_diagonal[maxsize_notdiagonal];
        count_diagonal6 = 0;
        count_notdiagonal6 = 0;
        success = true;

        int temp_count_notdiagonal = 0;
        int temp_count_diagonal = n;
        diagonal* temp_diagonal = new diagonal[2 * n];      //2*n т.к. в диагональную матрицу записываем и единичную матрицу
        int size = n * n * 0.15;

        for (int i = 0; i < count_diagonal; i++) {          //копирование диагональной матрицы во временную
            temp_diagonal[i].value = pack_diagonal[i].value;
            temp_diagonal[i].i = pack_diagonal[i].i;
        }

        for (int i = n; i < 2 * n; i++) {                               //заполнение диагональной матрицы единицами с n по 2*n (доп единичная матрица/она же будущая обратная)
            temp_diagonal[i].value = 1;
            //temp_diagonal[i].i = i - count_diagonal + 1;
            temp_diagonal[i].i = i - n + 1;
        }

        not_diagonal* temp_notdiagonal = new not_diagonal[2 * size];
        for (int i = 0; i < count_notdiagonal; i++) {                   //копирование недиагональной матрицы
            temp_notdiagonal[i].value = pack_notdiagonal[i].value;
            temp_notdiagonal[i].i = pack_notdiagonal1[i].i;
            temp_notdiagonal[i].j = pack_notdiagonal1[i].j;
        }

        bool found_diagonal = false;
        double value_diagonal = 1;

        for (int k = 1; k <= n; k++) {  //Проходим по каждой строке матрицы
            found_diagonal = false;
            for (int i = 0; i < count_diagonal; i++) {
                if (temp_diagonal[i].i == k) {                   //если диагональный элемент не равен нулю
                    found_diagonal = true;
                    value_diagonal = temp_diagonal[i].value;        //сохраняем значение диагонального элемента
                    temp_diagonal[i].value = temp_diagonal[i].value / value_diagonal;                 //поделили диагональный элемент на себя (делим строку на диагональный элемент)
                    break;
                }
            }
            int change_rowindex = 1;
            double change_value = 0;
            if (!found_diagonal) {      //если на диагонали ноль - ищем подходящую строчку среди массива недиагональных и меняем их местами
                for (int i = 0; i < count_notdiagonal; i++) {   //ищем строчку с ненулевым элементом с индексом j = k (равным индексу нулевого элемента на диагонали)
                    if (temp_notdiagonal[i].j == k) {
                        change_rowindex = temp_notdiagonal[i].i;    //поиск индекса работает!!!
                        change_value = temp_notdiagonal[i].value;

                        temp_notdiagonal[i].i = 0;
                        temp_notdiagonal[i].j = 0;
                        temp_notdiagonal[i].value = 0;          //удаляем недиагональный элемент (обнуляем)
                        found_diagonal = true;
                        break;
                    }
                }
                value_diagonal = change_value;
                //cout << change_rowindex << " ";       //проверка индекса
                temp_diagonal[count_diagonal].value = change_value;     //обмен строк местами (работает с элементами полностью на побочной диагонали!!!)
                temp_diagonal[count_diagonal].i = k;
                count_diagonal++;          //добавление элементов в диагональную матрицу

                //думаю, что нужно обменивать строки полностью то есть [i][j], [j][i] = [j][i], [i][j], а тут только [i][j] = [j][i]
                double temp_value = temp_diagonal[n + k - 1].value;     //переменная для сохранения значения единичной доп матрицы
                temp_diagonal[n + k - 1].value = 0;
                temp_diagonal[n + k - 1].i = 0;
                temp_count_diagonal--;      //удаление элемента из единичной матрицы (т.к. произошел обмен)

                temp_notdiagonal[size + temp_count_notdiagonal].value = temp_value;
                temp_notdiagonal[size + temp_count_notdiagonal].i = change_rowindex;
                temp_notdiagonal[size + temp_count_notdiagonal].j = k;
                temp_count_notdiagonal++;

            }
            if (!found_diagonal) {      //если повторно не нашли элемента, то обратной матрицы не существует
                success = false;       //может быть, не уверена в таком критерии отсутствия обратной матрицы
                break;
            }

            //Делим всю строку k на диагональный элемент
            for (int i = 0; i < count_notdiagonal; i++) {
                if (temp_notdiagonal[i].i == k) {
                    temp_notdiagonal[i].value = temp_notdiagonal[i].value / value_diagonal;     //делим всю строку основной матрицы на диагональный элемент
                }
            }
            for (int i = 0; i < temp_count_notdiagonal; i++) {
                if (temp_notdiagonal[size + i].i == k) {
                    temp_notdiagonal[size + i].value = temp_notdiagonal[size + i].value / value_diagonal;   //делим всю строку доп матрицы на диагональный элемент
                }
            }
            temp_diagonal[n + k - 1].value = temp_diagonal[n + k - 1].value / value_diagonal;       //делим диагональный элемент доп матрицы

            //Зануляем столбец с индексом k
            int row_index = 0;
            double del = 1;
            for (int i = 0; i < count_notdiagonal; i++) {
                row_index = 0;
                del = 1;
                if (temp_notdiagonal[i].value != 0 && temp_notdiagonal[i].j == k) {     //ищем ненулевой элемент в столбце
                    row_index = temp_notdiagonal[i].i;              //сохраняем индекс строки, из которой будем вычитать строку k
                    del = del * temp_notdiagonal[i].value;          //запоминаем множитель, на который нужно умножить строку
                    temp_notdiagonal[i].value = temp_notdiagonal[i].value - del;    //зануляем элемент
                }
                if (row_index > 0) {        //если строку нужно вычитать
                    //Зануление столбцов в основной матрице
                    for (int j = 0; j < count_notdiagonal; j++) {
                        bool found_element = false;
                        if (temp_notdiagonal[j].i == k && temp_notdiagonal[j].value != 0) {     //если в строке с индексом k есть ненулевой элемент
                            if (row_index == temp_notdiagonal[j].j) {
                                for (int p = 0; p < count_diagonal; p++) {
                                    if (temp_diagonal[p].i == row_index && temp_diagonal[p].i == temp_notdiagonal[j].j) {
                                        temp_diagonal[p].value = temp_diagonal[p].value - del * temp_notdiagonal[j].value;        //вычитаем строки друг из друга (вычитаем элементы вроде [1][1] - [3][1], [1][2] - [3][2] и тд)
                                        found_element = true;
                                        break;
                                    }
                                }
                            }
                            else {
                                for (int p = 0; p < count_notdiagonal; p++) {
                                    if (temp_notdiagonal[p].i == row_index && temp_notdiagonal[p].j == temp_notdiagonal[j].j) {
                                        temp_notdiagonal[p].value = temp_notdiagonal[p].value - del * temp_notdiagonal[j].value;        //вычитаем строки друг из друга (вычитаем элементы вроде [1][1] - [3][1], [1][2] - [3][2] и тд)
                                        found_element = true;
                                        break;
                                    }
                                }
                            }

                            if (!found_element && temp_notdiagonal[j].value != 0) {   //если не нашли элемент, который будет вычитаться
                                if (row_index == k) {   //если диагональный
                                    temp_diagonal[row_index - 1].value = -del * temp_notdiagonal[j].value;  //просто добавляем новый элемент в диагональный массив
                                    temp_diagonal[row_index - 1].i = row_index;
                                }
                                else {
                                    temp_notdiagonal[count_notdiagonal].value = -del * temp_notdiagonal[j].value;  //просто добавляем новый элемент в недиагональный массив
                                    temp_notdiagonal[count_notdiagonal].i = row_index;
                                    temp_notdiagonal[count_notdiagonal].j = temp_notdiagonal[j].j;
                                    count_notdiagonal++;   //увеличиваем количество
                                }
                            }
                        }
                    }

                    bool found_dop = false;     //найден ли элемент под диагональным в доп матрице
                    //Вычитаем элементы под диагональным в доп матрице
                    if (temp_diagonal[n + k - 1].value != 0) {
                        for (int j = size; j < size + temp_count_notdiagonal; j++) {       //ищем в доп матрице элемент с i = row_index для вычитания
                            if (temp_notdiagonal[j].i == row_index && temp_notdiagonal[j].j == k) {
                                temp_notdiagonal[j].value = temp_notdiagonal[j].value - del * temp_diagonal[n + k - 1].value;        //вычитаем строки в доп матрице друг из друга (вычитаем элементы вроде [2][1] - [1][1], [3][1] - [1][1] и тд)
                                found_dop = true;
                                break;
                            }
                        }
                        if (!found_dop) {
                            temp_notdiagonal[size + temp_count_notdiagonal].value = -del * temp_diagonal[n + k - 1].value;
                            temp_notdiagonal[size + temp_count_notdiagonal].i = row_index;
                            temp_notdiagonal[size + temp_count_notdiagonal].j = k;
                            temp_count_notdiagonal++;
                        }
                    }

                    //Поиск по недиагональным элементам для вычитания
                    for (int j = size; j < size + temp_count_notdiagonal; j++) {
                        bool found_element = false;
                        if (temp_notdiagonal[j].i == k && temp_notdiagonal[j].value != 0) {     //если в строке с индексом k в доп матрице есть ненулевой элемент
                            for (int p = size; p < size + temp_count_notdiagonal; p++) {
                                if (temp_notdiagonal[p].i == row_index && temp_notdiagonal[p].j == temp_notdiagonal[j].j) {
                                    temp_notdiagonal[p].value = temp_notdiagonal[p].value - del * temp_notdiagonal[j].value;        //вычитаем строки друг из друга (вычитаем элементы вроде [1][1] - [3][1], [1][2] - [3][2] и тд)
                                    found_element = true;
                                    break;
                                }
                            }
                            if (!found_element) {       //если не нашли в недиагональном, ищем в диагональном
                                for (int p = n; p < 2 * n; p++) {
                                    if (temp_diagonal[p].i == row_index && temp_diagonal[p].i == temp_notdiagonal[j].j) {
                                        temp_diagonal[p].value = temp_diagonal[p].value - del * temp_notdiagonal[j].value;        //вычитаем строки друг из друга (вычитаем элементы вроде [1][1] - [3][1], [1][2] - [3][2] и тд)
                                        found_element = true;
                                        break;
                                    }
                                }
                            }

                            if (!found_element && temp_notdiagonal[j].value != 0) {   //если не нашли элемент, который будет вычитаться
                                if (row_index == k) {   //если диагональный
                                    temp_diagonal[n + row_index - 1].value = -del * temp_notdiagonal[j].value;  //просто добавляем новый элемент в диагональный массив
                                    temp_diagonal[n + row_index - 1].i = n + row_index;
                                }
                                else {
                                    temp_notdiagonal[size + temp_count_notdiagonal].value = -del * temp_notdiagonal[j].value;  //просто добавляем новый элемент в недиагональный массив
                                    temp_notdiagonal[size + temp_count_notdiagonal].i = row_index;
                                    temp_notdiagonal[size + temp_count_notdiagonal].j = temp_notdiagonal[j].j;
                                    temp_count_notdiagonal++;   //увеличиваем количество
                                }
                            }
                        }
                    }
                }
            }
        }

        if (success){
            for (int i = n; i < 2*n; i++){
                if (temp_diagonal[i].value != 0){
                    pack_diagonal6[count_diagonal6].value = temp_diagonal[i].value;
                    pack_diagonal6[count_diagonal6].i = temp_diagonal[i].i;
                    count_diagonal6++;
                }
            }
            for (int i = size; i < size + temp_count_notdiagonal; i++){
                if (temp_notdiagonal[i].value != 0){
                    pack_notdiagonal6[count_notdiagonal6].value = temp_notdiagonal[i].value;
                    pack_notdiagonal6[count_notdiagonal6].i = temp_notdiagonal[i].i;
                    pack_notdiagonal6[count_notdiagonal6].j = temp_notdiagonal[i].j;
                    count_notdiagonal6++;
                }
            }
        }
        nulls = (count_diagonal6 == 0 && count_notdiagonal6 == 0);
        delete[] temp_diagonal;
        delete[] temp_notdiagonal;
    }


private slots:
    //НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(0);
    }

    //кнопка "выполнить операцию"
    void onContinueClicked() {
        if (selectedMatrixIndex == -1) {
            showMessageBox("Ошибка", "Выберите разреженную матрицу для операции.");
            stackedWidget->setCurrentIndex(13);
        }
        bool additionSuccessful = false;    //для отслеживания когда коэф. ненулевых эл. > 15%
        bool nulls = true;

        switch (selectedMatrixIndex) {
        case 1: interp_matrices(size[0], pack_diagonal1, pack_notdiagonal1, size[5], count_notdiagonal1, count_diagonal1, additionSuccessful, nulls); break;
        case 2: interp_matrices(size[1], pack_diagonal2, pack_notdiagonal2, size[5], count_notdiagonal2, count_diagonal2, additionSuccessful, nulls); break;
        case 3: interp_matrices(size[2], pack_diagonal3, pack_notdiagonal3, size[5], count_notdiagonal3, count_diagonal3, additionSuccessful, nulls); break;
        case 4: interp_matrices(size[3], pack_diagonal4, pack_notdiagonal4, size[5], count_notdiagonal4, count_diagonal4, additionSuccessful, nulls); break;
        case 5: interp_matrices(size[4], pack_diagonal5, pack_notdiagonal5, size[5], count_notdiagonal5, count_diagonal5, additionSuccessful, nulls); break;
        }
        if (additionSuccessful) {
            if (nulls){
                oper_warning();      //если результат операции - пустая матрица
                stackedWidget->setCurrentIndex(13);
            }else{
                performSorting(6);
                Obr = true;
                OperatViewPage* operView = new OperatViewPage(stackedWidget, size, filled, this);
                operView->setU();
                stackedWidget->addWidget(operView);
                stackedWidget->setCurrentWidget(operView);
            }
        }
        else {      //если нет обратной
            not_exist();
            stackedWidget->setCurrentIndex(13);
        }
    }

    //СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
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
};


//ВЫБОР МАТРИЦЫ ДЛЯ УДАЛЕНИЯ
class RemovMatrixInput : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLineEdit* dimensionInput;            //поле ввода размерности
    QString inputMethod;                  //выбранный метод ввода
    //чекбоксы выбра матриц
    QCheckBox* matrix1;
    QCheckBox* matrix2;
    QCheckBox* matrix3;
    QCheckBox* matrix4;
    QCheckBox* matrix5;

    //проверка заполненности матрицы
    bool isMatrixFilled(int index) const {
        return (index >= 0 && index < 5) ? filled[index] : false;
    }

    //обновление состояния выбора матрицы
    void updateCheckBoxStates() {
        matrix1->setEnabled(isMatrixFilled(0));
        matrix2->setEnabled(isMatrixFilled(1));
        matrix3->setEnabled(isMatrixFilled(2));
        matrix4->setEnabled(isMatrixFilled(3));
        matrix5->setEnabled(isMatrixFilled(4));
        matrix1->setChecked(false);
        matrix2->setChecked(false);
        matrix3->setChecked(false);
        matrix4->setChecked(false);
        matrix5->setChecked(false);
    }

public:
    RemovMatrixInput(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        QLabel* titleLabel = new QLabel("Выберите разреженную матрицу для удаления:", this);
        titleLabel->setAlignment(Qt::AlignCenter);;
        titleLabel->setFont(Fontstr);
        titleLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(titleLabel);

        //ЧЕКБОКСЫ ВЫБОРА МАТРИЦЫ
        QVBoxLayout* checkBoxLayout = new QVBoxLayout();
        matrix1 = new QCheckBox("Матрица №1", this);
        matrix2 = new QCheckBox("Матрица №2", this);
        matrix3 = new QCheckBox("Матрица №3", this);
        matrix4 = new QCheckBox("Матрица №4", this);
        matrix5 = new QCheckBox("Матрица №5", this);

        QCheckBox* matrices[] = { matrix1, matrix2, matrix3, matrix4, matrix5 };
        applyCheckBoxStyle(matrices, sizeof(matrices) / sizeof(matrices[0]));  //стиль

        //РАЗМЕР ТЕКСТА
        matrix1->setFont(Font);
        matrix2->setFont(Font);
        matrix3->setFont(Font);
        matrix4->setFont(Font);
        matrix5->setFont(Font);

        updateCheckBoxStates();

        connect(matrix1, &QCheckBox::clicked, this, &RemovMatrixInput::onMatrixCheckBoxClicked);
        connect(matrix2, &QCheckBox::clicked, this, &RemovMatrixInput::onMatrixCheckBoxClicked);
        connect(matrix3, &QCheckBox::clicked, this, &RemovMatrixInput::onMatrixCheckBoxClicked);
        connect(matrix4, &QCheckBox::clicked, this, &RemovMatrixInput::onMatrixCheckBoxClicked);
        connect(matrix5, &QCheckBox::clicked, this, &RemovMatrixInput::onMatrixCheckBoxClicked);

        checkBoxLayout->addWidget(matrix1);
        checkBoxLayout->addWidget(matrix2);
        checkBoxLayout->addWidget(matrix3);
        checkBoxLayout->addWidget(matrix4);
        checkBoxLayout->addWidget(matrix5);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(checkBoxLayout);

        // Кнопка "Продолжить"
        QPushButton* continueButton = new QPushButton("Продолжить", this);
        continueButton->setFixedSize(290, 50);
        continueButton->setFont(Font);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);

        //КНОПКИ
        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(continueButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        layout->addLayout(exitLayout);
        setLayout(layout);
        layout->setSpacing(2);
        layout->setContentsMargins(5, 5, 5, 5);

        //ПЕРЕХОДЫ НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(backButton, &QPushButton::clicked, this, &RemovMatrixInput::onBackButtonClicked);
        connect(continueButton, &QPushButton::clicked, this, &RemovMatrixInput::onContinueClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

protected:
    void showEvent(QShowEvent* event) override {
        QWidget::showEvent(event);
        updateCheckBoxStates();
    }

signals:
    void matrixSelected(int index);

public slots:
    //СОХРАНЕНИЕ ВЫБРАННОГО МЕТОДА ВВОДА МАТРИЦЫ
    void setInputMethod(const QString& method) {
        inputMethod = method;
    }

private slots:
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        selectedMatrixIndex = -1;
        stackedWidget->setCurrentIndex(0);
    }

    //УДАЛЕНИЕ МАТРИЦЫ
    void deleMatrix(diagonal*& pack_diagonal, int& count_diagonal, not_diagonal*& pack_notdiagonal, int& count_notdiagonal) {
        if (pack_diagonal != nullptr) {
            delete[] pack_diagonal;
            pack_diagonal = nullptr;
        }
        count_diagonal = 0;

        if (pack_notdiagonal != nullptr) {
            delete[] pack_notdiagonal;
            pack_notdiagonal = nullptr;
        }
        count_notdiagonal = 0;
        selectedMatrixIndex = -1;
    }

    //НАЖАТА КНОПКА "ПРОДОЛЖИТЬ"
    void onContinueClicked() {
        if (selectedMatrixIndex == -1) {
            showMessageBox("Ошибка", "Выберите разреженную матрицу для удаления.");
            return;
        }

        if (showBackWarning("Предупреждение", "Вы точно хотите удалить выбранную разреженную матрицу?")) {
            filled[selectedMatrixIndex - 1] = false;
            size[selectedMatrixIndex - 1] = 0;
            switch (selectedMatrixIndex) {
            case 1:
                deleMatrix(pack_diagonal1, count_diagonal1, pack_notdiagonal1, count_notdiagonal1);
                break;
            case 2:
                deleMatrix(pack_diagonal2, count_diagonal2, pack_notdiagonal2, count_notdiagonal2);
                break;
            case 3:
                deleMatrix(pack_diagonal3, count_diagonal3, pack_notdiagonal3, count_notdiagonal3);
                break;
            case 4:
                deleMatrix(pack_diagonal4, count_diagonal4, pack_notdiagonal4, count_notdiagonal4);
                break;
            case 5:
                deleMatrix(pack_diagonal5, count_diagonal5, pack_notdiagonal5, count_notdiagonal5);
                break;
            }
            stackedWidget->setCurrentIndex(0);
        } else {
            return;
        }
    }

    //СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
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
};


//ВЫВОД МАТРИЦЫ
class ViewPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QTableWidget* matrixTable;            //таблица для вывода матрицы
    QString filePath;                     //файл для сохранения туда матрицы
    QTimer* updateTimer;                  //таймер для заполнения
    int currentRow;                       //текущая строка, которую нужно заполнить
    int currentCol;                       //текущий столбец, который нужно заполнить
    LoadingDialog *loadingDialog = nullptr;
    QFuture<void> future;                 //хранение фоновой задачи
    bool cancelGeneration = false;        //флаг для отмены генерации

    //начало таймера
    void setupUpdateTimer() {
        updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &ViewPage::fillNextTableCell);
        currentRow = 0;
        currentCol = 0;
        updateTimer->start(1);
    }

    //переход к выводу таблицы с элементами матрицы
    void fillNextTableCell() {
        int cellsToFill = 10;   //количество ячеек, которые заполняем за один вызов
        for (int i = 0; i < cellsToFill && currentRow < size[selectedMatrixIndex - 1]; i++) {
            if (currentCol >= size[selectedMatrixIndex - 1]) {
                currentCol = 0;
                currentRow++;
            }
            if (currentRow >= size[selectedMatrixIndex - 1]) {
                updateTimer->stop();
                return;
            }

            //заполнение ячеек по выбранной матрице
            switch (selectedMatrixIndex) {
            case 1:
                fillCell(currentRow, currentCol, pack_diagonal1, count_diagonal1, pack_notdiagonal1, count_notdiagonal1);
                break;
            case 2:
                fillCell(currentRow, currentCol, pack_diagonal2, count_diagonal2, pack_notdiagonal2, count_notdiagonal2);
                break;
            case 3:
                fillCell(currentRow, currentCol, pack_diagonal3, count_diagonal3, pack_notdiagonal3, count_notdiagonal3);
                break;
            case 4:
                fillCell(currentRow, currentCol, pack_diagonal4, count_diagonal4, pack_notdiagonal4, count_notdiagonal4);
                break;
            case 5:
                fillCell(currentRow, currentCol, pack_diagonal5, count_diagonal5, pack_notdiagonal5, count_notdiagonal5);
                break;
            }

            currentCol++;
        }
    }

    //вывод таблицы с элементами мтарицы
    void fillCell(int row, int col, diagonal* diagPack, int diagCount, not_diagonal* notDiagPack, int notDiagCount) {
        bool foundValue = false;

        //заполнение диагональных значений
        if (diagPack != nullptr) {
            diagonal key = { .i = row + 1 };
            diagonal* it = std::lower_bound(diagPack, diagPack + diagCount, key, [](const diagonal& a, const diagonal& b) {
                return a.i < b.i;
            });
            //проверяем, подходит ли найденный элемент
            if (it != diagPack + diagCount && it->i == key.i) {
                if (row == col) {
                    QString valueStr = QString::number(it->value, 'g', 15);
                    matrixTable->setItem(row, col, new QTableWidgetItem(valueStr));
                    foundValue = true;
                }
            }
        }

        //заполнение недиагональных значений
        if (!foundValue && notDiagPack != nullptr) {
            not_diagonal key = { .i = row + 1, .j = col + 1 };
            not_diagonal* it = std::lower_bound(notDiagPack, notDiagPack + notDiagCount, key, [](const not_diagonal& a, const not_diagonal& b) {
                return std::tie(a.i, a.j) < std::tie(b.i, b.j);
            });
            if (it != notDiagPack + notDiagCount && it->i == key.i && it->j == key.j) {
                QString valueStr = QString::number(it->value, 'g', 15);
                matrixTable->setItem(row, col, new QTableWidgetItem(valueStr));
                foundValue = true;
            }
        }

        if (!foundValue) {
            matrixTable->setItem(row, col, new QTableWidgetItem("0"));
        }
    }

    //функция сохранения матрицы в файл
    void save_file(diagonal* pack_diagonal, not_diagonal* pack_notdiagonal, int count_notdiagonal, int count_diagonal, int n, const QString &filePath) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            QString data;
            //запоминаем размер
            data.append(QString::number(n) + "\n");

            //записываем диагональные элементы
            for (int i = 0; i < count_diagonal; i++) {
                if (cancelGeneration) {
                    file.close();
                    return; //прекращаем выполнение
                }
                data.append(QString("%1 %2 %3\n")
                    .arg(QString::number(pack_diagonal[i].value, 'g', 15))
                    .arg(pack_diagonal[i].i)
                    .arg(pack_diagonal[i].i));
            }

            //записываем не диагональные элементы
            for (int i = 0; i < count_notdiagonal; i++) {
                if (cancelGeneration) {
                    file.close();
                    return; //прекращаем выполнение
                }
                data.append(QString("%1 %2 %3\n")
                    .arg(QString::number(pack_notdiagonal[i].value, 'g', 15))
                    .arg(pack_notdiagonal[i].i)
                    .arg(pack_notdiagonal[i].j));
            }

            if (cancelGeneration) return;
            out << data;
            file.close();
        }
        else {
        }
    }

public:
    ViewPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr) :
        QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        matrixTable = new QTableWidget(this);  //ТАБЛИЦА

        QFont buttonFont;
        buttonFont.setPointSize(16);

        //СКРОЛЛ
        QScrollArea* scrollArea = new QScrollArea(this);
        scrollArea->setWidget(matrixTable);
        scrollArea->setWidgetResizable(true);
        setupScrollAreaStyles(scrollArea);
        layout->addWidget(scrollArea);

        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        //КНОПКИ
        QPushButton* fileButton = new QPushButton("Сохранить в файл", this);
        fileButton->setFont(buttonFont);
        fileButton->setFixedSize(200, 50);
        exitLayout->addWidget(fileButton);

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(buttonFont);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(buttonFont);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(fileButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        exitLayout->setContentsMargins(0, 10, 0, 0); // Отступы сверху 10 для визуального разделения
        exitLayout->setSpacing(10);

        layout->addLayout(exitLayout);
        setLayout(layout);

        connect(fileButton, &QPushButton::clicked, this, &ViewPage::onSaveFilePage);
        connect(backButton, &QPushButton::clicked, this, &ViewPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

    //передача выбранной матрицы
    void setSelectedMatrixIndex(int index) {
        selectedMatrixIndex = index;
        updateMatrixTable();  //обновить таблицу при изменении выбранной матрицы
    }

    //ТАБЛИЦА
    void updateMatrixTable() {
        if (selectedMatrixIndex < 1 || selectedMatrixIndex > 5) return;

        int n = size[selectedMatrixIndex - 1];
        matrixTable->setRowCount(n);
        matrixTable->setColumnCount(n);
        setupTableStyle(matrixTable);
        matrixTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        matrixTable->clearContents();
        matrixTable->setUpdatesEnabled(false);
        setupUpdateTimer();
        matrixTable->setUpdatesEnabled(true);
    }

signals:
    void backButtonClicked(); //сигнал для возврата

private slots:
    //НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(7);
    }

    //кнопка "сохранить в файл"
    void onSaveFilePage() {
        if (loadingDialog) {
            showMessageBox("Предупреждение", "Сохранение уже выполняется или отменяется. Пожалуйста, подождите.");
            return;
        }

        filePath = QFileDialog::getSaveFileName(this, "Выбрать файл для сохранения");
        if (filePath.isEmpty()) {
            return;
        }

        cancelGeneration = false;

        if (future.isRunning()) {
            showMessageBox("Предупреждение", "Обработка сохранения уже выполняется. Пожалуйста, дождитесь завершения.");
            return;
        }

        loadingDialog = new LoadingDialog(this);
        loadingDialog->setMessage("Сохранение матрицы в файл...");
        loadingDialog->show();

        connect(loadingDialog, &LoadingDialog::loadingCancelled, this, [&]() {
            cancelGeneration = true;                      //флаг для отмены генерации
            loadingDialog->accept();                      //закрытие диалога загрузки
            loadingDialog = nullptr;
            showMessageBox("Предупреждение", "Сохранение разреженной матрицы отменено.");
        });
        QCoreApplication::processEvents();

        future = QtConcurrent::run([=]() {
            if (cancelGeneration) return;
            switch (selectedMatrixIndex) {
            case 1: save_file(pack_diagonal1, pack_notdiagonal1, count_notdiagonal1, count_diagonal1, size[0], filePath); break;
            case 2: save_file(pack_diagonal2, pack_notdiagonal2, count_notdiagonal2, count_diagonal2, size[1], filePath); break;
            case 3: save_file(pack_diagonal3, pack_notdiagonal3, count_notdiagonal3, count_diagonal3, size[2], filePath); break;
            case 4: save_file(pack_diagonal4, pack_notdiagonal4, count_notdiagonal4, count_diagonal4, size[3], filePath); break;
            case 5: save_file(pack_diagonal5, pack_notdiagonal5, count_notdiagonal5, count_diagonal5, size[4], filePath); break;
            }

            if (cancelGeneration) return;
            QMetaObject::invokeMethod(this, [=]() {
                if (!cancelGeneration) {
                    loadingDialog->accept();                        //закрытие диалога загрузки
                    showMessageBox("Сохранение завершено", QString("Разреженная матрица сохранена."));
                    stackedWidget->setCurrentIndex(0);
                }
                loadingDialog = nullptr;
            }, Qt::QueuedConnection);
        });

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() {
            if (!future.isRunning() && !cancelGeneration) {
                timer->stop();           //остановить таймер
                loadingDialog->accept(); //закрыть диалог загрузки
                loadingDialog = nullptr;
            }
        });
        timer->start(100);
    }

};


//ВЫБОР МАТРИЦЫ ДЛЯ ВЫВОДА
class ViewPageInput : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLineEdit* dimensionInput;            //поле ввода размерности
    QString inputMethod;                  //выбранный метод ввода
    int choice;
    //чекбоксы выбора матриц
    QCheckBox* matrix1;
    QCheckBox* matrix2;
    QCheckBox* matrix3;
    QCheckBox* matrix4;
    QCheckBox* matrix5;

    //проверка заполненности матрицы
    bool isMatrixFilled(int index) const {
        return (index >= 0 && index < 5) ? filled[index] : false;
    }

    //обновление состояния выбора матрицы
    void updateCheckBoxStates() {
        matrix1->setEnabled(isMatrixFilled(0));
        matrix2->setEnabled(isMatrixFilled(1));
        matrix3->setEnabled(isMatrixFilled(2));
        matrix4->setEnabled(isMatrixFilled(3));
        matrix5->setEnabled(isMatrixFilled(4));

        matrix1->setChecked(false);
        matrix2->setChecked(false);
        matrix3->setChecked(false);
        matrix4->setChecked(false);
        matrix5->setChecked(false);
    }

public:
    ViewPageInput(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray),  choice(0),  selectedMatrixIndex(-1) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        QLabel* titleLabel = new QLabel("Выберите разреженную матрицу для вывода:", this);
        titleLabel->setAlignment(Qt::AlignCenter);;
        titleLabel->setFont(Fontstr);
        titleLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(titleLabel);

        //ЧЕКБОКСЫ ВЫБОРА МАТРИЦЫ
        QVBoxLayout* checkBoxLayout = new QVBoxLayout();
        matrix1 = new QCheckBox("Матрица №1", this);
        matrix2 = new QCheckBox("Матрица №2", this);
        matrix3 = new QCheckBox("Матрица №3", this);
        matrix4 = new QCheckBox("Матрица №4", this);
        matrix5 = new QCheckBox("Матрица №5", this);

        QCheckBox* matrices[] = { matrix1, matrix2, matrix3, matrix4, matrix5 };
        applyCheckBoxStyle(matrices, sizeof(matrices) / sizeof(matrices[0]));  //стиль

        //РАЗМЕР ТЕКСТА
        matrix1->setFont(Font);
        matrix2->setFont(Font);
        matrix3->setFont(Font);
        matrix4->setFont(Font);
        matrix5->setFont(Font);

        updateCheckBoxStates();

        connect(matrix1, &QCheckBox::clicked, this, &ViewPageInput::onMatrixCheckBoxClicked);
        connect(matrix2, &QCheckBox::clicked, this, &ViewPageInput::onMatrixCheckBoxClicked);
        connect(matrix3, &QCheckBox::clicked, this, &ViewPageInput::onMatrixCheckBoxClicked);
        connect(matrix4, &QCheckBox::clicked, this, &ViewPageInput::onMatrixCheckBoxClicked);
        connect(matrix5, &QCheckBox::clicked, this, &ViewPageInput::onMatrixCheckBoxClicked);

        checkBoxLayout->addWidget(matrix1);
        checkBoxLayout->addWidget(matrix2);
        checkBoxLayout->addWidget(matrix3);
        checkBoxLayout->addWidget(matrix4);
        checkBoxLayout->addWidget(matrix5);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        layout->addLayout(checkBoxLayout);

        //КНОПКИ
        QPushButton* continueButton = new QPushButton("Продолжить", this);
        continueButton->setFixedSize(290, 50);
        continueButton->setFont(Font);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);

        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(continueButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        layout->addLayout(exitLayout);
        setLayout(layout);
        layout->setSpacing(2);
        layout->setContentsMargins(5, 5, 5, 5);

        //ПЕРЕХОДЫ НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(backButton, &QPushButton::clicked, this, &ViewPageInput::onBackButtonClicked);
        connect(continueButton, &QPushButton::clicked, this, &ViewPageInput::onContinueButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

    void setChoice(int choice) {
        this->choice = choice; //установка выбранной операции
    }

protected:
    void showEvent(QShowEvent* event) override {
        QWidget::showEvent(event);
        updateCheckBoxStates();
    }

signals:
    void matrixSelected(int index);

public slots:
    //СОХРАНЕНИЕ ВЫБРАННОГО МЕТОДА ВВОДА МАТРИЦЫ
    void setInputMethod(const QString& method) {
        inputMethod = method;
    }

private slots:
    //НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(0);
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
        if (selectedMatrixIndex == -1) {
            showMessageBox("Ошибка", "Выберите разреженную матрицу для вывода.");
            return;
        }

        if (choice == 1) {
            ViewPage* viewPage = new ViewPage(stackedWidget, size, filled, this);
            viewPage->setSelectedMatrixIndex(selectedMatrixIndex);
            stackedWidget->addWidget(viewPage);
            stackedWidget->setCurrentWidget(viewPage);
        } else if (choice == 2) {
            LocationPage* locationPage = new LocationPage(stackedWidget, size, filled, selectedMatrixIndex, this);
            locationPage->setSelectedMatrixIndex(selectedMatrixIndex);
            stackedWidget->addWidget(locationPage);
            stackedWidget->setCurrentWidget(locationPage);
        } else {}
    }
};


//ЗАПОЛНЕНИЕ С КЛАВИАТУРЫ: траница ввода с клавиатуры в таблицу с элементами
class InputKeyboard : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QTableWidget* table;                  //таблица значений матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLabel* errorLabel;                   //отслеживание корректности ввода хначений в таблицу
    QStringList rowLabels;                //для нумерации строк таблицы
    int currentRow = 0;                   //счётчик строк
    LoadingDialog *loadingDialog = nullptr; //страница загрузки
    QFuture<void> future;                 //хранение фоновой задачи
    bool cancelGeneration = false;        //флаг для отмены генерации
    QLineEdit* dimensionInput;            //поле ввода размерности
    int n;                                //текущая размерность матрицы
    int previousSize;                     //предыдущая размерность матрицы
    bool isSizeSaved;                     //отслеживание корректности ввода размерности
    bool isTableCreated = false;          //отслеживания изменения размерности

    //функция ввода из таблицы в матрицу значений
    void EnterMatrix(diagonal*& pack_diagonal, not_diagonal*& pack_notdiagonal, int& count_diagonal, int& count_notdiagonal, int& n) {
        delete[] pack_diagonal;
        delete[] pack_notdiagonal;
        pack_diagonal = nullptr;
        pack_notdiagonal = nullptr;

        filled[selectedMatrixIndex - 1] = true;
        int MAX_NOT_DIAGONAL = n * n * 0.15;

        pack_diagonal = new diagonal[n];
        pack_notdiagonal = new not_diagonal[MAX_NOT_DIAGONAL];
        count_diagonal = 0;
        count_notdiagonal = 0;

        for (int row = 0; row < table->rowCount() ; ++row) {
            QTableWidgetItem* itemRow = table->item(row, 0);
            QTableWidgetItem* itemCol = table->item(row, 1);
            QTableWidgetItem* itemValue = table->item(row, 2);

            if (cancelGeneration) {
                filled[selectedMatrixIndex - 1] = false;
                return; //прекращаем выполнение
            }

            if (itemRow && itemCol && itemValue) {
                bool okRow, okCol, okValue;
                int realRowNumber = itemRow->text().toInt(&okRow);
                int realColNumber = itemCol->text().toInt(&okCol);
                double value = itemValue->text().toDouble(&okValue);

                if (realRowNumber == realColNumber) {
                    if (count_diagonal < n) {
                        pack_diagonal[count_diagonal] = { value, realRowNumber };
                        count_diagonal++;
                    }
                }
                else {
                    if (count_notdiagonal < MAX_NOT_DIAGONAL) {
                        pack_notdiagonal[count_notdiagonal] = { value, realRowNumber, realColNumber };
                        count_notdiagonal++;
                    }
                }
            }
        }
    }

public:
    InputKeyboard(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, int selectedMatrixIndex, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(selectedMatrixIndex) {

        QFont Font;
        Font.setPointSize(18);
        QFont Fontstr;
        Fontstr.setPointSize(15);

        QVBoxLayout* layout = new QVBoxLayout(this);
        QHBoxLayout* inputLayout = new QHBoxLayout();

        layout->addSpacing(70);
        QLabel* dimensionLabel1 = new QLabel("Введите размерность разреженной матрицы:", this);
        dimensionLabel1->setFont(Font);
        dimensionLabel1->setStyleSheet("QLabel { color: #001a33; }");
        inputLayout->addWidget(dimensionLabel1);

        //ВВОД РАЗМЕРНОСТИ
        dimensionInput = new QLineEdit(this);
        dimensionInput->setFixedSize(100, 30);
        dimensionInput->setMaxLength(5);
        applyLineEditStyle(dimensionInput);
        inputLayout->addWidget(dimensionInput);

        QLabel* dynamicTextLabel = new QLabel("х ", this);
        dynamicTextLabel->setFont(Fontstr);
        dynamicTextLabel->setStyleSheet("QLabel { color: #001a33; }");
        inputLayout->addWidget(dynamicTextLabel);

        QPushButton* saveButton = new QPushButton("Сохранить", this);
        saveButton->setFixedSize(200, 50);
        saveButton->setFont(Font);
        inputLayout->addWidget(saveButton);

        layout->addLayout(inputLayout);

        //ГЕНЕРАЦИЯ ТЕКСТА ИЗ ПОЛЯ ВВОДА
        connect(dimensionInput, &QLineEdit::textChanged, this, [=](const QString& text) {
            dynamicTextLabel->setText("х " + text);
        });
        layout->addSpacing(20);
        layout->addStretch();

        QLabel* dimensionLabel2 = new QLabel("Заполнение разреженной матрицы:", this);
        dimensionLabel2->setFont(Fontstr);
        dimensionLabel2->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(dimensionLabel2);

        layout->addSpacing(15);

        //ТАБЛИЦА
        table = new QTableWidget(this);
        table->setColumnCount(3);
        QStringList headerLabels;
        headerLabels << QString("Номер строки") << QString("Номер столбца") << QString("Значение");
        table->setHorizontalHeaderLabels(headerLabels);
        setupTableStyle(table);
        table->setColumnWidth(0, 100);
        table->setColumnWidth(1, 100);
        table->setColumnWidth(2, 100);
        table->setVisible(false);
        layout->addWidget(table);

        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &InputKeyboard::updateRowLabels);
        timer->start(10);

        //СКРОЛЛ
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(table);
        scrollArea->setFixedHeight(250);
        scrollArea->setFixedWidth(380);
        setupScrollAreaStyles(scrollArea);
        layout->addWidget(scrollArea);

        errorLabel = new QLabel("", this);
        errorLabel->setStyleSheet("color: red;");
        errorLabel->setWordWrap(true);
        errorLabel->setFixedHeight(40);
        layout->addWidget(errorLabel);

        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* createButton = new QPushButton("Создать", this);
        createButton->setFont(Fontstr);
        createButton->setFixedSize(200, 50);
        exitLayout->addWidget(createButton);

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Fontstr);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Fontstr);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(createButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(saveButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        exitLayout->setContentsMargins(0, 20, 0, 0);
        exitLayout->setSpacing(10);
        layout->addLayout(exitLayout);

        //просмотр таблицы при вводе
        connect(table, &QTableWidget::itemChanged, this, &InputKeyboard::onItemChanged);

        setLayout(layout);

        //ПЕРЕХОДЫ ПО КНОПКАМ
        connect(saveButton, &QPushButton::clicked, this, &InputKeyboard::handleContinue);
        connect(backButton, &QPushButton::clicked, this, &InputKeyboard::onBackButtonClicked);
        connect(createButton, &QPushButton::clicked, this, &InputKeyboard::saveData);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

    //СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
    void setSelectedMatrixIndex(int index) {
        selectedMatrixIndex = index;

        //очищение поля ввода
        if (selectedMatrixIndex > 0 && selectedMatrixIndex <= 5) {
            dimensionInput->clear();
        }
    }

signals:
    void showNextPage();

private slots:
    //проверка третей колонки (значение) и округление её до двух знаков после запятой
    void onItemChanged(QTableWidgetItem* item) {
        if (item && item->column() == 2) { //проверяем только третью колонку
            bool ok;
            double value = item->text().toDouble(&ok);
            if (ok) {
                if (value == static_cast<int>(value)) { //если целое
                    item->setText(QString::number(static_cast<int>(value)));
                } else { //если дробное
                    double roundedValue = std::round(value * 100.0) / 100.0;
                    item->setText(QString::number(roundedValue, 'f', 2)); //устанавливаем округлённое значение в ячейку
                }
            } else {
            }
        }
    }

    //ПРОВЕРКА
    bool validateInput(const QString& input, int& output, const QString& errorMessage) {
        bool ok;
        if (input.isEmpty() || !input.toInt(&ok) || !ok) {
            showMessageBox("Неверный ввод", errorMessage);
            return false;
        }

        output = input.toInt(&ok);
        if (!ok || output <= 0 || output > 10000) {
            showMessageBox("Неверный ввод", errorMessage);
            size[selectedMatrixIndex - 1] = 0;
            return false;
        }

        if (output < 5) {
            showMessageBox("Неверный ввод", "Слишком маленькая размерность разреженной матрицы.");
            size[selectedMatrixIndex - 1] = 0;
            return false;
        }

        return true;
    }

    //ПЕРЕХОД К ВВОДУ ЗНАЧЕНИЙ С КЛАВИАТУРЫ
    void handleContinue() {
        QString input = dimensionInput->text();

        if (validateInput(input, n, "Пожалуйста, введите числовое значение от 1 до 10 000.")) {
            isSizeSaved = true;

            if (selectedMatrixIndex > 0 && selectedMatrixIndex <= 5) {
                previousSize = size[selectedMatrixIndex - 1];   //прошлая размерность матрицы
                size[selectedMatrixIndex - 1] = n;              //настоящая размерность матрицы

                //кол-во строк таблицы
                int newRowCount = n * n * 0.1;

                //если размерность изменилась
                if (!isTableCreated) {
                    table->setRowCount(newRowCount);
                    table->setVisible(true);
                    updateRowLabels();
                    isTableCreated = true;
                //если не изменилась
                }else {
                    int currentRowCount = table->rowCount();

                    if (newRowCount < currentRowCount) {
                        for (int row = currentRowCount - 1; row >= newRowCount; --row) {
                            delete table->item(row, 0);
                            delete table->item(row, 1);
                            delete table->item(row, 2);
                        }
                    } else {}
                    table->setRowCount(newRowCount);
                }
                currentRow = 0;
                rowLabels.clear();
                updateRowLabels();
            }
        }
    }

    //нумерация строк
    void updateRowLabels() {
        //количество строк
        rowLabels.clear();
        int rows = size[selectedMatrixIndex - 1] * size[selectedMatrixIndex - 1] * 0.1;
        if (currentRow < rows) {
            rowLabels << QString("%1").arg(currentRow + 1);
            currentRow++;

            //обновление заголовка строк таблицы
            table->setVerticalHeaderLabels(rowLabels);
        } else {
            //остановка таймера, если все строки заполнены
            static_cast<QTimer*>(sender())->stop();
        }
    }

    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        if (showBackWarning("Предупреждение", "Вы точно хотите вернуться назад, не завершив ввод?")) {
            //очистка таблицы
            for (int row = 0; row < table->rowCount(); ++row) {
                for (int col = 0; col < table->columnCount(); ++col) {
                    QTableWidgetItem* item = table->item(row, col);
                    if (item) {
                        delete item;  //удаляем элемент из ячейки
                    }
                }
            }
            table->clearContents();
            size[selectedMatrixIndex - 1] = previousSize;
            stackedWidget->setCurrentIndex(2);
        } else {
            return;
        }
    }

    //очистка матрицы
    void clearMatrix(int& count_diagonal, int& count_notdiagonal, int& size){
        filled[selectedMatrixIndex - 1] = false;
        size = 0;
        count_diagonal = 0;
        count_notdiagonal = 0;
    }

    //ЗАПОЛНЕНИЕ МАТРИЦЫ ЗНАЧЕНИЯМИ ИЗ ТАБЛИЦЫ
    void saveData() {
        errorLabel->clear();
        bool hasNaturalError = false;
        bool hasRationalError = false;
        bool hasIndexError = false;
        bool isTableEmpty = true;

        for (int row = 0; row < table->rowCount(); ++row) {
            QTableWidgetItem* itemRow = table->item(row, 0);
            QTableWidgetItem* itemCol = table->item(row, 1);
            QTableWidgetItem* itemValue = table->item(row, 2);

            //проверка на наличие хотя бы одного заполненного элемента
            if (itemRow && !itemRow->text().isEmpty() &&
                itemCol && !itemCol->text().isEmpty() &&
                itemValue && !itemValue->text().isEmpty()) {
                isTableEmpty = false;
                break;
            }
        }

        //если таблица пустая
        if (isTableEmpty) {
            showMessageBox("Предупреждение", "Заполните таблицу перед сохранением.");
            return;
        }

        if (size[selectedMatrixIndex - 1] == 0) {
            showMessageBox("Внимание", "Вы не ввели корректную размерность разрежённой матрицы.");
            dimensionInput->clear();
            return;
        }


        for (int row = 0; row < table->rowCount(); ++row) {
            QTableWidgetItem* itemRow = table->item(row, 0);
            QTableWidgetItem* itemCol = table->item(row, 1);
            QTableWidgetItem* itemValue = table->item(row, 2);

            if (itemRow && itemCol && itemValue) {
                bool okRow, okCol, okValue;
                int realRowNumber = itemRow->text().toInt(&okRow);
                int realColNumber = itemCol->text().toInt(&okCol);
                double value = itemValue->text().toDouble(&okValue);

                if (!okRow || !okCol || realRowNumber <= 0 || realColNumber <= 0) {
                    hasNaturalError = true;
                }
                if (!okValue || value == 0) {
                    hasRationalError = true;
                }
                if (realRowNumber > size[selectedMatrixIndex-1] || realColNumber > size[selectedMatrixIndex-1]){
                    hasIndexError = true;
                }
            }
        }

        QString errorMessage;
        if (hasNaturalError) {
            errorMessage.append("&nbsp;&nbsp;&nbsp;&nbsp;!!! Номер строки/столбца должен быть натуральным.<br>");
        }
        if (hasRationalError) {
            errorMessage.append("&nbsp;&nbsp;&nbsp;&nbsp;!!! Значение должно быть рациональным числом, кроме нуля, записанное через точку ('.').<br>");
        }
        if (hasIndexError){
            errorMessage.append("&nbsp;&nbsp;&nbsp;&nbsp;!!! Индекс не должен быть больше размерности разреженной матрицы.<br>");
        }
        errorLabel->setText(errorMessage);

        if (errorMessage.isEmpty()) {
            if (showBackWarning("Предупреждение", "Вы точно хотите завершить ввод?")) {
                //если таблица пустая
                if (isTableEmpty) {
                    showMessageBox("Предупреждение", "Заполните таблицу перед сохранением.");
                    return;
                }

                loadingDialog = new LoadingDialog(this);
                loadingDialog->setMessage("Загрузка разреженной матрицы...");
                loadingDialog->show();

                cancelGeneration = false;

                connect(loadingDialog, &LoadingDialog::loadingCancelled, this, [&]() {
                    cancelGeneration = true;
                    loadingDialog->accept();
                    showMessageBox("Предупреждение", "Загрузка разреженной матрицы отменена. Матрица удалена.");
                    switch (selectedMatrixIndex) {
                    case 1: clearMatrix(count_diagonal1, count_notdiagonal1, size[0]); break;
                    case 2: clearMatrix(count_diagonal2, count_notdiagonal2, size[1]); break;
                    case 3: clearMatrix(count_diagonal3, count_notdiagonal3, size[2]); break;
                    case 4: clearMatrix(count_diagonal4, count_notdiagonal4, size[3]); break;
                    case 5: clearMatrix(count_diagonal5, count_notdiagonal5, size[4]); break;
                    default: break;
                    }
                    stackedWidget->setCurrentIndex(5);
                });

                QCoreApplication::processEvents();

                future = QtConcurrent::run([=]() {
                    if (cancelGeneration) return;
                    switch (selectedMatrixIndex) {
                    case 1: EnterMatrix(pack_diagonal1, pack_notdiagonal1, count_diagonal1, count_notdiagonal1, size[0]); break;
                    case 2: EnterMatrix(pack_diagonal2, pack_notdiagonal2, count_diagonal2, count_notdiagonal2, size[1]); break;
                    case 3: EnterMatrix(pack_diagonal3, pack_notdiagonal3, count_diagonal3, count_notdiagonal3, size[2]); break;
                    case 4: EnterMatrix(pack_diagonal4, pack_notdiagonal4, count_diagonal4, count_notdiagonal4, size[3]); break;
                    case 5: EnterMatrix(pack_diagonal5, pack_notdiagonal5, count_diagonal5, count_notdiagonal5, size[4]); break;
                    }
                    if (cancelGeneration) return;
                    performSorting(selectedMatrixIndex);

                    QMetaObject::invokeMethod(this, [=]() {
                        if (!cancelGeneration) {
                            loadingDialog->accept();
                            showMessageBox("Генерация завершена", QString("Разреженная матрица №%1 успешно загружена.").arg(selectedMatrixIndex));
                            for (int row = 0; row < table->rowCount(); ++row) {
                                for (int col = 0; col < table->columnCount(); ++col) {
                                    QTableWidgetItem* item = table->item(row, col);
                                    if (item) {
                                        delete item;
                                    }
                                }
                            }
                            stackedWidget->setCurrentIndex(0);
                        }
                    }, Qt::QueuedConnection);
                });

                QTimer* timer = new QTimer(this);
                connect(timer, &QTimer::timeout, this, [=]() {
                    if (!future.isRunning() && !cancelGeneration) {
                        timer->stop();
                        loadingDialog->accept();
                    }
                });
                timer->start(100);
            } else {
                return;
            }
        }
    }
};


//ЗАПОЛНЕНИЕ ЧЕРЕЗ ФАЙЛ
class InputFilePage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QString fileName;                     //файл для ввода
    LoadingDialog *loadingDialog = nullptr;  //страница загрузки
    QFuture<void> future;                 //хранение фоновой задачи
    bool cancelGeneration = false;        //флаг для отмены генерации

    //функция ввода из файла в матрицу значений
    void EnterMatrix(QFile& inputFile, diagonal*& pack_diagonal, not_diagonal*& pack_notdiagonal, int& count_diagonal, int& count_notdiagonal, int& size1) {
        delete[] pack_diagonal;
        delete[] pack_notdiagonal;
        pack_diagonal = nullptr;
        pack_notdiagonal = nullptr;

        QTextStream in(&inputFile);
        int n = 0;
        QString line;
        if (!in.atEnd()) {
            line = in.readLine();
            n = line.toInt();
            size1 = n;
        }

        filled[selectedMatrixIndex - 1] = true;
        int maxsize_notdiagonal = n * n * 0.15;

        pack_diagonal = new diagonal[n];
        pack_notdiagonal = new not_diagonal[maxsize_notdiagonal];
        count_diagonal = 0;
        count_notdiagonal = 0;

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList values = line.split(" ");

            bool row, col;
            double value = values[0].toDouble();
            int rowIndex = values[1].toInt(&row);
            int colIndex = values[2].toInt(&col);

            if (cancelGeneration) {
                filled[selectedMatrixIndex - 1] = false;
                inputFile.close();
                return; //прекращаем выполнение
            }

            // Диагональный элемент
            if (rowIndex == colIndex) {
                if (count_diagonal < n) {
                    pack_diagonal[count_diagonal].value = value;
                    pack_diagonal[count_diagonal].i = rowIndex;
                    count_diagonal++;
                }
            }
            // Недиагональный элемент
            else {
                if (count_notdiagonal < maxsize_notdiagonal) {
                    pack_notdiagonal[count_notdiagonal].value = value;
                    pack_notdiagonal[count_notdiagonal].i = rowIndex;
                    pack_notdiagonal[count_notdiagonal].j = colIndex;
                    count_notdiagonal++;
                }
            }
        }
        inputFile.close();
    }

public:
    InputFilePage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        :QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {

        QVBoxLayout* layout = new QVBoxLayout(this);

        QHBoxLayout* inputLayout = new QHBoxLayout();
        QLabel* dimensionLabel = new QLabel("Выбор файла:", this);
        QFont dimensionFont;
        dimensionFont.setPointSize(18);
        dimensionLabel->setStyleSheet("QLabel { color: #001a33; }");
        dimensionLabel->setFont(dimensionFont);

        QFont buttonFont;
        buttonFont.setPointSize(16);

        QPushButton* choiceButton = new QPushButton("Выбрать", this);
        choiceButton->setFont(buttonFont);
        choiceButton->setFixedSize(200, 50);
        inputLayout->addWidget(dimensionLabel);
        inputLayout->addWidget(choiceButton);
        layout->addLayout(inputLayout);

        // Кнопка "Сохранить"
        QPushButton* saveButton = new QPushButton("Сохранить", this);
        saveButton->setFont(buttonFont);
        saveButton->setFixedSize(200, 50);
        layout->addWidget(saveButton, 0, Qt::AlignCenter);

        // Подключаем сигнал нажатия кнопки к слоту
        connect(choiceButton, &QPushButton::clicked, this, [=]() {
            fileName = QFileDialog::getOpenFileName(this, "Выбрать файл");
        });
        connect(saveButton, &QPushButton::clicked, this, &InputFilePage::loadFileData);

        layout->addStretch();

        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(buttonFont);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(buttonFont);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(saveButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(choiceButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        exitLayout->setContentsMargins(0, 10, 0, 0);
        exitLayout->setSpacing(10);
        layout->addLayout(exitLayout);

        setLayout(layout);

        connect(backButton, &QPushButton::clicked, this, &InputFilePage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });

    }

    //СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
    void setSelectedMatrixIndex(int index) {
        selectedMatrixIndex = index;
    }

private slots:
    void clearMatrix(int& count_diagonal, int& count_notdiagonal, int& size){
        filled[selectedMatrixIndex - 1] = false;
        size = 0;
        count_diagonal = 0;
        count_notdiagonal = 0;
    }

    //кнопка "сохранить" нажата
    void loadFileData() {
        if (fileName.isEmpty()) {
            showMessageBox("Предупреждение", "Выберите файл для продолжения ввода.");
            return;
        }

        if (showBackWarning("Предупреждение", "Вы точно хотите завершить ввод?")) {
            loadingDialog = new LoadingDialog(this);
            loadingDialog->setMessage("Загрузка разреженной матрицы...");
            loadingDialog->show();

            cancelGeneration = false;

            connect(loadingDialog, &LoadingDialog::loadingCancelled, this, [&]() {
                cancelGeneration = true;
                loadingDialog->accept();
                showMessageBox("Предупреждение", "Загрузка разреженной матрицы отменена. Матрица удалена.");
                switch (selectedMatrixIndex) {
                case 1: clearMatrix(count_diagonal1, count_notdiagonal1, size[0]); break;
                case 2: clearMatrix(count_diagonal2, count_notdiagonal2, size[1]); break;
                case 3: clearMatrix(count_diagonal3, count_notdiagonal3, size[2]); break;
                case 4: clearMatrix(count_diagonal4, count_notdiagonal4, size[3]); break;
                case 5: clearMatrix(count_diagonal5, count_notdiagonal5, size[4]); break;
                default: break;
                }
                stackedWidget->setCurrentIndex(3);
            });

            QCoreApplication::processEvents();

            future = QtConcurrent::run([=]() {
                QFile inputFile(fileName);
                if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    return;
                }

                if (cancelGeneration) return;

                switch (selectedMatrixIndex) {
                case 1: EnterMatrix(inputFile, pack_diagonal1, pack_notdiagonal1, count_diagonal1, count_notdiagonal1, size[0]); break;
                case 2: EnterMatrix(inputFile, pack_diagonal2, pack_notdiagonal2, count_diagonal2, count_notdiagonal2, size[1]); break;
                case 3: EnterMatrix(inputFile, pack_diagonal3, pack_notdiagonal3, count_diagonal3, count_notdiagonal3, size[2]); break;
                case 4: EnterMatrix(inputFile, pack_diagonal4, pack_notdiagonal4, count_diagonal4, count_notdiagonal4, size[3]); break;
                case 5: EnterMatrix(inputFile, pack_diagonal5, pack_notdiagonal5, count_diagonal5, count_notdiagonal5, size[4]); break;
                }

                if (cancelGeneration) return;

                performSorting(selectedMatrixIndex);  //сортировка

                QMetaObject::invokeMethod(this, [=]() {
                    if (!cancelGeneration) {
                        loadingDialog->accept();
                        showMessageBox("Генерация завершена", QString("Разреженная матрица №%1 успешно загружена.").arg(selectedMatrixIndex));
                        stackedWidget->setCurrentIndex(0);
                    }
                }, Qt::QueuedConnection);

            });

            QTimer* timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, [=]() {
                if (!future.isRunning() && !cancelGeneration) {
                    timer->stop();
                    loadingDialog->accept();
                }
            });
            timer->start(100);
        } else {
            return;
        }
    }

    //кнопка "назад"
    void onBackButtonClicked() {
        if (showBackWarning("Предупреждение", "Вы точно хотите вернуться назад, не завершив ввод?")) {
            stackedWidget->setCurrentIndex(2);
        } else {
            return;
        }
    }
};


//ЗАПОЛНЕНИЕ СЛУЧАЙНЫМ ОБРАЗОМ
class RandomInputPage : public QWidget {
    Q_OBJECT
private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QLineEdit* dimensionInput;            //поле ввода размерности
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QString strn;                         //текст в окне ввода размерности
    bool isSizeSaved = false;                     //отслеживание корректности ввода размерности
    int n;                                //размерность матрицы
    int previousSize;     //предыдущая размерность матрицы
    LoadingDialog *loadingDialog = nullptr;  //страница загрузки
    QFuture<void> future;                 //хранение фоновой задачи
    bool cancelGeneration = false;        //флаг для отмены генерации

    //генерация значения (value)
    double generate() {
        double value;
        do {
            // Генерируем случайное число
            if (rand() % 2 == 0) { // 50% шанс на целое число
                value = static_cast<double>(rand() % 2001 - 1000); // От -1000 до 1000
            } else {
                // Генерируем дробное число
                value = (rand() % 2001 - 1000) + (rand() % 100) / 100.0;
            }
        } while (value == 0 || value == 0.00); // Пропускаем 0.00

        return value; // Возвращаем округленное значение
    }

    //функция генерации элементов матрицы
    void EnterMatrix(int countValues, diagonal*& pack_diagonal, not_diagonal*& pack_notdiagonal, int& count_diagonal, int& count_notdiagonal, int& n) {

        delete[] pack_diagonal;
        delete[] pack_notdiagonal;
        pack_diagonal = nullptr;
        pack_notdiagonal = nullptr;

        int maxsize_notdiagonal = n * n * 0.15;
        pack_diagonal = new diagonal[n];
        pack_notdiagonal = new not_diagonal[maxsize_notdiagonal];
        count_diagonal = 0;
        count_notdiagonal = 0;
        filled[selectedMatrixIndex - 1] = true;

        bool* occupied = new bool[n * n];
        for (int i = 0; i < (n * n); i++) occupied[i] = false;

        for (int k = 0; k < countValues; ) {
            int i = rand() % n + 1;
            int j = rand() % n + 1;
            double value = generate();

            if (cancelGeneration) {
                filled[selectedMatrixIndex - 1] = false;
                delete[] occupied;
                return; //прекращаем выполнение
            }

            if (i == j) {
                if (!occupied[(i - 1) * n + (j - 1)]) {
                    pack_diagonal[count_diagonal].value = value;
                    pack_diagonal[count_diagonal].i = i;
                    occupied[(i - 1) * n + (j - 1)] = true;
                    count_diagonal++;
                    k++;
                }
            }
            else {
                if (!occupied[(i - 1) * n + (j - 1)]) {
                    pack_notdiagonal[count_notdiagonal].value = value;
                    pack_notdiagonal[count_notdiagonal].i = i;
                    pack_notdiagonal[count_notdiagonal].j = j;
                    occupied[(i - 1) * n + (j - 1)] = true;
                    count_notdiagonal++;
                    k++;
                }
            }
        }
        delete[] occupied;
    }

public:
    RandomInputPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {

        srand(static_cast<unsigned int>(time(0)));

        QVBoxLayout* layout = new QVBoxLayout(this);
        QHBoxLayout* inputLayout = new QHBoxLayout();
        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        layout->addSpacing(70);
        QLabel* dimensionLabel = new QLabel("Введите размерность разреженной матрицы:", this);
        dimensionLabel->setFont(Fontstr);
        dimensionLabel->setStyleSheet("QLabel { color: #001a33; }");
        inputLayout->addWidget(dimensionLabel);

        //ВВОД РАЗМЕРНОСТИ
        dimensionInput = new QLineEdit(this);
        dimensionInput->setFixedSize(100, 30);
        dimensionInput->setMaxLength(5);
        applyLineEditStyle(dimensionInput);
        inputLayout->addWidget(dimensionInput);

        QLabel* dynamicTextLabel = new QLabel("х ", this);
        dynamicTextLabel->setFont(Font);
        dynamicTextLabel->setStyleSheet("QLabel { color: #001a33; }");
        inputLayout->addWidget(dynamicTextLabel);

        QPushButton* continueButton = new QPushButton("Сохранить", this);
        continueButton->setFixedSize(200, 50);
        continueButton->setFont(Font);
        inputLayout->addWidget(continueButton);

        inputLayout->setContentsMargins(0, 0, 0, 0);
        inputLayout->setSpacing(5);
        layout->addLayout(inputLayout);

        //ГЕНЕРАЦИЯ ТЕКСТА ИЗ ПОЛЯ ВВОДА
        connect(dimensionInput, &QLineEdit::textChanged, this, [=](const QString& text) {
            dynamicTextLabel->setText("х " + text);
        });

        //ЧЕКБОКСЫ
        layout->addSpacing(80);
        QCheckBox* nonZeroValuesCheckbox = new QCheckBox("Ввод количества ненулевых значений", this);
        nonZeroValuesCheckbox->setFont(Font);
        layout->addWidget(nonZeroValuesCheckbox, 0, Qt::AlignLeft);

        QLineEdit* nonZeroValuesInput = new QLineEdit(this);
        nonZeroValuesInput->setFixedSize(150, 30);
        nonZeroValuesInput->setEnabled(false);
        applyLineEditStyle(nonZeroValuesInput);
        layout->addWidget(nonZeroValuesInput, 0, Qt::AlignLeft);

        connect(nonZeroValuesCheckbox, &QCheckBox::toggled, this, [=](bool checked) {
            nonZeroValuesInput->setEnabled(checked);
            if (!checked) {
                nonZeroValuesInput->clear();
            }
        });

        layout->addSpacing(20);
        QLabel* orLabel = new QLabel("ИЛИ", this);
        orLabel->setFont(Fontstr);
        layout->addWidget(orLabel, 0, Qt::AlignLeft);

        layout->addSpacing(25);
        QCheckBox* tenPercentCheckbox = new QCheckBox("Сгенерировать 10% ненулевых значений от размерности разреженной матрицы", this);
        tenPercentCheckbox->setFont(Font);
        layout->addWidget(tenPercentCheckbox, 0, Qt::AlignLeft);

        QCheckBox* matrices[] = { tenPercentCheckbox, nonZeroValuesCheckbox };
        applyCheckBoxStyle(matrices, sizeof(matrices) / sizeof(matrices[0]));  //стиль

        QButtonGroup* checkboxGroup = new QButtonGroup(this);
        checkboxGroup->setExclusive(true);
        checkboxGroup->addButton(nonZeroValuesCheckbox);
        checkboxGroup->addButton(tenPercentCheckbox);

        layout->addStretch();

        //КНОПКИ
        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* generateButton = new QPushButton("Сгенерировать", this);
        generateButton->setFont(Font);
        generateButton->setFixedSize(200, 50);
        exitLayout->addWidget(generateButton);

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(continueButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(generateButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        exitLayout->setContentsMargins(0, 10, 0, 0);
        exitLayout->setSpacing(10);
        layout->addLayout(exitLayout);

        setLayout(layout);

        //НАЖАТИЕ КНОПКИ "СОХРАНИТЬ"
        connect(continueButton, &QPushButton::clicked, this, &RandomInputPage::onContinueButtonClicked);

        //НАЖАТИЕ КНОПКИ "СГЕНЕРИРОВАТЬ"
        connect(generateButton, &QPushButton::clicked, this, [=]() {
            if (future.isRunning()) {
                future.cancel();
            }

            cancelGeneration = false;
            //проверка, выбран ли хотя бы один чекбокс
            if (!nonZeroValuesCheckbox->isChecked() && !tenPercentCheckbox->isChecked()) {
                showMessageBox("Внимание", "Пожалуйста, выберите метод генерации ненулевых значений разреженной матрицы.");
                dimensionInput->clear();
                return;
            }
            //проверка сохранлась ли размерность
            if (!isSizeSaved) {
                showMessageBox("Внимание", "Вы не нажали кнопку \"Сохранить\", чтобы сохранить размерность разреженной матрицы.");
                dimensionInput->clear();
                return;
            }

            if (size[selectedMatrixIndex - 1] == 0) {
                showMessageBox("Внимание", "Вы не ввели корректную размерность разреженной матрицы.");
                dimensionInput->clear();
                return;
            }

            loadingDialog = new LoadingDialog(this);          //диалог загрузки
            loadingDialog->setMessage("Генерация разреженной матрицы...");
            loadingDialog->show();                            //показ диалога загрузки

            connect(loadingDialog, &LoadingDialog::loadingCancelled, this, [&]() {
                cancelGeneration = true;                      //флаг для отмены генерации
                loadingDialog->accept();                      //закрытие диалога загрузки
                showMessageBox("Предупреждение", "Генерация разреженной матрицы отменена. Матрица удалена.");
                dimensionInput->clear();
                isSizeSaved = false;
                switch (selectedMatrixIndex) {
                case 1: clearMatrix(count_diagonal1, count_notdiagonal1, size[0]); break;
                case 2: clearMatrix(count_diagonal2, count_notdiagonal2, size[1]); break;
                case 3: clearMatrix(count_diagonal3, count_notdiagonal3, size[2]); break;
                case 4: clearMatrix(count_diagonal4, count_notdiagonal4, size[3]); break;
                case 5: clearMatrix(count_diagonal5, count_notdiagonal5, size[4]); break;
                default: break;
                }
                stackedWidget->setCurrentIndex(4);
            });

            QCoreApplication::processEvents();

            future = QtConcurrent::run([=]() {
                int countValues = 0;
                if (cancelGeneration) return;
                if (nonZeroValuesCheckbox->isChecked()) {
                    QString nonZeroInput = nonZeroValuesInput->text();
                    bool ok;
                    countValues = nonZeroInput.toInt(&ok);

                    //проверка на корректность ввода ненулевых значений
                    if (nonZeroInput.isEmpty() || !nonZeroInput.toInt(&ok) || !ok || countValues <= 0 || countValues > 10000000) {
                        loadingDialog->accept();
                        showMessageWithDelay("Неверный ввод", "Количество ненулевых значений разреженной матрицы должно быть натуральным числом.");
                        return;
                    }

                    if (countValues < 0 || countValues > static_cast<int>(n * n * 0.1)) {
                        loadingDialog->accept();
                        showMessageWithDelay("Неверный ввод", "Количество ненулевых значений разреженной матрицы должно составлять не более 10%.");
                        return;
                    }
                }

                if (cancelGeneration) return;
                if (nonZeroValuesCheckbox->isChecked() && !nonZeroValuesInput->text().isEmpty()) {
                    switch (selectedMatrixIndex) {
                    case 1: EnterMatrix(countValues, pack_diagonal1, pack_notdiagonal1, count_diagonal1, count_notdiagonal1, size[0]); break;
                    case 2: EnterMatrix(countValues, pack_diagonal2, pack_notdiagonal2, count_diagonal2, count_notdiagonal2, size[1]); break;
                    case 3: EnterMatrix(countValues, pack_diagonal3, pack_notdiagonal3, count_diagonal3, count_notdiagonal3, size[2]); break;
                    case 4: EnterMatrix(countValues, pack_diagonal4, pack_notdiagonal4, count_diagonal4, count_notdiagonal4, size[3]); break;
                    case 5: EnterMatrix(countValues, pack_diagonal5, pack_notdiagonal5, count_diagonal5, count_notdiagonal5, size[4]); break;
                    }

                }
                else if (tenPercentCheckbox->isChecked()) {
                    int countValues = static_cast<int>(floor(n * n * 0.1));
                    switch (selectedMatrixIndex) {
                    case 1: EnterMatrix(countValues, pack_diagonal1, pack_notdiagonal1, count_diagonal1, count_notdiagonal1, size[0]); break;
                    case 2: EnterMatrix(countValues, pack_diagonal2, pack_notdiagonal2, count_diagonal2, count_notdiagonal2, size[1]); break;
                    case 3: EnterMatrix(countValues, pack_diagonal3, pack_notdiagonal3, count_diagonal3, count_notdiagonal3, size[2]); break;
                    case 4: EnterMatrix(countValues, pack_diagonal4, pack_notdiagonal4, count_diagonal4, count_notdiagonal4, size[3]); break;
                    case 5: EnterMatrix(countValues, pack_diagonal5, pack_notdiagonal5, count_diagonal5, count_notdiagonal5, size[4]); break;
                    }
                }
                if (cancelGeneration) return;
                performSorting(selectedMatrixIndex);

                QMetaObject::invokeMethod(this, [=]() {
                    if (!cancelGeneration) {
                        loadingDialog->accept();                        //закрытие диалога загрузки
                        showMessageBox("Генерация завершена", QString("Разреженная матрица №%1 сгенерировалась.").arg(selectedMatrixIndex));
                        dimensionInput->clear();
                        stackedWidget->setCurrentIndex(0);
                    }
                }, Qt::QueuedConnection);                              //выполнить это в главном потоке
            });
            QTimer* timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, [=]() {
                if (!future.isRunning() && !cancelGeneration) {
                    timer->stop();                                     //остановка таймера
                    loadingDialog->accept();
                }
            });
            timer->start(100);

        });

        connect(backButton, &QPushButton::clicked, this, &RandomInputPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

    //СОХРАНЕНИЕ НОМЕРА ВЫБРАННОЙ МАТРИЦЫ
    void setSelectedMatrixIndex(int index) {
        isSizeSaved = false;
        selectedMatrixIndex = index;
        dimensionInput->clear();
    }

signals:
    void showNextPage();

private:
    //ПРОВЕРКА
    bool validateInput(const QString& input, int& output, const QString& errorMessage) {
        bool ok;
        output = input.toInt(&ok);
        if (input.isEmpty() || !input.toInt(&ok) || !ok || output <= 0 || output > 10000 ) {
            showMessageBox("Неверный ввод", errorMessage);
            size[selectedMatrixIndex - 1] = 0;
            return false;
        }
        if (output < 5){
            showMessageBox("Предупреждение", "Слишком маленькая размерность разреженной матрицы.");
            size[selectedMatrixIndex - 1] = 0;
            return false;
        }
        return true;
    }

    void showMessageWithDelay(const QString &title, const QString &message) {
        QMetaObject::invokeMethod(this, [=]() {
            showMessageBox(title, message);
            cancelGeneration = true;                      //флаг для отмены генерации
            loadingDialog->accept();                      //закрытие диалога загрузки
            dimensionInput->clear();
            isSizeSaved = false;
        }, Qt::QueuedConnection);
    }

private slots:
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        if (showBackWarning("Предупреждение", "Вы точно хотите вернуться назад, не завершив ввод?")) {
            if (isSizeSaved){
                size[selectedMatrixIndex - 1] = previousSize;
            }
            stackedWidget->setCurrentIndex(2);
        } else {
            return;
        }
    }

    //очистка матрицы
    void clearMatrix(int& count_diagonal, int& count_notdiagonal, int& size){
        filled[selectedMatrixIndex - 1] = false;
        size = 0;
        count_diagonal = 0;
        count_notdiagonal = 0;
    }

    //сохранение размерности матрицы
    void onContinueButtonClicked() {
        strn = dimensionInput->text();

        //проверка на корректность ввода размерности
        if (validateInput(strn, n, "Пожалуйста, введите числовое значение от 1 до 10 000.")) {
            isSizeSaved = true;
            previousSize = size[selectedMatrixIndex - 1];
            size[selectedMatrixIndex - 1] = n;
        }
    }
};


//ВЫБОР МАТРИЦЫ ДЛЯ ВВОДА
class InputPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    int selectedMatrixIndex;              //номер выбранной матрицы
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    QLineEdit* dimensionInput;            //поле ввода размерности
    QString inputMethod;                  //выбранный метод ввода
    QCheckBox* matrix1;
    QCheckBox* matrix2;
    QCheckBox* matrix3;
    QCheckBox* matrix4;
    QCheckBox* matrix5;

public:
    InputPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray), selectedMatrixIndex(-1) {

        QVBoxLayout* layout = new QVBoxLayout(this);

        QFont Font;
        Font.setPointSize(15);
        QFont Fontstr;
        Fontstr.setPointSize(18);

        QLabel* titleLabel = new QLabel("Выберите разреженную матрицу, в которую нужно ввести значения:", this);
        titleLabel->setAlignment(Qt::AlignCenter);;
        titleLabel->setFont(Fontstr);
        titleLabel->setStyleSheet("QLabel { color: #001a33; }");
        layout->addWidget(titleLabel);

        //ЧЕКБОКСЫ ВЫБОРА МАТРИЦЫ
        QVBoxLayout* checkBoxLayout = new QVBoxLayout();
        matrix1 = new QCheckBox("Матрица №1", this);
        matrix2 = new QCheckBox("Матрица №2", this);
        matrix3 = new QCheckBox("Матрица №3", this);
        matrix4 = new QCheckBox("Матрица №4", this);
        matrix5 = new QCheckBox("Матрица №5", this);

        QCheckBox* matrices[] = { matrix1, matrix2, matrix3, matrix4, matrix5 };
        applyCheckBoxStyle(matrices, sizeof(matrices) / sizeof(matrices[0]));  //стиль

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
        QPushButton* continueButton = new QPushButton("Продолжить", this);
        continueButton->setFixedSize(290, 50);
        continueButton->setFont(Font);
        layout->addWidget(continueButton, 0, Qt::AlignCenter);

        //КНОПКИ
        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(continueButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        layout->addLayout(exitLayout);
        setLayout(layout);
        layout->setSpacing(2);
        layout->setContentsMargins(5, 5, 5, 5);

        //ПЕРЕХОДЫ НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(backButton, &QPushButton::clicked, this, &InputPage::onBackButtonClicked);
        connect(continueButton, &QPushButton::clicked, this, &InputPage::onContinueButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

public slots:
    //СОХРАНЕНИЕ ВЫБРАННОГО МЕТОДА ВВОДА МАТРИЦЫ
    void setInputMethod(const QString& method) {
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
            if (senderCheckBox == matrix1 || senderCheckBox == matrix2 || senderCheckBox == matrix3 || senderCheckBox == matrix4 || senderCheckBox == matrix5){
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
    }

    //ПЕРЕХОДЫ НА СТРАНИЦЫ ВВОДА МАТРИЦЫ
    void onContinueButtonClicked() {
        if (selectedMatrixIndex == -1) {
            showMessageBox("Ошибка", "Выберите разреженную матрицу для ввода.");
            stackedWidget->setCurrentIndex(2);
        }
        else if (filled[selectedMatrixIndex - 1] == true){
            if (showBackWarning("Предупреждение", "Эта матрица заполнена. Перезаписать её?")) {
                if (inputMethod == "Ввести с клавиатуры") {
                    InputKeyboard* keyboardPage = qobject_cast<InputKeyboard*>(stackedWidget->widget(5));
                    if (keyboardPage) {
                        keyboardPage->setSelectedMatrixIndex(selectedMatrixIndex);
                    }
                    stackedWidget->setCurrentIndex(5);
                }
                else if (inputMethod == "Ввести из текстового файла") {
                    InputFilePage* filePage = qobject_cast<InputFilePage*>(stackedWidget->widget(3));
                    if (filePage) {
                        filePage->setSelectedMatrixIndex(selectedMatrixIndex);
                    }
                    stackedWidget->setCurrentIndex(3);
                }
                else if (inputMethod == "Случайный ввод") {
                    RandomInputPage* randomPage = qobject_cast<RandomInputPage*>(stackedWidget->widget(4));
                    if (randomPage) {
                        randomPage->setSelectedMatrixIndex(selectedMatrixIndex);
                    }
                    stackedWidget->setCurrentIndex(4);
                }
            } else {
                return;
            }
        }else {
            if (inputMethod == "Ввести с клавиатуры") {
                InputKeyboard* keyboardPage = qobject_cast<InputKeyboard*>(stackedWidget->widget(5));
                if (keyboardPage) {
                    keyboardPage->setSelectedMatrixIndex(selectedMatrixIndex);
                }
                stackedWidget->setCurrentIndex(5);
            }
            else if (inputMethod == "Ввести из текстового файла") {
                InputFilePage* filePage = qobject_cast<InputFilePage*>(stackedWidget->widget(3));
                if (filePage) {
                    filePage->setSelectedMatrixIndex(selectedMatrixIndex);
                }
                stackedWidget->setCurrentIndex(3);
            }
            else if (inputMethod == "Случайный ввод") {
                RandomInputPage* randomPage = qobject_cast<RandomInputPage*>(stackedWidget->widget(4));
                if (randomPage) {
                    randomPage->setSelectedMatrixIndex(selectedMatrixIndex);
                }
                stackedWidget->setCurrentIndex(4);
            }
        }
    }
};


//ВЫБОР ВВОДА МАТРИЦЫ
class NewMatrixPage : public QWidget {
    Q_OBJECT

private:
    int* size;                            //матрица размерностей разреж. матриц
    bool* filled;                         //матрица заполненности разреж. матриц
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц

public:
    NewMatrixPage(QStackedWidget* stackedWidget, int* matrixSize, bool* filledArray, QWidget* parent = nullptr)
        : QWidget(parent), stackedWidget(stackedWidget), size(matrixSize), filled(filledArray) {

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

        //КНОПКИ
        QVBoxLayout* buttonLayout = new QVBoxLayout();
        QPushButton* input1 = new QPushButton("Ввести с клавиатуры", this);
        QPushButton* input2 = new QPushButton("Ввести из текстового файла", this);
        QPushButton* input3 = new QPushButton("Случайный ввод", this);

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
        QLabel* titleLabel = new QLabel("Выбор способа ввода разреженной матрицы", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setFont(Fontstr);
        titleLabel->setStyleSheet("QLabel { color: #001a33; }");
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
        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QPushButton* backButton = new QPushButton("Назад", this);
        backButton->setFont(Font);
        backButton->setFixedSize(200, 50);
        exitLayout->addWidget(backButton);

        QPushButton* exitButton = new QPushButton("Выход", this);
        exitButton->setFont(Font);
        exitButton->setFixedSize(200, 50);
        exitLayout->addWidget(exitButton);

        setupButton(input1, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(input2, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(input3, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(backButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(exitButton, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        layout->addLayout(exitLayout);
        setLayout(layout);

        //ПЕРЕХОД НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(input1, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        connect(input2, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        connect(input3, &QPushButton::clicked, this, &NewMatrixPage::onInputMethodSelected);
        connect(backButton, &QPushButton::clicked, this, &NewMatrixPage::onBackButtonClicked);
        connect(exitButton, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }

signals:
    void backButtonClicked();  //сигнал назад
    void inputMethodSelected(const QString& method); //сигнал для выбранного метода ввода

private slots:
    //СИГНАЛ НАЗАД
    void onBackButtonClicked() {
        stackedWidget->setCurrentIndex(0);
    }
    //ПЕРЕХОД  К ВЫБОРУ МАТРИЦЫ
    void onInputMethodSelected() {
        QPushButton* button = qobject_cast<QPushButton*>(sender());
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
    QStackedWidget* stackedWidget;        //указатель на виджет для хранения нескольких страниц
    int operation;                        //номер выбранной операции над матрицами
    int choice;
    int currentOperation;                 //вспомогательная переменная для operation
    int selectedMatrixIndex = -1;
    //кнопки
    QPushButton* newButton;
    QPushButton* viewButton;
    QPushButton* editButton;
    QPushButton* delButton;
    QPushButton* addButton;
    QPushButton* subButton;
    QPushButton* mulButton;
    QPushButton* inverseButton;
    QPushButton* locatButton;
    QPushButton* quit;
    //строки
    QLabel* dataLabel;
    QLabel* operationLabel;
    //макет
    QGridLayout* buttonLayout;

public:
    MainWindows(int* matrixSize, bool* filledArray) : size(matrixSize), filled(filledArray) {

        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(240, 255, 255));
        this->setPalette(palette);

        setWindowTitle("MathMat");
        setFixedSize(1000, 600);

        //главное представление с переключением страниц
        stackedWidget = new QStackedWidget(this);

        //ГЛАВНАЯ СТРАНИЦА
        QWidget* mainPage = new QWidget();
        QVBoxLayout* mainLayout = new QVBoxLayout(mainPage);

        //КНОПКИ
        newButton = new QPushButton("Новая матрица");
        viewButton = new QPushButton("Просмотр матрицы");
        editButton = new QPushButton("Редактировать");
        delButton = new QPushButton("Удалить");
        addButton = new QPushButton("Сложение двух матриц");
        subButton = new QPushButton("Вычитание двух матриц");
        mulButton = new QPushButton("Умножение двух матриц");
        inverseButton = new QPushButton("Обратная матрица");
        locatButton = new QPushButton("Физическое расположение");
        quit = new QPushButton("Выход");

        //РАЗМЕР КНОПОК
        QSize buttonSize(290, 50);
        QSize buttonSizeexit(200, 50);
        newButton->setFixedSize(buttonSize);
        viewButton->setFixedSize(buttonSize);
        editButton->setFixedSize(buttonSize);
        delButton->setFixedSize(buttonSize);
        addButton->setFixedSize(buttonSize);
        subButton->setFixedSize(buttonSize);
        mulButton->setFixedSize(buttonSize);
        inverseButton->setFixedSize(buttonSize);
        locatButton->setFixedSize(buttonSize);
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
        locatButton->setFont(Font);
        quit->setFont(Font);

        setupButton(newButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(viewButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(editButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(delButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(addButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(subButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(mulButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(inverseButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(locatButton, "#F0FFFF", "#A3C1DA", "#c8d9e8", "#003366", "#001a33");
        setupButton(quit, "#c8d9e8", "#a5c0d9", "#001a33", "#003366", "#c8d9e8");

        QFont Fontstr;
        Fontstr.setPointSize(18);
        dataLabel = new QLabel("Работа с данными:");
        operationLabel = new QLabel("Выполнить операцию:");
        dataLabel->setAlignment(Qt::AlignCenter);
        operationLabel->setAlignment(Qt::AlignCenter);
        dataLabel->setFont(Fontstr);
        operationLabel->setFont(Fontstr);
        dataLabel->setStyleSheet("QLabel { color: #001a33; }");
        operationLabel->setStyleSheet("QLabel { color: #001a33 }");


        //размещение кнопок в макете
        buttonLayout = new QGridLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->addItem(new QSpacerItem(20, 150, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 0);
        buttonLayout->addWidget(dataLabel, 1, 0);
        buttonLayout->addWidget(newButton, 2, 0);
        buttonLayout->addWidget(viewButton, 3, 0);
        buttonLayout->addWidget(editButton, 4, 0);
        buttonLayout->addWidget(locatButton, 5, 0);
        buttonLayout->addWidget(delButton, 6, 0);
        buttonLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), 6, 0);
        buttonLayout->addWidget(operationLabel, 1, 1);
        buttonLayout->addWidget(addButton, 2, 1);
        buttonLayout->addWidget(subButton, 3, 1);
        buttonLayout->addWidget(mulButton, 4, 1);
        buttonLayout->addWidget(inverseButton, 5, 1);
        buttonLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), 7, 0);

        mainLayout->addLayout(buttonLayout);

        //добавление кнопки "Выход" в главную страницу
        QHBoxLayout* exitLayout = new QHBoxLayout();
        exitLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        exitLayout->addWidget(quit);

        QSpacerItem* spacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        mainLayout->addItem(spacerItem);
        mainLayout->addLayout(exitLayout);

        //СТРАНИЦЫ
        stackedWidget->addWidget(mainPage);
        NewMatrixPage* newMatrixPage = new NewMatrixPage(stackedWidget, size, filled);
        InputPage* inputPage = new InputPage(stackedWidget, size, filled);
        InputFilePage* inputFilePage = new InputFilePage(stackedWidget, size, filled);
        RandomInputPage* randomInputPage = new RandomInputPage(stackedWidget, size, filled);
        InputKeyboard* keyboardPage = new InputKeyboard(stackedWidget, size, filled, 1);
        ViewPage* viewPage = new ViewPage(stackedWidget, size, filled);
        ViewPageInput* viewInputPage = new ViewPageInput(stackedWidget, size, filled);
        RemovMatrixInput* removInputMatrix = new RemovMatrixInput(stackedWidget, size, filled);
        OperationMatrixInput* operatInputMatrix = new OperationMatrixInput(stackedWidget, size, filled);
        OperatViewPage* operatPageMatrix = new OperatViewPage(stackedWidget, size, filled);
        SaveMatrixOperat* saveOperatMatrix = new SaveMatrixOperat(stackedWidget, size, filled);
        EditInputPage* editInputPage = new EditInputPage(stackedWidget, size, filled, selectedMatrixIndex);
        Iterpre_Matrix* matrixiterpre = new Iterpre_Matrix(stackedWidget, size, filled);
        stackedWidget->addWidget(newMatrixPage);
        stackedWidget->addWidget(inputPage);
        stackedWidget->addWidget(inputFilePage);
        stackedWidget->addWidget(randomInputPage);
        stackedWidget->addWidget(keyboardPage);
        stackedWidget->addWidget(viewPage);
        stackedWidget->addWidget(viewInputPage);
        stackedWidget->addWidget(removInputMatrix);
        stackedWidget->addWidget(operatInputMatrix);
        stackedWidget->addWidget(operatPageMatrix);
        stackedWidget->addWidget(saveOperatMatrix);
        stackedWidget->addWidget(editInputPage);
        stackedWidget->addWidget(matrixiterpre);

        QVBoxLayout* centralLayout = new QVBoxLayout(this);
        centralLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        centralLayout->addWidget(stackedWidget);
        centralLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        setLayout(centralLayout);

        //ПЕРЕХОД НА ДРУГИЕ СТРАНИЦЫ ПО КНОПКАМ
        connect(newButton, &QPushButton::clicked, this, &MainWindows::showNewMatrixPage);
        connect(editButton, &QPushButton::clicked, this, &MainWindows::showEditInputPage);
        connect(newMatrixPage, &NewMatrixPage::inputMethodSelected, inputPage, &InputPage::setInputMethod);

        connect(viewButton, &QPushButton::clicked, this, [this]() { showMatrixInput(1); });
        connect(locatButton, &QPushButton::clicked, this, [this]() { showMatrixInput(2); });
        connect(delButton, &QPushButton::clicked, this, &MainWindows::showRemovInput);
        connect(addButton, &QPushButton::clicked, this, [this]() { showOperationInput(1); });
        connect(subButton, &QPushButton::clicked, this, [this]() { showOperationInput(2); });
        connect(mulButton, &QPushButton::clicked, this, [this]() { showOperationInput(3); });
        connect(inverseButton, &QPushButton::clicked, this, &MainWindows::showIterpreInput);
        connect(quit, &QPushButton::clicked, this, [=]() {
            showExitConfirmation(this);
        });
    }


    void setOperation(int operation) {
        this->operation = operation;
    }


private slots:
    //переход на показ матрицы
    void showMatrixInput(int temp) {
        choice = temp;
        static_cast<ViewPageInput*>(stackedWidget->widget(7))->setChoice(choice);
        stackedWidget->setCurrentIndex(7);
    }
    void showNewMatrixPage() {
        stackedWidget->setCurrentIndex(1);  //переход на окно ввода матрицы
    }
    void showRemovInput() {
        stackedWidget->setCurrentIndex(8);  //перевод на удаление
    }
    //переход на операции
    void showOperationInput(int operation) {
        currentOperation = operation;
        static_cast<OperationMatrixInput*>(stackedWidget->widget(9))->setOperation(currentOperation);
        stackedWidget->setCurrentIndex(9);   //переход на операции
    }
    void showEditInputPage() {
        stackedWidget->setCurrentIndex(12);   //переход на редактирование
    }
    void showIterpreInput(){
        stackedWidget->setCurrentIndex(13);   //переход на обратную матрицу
    }
};


//УПРАВЛЕНИЕ ПЕРЕХОДАМИ МЕЖДУ ВХОДОМ И ГЛАВНОЙ СТРАНИЦЕЙ
class MainApp : public QObject {
    Q_OBJECT

public:
    MainApp(LandingPage* landingPage, MainWindows* mainWindow)
        : landingPage(landingPage), mainWindow(mainWindow) {
        connect(landingPage, &LandingPage::continueToMain, this, &MainApp::showMainWindow);

    }

private slots:
    //кнопка "продолжить"
    void showMainWindow() {
        landingPage->hide();  // Hide LandingPage
        mainWindow->show();   // Show MainWindows
    }

private:
    LandingPage* landingPage;  //страница приветствия (входа)
    MainWindows* mainWindow;   //главная страница (меню)
};


int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString& locale : uiLanguages) {
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

    LandingPage* landingPage = new LandingPage();
    MainWindows* mainWindow = new MainWindows(size, filled);
    mainWindow->setWindowIcon(QIcon(":/icon.ico"));
    MainApp* appManager = new MainApp(landingPage, mainWindow);

    landingPage->show();
    return a.exec();
}

#include "main.moc"
