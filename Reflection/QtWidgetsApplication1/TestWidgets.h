#pragma once

#include <QMainWindow>
#include "ui_TestWidgets.h"

class TestWidgets : public QMainWindow
{
	Q_OBJECT

public:
	TestWidgets(QWidget *parent = nullptr);
	~TestWidgets();

private:
	Ui::TestWidgetsClass ui;
};
