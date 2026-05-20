#pragma once
#include <QObject>

class AuthViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool    isLoggedIn    READ isLoggedIn    NOTIFY isLoggedInChanged)
    Q_PROPERTY(QString errorMessage  READ errorMessage  NOTIFY errorMessageChanged)

public:
    explicit AuthViewModel(QObject* parent = nullptr);

    bool    isLoggedIn()   const { return m_isLoggedIn; }
    QString errorMessage() const { return m_errorMessage; }

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
