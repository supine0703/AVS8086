#include "json.h"

#include <QJsonDocument>
#include <QFile>
#include <QIODevice>
#include <QDebug>

using namespace tools;

Json::Json()
{
}

Json::~Json()
{

}

bool Json::loadFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "无法打开文件" << fileName;
        return false;
    }

    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    jsonObject = loadDoc.object();

    file.close();
    return true;
}

bool Json::saveToFile(const QString& fileName) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "无法打开文件" << fileName;
        return false;
    }

    QJsonDocument saveDoc(jsonObject);
    file.write(saveDoc.toJson());

    file.close();
    return true;
}

void Json::setObject(const QJsonObject& jsonObj)
{
    jsonObject = jsonObj;
}

QJsonObject Json::getObject() const
{
    return jsonObject;
}
