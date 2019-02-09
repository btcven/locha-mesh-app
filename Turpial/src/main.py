"""
(c) Copyright 2019 locha.io developers
Licensed under a MIT license, see LICENSE file in the root folder
for a full text.
"""

from lib.drivers.display.ux import OLED


display = OLED()
display.set_ble(enabled=True, connected=False)
display.set_wap(enabled=True)
display.set_wst(enabled=True, connected=True)
display.refresh()
