FROM reivilo1234/qt-linux-cmake:ubuntu20-qt5.15.2

# Install libTiff
RUN apt-get update && apt-get install -y libtiff-dev 

