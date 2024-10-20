#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>

class card : public QLabel
{
    Q_OBJECT
public:
    explicit card(QWidget *parent = nullptr);
    void setImage(const QString &path);
    void setCoverImage(const QString &coverImagePath);
    void revealImage(); // Kartı açar.
    void hideImage(); // Kartı kapatır.

    QString getImagePath() const;
    bool isRevealed() const;

signals:
    void clicked(card *c);  // Kart tıklandığında sinyal

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    QString imagePath;
    bool revealed;
    QLabel *coverLabel; // Kapak resmi için QLabel
};


#endif // CARD_H
