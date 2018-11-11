from smartcore.core.algorithm import Algorithm
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtWidgets


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
        self.x = None
        self.y = None
        self.__interval = interval
        self.__win = pg.GraphicsWindow()
        self.__win.resize(width, height)
        self.__win.show()

        self.__layout = QtWidgets.QVBoxLayout()
        self.__win.setLayout(self.__layout)

        self.__plt = self.__win.addPlot(title=title)
        self.__plt.resize(width, height)
        self.__plt.showGrid(x=True, y=True)
        self.__plt.setLabel('left', y_label, 'V')
        self.__plt.setLabel('bottom', x_label, 's')
        self.__curve = self.__plt.plot([], pen=(255, 0, 0))

        self.__timer = QtCore.QTimer()
        self.__timer.timeout.connect(self.__update)
        self.__timer.start(self.__interval)

    def run(self, x, y):
        self.x = x
        self.y = y

    def __update(self):
        self.__curve.setData(self.x, self.y)
