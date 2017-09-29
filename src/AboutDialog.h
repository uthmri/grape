/* AboutDialog.h
 *
 * Copyright (C) 2016 UTHealth MRI Research
 *
 * This file is part of the GRAPE Diagram Editor.
 *
 * GRAPE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GRAPE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GRAPE.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtCore>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui>
#else
#include <QtWidgets>
#endif


class AboutDialog : public QDialog
{
    Q_OBJECT
    
private:
    QFrame* createAboutTab();
    QFrame* createLicenseTab();
    
public:
    AboutDialog(QWidget* parent = NULL);
    ~AboutDialog();
};

#endif
