
import json
import paho.mqtt.client as mqtt

BROKER_ADDRESS = "192.168.0.105"
BROKER_PORT = 1883
TOPIC = "#"     # catch all topics

def on_connect(client, userdata, flags, rc):
    if 0 == rc:
        print("Connected successfully to the MQTT broker.")
        client.subscribe(TOPIC)
    else:
        print(f"Connection failed with code {rc}")

def on_message(client, userdata, msg):
    data = json.loads(msg.payload.decode())
    data_pretty_string = json.dumps(data, indent=4)
    print(f"""
TOPIC: {msg.topic}
PAYLOAD: {data_pretty_string}
""")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
print(f"Connection to MQTT broker at {BROKER_ADDRESS}:{BROKER_PORT}...")
client.connect(BROKER_ADDRESS, BROKER_PORT, 10)
try:
    client.loop_forever()
finally:
    print("Terminating...")
    print("Disconnecting from broker.")
    client.disconnect()