"""
(c) Copyright 2019 locha.io developers
Licensed under a MIT license, see LICENSE file in the root folder
for the full text.
"""
import machine 
from . import ssd1306


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

    def text(self, txt, x, y, refresh=False):
        """
        :param txt: str, text, default None
        :param x: int. "x position in px", default None
        :param y: int. "y position in px", default None
        :param refresh: bool, "refresh screen after put the text", default False
        :return: None
        """
        if refresh:
            self.clear()
            self.display.text(txt, x, y)
            self.display.show()
        else:
            self.display.text(txt, x, y)

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

    def font(self):
        pass
