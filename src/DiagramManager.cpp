/* DiagramManager.cpp
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


#include <DiagramManager.h>
#include <Diagram.h>
#include <QtSvg>

DiagramManager::DiagramManager(Diagram* diagram) : QStackedWidget()
{
    mNewDiagramCount = 0;
	mPromptCloseUnsaved = true;
	mPromptOverwrite = true;
#ifndef WIN32
	mWorkingDirectory = QDir::home();
#endif

	mPrinter.setOrientation(QPrinter::Landscape);
	mPrinter.setPageSize(QPrinter::Letter);
	mPrinter.setPageMargins(0.5, 0.5, 0.5, 0.5, QPrinter::Inch);
	mPrinter.setResolution(600);

    mDiagram = diagram;
	addWidget(mDiagram);
	addWidget(new QWidget());
	setCurrentIndex(1);
}
//--------------------------------------------------------------------------------------------------
DiagramManager::DiagramManager(Diagram* diagram, const QString& fileSuffix, const QString& fileFilter) : QStackedWidget()
{
	mFileSuffix = fileSuffix;
	mFileFilter = fileFilter;

    mNewDiagramCount = 0;
	mPromptCloseUnsaved = true;
	mPromptOverwrite = true;
#ifndef WIN32
	mWorkingDirectory = QDir::home();
#endif

    mDiagram = diagram;
    addWidget(mDiagram);
	addWidget(new QWidget());
	setCurrentIndex(1);
}
//--------------------------------------------------------------------------------------------------
DiagramManager::~DiagramManager() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramManager::setFilePath(const QString& path)
{
	mFilePath = path;
}
//--------------------------------------------------------------------------------------------------
void DiagramManager::setFileInfo(const QString& suffix, const QString& filter)
{
	mFileSuffix = suffix;
	mFileFilter = filter;
}
//--------------------------------------------------------------------------------------------------
QString DiagramManager::filePath() const
{
	return mFilePath;
}
//--------------------------------------------------------------------------------------------------
QString DiagramManager::fileName() const
{
	QFileInfo fileInfo(mFilePath);
	return fileInfo.fileName();
}
//--------------------------------------------------------------------------------------------------
QString DiagramManager::fileSuffix() const
{
	return mFileSuffix;
}
//--------------------------------------------------------------------------------------------------
QString DiagramManager::fileFilter() const
{
	return mFileFilter;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramManager::setPromptOnClosingUnsaved(bool prompt)
{
	mPromptCloseUnsaved = prompt;
}
//--------------------------------------------------------------------------------------------------
void DiagramManager::setPromptOnOverwrite(bool prompt)
{
	mPromptOverwrite = prompt;
}
//--------------------------------------------------------------------------------------------------
bool DiagramManager::shouldPromptOnClosingUnsaved() const
{
	return mPromptCloseUnsaved;
}
//--------------------------------------------------------------------------------------------------
bool DiagramManager::shouldPromptOnOverwrite() const
{
	return mPromptOverwrite;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramManager::hasDiagram() const
{
	return (currentIndex() == 0);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramManager::newDiagram()
{
    bool diagramCreated = false;

    if (closeDiagram())
	{
        diagramCreated = true;
        mNewDiagramCount++;
		mFilePath = "Untitled " + QString::number(mNewDiagramCount);
        showDiagram();
	}

    return diagramCreated;
}
//--------------------------------------------------------------------------------------------------
bool DiagramManager::openDiagram()
{
    bool diagramOpened = false;
	QString filePath;

	if (promptToOpenFile(filePath, mFileFilter + ";;All Files (*)") && closeDiagram())
	{
		mFilePath = filePath;
        diagramOpened = mDiagram->load(mFilePath);
        if (!diagramOpened)
		{
            hideDiagram();
			mFilePath.clear();
			mDiagram->clear();
            emit diagramCleared();
		}
        else showDiagram();
	}

    return diagramOpened;
}
//--------------------------------------------------------------------------------------------------
bool DiagramManager::saveDiagram()
{
    bool diagramSaved = false;

    if (hasDiagram())
	{
		if (mFilePath.startsWith("Untitled"))
            diagramSaved = saveDiagramAs();
		else
            diagramSaved = mDiagram->save(mFilePath);
	}

    return diagramSaved;
}
//--------------------------------------------------------------------------------------------------
bool DiagramManager::saveDiagramAs()
{
    bool diagramSaved = false;

    if (hasDiagram())
	{
		QString filePath = mFilePath;
		if (promptToSaveFile(filePath, mFileFilter + ";;All Files (*)", mFileSuffix))
		{
			mFilePath = filePath;
            diagramSaved = mDiagram->save(mFilePath);
			emit diagramChanged(fileName());
		}
	}

    return diagramSaved;
}
//--------------------------------------------------------------------------------------------------
bool DiagramManager::closeDiagram()
{
    bool diagramClosed = true;

    if (hasDiagram())
	{
		QMessageBox::StandardButton button = QMessageBox::Yes;

        if (mPromptCloseUnsaved && !mDiagram->isClean())
		{
			button = QMessageBox::question(this, "Save Changes",
				"Save changes to " + fileName() + " before closing?",
				QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);

			if (button == QMessageBox::Yes)
			{
				if (mFilePath.startsWith("Untitled"))
				{
                    if (!saveDiagramAs()) button = QMessageBox::Cancel;
				}
                else saveDiagram();
			}
		}

        diagramClosed = (button != QMessageBox::Cancel);
        if (diagramClosed)
		{
            hideDiagram();
			mFilePath.clear();
			mDiagram->clear();
            emit diagramCleared();
		}
	}

    return diagramClosed;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramManager::loadDiagram(const QString& filePath)
{
    bool diagramLoaded = false;

    if (closeDiagram())
	{
		if (!filePath.isEmpty())
		{
			setFilePath(filePath);
            diagramLoaded = mDiagram->load(filePath);
            if (diagramLoaded) showDiagram();
		}

        if (!diagramLoaded)
		{
			mDiagram->clear();
            emit diagramCleared();
            newDiagram();
		}
	}

    return diagramLoaded;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramManager::exportPng()
{
    if (hasDiagram())
	{
		QString filePath = suggestFilePath("png");
		if (promptToSaveFile(filePath, "Portable Network Graphics (*.png);;All Files (*)", "png"))
		{
            DiagramPainter* painter = new DiagramPainter();
			QImage pngImage(suggestExportWidth(), suggestExportHeight(), QImage::Format_ARGB32);
            QRectF visibleRect = mDiagram->diagramRectWithBorder();

			setupPainterForExport(painter);

            mDiagram->deselectAll();

			painter->begin(&pngImage);
			painter->scale(pngImage.width() / visibleRect.width(), pngImage.height() / visibleRect.height());
			painter->translate(-visibleRect.left(), -visibleRect.top());
			painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
            mDiagram->render(painter, visibleRect);
			painter->end();

			pngImage.save(filePath, "PNG");

			delete painter;
		}
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramManager::exportSvg()
{
    if (hasDiagram())
	{
		QString filePath = suggestFilePath("svg");
		if (promptToSaveFile(filePath, "Scalable Vector Graphics (*.svg);;All Files (*)", "svg"))
		{
            DiagramPainter* painter = new DiagramPainter();
			QSvgGenerator svgImage;
            QRectF visibleRect = mDiagram->diagramRectWithBorder();

			svgImage.setSize(QSize(suggestExportWidth(), suggestExportHeight()));
			svgImage.setFileName(filePath);
			svgImage.setResolution(100);

			setupPainterForExport(painter);

            mDiagram->deselectAll();

			painter->begin(&svgImage);
			painter->scale(svgImage.size().width() / visibleRect.width(), svgImage.size().height() / visibleRect.height());
			painter->translate(-visibleRect.left(), -visibleRect.top());
			painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
            mDiagram->render(painter, visibleRect);
			painter->end();

			delete painter;
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramManager::printPreview()
{
    if (hasDiagram())
	{
		QPrintPreviewDialog printPreviewDialog(&mPrinter, this);
		connect(&printPreviewDialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printPages(QPrinter*)));

        mDiagram->deselectAll();
		printPreviewDialog.exec();
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramManager::printSetup()
{
    if (hasDiagram())
	{
		QPageSetupDialog pageSetupDialog(&mPrinter, this);

        mDiagram->deselectAll();
		pageSetupDialog.exec();
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramManager::printDiagram()
{
    if (hasDiagram())
	{
		QPrintDialog printDialog(&mPrinter, this);
		printDialog.setEnabledOptions(QAbstractPrintDialog::PrintShowPageSize);

        mDiagram->deselectAll();

		if (printDialog.exec() == QDialog::Accepted)
		{
			for(int i = 0; i < mPrinter.numCopies(); i++) printPages(&mPrinter);
		}
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramManager::printPdf()
{
    if (hasDiagram())
	{
		QString pdfPath = suggestFilePath("pdf");

        mDiagram->deselectAll();

		if (promptToSaveFile(pdfPath, "Portable Document Format (*.pdf);;All Files (*)", "pdf"))
		{
			mPrinter.setOutputFileName(pdfPath);
			printPages(&mPrinter);
			mPrinter.setOutputFileName("");
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramManager::showDiagram()
{
	setCurrentIndex(0);
    mDiagram->zoomFit();
	emit visibilityChanged(true);
    emit diagramChanged(fileName());
}
//--------------------------------------------------------------------------------------------------
void DiagramManager::hideDiagram()
{
	setCurrentIndex(1);
	emit visibilityChanged(false);
    emit diagramChanged("");
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramManager::printPages(QPrinter* printer)
{
    if (printer && mDiagram)
	{
        DiagramPainter* painter = new DiagramPainter();
        DiagramPainter* diagramPainter;
		QRectF visibleRect;
		qreal pageAspect, scale;

        visibleRect = mDiagram->diagramRectWithBorder();
		pageAspect = printer->pageRect().width() / (qreal)printer->pageRect().height();
		scale = std::min(printer->pageRect().width() / visibleRect.width(),
						 printer->pageRect().height() / visibleRect.height());

        diagramPainter = mDiagram->painter();
        painter->setDefaultFont(diagramPainter->defaultFont());
        painter->setDefaultPen(diagramPainter->defaultPen());
		for(int i = 0; i < painter->numberOfBrushes(); i++)
            painter->setBrushAt(i, diagramPainter->brushAt(i));

		painter->setMode(DiagramPainter::Normal);
		//painter->setMode(DiagramPainter::BlackOnWhite);
		painter->setDrawBackground(false);
		painter->setDrawBorder(true);
		painter->setDrawGrid(false);

		if (visibleRect.height() * pageAspect > visibleRect.width())
		{
			visibleRect.adjust(-(visibleRect.height() * pageAspect - visibleRect.width()) / 2, 0,
							   (visibleRect.height() * pageAspect - visibleRect.width()) / 2, 0);
		}
		else if (visibleRect.width() / pageAspect > visibleRect.height())
		{
			visibleRect.adjust(0, -(visibleRect.width() / pageAspect - visibleRect.height()) / 2,
							   0, (visibleRect.width() / pageAspect - visibleRect.height()) / 2);
		}

		painter->begin(printer);
		painter->scale(scale, scale);
		painter->translate(-visibleRect.left(), -visibleRect.top());
		painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);

        mDiagram->render(painter, visibleRect);

		painter->end();
		delete painter;
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramManager::setupPainterForExport(DiagramPainter* painter)
{
    DiagramPainter* diagramPainter = mDiagram->painter();

    painter->setDefaultFont(diagramPainter->defaultFont());
    painter->setDefaultPen(diagramPainter->defaultPen());
	for(int i = 0; i < painter->numberOfBrushes(); i++)
        painter->setBrushAt(i, diagramPainter->brushAt(i));

    painter->setMode(DiagramPainter::Normal);
	painter->setDrawBackground(true);
	painter->setDrawBorder(false);
	painter->setDrawGrid(false);
}
//--------------------------------------------------------------------------------------------------
QString DiagramManager::suggestFilePath(const QString& suffix) const
{
	QString filePath = mFilePath;

	if (filePath.startsWith("Untitled"))
		filePath = "";
	else if (filePath.endsWith("." + mFileSuffix, Qt::CaseInsensitive))
		filePath = filePath.left(filePath.length() - mFileSuffix.length() - 1) + "." + suffix;

	return filePath;
}
//--------------------------------------------------------------------------------------------------
int DiagramManager::suggestExportWidth() const
{
    return (mDiagram->units() == UnitsMils) ? mDiagram->width() / 5 : mDiagram->width() * 8;
}
//--------------------------------------------------------------------------------------------------
int DiagramManager::suggestExportHeight() const
{
    return (mDiagram->units() == UnitsMils) ? mDiagram->height() / 5 : mDiagram->height() * 8;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramManager::promptToOpenFile(QString& filePath, const QString& filters)
{
	QFileDialog::Options options = 0;
	if (!mPromptOverwrite) options |= QFileDialog::DontConfirmOverwrite;

	if (filePath.isEmpty() || filePath.startsWith("Untitled"))
		filePath = mWorkingDirectory.path();

	filePath = QFileDialog::getOpenFileName(this, "Open File", filePath, filters, NULL, options);
	if (!filePath.isEmpty())
	{
		QFileInfo fileInfo(filePath);
		mWorkingDirectory = fileInfo.dir();
	}

	return (!filePath.isEmpty());
}
//--------------------------------------------------------------------------------------------------
bool DiagramManager::promptToSaveFile(QString& filePath, const QString& filters, const QString& suffix)
{
	QFileDialog::Options options = 0;
	if (!mPromptOverwrite) options |= QFileDialog::DontConfirmOverwrite;

	if (filePath.isEmpty() || filePath.startsWith("Untitled"))
		filePath = mWorkingDirectory.path();

	filePath = QFileDialog::getSaveFileName(this, "Save File", filePath, filters, NULL, options);
	if (!filePath.isEmpty())
	{
		QFileInfo fileInfo(filePath);
		mWorkingDirectory = fileInfo.dir();

		if (!filePath.endsWith("." + suffix, Qt::CaseInsensitive))
			filePath += "." + suffix;
	}

	return (!filePath.isEmpty());
}
//--------------------------------------------------------------------------------------------------
