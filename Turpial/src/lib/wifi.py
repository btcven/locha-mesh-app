"""
(c) Copyright 2019 locha.io developers
Licensed under a MIT license, see LICENSE file in the root folder
for the full text.
"""
import network


class WiFi:
    def __init__(self):
        self.wap_conf = {}
        self.wst_conf = {}
        self.wst = network.WLAN(network.STA_IF)
        self.wap = network.WLAN(network.AP_IF)

    def st_connect(self, staconf):
        """
        :param staconf: Tuple {ssid: str, pass: str}
        :return: None
        """
        self.wst_conf = staconf

        if not self.wst.isconnected():
            print('connecting to network.. %s' % self.wst_conf['ssid'])
            self.wst.active(True)
            self.wst.connect(self.wst_conf['ssid'], self.wst_conf['pass'])
            while not self.wst.isconnected():
                pass
        print('network config;; ', self.wst.ifconfig())

    def ap_create(self, apconf):
        """
        :param apconf: Tuple {ssid: str, pass: str, auth: str}
        :return: None
        """
        self.wap_conf = apconf
        self.wap_conf = apconf['ssid']
        self.wap_conf = apconf['pass']
        self.wap_conf = apconf['auth']
        if not self.wap.isconnected():
            print('Creating AccessPoint %s ' % self.wap_conf['ssid'])
            self.wap.active(True)
            self.wap.config(essid=self.wap_conf['ssid'])
            self.wap.config(
                authmode=self.wap_conf['auth'], password=self.wap_conf['pass'])
            while self.wap.isconnected():
                print('new client')

    def aps_available(self):
        """
        :return: Tuple of available APs
        """
        self.wst.active(True)
        aps = self.wst.scan()
        return aps
