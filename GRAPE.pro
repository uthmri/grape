#-------------------------------------------------
#
# Project created by QtCreator 2016-02-09T14:48:37
#
#-------------------------------------------------

win32:TARGET = GRAPE
!win32:TARGET = GRAPE

DESTDIR = bin
INCLUDEPATH += src src/Diagram src/Items

QT += xml

CONFIG += qt \
        release \
        warn_on \
        rtti
CONFIG -= debug
CONFIG += console
CONFIG += c++11
QT += widgets printsupport svg

!win32:MOC_DIR = release
!win32:OBJECTS_DIR = release
!win32:RCC_DIR = release


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GRAPE
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    src/common.cpp \
    src/Diagram/Diagram.cpp \
    src/Diagram/DiagramArcItem.cpp \
    src/Diagram/DiagramBase.cpp \
    src/Diagram/DiagramBaseItems.cpp \
    src/Diagram/DiagramBoxItem.cpp \
    src/Diagram/DiagramCurveItem.cpp \
    src/Diagram/DiagramEllipseItem.cpp \
    src/Diagram/DiagramItem.cpp \
    src/Diagram/DiagramItemFactory.cpp \
    src/Diagram/DiagramItemGroup.cpp \
    src/Diagram/DiagramItemPoint.cpp \
    src/Diagram/DiagramLineItem.cpp \
    src/Diagram/DiagramMultipleItemProperties.cpp \
    src/Diagram/DiagramOvalItem.cpp \
    src/Diagram/DiagramPathItem.cpp \
    src/Diagram/DiagramPolygonItem.cpp \
    src/Diagram/DiagramPolylineItem.cpp \
    src/Diagram/DiagramProperties.cpp \
    src/Diagram/DiagramRoundItem.cpp \
    src/Diagram/DiagramTextItem.cpp \
    src/Diagram/DiagramTwoPortItem.cpp \
    src/Diagram/DiagramTypes.cpp \
    src/Diagram/DiagramUndo.cpp \
    src/Diagram/DiagramWidgets.cpp \
    src/Items/BETItem.cpp \
    src/Items/BiasCorrectionItem.cpp \
    src/Items/DelayItem.cpp \
    src/Items/BranchItem.cpp \
#    src/Items/ChartBoxItem.cpp \
#    src/Items/ChartEllipseItem.cpp \
#    src/Items/ChartPlusItem.cpp \
#    src/Items/ChartPolygonItem.cpp \
#    src/Items/ChartSumItem.cpp \
    src/Items/DividerItem.cpp \
#    src/Items/ElectricItems.cpp \
#    src/Items/Flow1Item.cpp \
#    src/Items/Flow2Item.cpp \
#    src/Items/Flow3Item.cpp \
    src/Items/ImageArithmeticItem.cpp \
    src/Items/ImageNormalizerItem.cpp \
    src/Items/ImageReaderItem.cpp \
    src/Items/ImageStackItem.cpp \
    src/Items/ImageSlicerItem.cpp \
    src/Items/ImageStatisticsItem.cpp \
    src/Items/ImageWriterItem.cpp \
    src/Items/LogicItems.cpp \
    src/Items/LogOperationItem.cpp \
    src/Items/LoopItem.cpp \
    src/Items/NodeItem.cpp \
    src/Items/ImageRegistrationItem.cpp \
    src/Items/RelOperatorItem.cpp \
    src/Items/ImageSegmentationItem.cpp \
    src/Items/SelectItem.cpp \
    src/Items/sinkdialog.cpp \
    src/Items/SinkItem.cpp \
    src/Items/SourceItem.cpp \
    src/Items/SystemcallItem.cpp \
    src/Items/DevelopItem.cpp \
    src/Nodes/arc.cpp \
    src/Nodes/bet.cpp \
    src/Nodes/end.cpp \
    src/Nodes/biascorrection.cpp \
    src/Nodes/delay.cpp \
    src/Nodes/branch.cpp \
    src/Nodes/divider.cpp \
    src/Nodes/imagearithmetic.cpp \
    src/Nodes/imagenormalizer.cpp \
    src/Nodes/imagereader.cpp \
    src/Nodes/imagestack.cpp \
    src/Nodes/imageslicer.cpp \
    src/Nodes/imagestatistics.cpp \
    src/Nodes/imagewriter.cpp \
    src/Nodes/line.cpp \
    src/Nodes/logoperation.cpp \
    src/Nodes/loop.cpp \
    src/Nodes/node.cpp \
    src/Nodes/imageregistration.cpp \
    src/Nodes/reloperator.cpp \
    src/Nodes/imagesegmentation.cpp \
    src/Nodes/select.cpp \
    src/Nodes/sink.cpp \
    src/Nodes/source.cpp \
    src/Nodes/systemcall.cpp \
    src/Nodes/develop.cpp \
    src/AboutDialog.cpp \
    src/DiagramItemDefaultsWidget.cpp \
    src/DiagramManager.cpp \
    src/DiagramToolsWidget.cpp \
    src/Pipeline.cpp \
    src/PreferencesDialog.cpp \
    src/xmlreclib.cpp \
    src/nodetreewidget.cpp \
    src/Nodes/nodeinterface.cpp \
    src/mpfit.c \
    src/Nodes/curvefitting.cpp \
    src/Items/EndItem.cpp \
    src/Nodes/imageprocessing.cpp \
    src/Items/ImageProcessingItem.cpp \
    src/Items/ParWriterItem.cpp \
    src/Nodes/parwriter.cpp \
    src/Items/ParReaderItem.cpp \
    src/Nodes/parreader.cpp \
    src/Items/CurveFittingItem.cpp \
    src/Items/ImageReformatItem.cpp \
    src/Nodes/imagereformat.cpp \
    src/datatypes.cpp \
    src/Items/SynchItem.cpp \
    src/Nodes/synch.cpp \
    src/image/ml/svm.cpp

