import paho.mqtt.client as mqtt
import time

BROKER_ADDRESS = "192.168.0.105"
BROKER_PORT = 1883
TOPIC = "test/topic"
MESSAGE = "Hey, MQTT!"

# Define a flag to check if a message is received
received_message = False

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected successfully to the MQTT broker.")
        client.subscribe(TOPIC)
    else:
        print(f"Connection failed with code {rc}.")

def on_message(client, userdata, msg):
    global received_message
    print(f"Received message '{msg.payload.decode()}' on topic '{msg.topic}'")
    if msg.topic == TOPIC and msg.payload.decode() == MESSAGE:
        received_message = True

def on_disconnect(client, userdata, rc):
    print("Disconnected from the MQTT broker.")

def test_mqtt_broker():
    client = mqtt.Client()

    client.on_connect = on_connect
    client.on_message = on_message
    client.on_disconnect = on_disconnect

    try:
        print(f"Connecting to MQTT broker at {BROKER_ADDRESS}:{BROKER_PORT}...")
        client.connect(BROKER_ADDRESS, BROKER_PORT, 10)  # 10-second timeout
        client.loop_start()  

        # Wait for the client to connect and subscribe
        time.sleep(2)

        print(f"Publishing message '{MESSAGE}' to topic '{TOPIC}'...")
        client.publish(TOPIC, MESSAGE)

        # Wait for the message to be processed
        time.sleep(2)

        if received_message:
            print("Message was successfully received.")
        else:
            print("Failed to receive the message.")

        client.loop_stop()
        client.disconnect()

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    test_mqtt_broker()
