QT += core
QT -= gui

CONFIG += c++1z

TARGET = FreqVocabular
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

DISTFILES += \
    all.txt \
    allNouns.txt \
    allNounsNoDouble.txt \
    allNounsNoDoubleNoSeldom.txt \
    allUTF.txt \
    words.txt \
    words1.txt \
    singleSolution.txt \
    singleSolutionCleaned1.txt \
    singleSolutionCleaned2.txt \
    singleSolutionCleaned3.txt \
    singleSolutionCleaned4.txt
