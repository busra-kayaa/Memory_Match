#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <random> // std::default_random_engine
#include <algorithm> // std::shuffle

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , firstCard(nullptr)
    , secondCard(nullptr)
    , failedAttempts(0)
{
    ui->setupUi(this);
    this->showFullScreen(); // Pencereyi tam ekran yapar.

    // Resimlerin yolları
    QList<QString> imagePaths = {
        ":/res/images/0.png",
        ":/res/images/1.png",
        ":/res/images/2.png",
        ":/res/images/3.png",
        ":/res/images/4.png",
        ":/res/images/0.png",
        ":/res/images/1.png",
        ":/res/images/2.png",
        ":/res/images/3.png",
        ":/res/images/4.png"
    };

    // Kartları karıştırmak için bir rastgele sayı üreteci oluştur
    std::random_device rd;  // Gerçek rastgelelik için
    std::default_random_engine rng(rd()); // Mersenne Twister rastgele sayı üreteci

    // Resimleri karıştır
    std::shuffle(imagePaths.begin(), imagePaths.end(), rng);

    const int cardWidth = 183;
    const int cardHeight = 275;

    const int xDistance = 210; // Kartlar arası yatay mesafe (Konumları deneyerek buldum.)
    const int yDistance = 310; // Kartlar arası dikey mesafe (Konumları deneyerek buldum.)

    QString coverImagePath =":/res/images/kapak.png";

    // Kartları oluştur ve resimleri rastgele ata
    for (int i=0; i<10; ++i ) {
        card *c = new card(this); // Card sınıfından yeni bir kart oluşturulur.
        c->setScaledContents(true); // Kartın içindeki resim, kartın boyutlarına otomatik olarak ölçeklendirilir.

        int raw = i / 5 ; // Kaçıncı Satırda (0,1,2,3,4' ün kalanı 0. bir şey olacağı için 0'ı alır.)(5,6,7,8,9 'un kalanı 1. bir şey olacağı için 1'i alır)
        int col = i % 5;  // Kaçıncı Sütunda ( 0%5 = 0, 1%5 = 1, 2%5 = 2, 3%5 = 3, 4%5 = 4, 5%5 = 0, 6%5 = 1, 7%5 = 2, 8%5 = 3, 9%5 = 4 )
        c->setGeometry(15 + col * xDistance, 15 + raw * yDistance, cardWidth, cardHeight);

        c->setFrameShape(QFrame::Box); //Kartın etrafına kutu şeklinde bir çerçeve çizilir

        // Eğer resim listesi boş değilse rastgele bir resim seçelim
        if (!imagePaths.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(imagePaths.size());

            // Seçilen resim
            QString selectedImage = imagePaths[randomIndex];
            c->setImage(selectedImage);  // Kartın resmi olarak bu resmi atıyoruz

            // Seçilen resmi listeden çıkaralım
            imagePaths.removeAt(randomIndex);

            // Kalan resim listesini kontrol edelim (debug için)
            qDebug() << "Kalan resimler:" << imagePaths;
        }

        c->setCoverImage(coverImagePath); // Kapak resmini ayarlıyoruz
        connect(c, &card::clicked, this, &MainWindow::cardClicked); // kart tıklandığında MainWindow sınıfındaki cardClicked fonksiyonunu tetiklemek için kullanılır.

        c->show();
        cards.append(c); // Tüm kartları listeye ekle
    }

    attemptLabel = new QLabel(this); // Başarısız tahmin sayısını göstermek için Label nesni oluşturulur.
    attemptLabel->setGeometry(300, 700, 500, 50); // Yazının konumu ayarlanır.
    attemptLabel->setText("Başarısız Tahmin Sayısı: ");

    QFont font = attemptLabel->font(); // attemptLabel'ın yazı fontunu al
    font.setPointSize(25); // Yazı büyüklüğü 25 punto yapılır.
    attemptLabel->setFont(font); // Değiştirilen fontu attemptLabel'a uygular.
    attemptLabel->setStyleSheet("QLabel { color: #FF5733; }"); // Renk turuncu yapılır.
    attemptLabel->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) { // ESC tuşuna basılınca
        this->showNormal(); // Tam ekrandan çıkar.
    }
    QMainWindow::keyPressEvent(event); // Üst sınıfın fonksiyonu çağrılır.
}

void MainWindow::cardClicked(card *clickedCard)
{
    // Eğer iki kart zaten açılmışsa başka kart açılmasına izin verilmez.
    if (firstCard && secondCard) {
        return; // İki kart açıkken yeni bir kart tıklanırsa hiçbir işlem yapılmaz ve fonksiyon geri döner.
    }

    // Tıklanan kartın resmini göster (yani kapak resmini gizleyip gerçek resmini açar).
    clickedCard->revealImage();

    if (!firstCard) {
        firstCard = clickedCard; // Eğer bu ilk açılan kartsa, firstCard değişkenine atanır.
    } else {
        secondCard = clickedCard; // Eğer zaten bir kart açıksa, bu ikinci kart olarak kaydedilir.

        if (firstCard->getImagePath() == secondCard->getImagePath()) {
            firstCard = nullptr;
            secondCard = nullptr;
            checkWinCondition();
        } else {
            failedAttempts++; // Başarısız tahmin sayısını artırır
            updateAttemptLabel(); // Başarısız tahmin sayısını kullanıcıya gösteren etiketi günceller
            QTimer::singleShot(900, this, &MainWindow::hideCards); // 900 milisaniye sonra hideCards() fonksiyonunu çağırır; bu da açık kartları gizler.
        }
    }
}

void MainWindow::hideCards()
{
    firstCard->hideImage();
    secondCard->hideImage();
    firstCard = nullptr;
    secondCard = nullptr; //Açık kartları sıfırlar, böylece kullanıcı yeni kartları tıklayabilir.
}

void MainWindow::checkWinCondition()
{
    bool allRevealed = true;
    for (card *c : cards) {
        if (!c->isRevealed()) {
            allRevealed = false;
            break;
        }
    }

    if (allRevealed) {
        QMessageBox::information(this, "TEBRİKLER", "TÜM KARTLARI EŞLEŞTİRDİNİZ. Başarısız Tahmin Sayısı: " + QString::number(failedAttempts));
    }
}

void MainWindow::updateAttemptLabel()
{
    attemptLabel->setText("Başarısız Tahmin Sayısı: " + QString::number(failedAttempts));
}
