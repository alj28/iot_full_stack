
import json
import paho.mqtt.client as mqtt
from dataclasses import dataclass
import database as db

_CONNECTION_RESULT_TABLE = {
    0   :   'Connection successful',
    1   :   'Connection refused - incorrect protocol version',
    2   :   'Connection refused - invalid client identifier',
    3   :   'Connection refused - server unavailable',
    4   :   'Connection refused - bad username or password',
    5   :   'Connection refused - not authorized',
}

BACKEND_URL = "http://192.168.0.105:8000/temperature-log/"  # solution until Docker networking is fixed
database = db.Database(db.DatabaseParam(address=BACKEND_URL))

@dataclass
class MqttParam:
    broker_address: str
    topics: list[str]

    broker_port: int = 1883
    keep_alive_time_s: int = 60

def on_connect(client, userdata, flags, rc):
    userdata.on_connect(client, flags, rc)

def on_message(client, userdata, msg):
    userdata.on_message(client, msg)

class Mqtt:
    def __init__(self, param: MqttParam):
        self._client = mqtt.Client(userdata=self, reconnect_on_failure=True)
        self._client.on_connect = on_connect
        self._client.on_message = on_message

        self._param = param

    def connect(self):
        print(f"Connecting to {self._param.broker_address}:{self._param.broker_port}...")
        self._client.connect(
            host = self._param.broker_address,
            port = self._param.broker_port,
            keepalive = self._param.keep_alive_time_s
        )
        self._client.loop_start()

    def disconnect(self):
        self._client.disconnect()

    def _print_connection_result(self, rc):
        if rc in _CONNECTION_RESULT_TABLE:
            print(f"{_CONNECTION_RESULT_TABLE[rc]}")
        else:
            print(f"Connection result {rc}")

    def on_connect(self, client, flags, rc):
        self._print_connection_result(rc)
        if 0 == rc:
            print(f"Subscribing to {len(self._param.topics)} topics...")
            for t in self._param.topics:
                print(f"\tSubscribing to {t}")
                self._client.subscribe(t)

    def on_message(self, client, msg):
        data = json.loads(msg.payload.decode())
        data_pretty_string = json.dumps(data, indent=4)
#        print(f"""
#TOPIC: {msg.topic}
#PAYLOAD: {data_pretty_string}
#""")
        if msg.topic == 'zigbee2mqtt/0xf4ce3620277fadba':
            serial_no = msg.topic.split('/')[-1]
            data = db.TemperatureLogWrite(
                temperature = float(data['temperature']),
                device = serial_no
            )
            database.write_temperature_log(data)
