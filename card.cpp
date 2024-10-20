#include "card.h"

card::card(QWidget *parent)
    : QLabel(parent)
    , revealed(false)
    , coverLabel(nullptr)
{
    // Kart başlangıçta kapak resmi ile oluşturuluyor
}

void card::setImage(const QString &path)
{
    imagePath = path;
    QPixmap pixmap(path);
    pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(pixmap);
}
/*
Qt::KeepAspectRatio: Resmin genişliği ve yüksekliği orantılı bir şekilde ayarlanır, böylece resmin bozulması önlenir.
Qt::SmoothTransformation: Resim daha pürüzsüz ve net görünür.
*/
void card::setCoverImage(const QString &coverImagePath)
{
    // Kapak resmi QLabel'i oluştur ve ayarla
    coverLabel = new QLabel(this);
    coverLabel->setPixmap(QPixmap(coverImagePath));
    coverLabel->setScaledContents(true);
    coverLabel->setGeometry(0, 0, width(), height());
    coverLabel->show();
}

void card::mousePressEvent(QMouseEvent *event) {
    emit clicked(this); // Tıklama sinyalini yay
    QLabel::mousePressEvent(event); // Üst sınıfın mousePressEvent'ini çağır
}

// emit clicked(this): Kart tıklandığında, bu kartın referansını yayar. Bu, MainWindow sınıfındaki cardClicked fonksiyonunu tetikler.

void card::revealImage()
{
    if (!revealed) {
        setPixmap(QPixmap(imagePath)); // Gerçek resmi göster
        coverLabel->hide(); // Kapak resmini gizle
        revealed = true; // Kart açıldı
    }
}

void card::hideImage() {
    if (revealed) {
        setPixmap(QPixmap()); // Resmi gizle
        coverLabel->show(); // Kapak resmini göster
        revealed = false; // Kart kapandı
    }
}

bool card::isRevealed() const {
    return revealed;
}

QString card::getImagePath() const {
    return imagePath;
}
