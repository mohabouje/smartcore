from smartcore.core.algorithm import Algorithm
import PySide2  ## this will force pyqtgraph to use PySide instead of PyQt4
import pyqtgraph as pg


class LinearChart(Algorithm):
    def __init__(self,
                 title: str= "",
                 y_label: str = "",
                 x_label: str = "",
                 width: int = 1000,
                 height: int = 600,
                 interval: int = 20,
                 name: str = "",
                 timestamp: int = 0):
        super().__init__(name=name, timestamp=timestamp)
        pg.setConfigOptions(antialias=False)

        self.__x = None
        self.__y = None
        self.__interval = interval
        self.__win = pg.GraphicsWindow()
        self.__win.resize(width, height)
        self.__win.setWindowTitle(title)
        self.__win.show()
        self.__win.raise_()
        pg.setConfigOptions(antialias=True)

        self.__plt = self.__win.addPlot(title=title)
        self.__plt.resize(width, height)
        self.__plt.setLabel('left', y_label, 'V')
        self.__plt.setLabel('bottom', x_label, 's')
        self.__plt.setDownsampling(auto=True, mode='peak')
        self.__plt.setClipToView(True)
        self.__plt.showGrid(x=True, y=True)
        self.__curve = self.__plt.plot(pen='#00A3E0', title=title)

    def run(self, x, y):
        self.__x = x
        self.__y = y
        self.update()

    def update(self):
        self.__curve.setData(self.__x, self.__y)
        pg.QtGui.QGuiApplication.instance().processEvents()