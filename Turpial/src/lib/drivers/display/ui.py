"""
(c) Copyright 2019 locha.io developers
Licensed under a MIT license, see LICENSE file in the root folder
for the full text.
"""
import machine 
from . import ssd1306

class display:
    def __init__(self, width=128, height=64, scl=5, sda=4, freq=400000):
        self.width = width
        self.height = height
        self.i2c = machine.I2C(scl=machine.Pin(scl, machine.Pin.OUT), sda=machine.Pin(sda), freq=freq)
        self.display = ssd1306.SSD1306_I2C(width, height, self.i2c)
        self.show = self.display.show
    
    def clear(self):
        self.display.fill(0)
        self.display.show()
