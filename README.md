## **GRAPE 0.1**
GRAPE is a **GRA**phical **P**ipeline **E**nvironment for image analysis in adaptive magnetic resonance imaging.  
See our publication here: <https://dx.doi.org/10.1007/s11548-016-1495-z>

## **MANUAL**
[Download here](doc/GRAPE_Manual.docx)

## **RELEASE NOTES**
Please see [RELEASE](RELEASE.md)

## **DIRECTORY CONTENTS**
  * bin: destination for GRAPE executable
  * doc: documentation
  * example: example GRAPE pipelines and sample images
  * share: application icons
  * src: GRAPE source code
  * test: unit test cases

## **REQUIREMENTS**
  * Qt 5.4.1 or later

## **COMPILING FROM SOURCE**
To compile GRAPE from source, download the latest release and compile according to machine instructions:

### Linux
```
qmake -makefile GRAPE.pro -spec linux-icc-64    # build with intel
-or-
qmake -makefile GRAPE.pro -spec linux-g++-64    # build with gnu

make -j4
```

### Windows
```
open GRAPE.pro in Qt creator
run qmake
run build
```

## **LICENSE**
Copyright Refaat Gabr, 2017  
GRAPE is distributed under the terms of the [GNU General Public License](LICENSE)
