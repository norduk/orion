#include "streamitem.h"
#include <QTimer>
#include <QApplication>

StreamItem::StreamItem(Channel* channel)
{
    this->channel = channel;
    update();
    updateIcon();
}

StreamItem::~StreamItem(){
    qDebug() << "Destroyer: StreamItem";
}


void StreamItem::update(){
        QString title = getName();
        QString image = "<img src=\"";
        image += channel->getPreviewPath().c_str();
        image += "\"></img>";
        QString tooltip = "<table><tr><td>"+getInfo()+"</td></tr>";
        tooltip += "<tr><td>"+image+"</td></tr>";

        if (!online()){
            tooltip += "<tr><td>Last seen: ";
            tooltip += this->getChannel()->lastOnline().c_str();
            tooltip += "</td></tr>";
            setTextColor(QColor(200, 200, 200));
        }
        else {
            QString viewercount = std::to_string(channel->getViewers()).c_str();
            tooltip += "<tr><td><b>Now streaming:</b> <i>"+viewercount+" viewers</i></td></tr>";
            setTextColor(QColor(0, 0, 0));
        }

        this->setText(title);

        tooltip += "</table>";
        QListWidgetItem::setToolTip(tooltip);
}

void StreamItem::updateIcon()
{
    if (!channel->getLogoPath().empty())
        setIcon(QIcon(channel->getLogoPath().c_str()));
    else
        setIcon(QIcon("logos/loading.gif"));
}

Channel* StreamItem::getChannel() const{
    return channel;
}

bool StreamItem::online() const{
    return channel->isOnline();
}

const QString StreamItem::getName(){
    if (!channel->getName().empty())
        return channel->getName().c_str();
    return channel->getUriName().c_str();
}

const QString StreamItem::getUriName(){
    return channel->getUriName().c_str();
}

const QString StreamItem::getInfo(){
    return channel->getInfo().c_str();
}

bool StreamItem::operator< (const QListWidgetItem& other) const{
    const StreamItem& stritem = dynamic_cast<const StreamItem&>(other);
    //return (this->online() != stritem.online()) ? this->online() : (QString::compare(this->text(),stritem.text()) < 0);
    //return (this->online() != stritem.online()) ? this->online() : (channel->getViewers() >= stritem.getChannel()->getViewers());

    if (online() == stritem.online()){ //BOTH ONLINE OR BOTH OFFLINE
        if (online()){  //BOTH ONLINE, COMPARISON BY VIEWER COUNT
            return (channel->getViewers() >= stritem.getChannel()->getViewers());
        }
        else{ //BOTH OFFLINE, COMPARISON BY DEFAULT QSTRING METHOD
            return (QString::compare(this->text(),stritem.text()) < 0);
        }
    }
    return online();    //OTHER IS ONLINE AND OTHER IS NOT
}
