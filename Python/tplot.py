# -*- coding: utf-8 -*-
import serial
import numpy as np

class ZedboardController:
    def __init__(self, com_port):
        self.com_port = com_port
        self.transceiver = serial.Serial(com_port, baudrate=115200)
    
    def setGPIOS(self, value):
        str_value = format(value, '03d')
        self.transceiver.write('sd{}\n'.format(str_value).encode())
        self.transceiver.flushOutput()
    
    def read_temperature_sensor(self, num_samples):
        to_return = []
        for i in range(num_samples):
            self.transceiver.write("gd\n".encode())
            self.transceiver.flushOutput()
            data = self.transceiver.read(6)
            to_return.append(float(data.decode()))
        
        return to_return
    
class Controller:
    def __init__(self, interface):
        self.interface = interface
        self.zedboard = ZedboardController('/dev/ttyACM0')
        self.connect_signals_to_methods()
        self.dataY = np.zeros(500)
        self.line = self.interface.graphComponent.plot()
    
#    def connect_signals_to_methods(self):
#        self.interface.sendButton.clicked.connect(self.led_button_clicked)
#        # ?? connect start button clicked signal
#        # ?? connect stop button clicked signal
#    
#    def led_button_clicked(self):
#        leds = [self.interface.led8_2,
#                self.interface.led7_2,
#                self.interface.led6_2,
#                self.interface.led5_2,
#                self.interface.led4_2,
#                self.interface.led3_2,
#                self.interface.led2_2,
#                self.interface.led1_2]
#        position = 0
#        result = 0
#        for led in leds:
#            if led.isChecked():
#                # Calculate result of binary representation of leds
#                # for example leds in off, off, off, off, off, off, one,one should have 3 as result
#                # Save the variable called result
#            position += 1
#            
#        self.interface.ledInformation.append('<h1>Leds changed: {}</h1>'.format(result))    
#        
#        self.zedboard.setGPIOS(result)
#    
#    def start_button_clicked(self):
#        self.interface.status.setStyleSheet("background-color: green")
#        self.timer = QtCore.QTimer()
#        self.timer.timeout.connect(self.timeout_reached)
#        self.timer.start(33)
#    
#    def stop_button_clicked(self):
#        if self.timer:
#            self.interface.status.setStyleSheet("background-color: red")
#            self.timer.stop()
            
    def timeout_reached(self):
        received = self.zedboard.read_temperature_sensor(1)
        self.dataY[:-1] = self.dataY[1:]
        self.dataY[-1] = received[0]
        self.line.setData(self.dataY)
        self.interface.lcd.display(received[0])
        
        
    def main():
        qt_app = QtGui.QApplication([])
        main_window = QtGui.QWidget()
        my_interface = myui.Ui_Dialog()
        my_interface.setupUi(main_window)
        c = Controller(my_interface)
        main_window.show()
        qt_app.exec_()

if __name__ == '__main__':
    main()