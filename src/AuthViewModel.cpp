#include "AuthViewModel.h"

AuthViewModel::AuthViewModel(QObject* parent) : QObject(parent) {}

void AuthViewModel::onLogin(const QString& username, const QString& password) {
    if (username == "admin" && password == "admin") {
        m_isLoggedIn    = true;
        m_errorMessage.clear();
        emit isLoggedInChanged();
        emit errorMessageChanged();
    } else {
        m_errorMessage = "Invalid username or password";
        emit errorMessageChanged();
    }
}

void AuthViewModel::onLogout() {
    m_isLoggedIn = false;
    emit isLoggedInChanged();
}