HEADERS  += mainwindow.h \
    src/common.h \
    src/Diagram/Diagram.h \
    src/Diagram/DiagramArcItem.h \
    src/Diagram/DiagramBase.h \
    src/Diagram/DiagramBaseItems.h \
    src/Diagram/DiagramBoxItem.h \
    src/Diagram/DiagramCurveItem.h \
    src/Diagram/DiagramEllipseItem.h \
    src/Diagram/DiagramItem.h \
    src/Diagram/DiagramItemFactory.h \
    src/Diagram/DiagramItemGroup.h \
    src/Diagram/DiagramItemPoint.h \
    src/Diagram/DiagramLineItem.h \
    src/Diagram/DiagramMultipleItemProperties.h \
    src/Diagram/DiagramOvalItem.h \
    src/Diagram/DiagramPathItem.h \
    src/Diagram/DiagramPolygonItem.h \
    src/Diagram/DiagramPolylineItem.h \
    src/Diagram/DiagramProperties.h \
    src/Diagram/DiagramRoundItem.h \
    src/Diagram/DiagramTextItem.h \
    src/Diagram/DiagramTwoPortItem.h \
    src/Diagram/DiagramTypes.h \
    src/Diagram/DiagramUndo.h \
    src/Diagram/DiagramWidgets.h \
    src/Items/BETItem.h \
    src/Items/BiasCorrectionItem.h \
    src/Items/DelayItem.h \
    src/Items/BranchItem.h \
#    src/Items/ChartBoxItem.h \
#    src/Items/ChartEllipseItem.h \
#    src/Items/ChartPlusItem.h \
#    src/Items/ChartPolygonItem.h \
#    src/Items/ChartSumItem.h \
    src/Items/DividerItem.h \
