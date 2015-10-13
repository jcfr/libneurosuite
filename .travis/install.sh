#!/bin/sh

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
    if [ "$QT_VERSION" = "qt5" ]; then
        sudo add-apt-repository -y ppa:beineri/opt-qt55
    fi
    sudo add-apt-repository -y ppa:smspillaz/cmake-2.8.12
    sudo apt-get update
    sudo apt-get install -y cmake

    if [ "$QT_VERSION" = "qt4" ]; then
        sudo apt-get install -y libqtgui4
    fi
    if [ "$QT_VERSION" = "qt5" ]; then
        sudo apt-get install -y qt55base qt55webkit
        source /opt/qt55/bin/qt55-env.sh
    fi
fi

if [ "$TRAVIS_OS_NAME" = "osx" ]; then
    brew update
    brew install cmake $QT_VERSION
    if [ "$QT_VERSION" = "qt5" ]; then
        brew link --force qt5
    fi
fi
