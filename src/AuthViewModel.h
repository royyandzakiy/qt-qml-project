#pragma once
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
class QQmlEngine;
class QJSEngine;

class AuthViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(AuthVM)
    QML_SINGLETON

public:
    explicit AuthViewModel(QObject* parent = nullptr);

    static AuthViewModel* create(QQmlEngine*, QJSEngine*) {
        return new AuthViewModel;
    }

    bool    isLoggedIn()   const { return m_isLoggedIn; }
    QString errorMessage() const { return m_errorMessage; }

    Q_PROPERTY(bool    isLoggedIn    READ isLoggedIn    NOTIFY isLoggedInChanged)
    Q_PROPERTY(QString errorMessage  READ errorMessage  NOTIFY errorMessageChanged)

signals:
    void isLoggedInChanged();
    void errorMessageChanged();

public slots:
    void onLogin(const QString& username, const QString& password);
    void onLogout();

private:
    bool    m_isLoggedIn{false};
    QString m_errorMessage;
};
