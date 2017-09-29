/* DraManager.h
 *
 * Copyright (C) 2013 Jason Allen
 *
 * This file is part of the Jade Diagram Editor.
 *
 * Jade is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Jade is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Jade.  If not, see <http://www.gnu.org/licenses/>
 */


#ifndef DIAGRAMMANAGER_H
#define DIAGRAMMANAGER_H

#include <DiagramTypes.h>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))

#else
#include <QtPrintSupport>
#endif

class Diagram;

class DiagramManager : public QStackedWidget
{
	Q_OBJECT

private:
    Diagram* mDiagram;

	QString mFilePath;
	QString mFileFilter;
	QString mFileSuffix;

	int mNewDiagramCount;
	bool mPromptCloseUnsaved;
	bool mPromptOverwrite;
	QDir mWorkingDirectory;
	QPrinter mPrinter;

public:
    DiagramManager(Diagram* diagram);
    DiagramManager(Diagram* diagram, const QString& fileSuffix, const QString& fileFilter);
    ~DiagramManager();

	void setFilePath(const QString& path);
	void setFileInfo(const QString& suffix, const QString& filter);
	QString filePath() const;
	QString fileName() const;
	QString fileSuffix() const;
	QString fileFilter() const;

	void setPromptOnClosingUnsaved(bool prompt);
	void setPromptOnOverwrite(bool prompt);
	bool shouldPromptOnClosingUnsaved() const;
	bool shouldPromptOnOverwrite() const;

    bool hasDiagram() const;

public slots:
    bool newDiagram();
    bool openDiagram();
    bool saveDiagram();
    bool saveDiagramAs();
    bool closeDiagram();

    bool loadDiagram(const QString& filePath);

	void exportPng();
	void exportSvg();

	void printPreview();
	void printSetup();
    void printDiagram();
	void printPdf();

signals:
	void visibilityChanged(bool visible);
    void diagramChanged(const QString& name);
    void diagramCleared();

private slots:
    void showDiagram();
    void hideDiagram();

	void printPages(QPrinter* printer);

private:
    void setupPainterForExport(DiagramPainter* painter);
	QString suggestFilePath(const QString& suffix) const;
	int suggestExportWidth() const;
	int suggestExportHeight() const;

	bool promptToOpenFile(QString& filePath, const QString& filters);
	bool promptToSaveFile(QString& filePath, const QString& filters, const QString& suffix);
};

#endif
