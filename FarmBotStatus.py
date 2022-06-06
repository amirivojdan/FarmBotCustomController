import logging
from threading import Thread, Semaphore
from time import sleep
from pySerialTransfer import pySerialTransfer


class ReceivingDataFrame(object):
    raw_encoder_x = 0.0
    raw_encoder_y = 0.0
    raw_encoder_z = 0.0


class FarmBotStatus(Thread):
    """Receives the packets from the FarmBot and interpret them according to
     the table provided by the manufacturer in
     https://github.com/farmbot/farmbot-arduino-firmware#codes-received-from-the-arduino """

    def __init__(self, reading_frequency, serial_bus: pySerialTransfer):
        Thread.__init__(self)
        self.done = False
        self.reading_update_interval = 1 / reading_frequency  # T=1/f
        self.serial_bus = serial_bus

        self.done = False
        self.lastUpdate = ReceivingDataFrame()

    def run(self):

        while not self.done:
            if self.serial_bus.available():
                recSize = 0

                self.lastUpdate.raw_encoder_x = self.serial_bus.rx_obj(obj_type='f', start_pos=recSize)
                recSize += pySerialTransfer.STRUCT_FORMAT_LENGTHS['i']

                self.lastUpdate.raw_encoder_y = self.serial_bus.rx_obj(obj_type='f', start_pos=recSize)
                recSize += pySerialTransfer.STRUCT_FORMAT_LENGTHS['i']

                self.lastUpdate.raw_encoder_z = self.serial_bus.rx_obj(obj_type='f', start_pos=recSize)
                recSize += pySerialTransfer.STRUCT_FORMAT_LENGTHS['f']

                print('{} | {} | {}'.format(self.lastUpdate.raw_encoder_x,
                                            self.lastUpdate.raw_encoder_y,
                                            self.lastUpdate.raw_encoder_z))

            elif self.serial_bus.status < 0:
                if self.serial_bus.status == pySerialTransfer.CRC_ERROR:
                    print('ERROR: CRC_ERROR')
                elif self.serial_bus.status == pySerialTransfer.PAYLOAD_ERROR:
                    print('ERROR: PAYLOAD_ERROR')
                elif self.serial_bus.status == pySerialTransfer.STOP_BYTE_ERROR:
                    print('ERROR: STOP_BYTE_ERROR')
                else:
                    print('ERROR: {}'.format(self.serial_bus.status))

            sleep(self.reading_update_interval)
