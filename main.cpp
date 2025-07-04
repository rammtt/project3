#include <QApplication>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMap>
#include <QTextStream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Анализатор символов");
    window.resize(300, 200);

    QVBoxLayout layout(&window);
    QPushButton button("Открыть файл");
    QLabel label("Результат появится здесь");
    label.setWordWrap(true);

    layout.addWidget(&button);
    layout.addWidget(&label);

    QObject::connect(&button, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&window, "Выберите файл", "", "Текстовые файлы (*.txt)");
        if (filePath.isEmpty()) return;

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            label.setText("Ошибка открытия файла");
            return;
        }

        QTextStream in(&file);
        QString text = in.readAll();
        file.close();

        QMap<QChar, int> charCount;
        for (QChar ch : text) {
            charCount[ch]++;
        }

        QString result = "Символы в файле:\n";
        for (auto it = charCount.begin(); it != charCount.end(); ++it) {
            QChar ch = it.key();
            QString display =
                ch == '\n' ? "\\n" :
                    ch == '\t' ? "\\t" :
                    ch == ' '  ? "пробел" :
                    QString("'%1'").arg(ch);

            result += QString("%1 - %2 шт.\n").arg(display).arg(it.value());
        }

        label.setText(result);
    });

    window.show();
    return app.exec();
}
