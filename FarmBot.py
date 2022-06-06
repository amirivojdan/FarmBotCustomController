from threading import Thread, Semaphore
from time import sleep
from CommunicationBus import CommunicationBus, enumerate_ports
from Extreme3dPro import Extreme3dPro
from FarmBotStatus import FarmBotStatus
import logging
from pySerialTransfer import pySerialTransfer


class SendingDataFrame(object):
    x_control = 5.0        # stop=0, forward=1, backward=-1
    x_speed = 5000.0       # frequency
    x_resolution = 32.0    # 8,16,32,64,128,256
    y_control = 6.0
    y_speed = 5000.0
    y_resolution = 32.0
    z_control = 7.0
    z_speed = 5000.0
    z_resolution = 32.0


class FarmBot(Thread):
    """High-level python class to wrap all underlying details required to control
    the FarmBot."""

    def __init__(self, port=None, baud_rate=115200, command_update_frequency=100,
                 status_update_frequency=1000):
        Thread.__init__(self)
        if not port:
            for available_port, desc, hwid in sorted(enumerate_ports()):
                if "Arduino Mega 2560" in desc:
                    if port:
                        logging.error("MORE THAN ONE ARDUINO COM PORT AVAILABLE!!!")
                        raise Exception("More than one Arduino COM port available."
                                        " Please explicitly define COM port "
                                        "in FarmBot constructor!")
                    port = available_port

        self.serial_bus = pySerialTransfer.SerialTransfer(port, baud_rate)
        self.status = FarmBotStatus(status_update_frequency, self.serial_bus)
        self.command_update_interval = 1 / command_update_frequency  # T=1/f
        self.done = False
        self.lastCommand = SendingDataFrame()
        self.joystick = Extreme3dPro()

    def connect(self):
        logging.debug("Connected!")
        self.serial_bus.open()
        sleep(5)

    def disconnect(self):
        logging.debug("Disconnected!")
        self.serial_bus.close()

    def run(self):
        logging.debug("Initializing...")
        self.status.start()

        while not self.done:

            self.joystick.update()
            #print("pitch:{pitch}  roll:{roll}  yaw:{yaw}".format(pitch=self.joystick.pitch,
            #                                                     roll=self.joystick.roll,
            #                                                      yaw=self.joystick.yaw))

            #i = 0
            #for btn in self.joystick.buttons_status:
                     #print("{btn_i} : {btn}".format(btn_i=i, btn=btn))
                     #i += 1

            #if self.joystick.pitch > 0.5:
                #self.serial_bus.send('W')

            #if self.joystick.pitch < -0.5:
                #self.serial_bus.send('S')

            #if abs(self.joystick.pitch) < 0.5:
                #self.serial_bus.send('Q')

            #if self.joystick.roll > 0.5:
                #self.serial_bus.send('D')

            #if self.joystick.roll < -0.5:
                #self.serial_bus.send('A')

            #if abs(self.joystick.roll) < 0.5:
                #self.serial_bus.send('E')

            sendSize = 0
            self.lastCommand.x_control = self.joystick.pitch
            self.lastCommand.y_control = self.joystick.roll
            self.lastCommand.z_control = self.joystick.yaw

            if self.joystick.buttons_status[0]:
                self.lastCommand.x_resolution = 64.0
                self.lastCommand.y_resolution = 64.0
                self.lastCommand.z_resolution = 64.0

            sendSize = self.serial_bus.tx_obj(self.lastCommand.x_control, start_pos=sendSize)
            sendSize = self.serial_bus.tx_obj(self.lastCommand.x_speed, start_pos=sendSize)
            sendSize = self.serial_bus.tx_obj(self.lastCommand.x_resolution, start_pos=sendSize)

            sendSize = self.serial_bus.tx_obj(self.lastCommand.y_control, start_pos=sendSize)
            sendSize = self.serial_bus.tx_obj(self.lastCommand.y_speed, start_pos=sendSize)
            sendSize = self.serial_bus.tx_obj(self.lastCommand.y_resolution, start_pos=sendSize)

            sendSize = self.serial_bus.tx_obj(self.lastCommand.z_control, start_pos=sendSize)
            sendSize = self.serial_bus.tx_obj(self.lastCommand.z_speed, start_pos=sendSize)
            sendSize = self.serial_bus.tx_obj(self.lastCommand.z_resolution, start_pos=sendSize)

            self.serial_bus.send(sendSize)

            sleep(self.command_update_interval)

