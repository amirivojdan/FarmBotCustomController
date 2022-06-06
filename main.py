import logging
from FarmBot import FarmBot
from time import sleep
from pySerialTransfer import pySerialTransfer

logging.basicConfig(level=logging.DEBUG)

if __name__ == '__main__':
    farmbot = FarmBot()
    farmbot.connect()
    farmbot.start()
    # FarmBotGUI(farmbot)

