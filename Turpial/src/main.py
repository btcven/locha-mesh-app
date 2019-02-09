"""
(c) Copyright 2019 locha.io developers
Licensed under a MIT license, see LICENSE file in the root folder
for a full text.
"""

from lib.drivers.display.ux import OLED
from lib.drivers.display import freeSans_17

oled = OLED()
display = oled.begin()
display.text(0, 0, "hello", freeSans_17)
display.refresh()
