#ifndef JSON_H
#define JSON_H

#include <QJsonObject>

namespace avs8086 {

class Json
{
public:
    Json();
    ~Json();

    QJsonValueRef operator[](const QString& key)
    {
        return jsonObject[key];
    }

    QJsonValue operator[] (const QString &key) const
    {
        return jsonObject[key];
    }

    // 从文件加载JSON
    bool loadFromFile(const QString& fileName);

    // 将JSON保存到文件
    bool saveToFile(const QString& fileName) const;

    // 设置JSON对象
    void setObject(const QJsonObject& jsonObj);

    // 获取JSON对象
    QJsonObject getObject() const;

private:
    QJsonObject jsonObject;
};

} // namespace avs8086

#endif // JSON_H
