"""
(c) Copyright 2019 locha.io developers
Licensed under a MIT license, see LICENSE file in the root folder
for the full text.
"""
import machine 
from . import ssd1306
from .writer import Writer
from .qr import QRCode
from . import freeSans_17
from . import freeSans_14


class OLED:
    def __init__(self, width=128, height=64):
        """
        :param width: screen width in pixels
        :param height: screen height in pixels
        """
        self.width = width
        self.height = height
        self.rst = machine.Pin(16, machine.Pin.OUT)
        self.rst.value(1)
        self.scl = machine.Pin(15, machine.Pin.OUT, machine.Pin.PULL_UP)
        self.sda = machine.Pin(4, machine.Pin.OUT, machine.Pin.PULL_UP)

        self.i2c = machine.I2C(scl=self.scl, sda=self.sda, freq=450000)
        self.display = ssd1306.SSD1306_I2C(self.width, self.height, self.i2c, addr=0x3c)
        self.contrast(128)
        self.show = self.display.show

    def begin(self):
        """
        :return: display instance
        """
        return self.display

    def clear(self):
        """
        clear screen
        :return: None
        """
        self.display.fill(0)
        self.display.show()

    def simple_text(self, txt, x, y):
        """
        :param txt: str, text, default None
        :param x: int. "x position in px", default None
        :param y: int. "y position in px", default None
        :return: None
        """
        self.display.text(txt, x, y)

    def position(self, x, y):
        """
        :param x:
        :param y:
        :return: None
        """
        Writer.set_textpos(self.display, y, x)

    def text(self, x, y, txt, font=freeSans_14):
        wri = Writer(self.display, font)
        self.position(x, y)
        wri.printstring(txt)

    def refresh(self):
        """
        refresh the screen
        :return: None
        """
        self.display.show()

    def contrast(self, n=128):
        """
        Set up the contrast.
        :param n: value for contrast 0 ~ 255
        :return: None
        """
        self.display.contrast(n)

    def line(self, x_init, y_init, x_ends, y_ends):
        """
        draw a line from; "x_init, y_init" to; "x_end, y_end"
        :param x_init: int
        :param y_init: int
        :param x_ends: int
        :param y_ends: int
        :return: None
        """
        self.display.line(x_init, y_init, x_ends, y_ends)

    def rectangle(self, x, y, width, height):
        """
        draw a rectangle from x, y
        :param x: X position in px.
        :param y: Y position in px.
        :param width: rectangle width, in px.
        :param height: rectangle height in px.
        :return: None
        """
        self.display.fill_rect(x, y, width, height, 1)

    def set_ble(self, enabled=False, connected=False, font=freeSans_14):
        if enabled:
            self.text(self.width - 2 * font.max_width(), 10, "BT", font)
        if connected:
            self.text(self.width - 3 * font.max_width(), 10, "*", font)

    def set_wap(self, enabled=False, connections=0, font=freeSans_14):
        if enabled:
            self.text(self.width - 2 * font.max_width(), 30, "AP", font)
        if connections > 0:
            self.text(self.width - 3 * font.max_width(), 30, str(connections), font)

    def set_wst(self, enabled=False, connected=False, font=freeSans_14):
        if enabled:
            self.text(self.width - 2 * font.max_width(), 50, "ST", font)
        if connected:
            self.text(self.width - 3 * font.max_width(), 50, "*", font)

    def qr(self, data, scale=1, border=0):
        qr_code = QRCode(border=border)
        qr_code.add_data(data)
        matrix = qr_code.get_matrix()

        self.clear()

        for y in range(len(matrix)*scale):
            for x in range(len(matrix[0])*scale):
                v = not matrix[int(y / scale)][int(x / scale)]
                self.display.pixel(x, y, v)
        self.show()
