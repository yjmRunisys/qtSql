/********************************************************************************
** Form generated from reading UI file 'brower.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BROWER_H
#define UI_BROWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <connectionwidget.h>

QT_BEGIN_NAMESPACE

class Ui_Brower
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    ConnectionWidget *connectionWidget;
    QTableView *table;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *Brower)
    {
        if (Brower->objectName().isEmpty())
            Brower->setObjectName(QStringLiteral("Brower"));
        Brower->resize(846, 469);
        verticalLayout = new QVBoxLayout(Brower);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        connectionWidget = new ConnectionWidget(Brower);
        connectionWidget->setObjectName(QStringLiteral("connectionWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(connectionWidget->sizePolicy().hasHeightForWidth());
        connectionWidget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(connectionWidget);

        table = new QTableView(Brower);
        table->setObjectName(QStringLiteral("table"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(table->sizePolicy().hasHeightForWidth());
        table->setSizePolicy(sizePolicy1);
        table->setContextMenuPolicy(Qt::ActionsContextMenu);
        table->setLineWidth(1);

        horizontalLayout->addWidget(table);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton = new QPushButton(Brower);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalLayout_2->setStretch(0, 5);
        horizontalLayout_2->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(Brower);

        QMetaObject::connectSlotsByName(Brower);
    } // setupUi

    void retranslateUi(QWidget *Brower)
    {
        Brower->setWindowTitle(QApplication::translate("Brower", "Form", 0));
        pushButton->setText(QApplication::translate("Brower", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class Brower: public Ui_Brower {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BROWER_H
