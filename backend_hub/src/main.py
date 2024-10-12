
import time
from mqtt import Mqtt, MqttParam


mqtt = Mqtt(MqttParam(
    broker_address = "192.168.0.105",
    topics = ["#"]
))

if '__main__' == __name__:
    mqtt.connect()
    while True:
        pass