/* pinentrydialog.h - A (not yet) secure Qt 4 dialog for PIN entry.
 * Copyright (C) 2002, 2008 Klarälvdalens Datakonsult AB (KDAB)
 * Copyright 2007 Ingo Klöcker
 * Copyright 2016 Intevation GmbH
 * Copyright (C) 2021 g10 Code GmbH
 *
 * Written by Steffen Hansen <steffen@klaralvdalens-datakonsult.se>.
 * Modified by Andre Heinecke <aheinecke@intevation.de>
 * Software engineering by Ingo Klöcker <dev@ingo-kloecker.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __PINENTRYDIALOG_H__
#define __PINENTRYDIALOG_H__

#include <QDialog>
#include <QStyle>
#include <QTimer>

#include "pinentry.h"

class QLabel;
class QPushButton;
class QLineEdit;
class PinLineEdit;
class QString;
class QProgressBar;
class QCheckBox;
class QAction;

QPixmap icon(QStyle::StandardPixmap which = QStyle::SP_CustomBase);

void raiseWindow(QWidget *w);

class PinEntryDialog : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString error READ error WRITE setError)
    Q_PROPERTY(QString pin READ pin WRITE setPin)
    Q_PROPERTY(QString prompt READ prompt WRITE setPrompt)
public:
    struct FormattedPassphraseOptions
    {
        bool formatPassphrase;
        QString hint;
    };
    struct ConstraintsOptions
    {
        bool enforce;
        QString shortHint;
        QString longHint;
        QString errorTitle;
    };

    explicit PinEntryDialog(QWidget *parent = 0, const char *name = 0,
                            int timeout = 0, bool modal = false,
                            bool enable_quality_bar = false,
                            const QString &repeatString = QString(),
                            const QString &visibiltyTT = QString(),
                            const QString &hideTT = QString());

    void setDescription(const QString &);
    QString description() const;

    void setError(const QString &);
    QString error() const;

    void setPin(const QString &);
    QString pin() const;

    QString repeatedPin() const;
    void setRepeatErrorText(const QString &);

    void setPrompt(const QString &);
    QString prompt() const;

    void setOkText(const QString &);
    void setCancelText(const QString &);

    void setQualityBar(const QString &);
    void setQualityBarTT(const QString &);

    void setGenpinLabel(const QString &);
    void setGenpinTT(const QString &);

    void setCapsLockHint(const QString &);

    void setFormattedPassphrase(const FormattedPassphraseOptions &options);

    void setConstraintsOptions(const ConstraintsOptions &options);

    void setPinentryInfo(pinentry_t);

    bool timedOut() const;

protected Q_SLOTS:
    void updateQuality(const QString &);
    void slotTimeout();
    void textChanged(const QString &);
    void focusChanged(QWidget *old, QWidget *now);
    void toggleVisibility();
    void onBackspace();
    void generatePin();
    void toggleFormattedPassphrase();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    /* reimp */ void showEvent(QShowEvent *event);

private Q_SLOTS:
    void cancelTimeout();
    void checkCapsLock();
    void onAccept();

private:
    enum PassphraseCheckResult {
        PassphraseNotChecked = -1,
        PassphraseNotOk = 0,
        PassphraseOk
    };
    PassphraseCheckResult checkConstraints();

private:
    QLabel    *_icon;
    QLabel    *_desc;
    QLabel    *_error;
    QLabel    *_prompt;
    QLabel    *_quality_bar_label;
    QProgressBar *_quality_bar;
    PinLineEdit *_edit;
    PinLineEdit *mRepeat;
    QLabel      *mRepeatError;
    QPushButton *_ok;
    QPushButton *_cancel;
    bool       _grabbed;
    bool       _have_quality_bar;
    bool       _timed_out;
    bool       _disable_echo_allowed;
    bool       mEnforceConstraints;
    bool       mFormatPassphrase;
    pinentry_t _pinentry_info;
    QTimer    *_timer;
    QString    mVisibilityTT,
               mHideTT;
    QAction   *mVisiActionEdit;
    QPushButton *mGenerateButton;
    QCheckBox *mVisiCB;
    QLabel    *mFormattedPassphraseHint;
    QLabel    *mFormattedPassphraseHintSpacer;
    QLabel    *mCapsLockHint;
    QLabel    *mConstraintsHint;
    QString   mConstraintsErrorTitle;
};

#endif // __PINENTRYDIALOG_H__