#    src/Items/ElectricItems.h \
#    src/Items/Flow1Item.h \
#    src/Items/Flow2Item.h \
#    src/Items/Flow3Item.h \
    src/Items/ImageArithmeticItem.h \
    src/Items/ImageNormalizerItem.h \
    src/Items/ImageReaderItem.h \
    src/Items/ImageStackItem.h \
    src/Items/ImageSlicerItem.h \
    src/Items/ImageStatisticsItem.h \
    src/Items/ImageWriterItem.h \
    src/Items/LogicItems.h \
    src/Items/LogOperationItem.h \
    src/Items/LoopItem.h \
    src/Items/ImageRegistrationItem.h \
    src/Items/RelOperatorItem.h \
    src/Items/ImageSegmentationItem.h \
    src/Items/SelectItem.h \
    src/Items/sinkdialog.h \
    src/Items/SinkItem.h \
    src/Items/SourceItem.h \
    src/Items/SystemcallItem.h \
    src/Items/DevelopItem.h \
    src/Nodes/arc.h \
    src/Nodes/bet.h \
    src/Nodes/end.h \
    src/Nodes/biascorrection.h \
    src/Nodes/delay.h \
    src/Nodes/branch.h \
    src/Nodes/divider.h \
    src/Nodes/imagearithmetic.h \
    src/Nodes/imagenormalizer.h \
    src/Nodes/imagereader.h \
    src/Nodes/imagestack.h \
    src/Nodes/imageslicer.h \
    src/Nodes/imagestatistics.h \
    src/Nodes/imagewriter.h \
    src/Nodes/line.h \
    src/Nodes/logoperation.h \
    src/Nodes/loop.h \
    src/Nodes/node.h \
    src/Nodes/imageregistration.h \
    src/Nodes/reloperator.h \
    src/Nodes/imagesegmentation.h \
    src/Nodes/select.h \
    src/Nodes/sink.h \
    src/Nodes/source.h \
    src/Nodes/systemcall.h \
    src/Nodes/develop.h \
    src/AboutDialog.h \
    src/datatypes.h \
    src/DiagramItemDefaultsWidget.h \
    src/DiagramManager.h \
    src/DiagramToolsWidget.h \
    src/Pipeline.h \
    src/PreferencesDialog.h \
    src/xmlreclib.h \
    src/nodetreewidget.h \
    src/Nodes/nodeinterface.h \
    src/Items/NodeItem.h \
    src/mpfit.h \
    src/Items/CurveFittingItem.h \
    src/Nodes/curvefitting.h \
    src/Items/EndItem.h \
    src/Nodes/imageprocessing.h \
    src/Items/ImageProcessingItem.h \
    src/Items/ParWriterItem.h \
    src/Nodes/parwriter.h \
    src/Items/ParReaderItem.h \
    src/Nodes/parreader.h \
    src/Items/ImageReformatItem.h \
    src/Nodes/imagereformat.h \
    src/Items/SynchItem.h \
    src/Nodes/Synch.h \
    src/image/filter/anisotropic_diffusion.hpp \
    src/image/filter/canny_edge.hpp \
    src/image/filter/filter_model.hpp \
    src/image/filter/gaussian.hpp \
    src/image/filter/gradient_magnitude.hpp \
    src/image/filter/laplacian.hpp \
    src/image/filter/mean.hpp \
    src/image/filter/sobel.hpp \
    src/image/io/2dseq.hpp \
    src/image/io/avi.hpp \
    src/image/io/bitmap.hpp \
    src/image/io/dicom.hpp \
    src/image/io/interface.hpp \
    src/image/io/io.hpp \
    src/image/io/mat.hpp \
    src/image/io/nifti.hpp \
    src/image/io/tiff.hpp \
    src/image/io/tiff_tag.hpp \
    src/image/ml/ada_boost.hpp \
    src/image/ml/cnn.hpp \
    src/image/ml/decision_tree.hpp \
    src/image/ml/em.hpp \
    src/image/ml/hmc.hpp \
    src/image/ml/k_means.hpp \
    src/image/ml/lg.hpp \
    src/image/ml/nb.hpp \
    src/image/ml/non_parametric.hpp \
    src/image/ml/svm.hpp \
    src/image/ml/utility.hpp \
    src/image/morphology/morphology.hpp \
    src/image/numerical/basic_op.hpp \
    src/image/numerical/dif.hpp \
    src/image/numerical/fft.hpp \
    src/image/numerical/index_algorithm.hpp \
    src/image/numerical/interpolation.hpp \
    src/image/numerical/matrix.hpp \
    src/image/numerical/numerical.hpp \
    src/image/numerical/optimization.hpp \
    src/image/numerical/resampling.hpp \
    src/image/numerical/slice.hpp \
    src/image/numerical/statistics.hpp \
    src/image/numerical/transformation.hpp \
    src/image/numerical/window.hpp \
    src/image/reg/bfnorm.hpp \
    src/image/reg/cdm.hpp \
    src/image/reg/lddmm.hpp \
    src/image/reg/linear.hpp \
    src/image/segmentation/disjoint_set.hpp \
    src/image/segmentation/fast_marching.hpp \
    src/image/segmentation/graph_cut.hpp \
    src/image/segmentation/otsu.hpp \
    src/image/segmentation/segmentation.hpp \
    src/image/segmentation/stochastic_competition.hpp \
    src/image/segmentation/watershed.hpp \
    src/image/utility/basic_image.hpp \
    src/image/utility/geometry.hpp \
    src/image/utility/multi_thread.hpp \
    src/image/utility/pixel_index.hpp \
    src/image/utility/pixel_value.hpp \
    src/image/vis/color_map.hpp \
    src/image/vis/march_cube.hpp \
    src/image/image.hpp

RESOURCES += share/GRAPE.qrc
OTHER_FILES += share/GRAPE.rc
RC_FILE = share/GRAPE.rc


FORMS    += mainwindow.ui
