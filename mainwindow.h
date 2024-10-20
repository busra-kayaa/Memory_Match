#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QList>
#include "card.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    QList<card *> cards; // Tüm kartları bu listede tutarız.

private slots:
    void cardClicked (card *clickedCard);

private:
    Ui::MainWindow *ui;
    QLabel *attemptLabel; // Başarısız tahmin sayısını göstermek için
    card *firstCard; //İlk seçilen kart
    card *secondCard; // İkinci seçilen kart
    int failedAttempts;
    QList<QString> imagePaths; // Resim yolları üye değişkeni olarak tanımlanıyor

    void checkWinCondition();
    void hideCards();
    void updateAttemptLabel();
    void shuffleImages();  // shuffleImages fonksiyonunun prototipi

};
#endif // MAINWINDOW_H
